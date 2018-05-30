using FirstFloor.ModernUI.Presentation;
using FirstFloor.ModernUI.Windows.Controls;
using NuvolaWPF.Properties;
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
using System.Security.Cryptography;

namespace NuvolaWPF.Pages.Login
{
    /// <summary>
    /// Interaction logic for RegisterPage.xaml
    /// </summary>
    public partial class RegisterPage : ModernWindow
    {
        public RegisterPage()
        {
            WindowStartupLocation = System.Windows.WindowStartupLocation.CenterScreen;

            InitializeComponent();
        }

        private void registerBtn_Click(object sender, RoutedEventArgs e)
        {
            int number = 0;
            Notifier n = new Notifier(cfg =>
            {
                cfg.PositionProvider = new WindowPositionProvider(
                    parentWindow: this,
                    corner: Corner.BottomRight,
                    offsetX: 10,
                    offsetY: 10);

                cfg.LifetimeSupervisor = new TimeAndCountBasedLifetimeSupervisor(
                    notificationLifetime: TimeSpan.FromSeconds(2),
                    maximumNotificationCount: MaximumNotificationCount.FromCount(1));

                cfg.Dispatcher = Application.Current.Dispatcher;
            });

            if (usernameBox.Text == "" || passwordBox.Password == "" || emailBox.Text == "" || sizeBox.Text == "")
            {
                n.ShowWarning("Please fill all required values.");
            }
            else if(int.TryParse(sizeBox.Text, out number) == false) // Checking if the given size is a number
            {
                n.ShowError("The drive size must be a number.");
            }
            else
            {
                string hashPassword = ComputeHash(passwordBox.Password, new MD5CryptoServiceProvider());

                string data = "202";
                data += usernameBox.Text.Length.ToString().PadLeft(2, '0');
                data += SocketHandler.Encipher(usernameBox.Text, "cipher");
                data += hashPassword.Length.ToString().PadLeft(2, '0');
                data += SocketHandler.Encipher(hashPassword, "cipher");
                data += emailBox.Text.Length.ToString().PadLeft(2, '0');
                data += SocketHandler.Encipher(emailBox.Text, "cipher");
                data += sizeBox.Text.Length.ToString().PadLeft(2, '0');
                data += SocketHandler.Encipher(sizeBox.Text, "cipher");

                Notifier notifier = new Notifier(cfg =>
                {
                    cfg.PositionProvider = new WindowPositionProvider(
                        parentWindow: this,
                        corner: Corner.BottomRight,
                        offsetX: 10,
                        offsetY: 10);

                    cfg.LifetimeSupervisor = new TimeAndCountBasedLifetimeSupervisor(
                        notificationLifetime: TimeSpan.FromSeconds(4),
                        maximumNotificationCount: MaximumNotificationCount.FromCount(1));

                    cfg.Dispatcher = Application.Current.Dispatcher;
                });

                SocketHandler sh = new SocketHandler();
                try
                {
                    sh.sendData(data);
                    string result = sh.recvData();
                    if (result.Equals("1010"))
                    {
                        this.Close();
                    }
                    else if (result.Equals("1011"))
                    {
                        notifier.ShowWarning("Password is illegal");
                    }
                    else if (result.Equals("1013"))
                    {
                        notifier.ShowWarning("Username is illegal");
                    }
                    else if (result.Equals("1015"))
                    {
                        notifier.ShowWarning("Not enough room on the computer");
                    }
                }
                catch (SocketException ex)
                {
                    notifier.ShowError(ex.Message);
                }
                catch (Exception ex)
                {
                    notifier.ShowError(ex.Message);
                }
            }
        }

        private string ComputeHash(string input, HashAlgorithm algorithm)
        {
            Byte[] inputBytes = Encoding.UTF8.GetBytes(input);
            Byte[] hashedBytes = algorithm.ComputeHash(inputBytes);

            string hPassword = BitConverter.ToString(hashedBytes);
            hPassword = hPassword.Replace("-", "");
            return hPassword;
        }

        private void exitToLoginBtn_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void ModernWindow_Closed(object sender, EventArgs e)
        {
            //wdhodges: When the user closes the main window the application will
            //shut down, so here we take the current theme, font size and accent colour
            //then save it to the user settings.

            NuvolaWPF.Properties.Settings.Default.ChosenTheme = AppearanceManager.Current.ThemeSource;
            NuvolaWPF.Properties.Settings.Default.ChosenFontSize = AppearanceManager.Current.FontSize;
            NuvolaWPF.Properties.Settings.Default.ChosenAccent = AppearanceManager.Current.AccentColor;
            NuvolaWPF.Properties.Settings.Default.Save();
        }

        private void ModernWindow_Loaded(object sender, RoutedEventArgs e)
        {
            //wdhodges:  This will be called when the user first launches the window
            //because this is a simple application, however this would not necessarily
            //be the best place for this piece of code, it's just here for simplicity with
            //this example so they are together.
            //Nonetheless here is where we set the current accent, font size and theme based on
            //any previous settings the user had saved

            if (NuvolaWPF.Properties.Settings.Default.ChosenTheme != null)
                AppearanceManager.Current.ThemeSource = NuvolaWPF.Properties.Settings.Default.ChosenTheme;

            //I've pre-set the values for these so null isn't an option
            AppearanceManager.Current.FontSize = NuvolaWPF.Properties.Settings.Default.ChosenFontSize;
            AppearanceManager.Current.AccentColor = NuvolaWPF.Properties.Settings.Default.ChosenAccent;
        }
    }
}
