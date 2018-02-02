using FirstFloor.ModernUI.Windows.Controls;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Net.Sockets;

namespace NuvolaWPF.Pages.Login
{
    /// <summary>
    /// Interaction logic for LoginPage.xaml
    /// </summary>
    public partial class LoginPage : ModernWindow
    {
        public LoginPage()
        {
            WindowStartupLocation = System.Windows.WindowStartupLocation.CenterScreen;
            InitializeComponent();
        }

        private void loginBtn_Click(object sender, RoutedEventArgs e)
        {
            string data = "200";
            data += usernameBox.Text.Length.ToString().PadLeft(2, '0');
            data += usernameBox.Text;
            data += passwordBox.Password.Length.ToString().PadLeft(2, '0');
            data += passwordBox.Password;

            SocketHandler sh = new SocketHandler();
            try
            {
                sh.sendData(data);
                string result = sh.recvData();
                if (result.Equals("1000"))
                {
                    LoadingPage app = new LoadingPage();
                    this.Close();
                    app.Show();
                }

                //LoadingPage app = new LoadingPage();
                //this.Close();
                //app.Show();
            }
            catch(SocketException)
            {
                // TODO: handle socket exception
            }
        }
    }
}
