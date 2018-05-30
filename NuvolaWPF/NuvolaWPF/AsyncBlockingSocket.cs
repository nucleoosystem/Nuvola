using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

using ToastNotifications;
using ToastNotifications.Lifetime;
using ToastNotifications.Position;
using ToastNotifications.Messages;
using ToastNotifications.Messages.Core;

namespace NuvolaWPF
{
    class AsyncBlockingSocket
    {
        public static Socket s = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        Notifier notifier;
        static Notifier precentageNotifier;
        
        public AsyncBlockingSocket()
        {
        }

        public void listenToServer()
        {
            Socket listener = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            IPAddress ipAddress = IPAddress.Parse("127.0.0.1");
            IPEndPoint ipe = new IPEndPoint(ipAddress, 22222);
            
            try
            {
                listener.Bind(ipe);
                listener.Listen(1);


                Console.WriteLine("Waiting for a connection...");
                s = listener.Accept();
            }
            catch (ArgumentNullException ae)
            {
                Console.WriteLine("ArgumentNullException : {0}", ae.ToString());
            }
            catch (SocketException se)
            {
                Console.WriteLine("SocketException : {0}", se.ToString());
            }
            catch (Exception e)
            {
                Console.WriteLine("Unexpected exception : {0}", e.ToString());
            }
        }

        public void listenForConnection()
        {
            notifier = initNotifier();
            /*App.Current.Dispatcher.Invoke(() =>
            {
                string message = "You were invited to the group";
                notifier.ShowInformation(message);
                notifier.ShowSuccess(message);
                notifier.ShowWarning(message);
                notifier.ShowError(message);
            });*/

            listenToServer();
            while(true)
            {
                int msgCode = getMsgCode();

                App.Current.Dispatcher.Invoke(() =>
                {
                    switch(msgCode)
                    {
                        case 207:
                            handleInviteToGroup();
                            break;
                        case 203:
                            handleUploadFile();
                            break;
                        case 302:
                            handleFileUploadPercent();
                            break;
                        case 500:
                            handleGeneralMessage();
                            break;
                        case 501:
                            handleErrorMessage();
                            break;
                        default:
                            MessageBox.Show("Unknown Message Received: " + msgCode.ToString());
                            break;
                    }
                 });
            }
        }

        public static void sendData(string data)
        {
            ASCIIEncoding asen = new ASCIIEncoding();
            byte[] ba = asen.GetBytes(data);
            s.Send(ba);
        }

        public void handleFileUploadPercent()
        {
            string fileName = recvData(3);
            int percentage = getMsgCode(); // The legnth of the percentage is 3 so we can use the getMsgCode function
            if (percentage == 0)
            {
                precentageNotifier = initPrecentageNotifier();
                precentageNotifier.ShowInformation("Starting.");
            }
            else if(percentage == 100)
            {
                precentageNotifier = initPrecentageNotifier();
                precentageNotifier.ShowInformation("Finished.");
            }
        }

        public void handleInviteToGroup()
        {
            string message = recvData(3);
            Pages.MessagesPage.list.Add(new Tuple<string, string>("Group Invite", message));
            notifier.ShowInformation("A new invite arrived");
        }

        public void handleUploadFile()
        {
            string message = recvData(3);
            Pages.MessagesPage.list.Add(new Tuple<string, string>("Request To Send File", message));
            notifier.ShowInformation("Someone has sent you a file");
        }

        public void handleGeneralMessage()
        {
            string message = recvData(3);
            Pages.MessagesPage.list.Add(new Tuple<string, string>("General Message", message));
            notifier.ShowInformation("A New Message arrived");
        }

        public void handleErrorMessage()
        {
            string message = recvData(3);
            notifier.ShowError(message);
        }

        public static Notifier initPrecentageNotifier() 
        {
            Notifier notifier = new Notifier(cfg =>
            {
                cfg.DisplayOptions.TopMost = true;

                cfg.PositionProvider = new WindowPositionProvider(
                    parentWindow: Application.Current.MainWindow,
                    corner: Corner.BottomRight,
                    offsetX: 10,
                    offsetY: 10);

                cfg.LifetimeSupervisor = new TimeAndCountBasedLifetimeSupervisor(
                    notificationLifetime: TimeSpan.FromSeconds(4),
                    maximumNotificationCount: MaximumNotificationCount.FromCount(1));

                cfg.Dispatcher = Application.Current.Dispatcher;
            });

            return notifier;
        }

        public static Notifier initNotifier()
        {
            Notifier notifier = new Notifier(cfg =>
            {
                cfg.PositionProvider = new WindowPositionProvider(
                    parentWindow: Application.Current.MainWindow,
                    corner: Corner.BottomRight,
                    offsetX: 10,
                    offsetY: 10);

                cfg.LifetimeSupervisor = new TimeAndCountBasedLifetimeSupervisor(
                    notificationLifetime: TimeSpan.FromSeconds(4),
                    maximumNotificationCount: MaximumNotificationCount.FromCount(4));

                cfg.Dispatcher = Application.Current.Dispatcher;
            });

            return notifier;
        }

        public string recvData(int length)
        {
            byte[] ba = new byte[length];
            s.Receive(ba);
            string size = System.Text.Encoding.Default.GetString(ba);

            byte[] msg = new byte[int.Parse(size)];
            s.Receive(msg);

            return Decipher(System.Text.Encoding.Default.GetString(msg), "cipher");
        }

        public int getMsgCode()
        {
            byte[] ba = new byte[3];
            s.Receive(ba);
            string msgCode = System.Text.Encoding.Default.GetString(ba);
            return int.Parse(msgCode);
        }

        private static int Mod(int a, int b)
        {
            return (a % b + b) % b;
        }

        private static string Cipher(string input, string key, bool encipher)
        {
            for (int i = 0; i < key.Length; ++i)
                if (!char.IsLetter(key[i]))
                    return null; // Error

            string output = string.Empty;
            int nonAlphaCharCount = 0;

            for (int i = 0; i < input.Length; ++i)
            {
                if (char.IsLetter(input[i]))
                {
                    bool cIsUpper = char.IsUpper(input[i]);
                    char offset = cIsUpper ? 'A' : 'a';
                    int keyIndex = (i - nonAlphaCharCount) % key.Length;
                    int k = (cIsUpper ? char.ToUpper(key[keyIndex]) : char.ToLower(key[keyIndex])) - offset;
                    k = encipher ? k : -k;
                    char ch = (char)((Mod(((input[i] + k) - offset), 26)) + offset);
                    output += ch;
                }
                else
                {
                    output += input[i];
                    ++nonAlphaCharCount;
                }
            }

            return output;
        }

        public static string Encipher(string input, string key)
        {
            return Cipher(input, key, true);
        }

        public static string Decipher(string input, string key)
        {
            return Cipher(input, key, false);
        }
    }
}
