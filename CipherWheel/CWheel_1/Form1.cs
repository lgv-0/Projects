using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace CWheel_
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Encrypt_Click(object sender, EventArgs e)
        {
            //Ready the strings!
            string Original = textBox1.Text, Build = "";

            //Ready the randoms!
            Random nextGen = new Random();

            //Loop every character in the original string
            foreach (char character in Original.ToCharArray())
            {
                //Match to a string cause honestly gonna be using strings 24/7
                string Current = character.ToString();

                bool FoundMatch = false;

                //If current is not a space (temporary)
                if (Current != " ")
                {
                    //Loop every character in table till we match our string
                    for (int i = 0; i < tableLayoutPanel1.RowCount; i++)
                    {
                        //If the character in our string equals the letter
                        if (Current.ToUpper() == tableLayoutPanel1.GetControlFromPosition(0, i).Text)
                        {
                            //Get a random value to either use the 1st or 2nd match character then replace
                            if (nextGen.Next(0, 50) > 25)
                                Current = tableLayoutPanel1.GetControlFromPosition(1, i).Text;
                            else
                                Current = tableLayoutPanel1.GetControlFromPosition(2, i).Text;

                            //Notify match and break the loop
                            FoundMatch = true;
                            break;
                        }
                    }

                    //No suitable match was found, erase the string
                    if (!FoundMatch)
                        Current = "";
                }

                //Add current to string rebuild
                Build += Current;
            }

            //Place encrypted string in box
            richTextBox1.Text = Build;
        }

        private void Decrypt_Click(object sender, EventArgs e)
        {
            //Ready the strings!
            string Encrypted = richTextBox1.Text, Build = "";

            foreach (char character in Encrypted.ToCharArray())
            {
                //Match to string
                string Current = character.ToString();

                //If current is not a space (temporary)
                if (Current != " ")
                {
                    Current = Current.ToUpper();
                    //Loop every row
                    for (int i = 0; i < tableLayoutPanel1.RowCount; i++)
                    {
                        //If the character in our string equals one of the matches
                        if (Current == tableLayoutPanel1.GetControlFromPosition(1, i).Text || 
                            Current == tableLayoutPanel1.GetControlFromPosition(2, i).Text)
                        {
                            //Set our match letter as replacement
                            Current = tableLayoutPanel1.GetControlFromPosition(0, i).Text;

                            break;
                        }
                    }
                }

                //Add current to string rebuild
                Build += Current;
            }

            textBox1.Text = Build;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            InitDefaultCharacters();
        }

        //Hold original setup so we can use the swapset
        public char[] OriginalColumnA = new char[26], OriginalColumnB = new char[26];
        public void SetupHolders()
        {
            OriginalColumnA[0] = A1.Text.ToCharArray()[0]; OriginalColumnA[1] = B1.Text.ToCharArray()[0]; OriginalColumnA[2] = C1.Text.ToCharArray()[0]; OriginalColumnA[3] = D1.Text.ToCharArray()[0]; OriginalColumnA[4] = E1.Text.ToCharArray()[0]; OriginalColumnA[5] = F1.Text.ToCharArray()[0]; OriginalColumnA[6] = G1.Text.ToCharArray()[0]; OriginalColumnA[7] = H1.Text.ToCharArray()[0]; OriginalColumnA[8] = I1.Text.ToCharArray()[0]; OriginalColumnA[9] = J1.Text.ToCharArray()[0]; OriginalColumnA[10] = K1.Text.ToCharArray()[0]; OriginalColumnA[11] = L1.Text.ToCharArray()[0]; OriginalColumnA[12] = M1.Text.ToCharArray()[0]; OriginalColumnA[13] = N1.Text.ToCharArray()[0]; OriginalColumnA[14] = O1.Text.ToCharArray()[0]; OriginalColumnA[15] = P1.Text.ToCharArray()[0]; OriginalColumnA[16] = Q1.Text.ToCharArray()[0]; OriginalColumnA[17] = R1.Text.ToCharArray()[0]; OriginalColumnA[18] = S1.Text.ToCharArray()[0]; OriginalColumnA[19] = T1.Text.ToCharArray()[0]; OriginalColumnA[20] = U1.Text.ToCharArray()[0]; OriginalColumnA[21] = V1.Text.ToCharArray()[0]; OriginalColumnA[22] = W1.Text.ToCharArray()[0]; OriginalColumnA[23] = X1.Text.ToCharArray()[0]; OriginalColumnA[24] = Y1.Text.ToCharArray()[0]; OriginalColumnA[25] = Z1.Text.ToCharArray()[0];
            OriginalColumnB[0] = A2.Text.ToCharArray()[0]; OriginalColumnB[1] = B2.Text.ToCharArray()[0]; OriginalColumnB[2] = C2.Text.ToCharArray()[0]; OriginalColumnB[3] = D2.Text.ToCharArray()[0]; OriginalColumnB[4] = E2.Text.ToCharArray()[0]; OriginalColumnB[5] = F2.Text.ToCharArray()[0]; OriginalColumnB[6] = G2.Text.ToCharArray()[0]; OriginalColumnB[7] = H2.Text.ToCharArray()[0]; OriginalColumnB[8] = I2.Text.ToCharArray()[0]; OriginalColumnB[9] = J2.Text.ToCharArray()[0]; OriginalColumnB[10] = K2.Text.ToCharArray()[0]; OriginalColumnB[11] = L2.Text.ToCharArray()[0]; OriginalColumnB[12] = M2.Text.ToCharArray()[0]; OriginalColumnB[13] = N2.Text.ToCharArray()[0]; OriginalColumnB[14] = O2.Text.ToCharArray()[0]; OriginalColumnB[15] = P2.Text.ToCharArray()[0]; OriginalColumnB[16] = Q2.Text.ToCharArray()[0]; OriginalColumnB[17] = R2.Text.ToCharArray()[0]; OriginalColumnB[18] = S2.Text.ToCharArray()[0]; OriginalColumnB[19] = T2.Text.ToCharArray()[0]; OriginalColumnB[20] = U2.Text.ToCharArray()[0]; OriginalColumnB[21] = V2.Text.ToCharArray()[0]; OriginalColumnB[22] = W2.Text.ToCharArray()[0]; OriginalColumnB[23] = X2.Text.ToCharArray()[0]; OriginalColumnB[24] = Y2.Text.ToCharArray()[0]; OriginalColumnB[25] = Z2.Text.ToCharArray()[0];
        }
        //Scroll based on swapset
        public void ScrollCharacters()
        {
            //If holders not setup yet do it
            if (OriginalColumnA[0] == default(char))
                SetupHolders();

            //Create our new bases
            char[] NewPosColumnA = new char[26];
            char[] NewPosColumnB = new char[26];

            //Iterate alphabet for first column
            for (int i = 0; i < 26; i++)
            {
                //So we need to calculate by adding/subtracting the position set as the "Swapset"
                //AKA position 0 is a dash for the letter A, so if the swapset is 3, add 3
                //to the position currently and place it there, making it the sub for D

                //Grab new offset
                int Position = i + (int)Offset.Value;

                //Offset is larger than max, need to wrap around
                if (Position > 25)
                    Position = Position - 26;

                //Set new values to the new base
                NewPosColumnA[Position] = OriginalColumnA[i];
            }

            //Iterate alphabet for second column
            for (int i = 0; i < 26; i++)
            {
                //Grab new offset
                int Position = i - (int)Offset.Value;

                //Offset is larger than max, need to wrap around
                if (Position < 0)
                    Position = Position + 26;

                //Set new values to the new base
                NewPosColumnB[Position] = OriginalColumnB[i];
            }

            A1.Text = NewPosColumnA[0].ToString(); B1.Text = NewPosColumnA[1].ToString(); C1.Text = NewPosColumnA[2].ToString(); D1.Text = NewPosColumnA[3].ToString(); E1.Text = NewPosColumnA[4].ToString(); F1.Text = NewPosColumnA[5].ToString(); G1.Text = NewPosColumnA[6].ToString(); H1.Text = NewPosColumnA[7].ToString(); I1.Text = NewPosColumnA[8].ToString(); J1.Text = NewPosColumnA[9].ToString(); K1.Text = NewPosColumnA[10].ToString(); L1.Text = NewPosColumnA[11].ToString(); M1.Text = NewPosColumnA[12].ToString(); N1.Text = NewPosColumnA[13].ToString(); O1.Text = NewPosColumnA[14].ToString(); P1.Text = NewPosColumnA[15].ToString(); Q1.Text = NewPosColumnA[16].ToString(); R1.Text = NewPosColumnA[17].ToString(); S1.Text = NewPosColumnA[18].ToString(); T1.Text = NewPosColumnA[19].ToString(); U1.Text = NewPosColumnA[20].ToString(); V1.Text = NewPosColumnA[21].ToString(); W1.Text = NewPosColumnA[22].ToString(); X1.Text = NewPosColumnA[23].ToString(); Y1.Text = NewPosColumnA[24].ToString(); Z1.Text = NewPosColumnA[25].ToString();
            A2.Text = NewPosColumnB[0].ToString(); B2.Text = NewPosColumnB[1].ToString(); C2.Text = NewPosColumnB[2].ToString(); D2.Text = NewPosColumnB[3].ToString(); E2.Text = NewPosColumnB[4].ToString(); F2.Text = NewPosColumnB[5].ToString(); G2.Text = NewPosColumnB[6].ToString(); H2.Text = NewPosColumnB[7].ToString(); I2.Text = NewPosColumnB[8].ToString(); J2.Text = NewPosColumnB[9].ToString(); K2.Text = NewPosColumnB[10].ToString(); L2.Text = NewPosColumnB[11].ToString(); M2.Text = NewPosColumnB[12].ToString(); N2.Text = NewPosColumnB[13].ToString(); O2.Text = NewPosColumnB[14].ToString(); P2.Text = NewPosColumnB[15].ToString(); Q2.Text = NewPosColumnB[16].ToString(); R2.Text = NewPosColumnB[17].ToString(); S2.Text = NewPosColumnB[18].ToString(); T2.Text = NewPosColumnB[19].ToString(); U2.Text = NewPosColumnB[20].ToString(); V2.Text = NewPosColumnB[21].ToString(); W2.Text = NewPosColumnB[22].ToString(); X2.Text = NewPosColumnB[23].ToString(); Y2.Text = NewPosColumnB[24].ToString(); Z2.Text = NewPosColumnB[25].ToString();
        }

        public void InitDefaultCharacters()
        {
            A1.Text = "-";
            A2.Text = "1";

            B1.Text = "\"";
            B2.Text = "9";

            C1.Text = "4";
            C2.Text = "5";

            D1.Text = "3";
            D2.Text = "6";

            E1.Text = "$";
            E2.Text = ".";

            F1.Text = "^";
            F2.Text = ",";

            G1.Text = ")";
            G2.Text = "=";

            H1.Text = "#";
            H2.Text = "!";

            I1.Text = "@";
            I2.Text = "%";

            J1.Text = "{";
            J2.Text = "+";

            K1.Text = "(";
            K2.Text = "/";

            L1.Text = "*";
            L2.Text = ":";

            M1.Text = ";";
            M2.Text = "_";

            N1.Text = "0";
            N2.Text = "7";

            O1.Text = "2";
            O2.Text = "D";

            P1.Text = "?";
            P2.Text = "<";

            Q1.Text = ">";
            Q2.Text = "]";

            R1.Text = "}";
            R2.Text = "~";

            S1.Text = "8";
            S2.Text = "[";

            T1.Text = "`";
            T2.Text = "P";

            U1.Text = "•";
            U2.Text = "™";

            V1.Text = "N";
            V2.Text = "¥";

            W1.Text = "H";
            W2.Text = "R";

            X1.Text = "©";
            X2.Text = "‰";

            Y1.Text = "½";
            Y2.Text = "§";

            Z1.Text = "¿";
            Z2.Text = "¢";
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            LetterCounter_Input.Text = "Letter Count: " + textBox1.Text.Length.ToString();
        }

        private void Offset_ValueChanged(object sender, EventArgs e)
        {
            ScrollCharacters();
        }
    }
}
