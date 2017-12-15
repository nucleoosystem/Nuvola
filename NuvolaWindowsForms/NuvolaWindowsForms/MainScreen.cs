using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Diagnostics; 
using System.IO; 


namespace NuvolaWindowsForms
{
    public partial class MainScreen : Form
    {
        [DllImport("user32.dll")]
        private static extern IntPtr SendMessage(IntPtr hWnd, int Msg, int wParam, [MarshalAs(UnmanagedType.LPWStr)] string lParam);

        public MainScreen()
        {
            InitializeComponent();
        }

        private void MainScreen_Load(object sender, EventArgs e)
        {
            /* Holds the Ips of the other users on the network */
            FileStream fs = File.Open("IP.txt", FileMode.OpenOrCreate);
            fs.Close();

            this.Text = "Nuvola";
            this.WindowState = FormWindowState.Maximized;
            this.MaximumSize = Screen.PrimaryScreen.WorkingArea.Size;

            SendMessage(searchBox.Handle, 0x1501, 1, "Search...");
            searchCombo.SelectedIndex = searchCombo.FindString("All");
        }

        private void sideExitBtn_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void sideFilesBtn_Click(object sender, EventArgs e)
        {
            mainPanel.Controls.Clear();

            Files myForm = new Files();
            myForm.TopLevel = false;
            myForm.AutoScroll = true;
            mainPanel.Controls.Add(myForm);
            myForm.Show();
        }

        private void sideDevicesBtn_Click(object sender, EventArgs e)
        {
            mainPanel.Controls.Clear();

            Devices myForm = new Devices();
            myForm.TopLevel = false;
            myForm.AutoScroll = true;
            mainPanel.Controls.Add(myForm);
            myForm.Show();
        }

        private void searchBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            if(e.KeyChar == (char)Keys.Enter)
            {
                e.Handled = true;
                mainPanel.Controls.Clear();

                Search myForm = new Search(searchBox.Text, searchCombo.SelectedItem.ToString());
                myForm.TopLevel = false;
                myForm.AutoScroll = true;
                mainPanel.Controls.Add(myForm);
                myForm.Show();
            }
        }

        private void sideUploadBtn_Click(object sender, EventArgs e)
        {
            mainPanel.Controls.Clear();

            Upload myForm = new Upload();
            myForm.TopLevel = false;
            myForm.AutoScroll = true;
            mainPanel.Controls.Add(myForm);
            myForm.Show();
        }
    }
}
