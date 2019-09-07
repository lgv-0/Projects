using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

//Used to create contexts that appear when you right click a file on your computer

namespace ContextCreator
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void GetList(string filter)
        {
            TopEntries.Items.Clear();

            RegistryKey i = Registry.ClassesRoot.OpenSubKey("");
            string[] keyNames = i.GetSubKeyNames();
            i.Close();

            foreach (string cur in keyNames)
            {
                i = Registry.ClassesRoot.OpenSubKey(cur);

                try
                {
                    if (cur.ToLower().Contains(filter.ToLower()))
                        foreach (string fur in i.GetSubKeyNames())
                            if (fur.ToLower() == "shell")
                                TopEntries.Items.Add(cur);
                }
                catch (Exception)
                {

                }

                i.Close();
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            GetList("Desktop");
        }

        private void Search_Click(object sender, EventArgs e)
        {
            GetList(Filter.Text);
        }

        private string PathCurrent = "";

        private void UpdateCommandList()
        {
            CommandList.Items.Clear();
            SelectedVariables.Items.Clear();

            RegistryKey i = Registry.ClassesRoot.OpenSubKey(PathCurrent);

            try
            {
                foreach (string Command in i.GetSubKeyNames())
                    CommandList.Items.Add(Command);
            }
            catch (Exception)
            {

            }

            i.Close();
        }

        private void TopEntries_SelectedIndexChanged(object sender, EventArgs e)
        {
            CommandList.Items.Clear();
            SelectedVariables.Items.Clear();
            ClearVars();
            UpdateButton.Enabled = false;
            DeleteButton.Enabled = false;

            if (TopEntries.SelectedIndex == -1)
                return;

            PathCurrent = TopEntries.Items[TopEntries.SelectedIndex].ToString() + "\\Shell";

            UpdateCommandList();
        }

        private void CommandList_SelectedIndexChanged(object sender, EventArgs e)
        {
            SelectedVariables.Items.Clear();
            ClearVars();

            if (CommandList.SelectedIndex == -1)
                return;

            string selected = CommandList.Items[CommandList.SelectedIndex].ToString();

            RegistryKey i = Registry.ClassesRoot.OpenSubKey(PathCurrent + "\\" + selected);
            string[] values = i.GetValueNames();
            i.Close();
            foreach (string cur in values)
            {
                string use = cur;
                if (use == "")
                    use = "(Default)";
                i = Registry.ClassesRoot.OpenSubKey(PathCurrent + "\\" + selected);
                SelectedVariables.Items.Add(use + "=" + i.GetValue(cur).ToString());
                i.Close();
            }

            i = Registry.ClassesRoot.OpenSubKey(PathCurrent + "\\" + selected + "\\command");
            if (i != null)
            {
                string[] names = i.GetValueNames();
                bool delegateEx = names.Contains("DelegateExecute");

                if (delegateEx)
                    SelectedVariables.Items.Add("Command=" + i.GetValue("DelegateExecute").ToString());
                else
                    foreach (string name in names)
                        SelectedVariables.Items.Add("Command=" + i.GetValue(name).ToString());

                i.Close();
            }

            AutoVars();
        }

        private void ClearVars()
        {
            VarName.Text = "";
            VarCommand.Text = "";
            varIconLiteral.Text = "";
            VarIcon.Value = 0;
            VarPosition.SelectedIndex = 0;
        }

        private void AutoVars()
        {
            ClearVars();

            foreach (string CurrentItem in SelectedVariables.Items)
            {
                string use = CurrentItem;
                int Equals = use.IndexOf('=');
                switch (CurrentItem.Substring(0, Equals))
                {
                    case "Command":
                        VarCommand.Text = CurrentItem.Substring(Equals + 1, use.Length - Equals - 1);
                        break;
                    case "(Default)":
                        VarName.Text = CurrentItem.Substring(Equals + 1, use.Length - Equals - 1);
                        break;
                    case "Icon":
                        varIconLiteral.Text = CurrentItem.Substring(Equals + 1, use.Length - Equals - 1);
                        string getIconNumeric = CurrentItem.Substring(Equals + 1, use.Length - Equals - 1);
                        int x = 0;
                        if (getIconNumeric.Length > 34)
                            Int32.TryParse(getIconNumeric.Substring(34, getIconNumeric.Length - 34), out x);
                        VarIcon.Value = x;
                        break;
                    case "Position":
                        string swapper = CurrentItem.Substring(Equals + 1, use.Length - Equals - 1);
                        if (swapper == VarPosition.Items[1].ToString())
                            VarPosition.SelectedIndex = 1;
                        else
                            VarPosition.SelectedIndex = 2;
                        break;
                    default:
                        break;
                }
            }

            CreateButton.Enabled = true;

            if (VarName.Text.Contains(".dll"))
            {
                UpdateButton.Enabled = false;
                DeleteButton.Enabled = false;
            }
            else
            {
                UpdateButton.Enabled = true;
                DeleteButton.Enabled = true;
            }
        }

        private void SelectedVariables_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void UpdateButton_Click(object sender, EventArgs e)
        {
            string selected = CommandList.Items[CommandList.SelectedIndex].ToString();

            RegistryKey i = Registry.ClassesRoot.OpenSubKey(PathCurrent + "\\" + selected, true);
            if (VarName.Text != "")
                i.SetValue("", VarName.Text);
            if (VarPosition.SelectedIndex != 0)
                i.SetValue("Position", VarPosition.Text);
            if (VarIcon.Value != 0)
                i.SetValue("Icon", "%systemroot%\\system32\\SHELL32.dll," + VarIcon.Value.ToString());
            i.Close();

            i = Registry.ClassesRoot.OpenSubKey(PathCurrent + "\\" + selected + "\\command", true);
            i.SetValue("", VarCommand.Text);
            i.Close();
        }

        private void CreateButton_Click(object sender, EventArgs e)
        {
            string build = PathCurrent;
            RegistryKey i = Registry.ClassesRoot.OpenSubKey(build, true);
            i.CreateSubKey(VarName.Text);
            i.Close();
            build += "\\" + VarName.Text;
            i = Registry.ClassesRoot.OpenSubKey(build, true);
            i.SetValue("", VarName.Text);
            if (VarPosition.SelectedIndex != 0)
                i.SetValue("Position", VarPosition.Text);
            i.SetValue("Icon", "%systemroot%\\system32\\SHELL32.dll," + VarIcon.Value.ToString());
            i.CreateSubKey("command");
            i.Close();
            build += "\\command";
            i = Registry.ClassesRoot.OpenSubKey(build, true);
            i.SetValue("", VarCommand.Text);
            i.Close();
            UpdateCommandList();
        }

        private void DeleteButton_Click(object sender, EventArgs e)
        {
            RegistryKey i = Registry.ClassesRoot.OpenSubKey(PathCurrent, true);
            i.DeleteSubKeyTree(CommandList.Items[CommandList.SelectedIndex].ToString());
            CommandList.Items.Remove(CommandList.Items[CommandList.SelectedIndex].ToString());
            i.Close();
        }
    }
}
