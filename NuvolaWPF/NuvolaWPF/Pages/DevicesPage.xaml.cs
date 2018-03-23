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
using System.IO;
using System.Net.Sockets;

using ToastNotifications;
using ToastNotifications.Lifetime;
using ToastNotifications.Position;
using ToastNotifications.Messages;

namespace NuvolaWPF.Pages
{
    /// <summary>
    /// Interaction logic for DevicesPage.xaml
    /// </summary>
    /// 
    public partial class DevicesPage : UserControl
    {
        public Dictionary<string, List<string>> ips = new Dictionary<string,List<string>>();

        public DevicesPage()
        {
            InitializeComponent();
        }

        private void readIpsFromFile()
        {
            string line;
            int linesNumber;

            linesNumber = File.ReadLines("IP.txt").Count();
            System.IO.StreamReader file = new System.IO.StreamReader("IP.txt");

            while ((line = file.ReadLine()) != null)
            {
                if (!ips.ContainsKey(line.Split(' ')[0]))
                {
                    ListBoxItem newItem = new ListBoxItem();
                    newItem.HorizontalContentAlignment = System.Windows.HorizontalAlignment.Left;
                    newItem.FontSize = 26;
                    newItem.Content = line.Split(' ')[0];
                    ipsList.Items.Add(newItem);

                    List<string> values = new List<string>();
                    values.Add(line.Split(' ')[1]);
                    values.Add(line.Split(' ')[2]);
                    values.Add(line.Split(' ')[3]);
                    ips.Add(line.Split(' ')[0], values);
                }
            }

            file.Close();
        }

        private void PlaceholdersListBox_OnPreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            var item = ItemsControl.ContainerFromElement(sender as ListBox, e.OriginalSource as DependencyObject) as ListBoxItem;
            if (item != null)
            {
                ipLbl.Content = item.Content;
                usernameLbl.Content = ips[item.Content.ToString()][0];
                emailLbl.Content = ips[item.Content.ToString()][1];
                cloudSizeLbl.Content = ips[item.Content.ToString()][2];
            }
        }

        private void sendBtn_Click(object sender, RoutedEventArgs e)
        {
            string fileName = " ";

            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();
            // Display OpenFileDialog by calling ShowDialog method
            Nullable<bool> result = dlg.ShowDialog();

            // Get the selected file name and display in a TextBox
            if (result == true)
            {
                // Open document
                fileName = dlg.FileName;

                string data = "203" + fileName.Length.ToString().PadLeft(3, '0');
                data += SocketHandler.Encipher(fileName, "cipher");
                data += "0"; // No encryption   
                data += "01" + "1"; // One user

                if (ipLbl.Content != null)
                {
                    data += ipLbl.Content.ToString().Length.ToString().PadLeft(2, '0');
                    data += SocketHandler.Encipher(ipLbl.Content.ToString(), "cipher"); 
                }

                SocketHandler sh = new SocketHandler();
                try
                {
                    sh.sendData(data);
                }
                catch (SocketException ex)
                {
                    Notifier n = AsyncBlockingSocket.initNotifier();
                    n.ShowError(ex.ToString());
                }
                catch (Exception ex)
                {
                    Notifier n = AsyncBlockingSocket.initNotifier();
                    n.ShowError(ex.ToString());
                }
            }
        }

        private void UserControl_Loaded(object sender, RoutedEventArgs e)
        {
            readIpsFromFile();            
        }
    }
}
