using System;
using System.Reflection;
using UnityEngine;
using SDG.Unturned;
using Rocket.Core.Plugins;
using Rocket.Core.Logging;
using Rocket.API;
using Rocket.Unturned.Chat;
using Rocket.Core;
using System.Collections.Generic;

namespace Uber_Vaults
{
    public class VaultCommand : IRocketCommand
    {
        public AllowedCaller AllowedCaller
        {
            get { return AllowedCaller.Player; }
        }

        public string Name
        {
            get { return "vault"; }
        }

        public string Help
        {
            get { return "Open your vault"; }
        }

        public string Syntax
        {
            get { return "/vault"; }
        }

        public List<string> Aliases
        {
            get { return new List<string>(); }
        }

        public void Execute(IRocketPlayer caller, string[] command)
        {
            Rocket.Unturned.Player.UnturnedPlayer player = (Rocket.Unturned.Player.UnturnedPlayer)caller;
            if (player == null)
                return;
            foreach (VaultStorage vaults in Uber_VaultS.vaultList)
                if (vaults.SteamID == player.CSteamID)
                {
                    byte x, y;
                    ushort plant, index;
                    BarricadeRegion regionBarricade;
                    if (BarricadeManager.tryGetInfo(vaults.Locker, out x, out y, out plant, out index, out regionBarricade))
                    {
                        InteractableStorage IStorage = (InteractableStorage)regionBarricade.drops[index].interactable;
                        IStorage.isOpen = true;
                        IStorage.opener = player.Player;
                        //IStorage.name = "U" + player.CSteamID.ToString();

                        if (vaults.alreadyOpened == false)
                        {
                            IStorage.items.resize((byte)vaults.SizeX, (byte)vaults.SizeY);
                            IStorage.items.clear();
                            if (vaults.ItemList.getItemCount() > 0)
                                for (byte i = 0; i < vaults.ItemList.getItemCount(); i++)
                                {
                                    ItemJar currentItemJar = vaults.ItemList.getItem(i);
                                    Item CurrentItem = currentItemJar.item;
                                    IStorage.items.addItem(vaults.ItemList.getItem(i).x, vaults.ItemList.getItem(i).y, vaults.ItemList.getItem(i).rot, vaults.ItemList.getItem(i).item);
                                }
                            vaults.alreadyOpened = true;
                        }

                        player.Inventory.isStoring = true;
                        player.Inventory.storage = IStorage;
                        player.Inventory.updateItems(PlayerInventory.STORAGE, IStorage.items);
                        player.Inventory.sendStorage();
                    }
                }
        }

        public List<string> Permissions
        {
            get
            {
                return new List<string>
                {
                  "uber.vault"
                };
            }
        }
    }
}
