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
    /// Interaction logic for UploadPage.xaml
    /// </summary>
    public partial class UploadPage : UserControl
    {
        public UploadPage()
        {
            InitializeComponent();
        }

        private void uploadBtn_Click(object sender, RoutedEventArgs e)
        {
            // Create OpenFileDialog
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();

            // Display OpenFileDialog by calling ShowDialog method
            Nullable<bool> result = dlg.ShowDialog();

            // Get the selected file name and display in a TextBox
            if (result == true)
            {
                // Open document
                string filename = dlg.FileName;
                fileNameLbl.Content = filename;
            }
        }

        private void sendBtn_Click(object sender, RoutedEventArgs e)
        {
            string fileName = fileNameLbl.Content.ToString();
            int encrypt = 0;

            if(isEncrypt.IsChecked == true)
            {
                encrypt = 1;
            }

            string data = "203";
            data += fileName.Length.ToString().PadLeft(2, '0');
            data += fileName;
            data += encrypt.ToString();

            SocketHandler sh = new SocketHandler();
            try
            {
                sh.sendData(data);
            }
            catch (SocketException)
            {
                errorLbl.Content = "Can't connect to the server";
            }
        }
    }
}
