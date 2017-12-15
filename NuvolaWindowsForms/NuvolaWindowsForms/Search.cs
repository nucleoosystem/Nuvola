using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace NuvolaWindowsForms
{
    public partial class Search : Form
    {
        public Search(string searchText, string searchType)
        {
            this.FormBorderStyle = FormBorderStyle.None;
            InitializeComponent();

            switch(searchType)
            {
                case "Groups":
                    handleGroupsSearch(searchText);
                    break;
                case "Users":
                    handleUsersSearch(searchText);
                    break;
                case "Files":
                    handleFilesSearch(searchText);
                    break;
                default:
                    HandleAllSearch(searchText);
                    break;
            }
        }

        private void handleFilesSearch(string searchText)
        {
            
        }

        private void handleGroupsSearch(string searchText)
        {

        }

        private void handleUsersSearch(string searchText)
        {

        }

        private void HandleAllSearch(string searchText)
        {

        }
    }
}
