using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SieciowyInkScape
{
    public partial class MainForm : Form
    {
        enum ErrorCodes
        {
            BAD_HEADER, PACKET_TOO_LONG, SEND_TIMEDOUT
        }
        struct Codes // po konwersji littleendian -> bigendian litery będą zamienione
        {
            public const UInt32 OwO = ('O' << 24 | 'w' << 16 | 'O' << 8 | '!' << 0); // OwO!

            public const UInt32 MessageOutbound = ('m' << 24 | 's' << 16 | 'g' << 8 | '0' << 0); // msg0
            public const UInt32 MessageInbound = ('m' << 24 | 's' << 16 | 'g' << 8 | '1' << 0); // msg1
        }

        public class MessageCreator
        {
            MainForm parent;

            public MessageCreator(MainForm parent)
            {
                this.parent = parent;
            }

            public byte[] SendChatMessage(string message)
            {
                byte[] toRet = new byte[4 + 4 + message.Length]; // msg0(byte[4])|length(int32)|message(char[])
                Array.Copy(parent.SocketSetUInt32(Codes.MessageOutbound), 0, toRet, 0, 4);
                Array.Copy(parent.SocketSetUInt32((uint)message.Length), 0, toRet, 4, 4);
                Array.Copy(parent.SocketSetString(message), 0, toRet, 8, message.Length);

                return toRet;
            }
        }

        byte[] buf;
        Socket socket;
        Thread socketThread;
        MessageCreator messageCreator;

        public MainForm()
        {
            InitializeComponent();

            messageCreator = new MessageCreator(this);
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            Show();

            bool retry;

            do
            {
                retry = false;

                LoginForm loginForm = new LoginForm();
                loginForm.ShowDialog();

                if (loginForm.DialogResult == DialogResult.OK)
                {
                    if (Connect(loginForm.hostname, loginForm.port) == false)
                    {
                        MessageBox.Show("Połączenie z serwerem nieudane", "Błąd", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        retry = true;
                    }
                }
                else
                {
                    Close();
                }
            }
            while (retry);


        }

        void MakeError(ErrorCodes errorCode)
        {
            switch(errorCode)
            {
                case ErrorCodes.BAD_HEADER:
                    throw new Exception("Zły nagłówek komunikatu!");
                    break;
                case ErrorCodes.SEND_TIMEDOUT:
                    throw new Exception("Wysłanie komunikatu dostało timeout!");
                    break;
                case ErrorCodes.PACKET_TOO_LONG:
                    throw new Exception("Pakiet był za długi!");
                    break;
            }
        }

        bool Connect(string server, int port)
        {
            try
            {
                buf = new byte[4096];
                socket = ConnectSocket(server, port);
                if (socket == null) return false;

                socket.ReceiveTimeout = 1000;
                socket.SendTimeout = 1000;



                socketThread = new Thread(new ThreadStart(SocketThread));
                socketThread.Start();

                return true;
            }
            catch
            {
                return false;
            }
           
        }

        void SocketReceive(uint size)
        {
            if(size > 1024*1024) MakeError(ErrorCodes.PACKET_TOO_LONG);

            while (size > buf.Length) buf = new byte[buf.Length * 2];

            bool retry;

            do
            {
                retry = false;

                try
                {
                    socket.Receive(buf, (int)size, SocketFlags.None);
                }
                catch (SocketException ex)
                {
                    if (ex.ErrorCode == 10060)
                    {
                        retry = true;
                        // operation timedout, OK
                    }
                    else
                    {
                        throw ex;
                    }
                }
            }
            while (retry);


        }

        void SocketSend(byte[] message)
        {
            try
            {
                socket.Send(SocketSetUInt32(Codes.OwO));
                socket.Send(message);
            }
            catch (SocketException ex)
            {
                if (ex.ErrorCode == 10060)
                {
                    MakeError(ErrorCodes.SEND_TIMEDOUT);
                }
                else
                {
                    throw ex;
                }
            }
        }

        byte[] SocketSetUInt32(UInt32 value)
        {
            byte[] toRet = System.BitConverter.GetBytes(value);
            if (System.BitConverter.IsLittleEndian) Array.Reverse(toRet);

            return toRet;
        }
        byte[] SocketSetString(string value)
        {
            byte[] toRet = Encoding.ASCII.GetBytes(value);

            return toRet;
        }

        UInt32 SocketGetUInt32()
        {
            if (System.BitConverter.IsLittleEndian) Array.Reverse(buf, 0, 4);
            return System.BitConverter.ToUInt32(buf, 0);
        }
        string SocketGetString(uint size)
        {
            return Encoding.ASCII.GetString(buf, 0, (int)size);
        }

        void SocketThread()
        {
            while(true)
            {
                SocketReceive(4);
                UInt32 header = SocketGetUInt32();
                if(header == Codes.OwO) // początek komunikatu
                {
                    SocketReceive(4);
                    UInt32 messageType = SocketGetUInt32();

                    switch(messageType)
                    {
                        case Codes.MessageInbound:
                            UInt32 usernameLength;
                            string username;
                            UInt32 messageLength;
                            string message;

                            SocketReceive(4);
                            usernameLength = SocketGetUInt32();
                            SocketReceive(usernameLength);
                            username = SocketGetString(usernameLength);
                            SocketReceive(4);
                            messageLength = SocketGetUInt32();
                            SocketReceive(messageLength);
                            message = SocketGetString(messageLength);

                            chatBox.Invoke(new MethodInvoker(delegate ()
                            {
                                chatBox.AppendText(username + ": " + message + "\n");
                            }));

                            break;
                    }
                }
                else
                {
                    MakeError(ErrorCodes.BAD_HEADER);
                }
            }
            

        }

        private static Socket ConnectSocket(string server, int port)
        {
            Socket s = null;
            IPHostEntry hostEntry = null;

            // Get host related information.
            hostEntry = Dns.GetHostEntry(server);

            // Loop through the AddressList to obtain the supported AddressFamily. This is to avoid
            // an exception that occurs when the host IP Address is not compatible with the address family
            // (typical in the IPv6 case).
            foreach (IPAddress address in hostEntry.AddressList)
            {
                try
                {
                    IPEndPoint ipe = new IPEndPoint(address, port);
                    Socket tempSocket =
                        new Socket(ipe.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

                    tempSocket.Connect(ipe);

                    if (tempSocket.Connected)
                    {
                        s = tempSocket;
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
                catch
                {
                    continue;
                }
            }
            return s;
        }

        private void messageBox_KeyPressed(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == '\r')
            {
                SocketSend(messageCreator.SendChatMessage(messageBox.Text));
                chatBox.AppendText("Ja: " + messageBox.Text + "\n");
                messageBox.Clear();
            }
        }
    }
}
