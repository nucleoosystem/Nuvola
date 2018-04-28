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
        public static List<Tuple<string, string>> list = new List<Tuple<string, string>>(); // Holds the messages

        public MessagesPage()
        {
            InitializeComponent();
        }

        private void UserControl_Loaded(object sender, RoutedEventArgs e)
        {
            bool wasFound = false;
            foreach(var msg in list)
            {
                foreach(ListBoxItem item in titleList.Items)
                {
                    string title = item.Content.ToString();
                    int index = FindMyStringInList(titleList, msg.Item1);
                    if (index != -1)
                    {
                        string content = list.ElementAt(index).Item2;
                        if (msg.Item1.Equals(title) == true && msg.Item2.Equals(content) == true)
                        {
                            wasFound = true;
                            break;
                        }
                    }
                }

                if(!wasFound)
                {
                    ListBoxItem newItem = new ListBoxItem();
                    newItem.HorizontalContentAlignment = System.Windows.HorizontalAlignment.Left;
                    newItem.FontSize = 26;
                    newItem.FontWeight = FontWeights.Bold;
                    newItem.Content = msg.Item1;
                    titleList.Items.Add(newItem);
                }
                wasFound = false;
            }
        }

        private void titleList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            ((ListBoxItem)titleList.SelectedItem).FontWeight = FontWeights.Regular;
            msgContent.Text = list.ElementAt(titleList.SelectedIndex).Item2;
        }

        int FindMyStringInList(ListBox lb, string searchString)
        {
            for (int i = 0; i < lb.Items.Count; i++)
            {
                string lbString = lb.Items[i].ToString();
                if (lbString.Contains(searchString))
                    return i;
            }
            return -1;
        }
    }
}