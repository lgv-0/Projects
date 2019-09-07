//Online game called unturned allows you to host your own game servers.
//Online boards collect these and allow voting to show servers as "top" or best.
//This program exploits that system by simply calling the backend php and resetting
//the cookies given that are supposed to keep you from rapidly setting your own server higher.

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Net;
using System.Text;
using System.Windows.Forms;

namespace AutoBump_Forms
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            current = DateTime.Now;
        }

        public static DateTime? current;
        private static CookieContainer cooks = new CookieContainer();
        private static WebClientEx client = new WebClientEx(cooks);
        WebHeaderCollection Backup = client.Headers;
        public string Token = "";

        private void MainLoop_Tick(object sender, EventArgs e)
        {
            if ((DateTime.Now - current.Value).TotalSeconds > (double)Interval.Value)
            {
                client = new WebClientEx(cooks);
                current = DateTime.Now;
                LogBox.Text = "";
                LogBox.Text = "[" + DateTime.Now.Hour + ":" + DateTime.Now.Minute + ":" + DateTime.Now.Second + "]" + " Trying new bump";
                if (!TryFirstConnect())
                    return;
                System.Threading.Thread.Sleep(300);
                VerifyCookies(out bool FoundRememberToken);
                if (!FoundRememberToken)
                {
                    TryLogin();
                    VerifyCookies(out FoundRememberToken);
                }
                if (FoundRememberToken)
                    TryBump();
                else
                    LogBox.Text += "\nCould not login to UnturnedSL.com";
                client.Dispose();
                GC.Collect();
            }
        }

        //Handles cookie list, while also handling the bump-expiration & remember-ids
        private void VerifyCookies(out bool fMemberToken)
        {
            fMemberToken = false;
            CookieCollection Cookies = cooks.GetCookies(new Uri("https://unturnedsl.com"));
            for (int i = 0; i < Cookies.Count; i++)
            {
                string Msg = Cookies[i].Name;
                if (Msg.Contains("remember"))
                    fMemberToken = true;
                if (Msg.Contains("bump"))
                {
                    Cookies[i].Expires = DateTime.Now.AddDays(-1);
                    Msg += " *Set to be expired [IGNORE]";
                }
            }
        }

        private bool TryFirstConnect()
        {
            bool CouldConnect = false;

            Stream str = client.OpenRead("https://unturnedsl.com/auth/login");
            StreamReader reader = new StreamReader(str);
            string ReturnValue = reader.ReadToEnd();
            reader.Close();
            str.Close();

            string ToFind = "<input type=\"hidden\" name=\"_token\" value=\"";
            int start = ReturnValue.IndexOf(ToFind) + 42;
            ToFind = "\">";
            int end = ReturnValue.IndexOf(ToFind, start);
            LogBox.Text += "\nReading For Access Token @ " + start.ToString() + "-" + end.ToString();
            if (Token.Length < 3)
                Token = ReturnValue.Substring(start, end - start);
            LogBox.Text += "\nToken: " + Token;
            if (Token.Length > 3)
                CouldConnect = true;

            return CouldConnect;
        }

        private void TryLogin()
        {
            string PostData = "";
            string HtmlResult = "";
            LogBox.Text += "\nNo Login - Attempting to login";
            PostData = "_token=" + Token + "&email=" + Username.Text + "&password=" + Password.Text + "&remember=1";
            HtmlResult = SendPost("https://unturnedsl.com/auth/login", PostData);
        }

        private void TryBump()
        {
            string PostData = "";
            string HtmlResult = "";            LogBox.Text += "\nFound Login - Attempting to bump server ID:" + ServerID.Value.ToString();
            PostData = "_token=" + Token;
            HtmlResult = SendPost("https://unturnedsl.com/dashboard/listed/bump/" + ServerID.Value.ToString(), PostData);
            if (HtmlResult.Contains("Server bumped,"))
                LogBox.Text += "\nBumped server @ " + "[" + DateTime.Now.Hour + ":" + DateTime.Now.Minute + ":" + DateTime.Now.Second + "]";
            else
                LogBox.Text += "\nCould not bump server :(";
        }

        private string SendPost(string URL, string DATA)
        {
            string Result;
            client.Headers[HttpRequestHeader.ContentType] = "application/x-www-form-urlencoded";
            Result = client.UploadString(URL, DATA);
            client.Headers = Backup;
            return Result;
        }

        private void StartButton_Click(object sender, EventArgs e)
        {
            MainLoop.Start();
        }

        private void StopButton_Click(object sender, EventArgs e)
        {
            MainLoop.Stop();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            MessageBox.Show(null, "-", "Information | -", MessageBoxButtons.OK, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1, MessageBoxOptions.ServiceNotification);
        }
    }
}
