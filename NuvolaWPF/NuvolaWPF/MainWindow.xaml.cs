using FirstFloor.ModernUI.Presentation;
using FirstFloor.ModernUI.Windows.Controls;
using NuvolaWPF.Properties;
using System;
using System.Collections.Generic;
using System.ComponentModel;
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
using System.Windows.Shell;

namespace NuvolaWPF
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : ModernWindow
    {
        public MainWindow()
        {
            this.WindowState = WindowState.Maximized;
            InitializeComponent();
        }

        private void ModernWindow_Closed(object sender, EventArgs e)
        {
            //wdhodges: When the user closes the main window the application will
            //shut down, so here we take the current theme, font size and accent colour
            //then save it to the user settings.

            Settings.Default.ChosenTheme = AppearanceManager.Current.ThemeSource;
            Settings.Default.ChosenFontSize = AppearanceManager.Current.FontSize;
            Settings.Default.ChosenAccent = AppearanceManager.Current.AccentColor;
           
            Settings.Default.Save();
        }

        private void ModernWindow_Loaded(object sender, RoutedEventArgs e)
        {
            //wdhodges:  This will be called when the user first launches the window
            //because this is a simple application, however this would not necessarily
            //be the best place for this piece of code, it's just here for simplicity with
            //this example so they are together.
            //Nonetheless here is where we set the current accent, font size and theme based on
            //any previous settings the user had saved

            if (Settings.Default.ChosenTheme != null)
                AppearanceManager.Current.ThemeSource = Settings.Default.ChosenTheme;

            //I've pre-set the values for these so null isn't an option
            AppearanceManager.Current.FontSize = Settings.Default.ChosenFontSize;
            AppearanceManager.Current.AccentColor = Settings.Default.ChosenAccent;
        }
    }
}
