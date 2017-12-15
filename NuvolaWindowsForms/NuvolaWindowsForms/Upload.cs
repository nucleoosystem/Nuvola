using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Net.Sockets;
using System.Net;

namespace NuvolaWindowsForms
{
    public partial class Upload : Form
    {
        public Upload()
        {
            this.FormBorderStyle = FormBorderStyle.None;
            InitializeComponent();
            errorLbl.Text = " ";
        }

        private void chooseFileBtn_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                filePathBox.Text = openFileDialog1.FileName;
            }  
        }

        private void uploadBtn_Click(object sender, EventArgs e)
        {
            string fileName = filePathBox.Text;
            int encrypt = 0;

            if(optionsList.CheckedItems.Count > 0)
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
            catch(SocketException)
            {
                errorLbl.Text = "Can't connect to the server";
            }
        }
    }
}
