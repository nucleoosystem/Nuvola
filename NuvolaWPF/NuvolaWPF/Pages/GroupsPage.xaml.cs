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

namespace NuvolaWPF.Pages
{
    /// <summary>
    /// Interaction logic for GroupsPage.xaml
    /// </summary>
    public partial class GroupsPage : UserControl
    {
        static bool isClosing = false;
        public Dictionary<string, List<string>> groups = new Dictionary<string, List<string>>();

        public GroupsPage()
        {
            InitializeComponent();
        }

        private void GetInfoAboutGroups()
        {
            try
            {
                SocketHandler sh = new SocketHandler();
                sh.sendData("208");
                sh.getMsgCode();
                int amount = int.Parse(sh.recvDataWithSize());

                for (int i = 0; i < amount; i++)
                {
                    string groupName = sh.recvDataWithSize();
                    if (!groups.ContainsKey(groupName))
                    {
                        ListBoxItem newItem = new ListBoxItem();
                        newItem.HorizontalContentAlignment = System.Windows.HorizontalAlignment.Left;
                        newItem.FontSize = 26;
                        newItem.Content = groupName;
                        groupsList.Items.Add(newItem);

                        List<string> values = new List<string>();
                        values.Add(sh.recvDataWithSize());

                        groups[newItem.Content.ToString()] = values;
                    }
                    else
                    {
                        sh.recvDataWithSize();
                    }
                }
            }
            catch(SocketException)
            {

            }
        }

        private void PlaceholdersListBox_OnPreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            var item = ItemsControl.ContainerFromElement(sender as ListBox, e.OriginalSource as DependencyObject) as ListBoxItem;
            if (item != null)
            {
                groupNameLbl.Content = item.Content;
                usersLbl.Content = groups[item.Content.ToString()][0];
                usersCombo.Items.Clear();

                try
                {
                    SocketHandler sh = new SocketHandler();
                    sh.sendData("210");

                    sh.getMsgCode();
                    int amount = int.Parse(sh.recvDataWithSize());
                    for(int i = 0; i < amount; i++)
                    {
                        string username = sh.recvDataWithSize();
                        usersCombo.Items.Add(username);
                    }
                }
                catch(SocketException)
                {
                    // TODO : Handle exception
                }
            }
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            if (!isClosing)
            {
                GetInfoAboutGroups();
                isClosing = true;
            }
            else
            {
                isClosing = false;
            }
        }

        private void uploadBtn_Click(object sender, RoutedEventArgs e)
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
            }

            string data = "209";
            data += groupNameLbl.Content.ToString().PadLeft(2, '0');
            data += groupNameLbl.Content.ToString();
            data += fileName.Length.ToString().PadLeft(2, '0');
            data += fileName;

            SocketHandler sh = new SocketHandler();
            try
            {
                sh.sendData(data);
            }
            catch(SocketException)
            {
                // TODO : Handle Socket Exception
            }
        }
    }
}
