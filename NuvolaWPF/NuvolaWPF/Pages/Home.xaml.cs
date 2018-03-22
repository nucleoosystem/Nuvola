using System;
using System.Collections.Generic;
using System.IO;
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

namespace NuvolaWPF.Pages
{
    /// <summary>
    /// Interaction logic for Home.xaml
    /// </summary>
    public partial class Home : UserControl
    {
        public Home()
        {
            InitializeComponent();

            ReadLogs();
        }

        public void ReadLogs()
        {
            string line;
            int linesNumber;

            linesNumber = File.ReadLines("Logs.txt").Count();
            System.IO.StreamReader file = new System.IO.StreamReader("Logs.txt");

            int count = 1;
            while ((line = file.ReadLine()) != null && count < 6)
            {
                logBox.Text += count.ToString() + ". " + line + Environment.NewLine;  
                count++;
            }
            if (count > 1)
                logBox.Text = logBox.Text.Remove(logBox.Text.Length - 2);

            file.Close();
        }
    }
}
