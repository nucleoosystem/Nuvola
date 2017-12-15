using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace NuvolaWindowsForms
{
    public partial class Login : Form
    {
        public Login()
        {
            InitializeComponent();
        }

        private void Login_Load(object sender, EventArgs e)
        {
            this.FormBorderStyle = FormBorderStyle.FixedDialog;
            this.Text = "Nuvola";
            this.MaximizeBox = false;

            passwordBox.PasswordChar = '*';
            signPasswordBox.PasswordChar = '*';
            errorLbl.Text = " ";
            signErrorLbl.Text = " ";
            signupGroup.Hide();
        }

        private void loginBtn_Click(object sender, EventArgs e)
        {
            /* For testing */
            MainScreen main1 = new MainScreen();
            this.Hide();
            //main1.Closed += (s, args) => this.Close();
            main1.ShowDialog();

            this.Close();
            /* For testing */

            SocketHandler sh = new SocketHandler();
            string data = "200" + usernameBox.Text.Length.ToString().PadLeft(2, '0') + usernameBox.Text +
                passwordBox.Text.Length.ToString().PadLeft(2, '0') + passwordBox.Text;

            
            try
            {
                sh.sendData(data);
                string result = sh.recvData();
                if (!result.Equals("1000"))
                {
                    errorLbl.Text = "Wrong username or password";
                }
                else
                {
                    MainScreen main = new MainScreen();
                    this.Hide();
                    main.Closed += (s, args) => this.Close();
                    main.Show();
                }
            }
            catch(Exception)
            {
                errorLbl.Text = "Can't connect to the server";
            }
        }

        private void exitBtn_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void sideSignupBtn_Click(object sender, EventArgs e)
        {
            loginGroup.Hide();
            signupGroup.Visible = true;
        }

        private void sideLoginBtn_Click(object sender, EventArgs e)
        {
            signupGroup.Hide();
            loginGroup.Visible = true;
        }

        private void signupBtn_Click(object sender, EventArgs e)
        {
            SocketHandler sh = new SocketHandler();
            string data = "202" + signUsernameBox.Text.Length.ToString().PadLeft(2, '0') + signUsernameBox.Text +
                signPasswordBox.Text.Length.ToString().PadLeft(2, '0') + signPasswordBox.Text +
                signEmailBox.Text.Length.ToString().PadLeft(2, '0') + signEmailBox.Text +
                signSizeBox.Text.Length.ToString().PadLeft(2, '0') + signSizeBox.Text;


            try
            {
                sh.sendData(data);
                string result = sh.recvData();
                
                switch(result)
                {
                    case "1010":
                        signErrorLbl.Text = "Success";
                        break;
                    case "1011":
                        signErrorLbl.Text = "Pass illegal";
                        break;
                    case "1012":
                        signErrorLbl.Text = "Username already exists";
                        break;
                    case "1013":
                        signErrorLbl.Text = "Username is illegal";
                        break;
                    case "1015":
                        signErrorLbl.Text = "Not enough room on the computer";
                        break;
                }
                if (result.Equals("1000"))
                {
                    signErrorLbl.Text = "Success";
                }
            }
            catch (Exception)
            {
                signErrorLbl.Text = "Can't connect to the server";
            }
        }
    }
}
