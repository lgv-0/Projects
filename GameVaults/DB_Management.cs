using System;
using MySql.Data.MySqlClient;
using Rocket.Core.Logging;
using System.Globalization;
using Rocket.API.Serialisation;
using Rocket.API;
using System.Text.RegularExpressions;

/*
	Weapon Metadata Structure:
	0: Sight Attachment (strangely not 100% matched to Item ID)
	1: 
	2: Tactical Attachment Item ID
	3: 
	4: Grip Attachment Item ID
	5: 
	6: Barrel Attachment Item ID
	7: 
	8: Magazine Item ID
	9: 
	10: Ammo
*/

namespace Uber_Vaults
{
    public class DB_Management
    {
        internal DB_Management()
        {
            new I18N.West.CP1250(); //Workaround for database encoding issues with mono
        }

        private MySqlConnection createConnection()
        {
            MySqlConnection connection = null;
            try
            {
                connection = new MySqlConnection(String.Format("SERVER=192.168.1.90;DATABASE=unturned;UID=unturned;PASSWORD=redacted;PORT=3306;"));
            }
            catch (Exception ex)
            {
                Logger.LogException(ex);
            }
            return connection;
        }

        public void PlayerConnect(Rocket.Unturned.Player.UnturnedPlayer pPlayer)
        {
            MySqlConnection connection = createConnection();
            MySqlCommand command = connection.CreateCommand();
            connection.Open();
            //////////////////
            command.CommandText = "select * from `itemlist` WHERE `steamId`='" + pPlayer.CSteamID.ToString() + "';";
            SDG.Unturned.Items ItemListN = new SDG.Unturned.Items(0);
            MySqlDataReader reader = command.ExecuteReader();
            while (reader.Read())
            {
                SDG.Unturned.Item NItem = new SDG.Unturned.Item(ushort.Parse((string)reader["id"], CultureInfo.InvariantCulture), 1, (byte)(int)reader["quality"]);
                NItem.durability = (byte)(int)reader["durability"];

                ItemListN.addItem((byte)(int)reader["x"], (byte)(int)reader["y"], (byte)(int)reader["rot"], NItem);

                SDG.Unturned.ItemAsset ia = (SDG.Unturned.ItemAsset)SDG.Unturned.Assets.find(SDG.Unturned.EAssetType.ITEM, NItem.id);
                int metadataSize = reader.GetInt32("data_size");
                if (metadataSize > 0)
                {
                    byte[] metadata = new byte[metadataSize];
                    reader.GetBytes(reader.GetOrdinal("data"), 0, metadata, 0, metadataSize);
                    NItem.metadata = metadata;
                }
            }
            reader.Close();

            command.CommandText = "select * from `vaults`;";
            SDG.Unturned.Barricade NewBarricade = new SDG.Unturned.Barricade(366);
            UnityEngine.Transform NewBTransform = SDG.Unturned.BarricadeManager.dropBarricade(NewBarricade, null, new UnityEngine.Vector3(-20, -20, -20), 0, 0, 0, ulong.Parse(pPlayer.Id), 29832);
            reader = command.ExecuteReader();
            while (reader.Read())
            {
                if ((string)reader["steamId"] == pPlayer.CSteamID.ToString())
                {
                    //Create new vaultstorage in home list
                    Uber_VaultS.vaultList.Add(new VaultStorage()
                                                                                        { SizeX = (int)reader["sizex"],
                                                                                                SizeY = (int)reader["sizey"],
                                                                                                Player = pPlayer,
                                                                                                SteamID = pPlayer.CSteamID,
                                                                                                Locker = NewBTransform,
                                                                                                alreadyOpened = false,
                                                                                                ItemList = ItemListN
                                                                                        });
                    reader.Close();
                    connection.Close();
                    return;
                }
            }
            reader.Close();

            ////////////////// ////////////////// ////////////////// ////////////////// ////////////////// //////////////////
            //In this case, no vault was found to be saved. Need to make a vault and push to db
            command.CommandText = "INSERT INTO `vaults` (`steamId`, `sizex`, `sizey`) VALUES ('" + pPlayer.CSteamID.ToString() + "', '3', '2');";
            command.ExecuteScalar();
            {
                Uber_VaultS.vaultList.Add(new VaultStorage()
                {
                    SizeX = 3,
                    SizeY = 2,
                    Player = pPlayer,
                    SteamID = pPlayer.CSteamID,
                    Locker = NewBTransform,
                    alreadyOpened = false
                });
            }
            //////////////////
            connection.Close();
        }

        public MySqlCommand PrepareCommand(string query, object[] bindings, MySqlConnection connection)
        {
            for (int i = 0; i < bindings.Length; i++)
            {
                var regex = new Regex(Regex.Escape("?"));
                query = regex.Replace(query, "@param" + i, 1);
            }

            var cmd = connection.CreateCommand();
            cmd.CommandText = query;
            cmd.Prepare();
            int index = 0;
            foreach (object o in bindings)
            {
                cmd.Parameters.AddWithValue("@param" + index, o);
                index++;
            }
            return cmd;
        }

        public void PlayerDisconnect(Rocket.Unturned.Player.UnturnedPlayer pPlayer)
        {
            MySqlConnection connection = createConnection();
            MySqlCommand command = connection.CreateCommand();
            connection.Open();
            //////////////////
            command.CommandText = "DELETE from `itemlist` WHERE `steamId`='" + pPlayer.CSteamID.ToString() + "';";
            command.ExecuteScalar();
            {
                foreach (VaultStorage store in Uber_VaultS.vaultList)
                    if (store.SteamID == pPlayer.CSteamID)
                    {
                        byte x, y;
                        ushort plant, index;
                        SDG.Unturned.BarricadeRegion regionBarricade;
                        if (SDG.Unturned.BarricadeManager.tryGetInfo(store.Locker, out x, out y, out plant, out index, out regionBarricade))
                        {
                            SDG.Unturned.InteractableStorage IStorage = (SDG.Unturned.InteractableStorage)regionBarricade.drops[index].interactable;
                            for (byte i = 0; i < IStorage.items.getItemCount(); i++)
                            {
                                SDG.Unturned.ItemJar iJar = IStorage.items.getItem(i);
                                SDG.Unturned.Item Item = iJar.item;
                                SDG.Unturned.ItemAsset iAsset = (SDG.Unturned.ItemAsset)SDG.Unturned.Assets.find(SDG.Unturned.EAssetType.ITEM, Item.id);

                                string query = "INSERT INTO `itemlist` (`steamID`, `id`, `quality`, `durability`, `x`, `y`, `rot`, `data`, `data_size`) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
                                MySqlCommand cmd = PrepareCommand(query, new object[] { pPlayer.CSteamID.ToString(), Item.id.ToString(), Item.quality, Item.durability, iJar.x, iJar.y, iJar.rot, Item.metadata , Item.metadata.Length }, connection);
                                cmd.ExecuteNonQuery();
                            }
                        }

                        SDG.Unturned.BarricadeManager.damage(store.Locker, 30000f, 1, false);
                        //SDG.Unturned.BarricadeManager.Destroy(store.Locker.gameObject);
                        Uber_VaultS.vaultList.Remove(store);
                        break;
                    }
            }
            //////////////////
            connection.Close();
        }
    }
}
