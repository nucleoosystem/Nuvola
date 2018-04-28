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
                        int length = sh.getDataLen(2);
                        if (length != 0)
                        {
                            string data = sh.recvDataWithGivenSize(length);
                            values.Add(data);
                        }
                        else
                        {
                            values.Add("");
                        }

                        groups[newItem.Content.ToString()] = values;
                    }
                    else
                    {
                        List<string> values = new List<string>();
                        int length = sh.getDataLen(2);
                        if (length != 0)
                        {
                            string data = sh.recvDataWithGivenSize(length);
                            values.Add(data);
                        }
                        else
                        {
                            values.Add("");
                        }

                        groups[groupName] = values;
                    }
                }
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

        private void PlaceholdersListBox_OnPreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            var item = ItemsControl.ContainerFromElement(sender as ListBox, e.OriginalSource as DependencyObject) as ListBoxItem;
            if (item != null)
            {
                usersCombo.Items.Clear();
                usersList.Items.Clear();
                
                groupNameLbl.Content = item.Content;

                string[] users = groups[item.Content.ToString()][0].Split('/');
                for (int i = 0; i < users.Length; i++)
                {
                    if(users[i] != "")
                        usersList.Items.Add(users[i]);
                }
               
                try
                {
                    SocketHandler sh = new SocketHandler();
                    sh.sendData("210");

                    sh.getMsgCode();
                    int amount = int.Parse(sh.recvDataWithSize());
                    for(int i = 0; i < amount; i++)
                    {
                        string username = sh.recvDataWithSize();
                        if(!usersList.Items.Contains(username))
                            usersCombo.Items.Add(username);
                    }
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

                string data = "209";
                if (groupNameLbl.Content == null)
                    return;
                data += groupNameLbl.Content.ToString().Length.ToString().PadLeft(2, '0');
                data += SocketHandler.Encipher(groupNameLbl.Content.ToString(), "cipher");
                data += fileName.Length.ToString().PadLeft(3, '0');
                data += SocketHandler.Encipher(fileName, "cipher");

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

        private void addBtn_Click(object sender, RoutedEventArgs e)
        {
            if(usersCombo.SelectedValue != null)
            {
                string username = usersCombo.SelectedValue.ToString();
                string groupName = groupNameLbl.Content.ToString();

                string data = "207";
                data += username.Length.ToString().PadLeft(2, '0');
                data += SocketHandler.Encipher(username, "cipher");
                data += groupName.Length.ToString().PadLeft(2, '0');
                data += SocketHandler.Encipher(groupName, "cipher");

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

                usersList.Items.Add(username);
                usersCombo.Items.Remove(username);
            }
            else
            {
                Notifier n = AsyncBlockingSocket.initNotifier();
                n.ShowError("Please pick a valid user.");
            }
        }

        private void deleteBtn_Click(object sender, RoutedEventArgs e)
        {
            Button btn = sender as Button;
            string username = btn.DataContext as string;
            string groupName = groupNameLbl.Content.ToString();

            string data = "212";
            data += username.Length.ToString().PadLeft(2, '0');
            data += SocketHandler.Encipher(username, "cipher"); ;
            data += groupName.Length.ToString().PadLeft(2, '0');
            data += SocketHandler.Encipher(groupName, "cipher"); ;

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

            GetInfoAboutGroups();
            updateView(groupName);
        }

        private void updateView(string groupName)
        {
            usersCombo.Items.Clear();
            usersList.Items.Clear();

            groupNameLbl.Content = groupName;

            string[] users = groups[groupName][0].Split('/');
            for (int i = 0; i < users.Length; i++)
            {
                if (users[i] != "")
                    usersList.Items.Add(users[i]);
            }

            try
            {
                SocketHandler sh = new SocketHandler();
                sh.sendData("210");

                sh.getMsgCode();
                int amount = int.Parse(sh.recvDataWithSize());
                for (int i = 0; i < amount; i++)
                {
                    string username = sh.recvDataWithSize();
                    usersCombo.Items.Add(username);
                }
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

        private void leaveBtn_Click(object sender, RoutedEventArgs e)
        {
            if(groupNameLbl.Content != null)
            {
                string groupName = groupNameLbl.Content.ToString();

                string data = "215";
                data += groupName.Length.ToString().PadLeft(2, '0');
                data += SocketHandler.Encipher(groupName, "cipher");

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

                GetInfoAboutGroups();
                updateView(groupName);

                groupsList.Items.RemoveAt(groupsList.SelectedIndex);
                groupsList.SelectedItem = null;

                usersCombo.Items.Clear();
                usersList.Items.Clear();

                groupNameLbl.Content = null;
            }
        }
    }
}
