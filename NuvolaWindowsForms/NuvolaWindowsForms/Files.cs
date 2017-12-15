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
using System.Reflection;

namespace NuvolaWindowsForms
{
    public partial class Files : Form
    {
        public static List<Panel> panels = new List<Panel>();
        public const string folderPath = @"C:\Users\ShakedNissanov\Documents\SchoolWork\MyWork\CloudProject\NuvolaWindowsForms\Files";
        private const int panelWidth = 200;
        private const int panelHeight = 200;
        public Files()
        {
            this.WindowState = FormWindowState.Maximized;
            this.FormBorderStyle = FormBorderStyle.None;
            panels.Clear();
            InitializeComponent();
        }

        private void Files_Load(object sender, EventArgs e)
        {
            allFilesLbl.Location = new Point(latestLbl.Location.X, latestLbl.Location.Y + panelHeight + 100);
            
            var sortedFiles = new DirectoryInfo(folderPath).GetFiles()
                                                  .OrderBy(f => f.LastWriteTime)
                                                  .ToList(); // Get all files in a directory

            var dirs = Directory.GetDirectories(folderPath); // Get all sub-folders in a directory
            for(int i = 0; i < dirs.Length; i++)
                dirs[i] = dirs[i].Replace(folderPath + "\\", "");

            for(int i = 0; i < sortedFiles.Count(); i++)
            {
                createNewPanel(sortedFiles[i].Name, getImageByType(sortedFiles[i].Name));
            }
            for(int i = 0; i < dirs.Length; i++)
            {
                createNewPanel(dirs[i], Properties.Resources.folder);
            }
        }


        public static Image getImageByType(string name)
        {
            string[] parts = name.Split('.');
            string type = parts.Last();
            
            switch(type)
            {
                case "docx":
                    return Properties.Resources.Microsoft_Word_logo;
                case "txt":
                    return Properties.Resources.txtFile;
                default:
                    return Properties.Resources.unknown;
            }
        }

        public void createNewPanel(string fileName, Image fileImage)
        {
            Panel panel = new Panel();
            PictureBox picture = new PictureBox();
            Label name = new Label();

            picture.Size = new Size(panelWidth, panelHeight - 50);
            picture.BackgroundImageLayout = ImageLayout.Stretch;
            picture.SizeMode = PictureBoxSizeMode.StretchImage;
            picture.Image = fileImage;
            name.Font = new Font("Century Gothic", 16);
            name.Text = fileName;
            name.Size = new Size(panelWidth, 25);


            if (panels.Count == 0)
            {
                panel.Location = new Point(latestLbl.Location.X, latestLbl.Location.Y + 40);
            }
            else if(panels.Count < 3)
            {
                panel.Location = new Point(panels.Last().Location.X + panelWidth, panels.Last().Location.Y);
            }
            else if(panels.Count == 3)
            {
                panel.Location = new Point(allFilesLbl.Location.X, allFilesLbl.Location.Y + 40);
            }
            else if(panels.Count % 3 == 0)
            {
                panel.Location = new Point(panels.ElementAt(panels.Count - 3).Location.X, panels.Last().Location.Y + panelHeight);
            }
            else if(panels.Count > 3)
            {
                panel.Location = new Point(panels.Last().Location.X + panelWidth, panels.Last().Location.Y);
            }

            panel.Size = new Size(panelWidth, panelHeight);
            panel.BorderStyle = BorderStyle.FixedSingle;
            picture.Location = new Point(name.Location.X, name.Location.Y + 40);
            panel.Controls.Add(name);
            panel.Controls.Add(picture);
            panel.DoubleClick += new EventHandler(panel_DoubleClick);
            picture.DoubleClick += new EventHandler(panel_DoubleClick);
            name.DoubleClick += new EventHandler(panel_DoubleClick);
           
            panels.Add(panel);
            this.Controls.Add(panel);           
        }

        private void panel_DoubleClick(object sender, EventArgs e)
        {
            Panel panel;
            
            var type = sender.GetType();
            string name = type.Name;
            if(name == "PictureBox")
            {
                PictureBox pic = sender as PictureBox;
                panel = pic.Parent as Panel;
            }
            else if(name == "Label")
            {
                Label lbl = sender as Label;
                panel = lbl.Parent as Panel;
            }
            else
            {
                panel = sender as Panel;
            }

            string filePath = folderPath + "\\" + panel.Controls[0].Text;

            System.Diagnostics.Process.Start(filePath);
        }
    }
}
