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
            ASCIIEncoding asen = new ASCIIEncoding();
            byte[] ba = asen.GetBytes(data);
            s.Send(ba);
        }

        public string recvData()
        {
            byte[] ba = new byte[4];
            s.Receive(ba);
            return System.Text.Encoding.Default.GetString(ba);
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
            return System.Text.Encoding.Default.GetString(msg);
        }

        public int getMsgCode()
        {
            byte[] ba = new byte[3];
            s.Receive(ba);
            string msgCode = System.Text.Encoding.Default.GetString(ba);
            return int.Parse(msgCode);
        }
    }
}
