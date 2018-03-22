using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace NuvolaWPF
{
    class AsyncBlockingSocket
    {
        static Socket s = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

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
            listenToServer();
            while(true)
            {
                int msgCode = getMsgCode();

                switch(msgCode)
                {
                    default:
                        MessageBox.Show("Unknown Message Received: " + msgCode.ToString());
                        break;
                }
            }
        }

        public string recvData()
        {
            byte[] ba = new byte[2];
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
