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
using System.Net.Mail;

using ToastNotifications;
using ToastNotifications.Lifetime;
using ToastNotifications.Position;
using ToastNotifications.Messages;

namespace NuvolaWPF.Pages.Login
{
    /// <summary>
    /// Interaction logic for ForgotPassPage.xaml
    /// </summary>
    public partial class ForgotPassPage : ModernWindow
    {
        public ForgotPassPage()
        {
            WindowStartupLocation = System.Windows.WindowStartupLocation.CenterScreen;

            InitializeComponent();
        }

        private void sendBtn_Click(object sender, RoutedEventArgs e)
        {
            Notifier notifier = new Notifier(cfg =>
            {
                cfg.PositionProvider = new WindowPositionProvider(
                    parentWindow: this,
                    corner: Corner.BottomRight,
                    offsetX: 10,
                    offsetY: 10);

                cfg.LifetimeSupervisor = new TimeAndCountBasedLifetimeSupervisor(
                    notificationLifetime: TimeSpan.FromSeconds(8),
                    maximumNotificationCount: MaximumNotificationCount.FromCount(4));

                cfg.Dispatcher = Application.Current.Dispatcher;
            }); 

            string toEmail = emailBox.Text;
            string subject = "Password-Nuvola:no-reply";
            string message = "Your password is 123456";
            string myEmailPass = ""; // Insert my password

           try
           { 
                MailMessage mail = new MailMessage("shaked.nissanov@gmail.com", toEmail);
                mail.Subject = subject;
                mail.Body = message;
                SmtpClient client = new SmtpClient();
                client.Host = "smtp.googlemail.com";
                client.Port = 587;
                client.UseDefaultCredentials = false;
                client.DeliveryMethod = SmtpDeliveryMethod.Network;
                client.EnableSsl = true;
                client.Credentials = new System.Net.NetworkCredential("shaked.nissanov@gmail.com", myEmailPass);
                client.Send(mail);
            }
            catch(Exception ex)
            {
                notifier.ShowError(ex.Message);   
            }
        }

        private void backBtn_Click(object sender, RoutedEventArgs e)
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
