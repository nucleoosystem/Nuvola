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
using System.Windows.Threading;

namespace NuvolaWPF.Pages
{
    /// <summary>
    /// Interaction logic for FilesPage.xaml
    /// </summary>
    public partial class FilesPage : UserControl
    {
        static bool isClosing = false;

        public FilesPage()
        {
            InitializeComponent();
            string path = "N:\\";
            webView.Navigate(path);
            backButton.IsEnabled = false;

            webView.Navigated += WebView_Navigated;
        }

        private void WebView_Navigated(object sender, System.Windows.Navigation.NavigationEventArgs e)
        {
            backButton.IsEnabled = webView.CanGoBack;
        }

        private void OnButtonClick(object sender, RoutedEventArgs e)
        {
            webView.GoBack();
        }

        private void Page_Loaded(object sender, RoutedEventArgs e)
        {
            if (isClosing)
            {
                webView.Visibility = Visibility.Hidden;
                isClosing = false;
            }
            else
            {
                webView.Visibility = Visibility.Visible;
                isClosing = true;
            }
        }
    }
}
