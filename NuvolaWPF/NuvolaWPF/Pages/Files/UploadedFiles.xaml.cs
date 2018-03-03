﻿using System;
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
            catch (SocketException)
            {

            }
        }

        public void AddRowToGrid(string fileName, string fileType, string fileLoc, string fileSize)
        {
            File newFile = new File { name = fileName, type = fileType, user = fileLoc, size = fileSize };
            dataGrid.Items.Add(newFile);
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            DataGrid dataGridVar = dataGrid;
            DataGridRow Row = (DataGridRow)dataGridVar.ItemContainerGenerator.ContainerFromIndex(dataGridVar.SelectedIndex);
            DataGridCell RowAndColumn = (DataGridCell)dataGridVar.Columns[0].GetCellContent(Row).Parent;
            string fileName = ((TextBlock)RowAndColumn.Content).Text;

            MessageBox.Show(fileName, fileName);
        }
    }

    public class File
    {
        public string name { get; set; }
        public string type { get; set; }
        public string user { get; set; }
        public string size { get; set; }
    }
}
