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

namespace NuvolaWPF.Pages
{
    /// <summary>
    /// Interaction logic for DevicesPage.xaml
    /// </summary>
    /// 
    public partial class MessagesPage : UserControl
    {
        public static Dictionary<string, string> msgs = new Dictionary<string, string>();

        public MessagesPage()
        {
            InitializeComponent();
        }

        private void PlaceholdersListBox_OnPreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            var item = ItemsControl.ContainerFromElement(sender as ListBox, e.OriginalSource as DependencyObject) as ListBoxItem;
            if (item != null)
            {
                msgContent.Text = msgs[item.Content.ToString()];
            }
        }

        private void UserControl_Loaded(object sender, RoutedEventArgs e)
        {
            titleList.Items.Clear();
            titleList.Items.Refresh();

            foreach(var msg in msgs)
            {
                ListBoxItem newItem = new ListBoxItem();
                newItem.HorizontalContentAlignment = System.Windows.HorizontalAlignment.Left;
                newItem.FontSize = 26;
                newItem.Content = msg.Key;
                titleList.Items.Add(newItem);
            }
        }

        private void yesBtn_Click(object sender, RoutedEventArgs e)
        {
            AsyncBlockingSocket.sendData("4001");   
        }

        private void noBtn_Click(object sender, RoutedEventArgs e)
        {
            AsyncBlockingSocket.sendData("4000"); 
        }
    }
}