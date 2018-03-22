using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Net;

namespace NuvolaWPF
{
    class SocketHandler
    {
        static Socket s = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

        public SocketHandler()
        {
            s = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            IPAddress ipAddress = IPAddress.Parse("127.0.0.1");
            IPEndPoint ipe = new IPEndPoint(ipAddress, 22223);

            try
            {
                s.Connect(ipe);
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

        public void sendData(string data)
        {
            data = Encipher(data, "cipher");
            ASCIIEncoding asen = new ASCIIEncoding();
            byte[] ba = asen.GetBytes(data);
            s.Send(ba);
        }

        public string recvData()
        {
            byte[] ba = new byte[4];
            s.Receive(ba);
            return Decipher(System.Text.Encoding.Default.GetString(ba), "cipher");
        }

        public int receiveAmountOfGroups()
        {
            byte[] ba = new byte[2];
            s.Receive(ba);
            return int.Parse(System.Text.Encoding.Default.GetString(ba));
        }

        public string recvDataWithSize()
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

        public int getDataLen(int padding)
        {
            byte[] ba = new byte[padding];
            s.Receive(ba);
            string length = System.Text.Encoding.Default.GetString(ba);
            return int.Parse(length);
        }

        public string recvDataWithGivenSize(int length)
        {
            byte[] ba = new byte[length];
            s.Receive(ba);
            string data = System.Text.Encoding.Default.GetString(ba);
            return Decipher(data, "cipher");
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
