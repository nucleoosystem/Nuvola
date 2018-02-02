using FirstFloor.ModernUI.Windows.Controls;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
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
using System.Windows.Threading;
using System.Net.Sockets;

namespace NuvolaWPF.Pages
{
    /// <summary>
    /// Interaction logic for LoadingPage.xaml
    /// </summary>
    public partial class LoadingPage : ModernWindow
    {
        public LoadingPage()
        {
            WindowStartupLocation = System.Windows.WindowStartupLocation.CenterScreen;
            InitializeComponent();

            WaitFunction();     
        }

        private void OpenNewWindow()
        {
            var app = new MainWindow();
            this.Close();
            app.Show();
        }

        private void WaitForMsg()
        {
            SocketHandler sh = new SocketHandler();
            sh.sendData("211");
            try
            {
                while (true)
                {
                    string data = sh.getMsgCode().ToString();
                    if (data.Equals("105"))
                        break;
                }
            }
            catch(SocketException e)
            {
                MessageBox.Show(e.Message);
            }
        }

        async void WaitFunction()
        {
            await Task.Run(() => WaitForMsg());
            OpenNewWindow();
        }
    }
}
