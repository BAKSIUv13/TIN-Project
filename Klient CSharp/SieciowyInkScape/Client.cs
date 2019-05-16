using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace SieciowyInkScape
{
    class Client
    {

        byte[] buf;
        Socket socket;
        Thread socketThread;
        public ClientMachine clientMachine;

        MainForm mainForm;

        public Client(MainForm mainForm, Size areaSize)
        {
            clientMachine = new ClientMachine(this, areaSize);
            this.mainForm = mainForm;
        }

        enum ErrorCodes
        {
            BAD_HEADER, PACKET_TOO_LONG, SEND_TIMEDOUT, RECEIVED_TOO_MUCH, RECEIVE_FAILURE, SEND_FAILURE
        }
        class Codes // po konwersji littleendian -> bigendian litery będą zamienione
        {
            static UInt32 getCode(string text)
            {
                if (text.Length != 4) throw new Exception("header name must be 4 characters long");

                return ((UInt32)text[0] << 24 | (UInt32)text[1] << 16 | (UInt32)text[2] << 8 | (UInt32)text[3] << 0);
            }
            public static UInt32 OwO = getCode("OwO!"); // OwO!

            public static UInt32 MessageOutbound = getCode("msg0"); // msg0
            public static UInt32 MessageInbound = getCode("msg1"); // msg1
            public static UInt32 MouseOutbound = getCode("mysz");
            public static UInt32 MouseInbound = getCode("maus");

        }

        public class ClientMachine
        {
            Client parent;
            public DrawingAreaState drawingArea;

            string usedNick;

            public ClientMachine(Client parent, Size areaSize)
            {
                drawingArea = new DrawingAreaState(new Point(areaSize.Width, areaSize.Width));
                drawingArea.mousePositions["nkpkiller"] = new DrawingAreaState.MousePosition(0.4f, 0.3f, "nkpkiller");
                drawingArea.mousePositions["LubiePierogi"] = new DrawingAreaState.MousePosition(0.7f, 0.4f, "LubiePierogi");
                drawingArea.mousePositions["Baksiu"] = new DrawingAreaState.MousePosition(0.1f, 0.8f, "Baksiu");

                this.parent = parent;
            }

            public void SendChatMessage(string message)
            {
                MemoryStream str = new MemoryStream();
                StreamWriter wrt = new StreamWriter(str);

                wrt.Write("mesg".ToArray());
                wrt.Write(IPAddress.HostToNetworkOrder(message.Length));
                wrt.Write(message.ToArray());

                parent.SocketSend(str.ToArray());
            }

            public void SendMousePosition(PointF position)
            {
                MemoryStream str = new MemoryStream();
                StreamWriter wrt = new StreamWriter(str);

                wrt.Write("maus".ToArray());
                wrt.Write(position.X);
                wrt.Write(position.Y);

                parent.SocketSend(str.ToArray());
            }

            public void Login(string login, string password)
            {
                usedNick = login;

                MemoryStream str = new MemoryStream();
                StreamWriter wrt = new StreamWriter(str);

                wrt.Write("logo".ToArray());
                wrt.Write(IPAddress.HostToNetworkOrder(login.Length));
                wrt.Write(login.ToArray());
                wrt.Write(IPAddress.HostToNetworkOrder(password.Length));
                wrt.Write(password.ToArray());
                               
                parent.SocketSend(str.ToArray());
            }
        }

        void MakeError(ErrorCodes errorCode)
        {
            switch (errorCode)
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
                case ErrorCodes.RECEIVED_TOO_MUCH:
                    throw new Exception("Socket odebrał za dużo danych!");
                    break;
                case ErrorCodes.RECEIVE_FAILURE:
                    throw new Exception("Socket przy odbiorze zgłosił size <= 0");
                    break;
                case ErrorCodes.SEND_FAILURE:
                    throw new Exception("Socket przy wysyłaniu zgłosił size <= 0");
                    break;
                default:
                    throw new Exception("Nieznany błąd o ID: " + (errorCode).ToString());
                    break;

            }
        }


        void SocketThread()
        {

            while (true)
            {

                UInt32 header = SocketReceiveUInt32();
                if (header == Codes.OwO) // początek komunikatu
                {
                    UInt32 messageType = SocketReceiveUInt32();

                    if (messageType == Codes.MessageInbound)
                    {
                        UInt32 usernameLength;
                        string username;
                        UInt32 messageLength;
                        string message;

                        usernameLength = SocketReceiveUInt32();
                        username = SocketReceiveString(usernameLength);
                        messageLength = SocketReceiveUInt32();
                        message = SocketReceiveString(messageLength);

                        mainForm.chatBox.Invoke(new System.Windows.Forms.MethodInvoker(delegate ()
                        {
                            mainForm.chatBox.AppendText(username + ": " + message + "\n");
                        }));

                        break;
                    }
                    if (messageType == Codes.MouseInbound)
                    {
                        UInt32 xpos = SocketReceiveUInt32();
                        UInt32 ypos = SocketReceiveUInt32();
                        UInt32 usernameLength = SocketReceiveUInt32();
                        string username = SocketReceiveString(usernameLength);

                        clientMachine.drawingArea.Access();
                        clientMachine.drawingArea.mousePositions[username] = new DrawingAreaState.MousePosition(xpos, ypos, username);
                        clientMachine.drawingArea.Exit();
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

        public bool Connect(string server, int port)
        {
            try
            {
                buf = new byte[4096];
                socket = ConnectSocket(server, port);
                if (socket == null) return false;

                //socket.ReceiveTimeout = 1000;
                //socket.SendTimeout = 1000;

                


                socketThread = new Thread(new ThreadStart(SocketThread));
                socketThread.Start();

                return true;
            }
            catch
            {
                return false;
            }

        }

        public void SocketReceive(int size)
        {
            if (size > 1024 * 1024) MakeError(ErrorCodes.PACKET_TOO_LONG);

            while (size > buf.Length) buf = new byte[buf.Length * 2];

            int received = 0;

            while (received != size)
            {
                if (received > size) MakeError(ErrorCodes.RECEIVED_TOO_MUCH);

                int ret = socket.Receive(buf, received, size - received, SocketFlags.None);
                if (ret <= 0) MakeError(ErrorCodes.RECEIVE_FAILURE);

                received += ret;
            }

            /* bool retry;
            
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
            */

        }

        public void SocketSend(byte[] message)
        {
            byte[] toSend = new byte[4 + message.Length];
            Array.Copy(SocketSetUInt32(Codes.OwO), 0, toSend, 0, 4);
            Array.Copy(message, 0, toSend, 4, message.Length);

            int sent = 0;
            while (toSend.Length != sent)
            {
                int ret = socket.Send(toSend, sent, toSend.Length - sent, SocketFlags.None);
                if (ret <= 0) MakeError(ErrorCodes.SEND_FAILURE);

                sent += ret;

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

        UInt32 SocketReceiveUInt32()
        {
            SocketReceive(4);
            if (System.BitConverter.IsLittleEndian) Array.Reverse(buf, 0, 4);
            return System.BitConverter.ToUInt32(buf, 0);
        }
        string SocketReceiveString(UInt32 size)
        {
            SocketReceive((int)size);
            return Encoding.ASCII.GetString(buf, 0, (int)size);
        }

       

        
    }
}
