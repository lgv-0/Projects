using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Uber_Vaults
{
    public class VaultStorage
    {
        public UnityEngine.Transform Locker;
        public bool alreadyOpened;
        public SDG.Unturned.Items ItemList;
        public Rocket.Unturned.Player.UnturnedPlayer Player;
        public Steamworks.CSteamID SteamID;
        public int SizeX;
        public int SizeY;
    }
}
