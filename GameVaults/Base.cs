using System;
using System.Reflection;
using UnityEngine;
using SDG.Unturned;
using Rocket.Core.Plugins;
using Rocket.Unturned.Chat;
using Rocket.Core;
using System.Collections.Generic;

namespace Uber_Vaults
{
    public class Uber_VaultS : RocketPlugin
    {
        public static Uber_VaultS Instance;
        public static DB_Management DBManager;
        public static List<VaultStorage> vaultList;

        protected override void Load()
        {
            Rocket.Core.Logging.Logger.Log("Loading Uber Vaults");

            Instance = this;
            Rocket.Core.Logging.Logger.Log("- Instance Set");

            DBManager = new DB_Management();
            Rocket.Core.Logging.Logger.Log("- Manager Set");

            vaultList = new List<VaultStorage>();
            Rocket.Core.Logging.Logger.Log("- Vault Storage Set");

            Rocket.Unturned.U.Events.OnPlayerConnected += DBManager.PlayerConnect;
            Rocket.Unturned.U.Events.OnPlayerDisconnected += DBManager.PlayerDisconnect;
            Rocket.Core.Logging.Logger.Log("- Hooks Set");

            Rocket.Core.Logging.Logger.Log("Uber Vaults Loaded\n----------------------------------");
        }

        protected override void Unload()
        {
            Rocket.Core.Logging.Logger.Log("Unload");
        }

        private void Do()
        {
            try
            {
                if (State != Rocket.API.PluginState.Loaded)
                    return;


            }
            catch (Exception ex)
            {
                Rocket.Core.Logging.Logger.LogException(ex);
            }
        }
    }
}
