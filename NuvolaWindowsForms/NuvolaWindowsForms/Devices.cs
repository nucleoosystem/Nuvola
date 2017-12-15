using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using System.Net.Sockets;
using System.Net;
using System.DirectoryServices;
using System.IO;


namespace NuvolaWindowsForms
{
    public partial class Devices : Form
    {
        static List<Tuple<string, string>> savedIps = new List<Tuple<string, string>>();
        static int counter = 0;

        public Devices()
        {
            this.FormBorderStyle = FormBorderStyle.None;
            InitializeComponent();
        }

        private void Devices_Load(object sender, EventArgs e)
        {
            counter = 0;

            savedIps.ForEach(tuple =>
            {
                dataGrid.Rows.Add(tuple.Item1, tuple.Item2);
                dataGrid.Rows[dataGrid.Rows.Count - 1].Cells["Message"].Value = "Contact";
            });
        }

        public string GetHostName(string ipAddress)
        {
            try
            {
                IPHostEntry entry = Dns.GetHostEntry(ipAddress);
                if (entry != null)  
                {
                    return entry.HostName;
                }
            }
            catch (SocketException)
            {
                return null;
            }

            return null;
        }

        private void addBtn_Click(object sender, EventArgs e)
        {
            string ip = ipBox.Text;
            string hostName = " ";

            if (ValidateIPv4(ip))
            {
                var task = Task.Run(() => GetHostName(ip));
                if (task.Wait(TimeSpan.FromSeconds(0.2)))
                    hostName = task.Result;

                dataGrid.Rows.Add(hostName, ip);
                dataGrid.Rows[dataGrid.Rows.Count - 1].Cells["Message"].Value = "Contact";

                savedIps.Add(new Tuple<string, string>(hostName, ip));
            }
        }

        private void dataGrid_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            var senderGrid = (DataGridView)sender;

            if (senderGrid.Columns[e.ColumnIndex] is DataGridViewButtonColumn &&
                e.RowIndex >= 0)
            {
                //TODO - Button Clicked - Execute Code Here
            }
        }

        private void refreshBtn_Click(object sender, EventArgs e)
        {
            string line;
            int linesNumber;

            linesNumber = File.ReadLines("IP.txt").Count();
            System.IO.StreamReader file = new System.IO.StreamReader("IP.txt");

            while ((line = file.ReadLine()) != null && counter < linesNumber)
            {
                string hostName = " ";

                var task = Task.Run(() => GetHostName(line));
                if (task.Wait(TimeSpan.FromSeconds(0.2)))
                    hostName = task.Result;

                dataGrid.Rows.Add(hostName, line);
                dataGrid.Rows[dataGrid.Rows.Count - 1].Cells["Message"].Value = "Contact";
                savedIps.Add(new Tuple<string, string>(hostName, line));
                counter += 1;
            }

            file.Close();
        }

        public static bool ValidateIPv4(string ipString)
        {
            if (ipString.Count(c => c == '.') != 3) return false;
            IPAddress address;
            return IPAddress.TryParse(ipString, out address);
        }
    }
}
