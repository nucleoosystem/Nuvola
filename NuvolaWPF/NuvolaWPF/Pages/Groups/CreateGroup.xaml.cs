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

using ToastNotifications;
using ToastNotifications.Lifetime;
using ToastNotifications.Position;
using ToastNotifications.Messages;

namespace NuvolaWPF.Pages.Groups
{
    /// <summary>
    /// Interaction logic for CreateGroup.xaml
    /// </summary>
    public partial class CreateGroup : UserControl
    {
        public CreateGroup()
        {
            InitializeComponent();
        }

        private void createBtn_Click(object sender, RoutedEventArgs e)
        {
            string name = groupNameBox.Text;
            string password = "null";

            string data = "206";
            data += name.Length.ToString().PadLeft(2, '0');
            data += SocketHandler.Encipher(name, "cipher");
            data += password.Length.ToString().PadLeft(2, '0');
            data += SocketHandler.Encipher(password, "cipher");

            SocketHandler sh = new SocketHandler();
            try
            {
                sh.sendData(data);
            }
            catch(SocketException)
            {
                // Handle Exception
            }
        }
    }
}
