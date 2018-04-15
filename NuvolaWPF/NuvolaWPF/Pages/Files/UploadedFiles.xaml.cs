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

namespace NuvolaWPF.Pages.Files
{
    /// <summary>
    /// Interaction logic for UploadedFiles.xaml
    /// </summary>
    public partial class UploadedFiles : UserControl
    {
        static bool isPageClosing = false;

        public UploadedFiles()
        {
            InitializeComponent();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            if (!isPageClosing)
            {
                dataGrid.Items.Clear();
                dataGrid.Items.Refresh();
                GetInfoAboutFiles();
                isPageClosing = true;
            }
            else
            {
                isPageClosing = false;
            }
        }

        private void GetInfoAboutFiles()
        {
            try
            {
                SocketHandler sh = new SocketHandler();
                sh.sendData("213");
                sh.getMsgCode();
                int amount = int.Parse(sh.recvDataWithSize());

                for(int i = 0; i < amount; i++)
                {
                    int length = sh.getDataLen(3);
                    string fileName = sh.recvDataWithGivenSize(length);
                    length = sh.getDataLen(2);
                    string fileType = sh.recvDataWithGivenSize(length);
                    length = sh.getDataLen(2);
                    string fileSize = sh.recvDataWithGivenSize(length);
                    length = sh.getDataLen(2);
                    string fileLoc = sh.recvDataWithGivenSize(length);

                    AddRowToGrid(fileName, fileType, fileLoc, fileSize);
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

        public void AddRowToGrid(string fileName, string fileType, string fileLoc, string fileSize)
        {
            File newFile = new File { name = fileName, type = fileType, user = fileLoc, size = Convert.ToInt32(fileSize) };
            dataGrid.Items.Add(newFile);
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            DataGrid dataGridVar = dataGrid;
            DataGridRow Row = (DataGridRow)dataGridVar.ItemContainerGenerator.ContainerFromIndex(dataGridVar.SelectedIndex);

            DataGridCell atUserCol = (DataGridCell)dataGridVar.Columns[2].GetCellContent(Row).Parent; 
            DataGridCell fileNameCol = (DataGridCell)dataGridVar.Columns[0].GetCellContent(Row).Parent;
            string atUser = ((TextBlock)atUserCol.Content).Text;
            string fileName = ((TextBlock)fileNameCol.Content).Text;

            string data = "214";
            data += fileName.Length.ToString().PadLeft(2, '0');
            data += SocketHandler.Encipher(fileName, "cipher");
            data += atUser.Length.ToString().PadLeft(2, '0');
            data += SocketHandler.Encipher(atUser, "cipher");

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

    public class File
    {
        public string name { get; set; }
        public string type { get; set; }
        public string user { get; set; }
        public int size { get; set; }
    }
}
