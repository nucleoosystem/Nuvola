using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
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

namespace NuvolaWPF.Pages.Settings
{
    /// <summary>
    /// Interaction logic for VHDSettings.xaml
    /// </summary>
    public partial class VHDSettings : UserControl
    {
        public VHDSettings()
        {
            InitializeComponent();
        }

        private void deleteBtn_Click(object sender, RoutedEventArgs e)
        {
            SocketHandler sh = new SocketHandler();
            try
            {
                string data = "216";
                sh.sendData(data);
            }
            catch(SocketException ex)
            {
                MessageBox.Show(ex.Data.ToString());
            }
        }

        private void saveBtn_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}
