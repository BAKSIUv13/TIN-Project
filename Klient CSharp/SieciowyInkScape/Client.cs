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
    partial class Client
    {
        public string ipAddress;
        public int port;
        public bool connected = false;
        public bool loggedIn = false;
        string loggedUsername = "";

        byte[] buf;
        Socket socket;
        Thread socketThread;
        public ClientMachine clientMachine;

        volatile bool connecting = false;

        MainForm mainForm;

        public string LoggedUsername { get => loggedUsername; }

        public Client(MainForm mainForm, Size areaSize)
        {
            clientMachine = new ClientMachine(this, areaSize);
            this.mainForm = mainForm;
        }

        enum ErrorCodes
        {
            BAD_HEADER, PACKET_TOO_LONG, SEND_TIMEDOUT, RECEIVED_TOO_MUCH, RECEIVE_FAILURE, SEND_FAILURE
        }



        public class ClientMachine
        {
            Client parent;
            public DrawingAreaState drawingArea;

            private string usedNick;
            public string UsedNick { get => usedNick; }

            public ClientMachine(Client parent, Size areaSize)
            {
                drawingArea = new DrawingAreaState(new Point(areaSize.Width, areaSize.Width));
               // drawingArea.mousePositions["nkpkiller"] = new DrawingAreaState.MousePosition(0.4f, 0.3f, "nkpkiller");
                //drawingArea.mousePositions["LubiePierogi"] = new DrawingAreaState.MousePosition(0.7f, 0.4f, "LubiePierogi");
               // drawingArea.mousePositions["Baksiu"] = new DrawingAreaState.MousePosition(0.1f, 0.8f, "Baksiu");

                this.parent = parent;
            }

            void ListConcat(List<byte> list, byte[] array)
            {
                for (int x = 0; x < array.Length; x++) list.Add(array[x]);
            }
            void ListConcat(List<byte> list, char[] array)
            {
                for (int x = 0; x < array.Length; x++) list.Add((byte)array[x]);
            }

            public void SendChatMessage(string message)
            {
                List<byte> bytes = new List<byte>();
                ListConcat(bytes, parent.SocketSetString("mesg"));
                byte[] messageArray = parent.SocketSetString(message);
                ListConcat(bytes, parent.SocketSetInt32(messageArray.Length));
                ListConcat(bytes, messageArray);

                parent.SocketSend(bytes.ToArray());
            }

            public void SendMousePosition(PointF position)
            {
                List<byte> bytes = new List<byte>();

                ListConcat(bytes, parent.SocketSetString("maus"));
                ListConcat(bytes, parent.SocketSetDouble((double)position.X));
                ListConcat(bytes, parent.SocketSetDouble((double)position.Y));

                parent.SocketSend(bytes.ToArray());
            }

            public void SendRectangle(DrawingAreaState.RectangleObject rect)
            {
                List<byte> bytes = new List<byte>();

                ListConcat(bytes, parent.SocketSetString("crea"));
                ListConcat(bytes, parent.SocketSetString("rect"));
                ListConcat(bytes, parent.SocketSetByte(rect.color.R));
                ListConcat(bytes, parent.SocketSetByte(rect.color.G));
                ListConcat(bytes, parent.SocketSetByte(rect.color.B));
                ListConcat(bytes, parent.SocketSetDouble((double)rect.xpos));
                ListConcat(bytes, parent.SocketSetDouble((double)rect.ypos));
                ListConcat(bytes, parent.SocketSetDouble((double)rect.width));
                ListConcat(bytes, parent.SocketSetDouble((double)rect.height));


                parent.SocketSend(bytes.ToArray());
            }

            public void SendLine(DrawingAreaState.LineObject line)
            {
                List<byte> bytes = new List<byte>();

                ListConcat(bytes, parent.SocketSetString("crea"));
                ListConcat(bytes, parent.SocketSetString("line"));
                ListConcat(bytes, parent.SocketSetByte(line.color.R));
                ListConcat(bytes, parent.SocketSetByte(line.color.G));
                ListConcat(bytes, parent.SocketSetByte(line.color.B));
                ListConcat(bytes, parent.SocketSetDouble((double)line.xpos));
                ListConcat(bytes, parent.SocketSetDouble((double)line.ypos));
                ListConcat(bytes, parent.SocketSetDouble((double)line.xpos2));
                ListConcat(bytes, parent.SocketSetDouble((double)line.ypos2));


                parent.SocketSend(bytes.ToArray());
            }

            public void Login(string login, string password)
            {
                usedNick = login;

                List<byte> bytes = new List<byte>();

                ListConcat(bytes, parent.SocketSetString("logo"));
                byte[] loginArray = parent.SocketSetString(login);
                ListConcat(bytes, parent.SocketSetInt32(loginArray.Length));
                ListConcat(bytes, loginArray);
                byte[] passArray = parent.SocketSetString(password);
                ListConcat(bytes, parent.SocketSetInt32(passArray.Length));
                ListConcat(bytes, passArray);
                               
                parent.SocketSend(bytes.ToArray());
            }

            public void Logout()
            {
                parent.loggedUsername = "";
                parent.loggedIn = false;

                List<byte> bytes = new List<byte>();

                ListConcat(bytes, parent.SocketSetString("lout"));

                parent.SocketSend(bytes.ToArray());
            }
        }

     


        

        private static Socket ConnectSocket(string server, int port)
        {
            Socket s = null;
            IPAddress address = System.Net.IPAddress.Parse(server);

            try
            {
                IPEndPoint ipe = new IPEndPoint(address, port);
                Socket tempSocket =
                    new Socket(ipe.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

                tempSocket.Connect(ipe);

                if (tempSocket.Connected)
                {
                    s = tempSocket;
                }
                else
                {
                    throw new SocketException();
                }
            }
            catch(SocketException sEx)
            {
                throw;
            }
            
            return s;
        }

        public void Disconnect()
        {
            loggedIn = false;
            connected = false;

            socket.Close();
        }

        public void Connect(string server, int port)
        {
            this.ipAddress = server;
            this.port = port;

            Connect();
        }
        public void Connect()
        {
            if (connecting)
            {
                mainForm.Invoke(new Action(() =>
                {
                    ConnectionFailedEventArgs arg = new ConnectionFailedEventArgs(ConnectionFailedEventArgs.ConnectionFailReasons.alreadyConnecting);
                    ConnectionFailed(this, arg);
                }
                ));
                
                return;
            }
            connecting = true;
            Thread connectionThread = new Thread(new ThreadStart(Connect_newthread));
            connectionThread.Start();
        }
        void Connect_newthread()
        {
            try
            {
                if(connected)
                {
                    mainForm.Invoke(new Action(() =>
                    {
                        ConnectionFailedEventArgs arg = new ConnectionFailedEventArgs(ConnectionFailedEventArgs.ConnectionFailReasons.alreadyConnected);
                        ConnectionFailed(this, arg);
                    }
                    ));
                    return;
                }
                try
                {
                    buf = new byte[4096];
                    socket = ConnectSocket(ipAddress, port);
                    if (socket == null)
                    {
                        mainForm.Invoke(new Action(() =>
                        {
                            ConnectionFailedEventArgs arg = new ConnectionFailedEventArgs(ConnectionFailedEventArgs.ConnectionFailReasons.socketNotCreated);
                            ConnectionFailed(this, arg);
                        }
                        ));
                    }

                    connected = true;

                    mainForm.Invoke(new Action(() =>
                    {
                        EventArgs arg = new EventArgs();
                        ConnectionSucceeded(this, arg);
                    }
                    ));

                    socketThread = new Thread(new ThreadStart(SocketReceiverThread));
                    socketThread.Start();
                }
                catch(SocketException sEx)
                {
                    mainForm.Invoke(new Action(() =>
                    {
                        ConnectionFailedEventArgs arg = new ConnectionFailedEventArgs(sEx);
                        ConnectionFailed(this, arg);
                    }
                    ));
                }
            }
            catch
            {
                throw;
            }
            finally
            {
                connecting = false;
            }
        }


        public void SocketReceive(int size)
        {
            if (size > 1024 * 1024)
            {
                mainForm.Invoke(new Action(() =>
                {
                    LogicErrorEventArgs arg = new LogicErrorEventArgs(LogicErrorEventArgs.Errors.RECEIVE_REQUEST_TOO_LARGE, true);
                    LogicErrorHappened(this, arg);
                    ConnectionFailedEventArgs arg2 = new ConnectionFailedEventArgs(ConnectionFailedEventArgs.ConnectionFailReasons.criticalLogicErrorHappened);
                    ConnectionFailed(this, arg2);
                }
                ));
                Disconnect();
                return;
            }

            while (size > buf.Length) buf = new byte[buf.Length * 2];

            int received = 0;

            while (received != size)
            {
                if (received > size)
                {
                    mainForm.Invoke(new Action(() =>
                    {
                        LogicErrorEventArgs arg = new LogicErrorEventArgs(LogicErrorEventArgs.Errors.RECEIVED_TOO_MUCH, true);
                        LogicErrorHappened(this, arg);
                        ConnectionFailedEventArgs arg2 = new ConnectionFailedEventArgs(ConnectionFailedEventArgs.ConnectionFailReasons.criticalLogicErrorHappened);
                        ConnectionFailed(this, arg2);
                    }
                    ));
                    Disconnect();
                }

                int ret = socket.Receive(buf, received, size - received, SocketFlags.None);
                if (ret <= 0)
                {
                    mainForm.Invoke(new Action(() =>
                    {
                        LogicErrorEventArgs arg = new LogicErrorEventArgs(LogicErrorEventArgs.Errors.SOCKET_CLOSED, true);
                        LogicErrorHappened(this, arg);
                        ConnectionFailedEventArgs arg2 = new ConnectionFailedEventArgs(ConnectionFailedEventArgs.ConnectionFailReasons.criticalLogicErrorHappened);
                        ConnectionFailed(this, arg2);
                    }
                    ));
                    Disconnect();
                }

                received += ret;
            }
        }

        public void SocketSend(byte[] message)
        {
            byte[] toSend = new byte[4 + message.Length];
            Array.Copy(SocketSetString("OwO!"), 0, toSend, 0, 4);
            Array.Copy(message, 0, toSend, 4, message.Length);

            int sent = 0;
            while (toSend.Length != sent)
            {
                int ret = socket.Send(toSend, sent, toSend.Length - sent, SocketFlags.None);
                if (ret <= 0)
                {
                    mainForm.Invoke(new Action(() =>
                    {
                        LogicErrorEventArgs arg = new LogicErrorEventArgs(LogicErrorEventArgs.Errors.SEND_FAILURE, true);
                        LogicErrorHappened(this, arg);
                        ConnectionFailedEventArgs arg2 = new ConnectionFailedEventArgs(ConnectionFailedEventArgs.ConnectionFailReasons.criticalLogicErrorHappened);
                        ConnectionFailed(this, arg2);
                    }
                    ));
                    Disconnect();
                }

                sent += ret;

            }
        }

        byte[] SocketSetInt64(Int64 value)
        {
            byte[] toRet = System.BitConverter.GetBytes(value);
            if (System.BitConverter.IsLittleEndian) Array.Reverse(toRet);

            return toRet;
        }
        byte[] SocketSetUInt64(UInt64 value)
        {
            byte[] toRet = System.BitConverter.GetBytes(value);
            if (System.BitConverter.IsLittleEndian) Array.Reverse(toRet);

            return toRet;
        }
        byte[] SocketSetInt32(Int32 value)
        {
            byte[] toRet = System.BitConverter.GetBytes(value);
            if (System.BitConverter.IsLittleEndian) Array.Reverse(toRet);

            return toRet;
        }
        byte[] SocketSetUInt32(UInt32 value)
        {
            byte[] toRet = System.BitConverter.GetBytes(value);
            if (System.BitConverter.IsLittleEndian) Array.Reverse(toRet);

            return toRet;
        }
        byte[] SocketSetInt16(Int16 value)
        {
            byte[] toRet = System.BitConverter.GetBytes(value);
            if (System.BitConverter.IsLittleEndian) Array.Reverse(toRet);

            return toRet;
        }
        byte[] SocketSetUInt16(UInt16 value)
        {
            byte[] toRet = System.BitConverter.GetBytes(value);
            if (System.BitConverter.IsLittleEndian) Array.Reverse(toRet);

            return toRet;
        }
        byte[] SocketSetByte(byte value)
        {
            byte[] toRet = System.BitConverter.GetBytes(value);

            return toRet;
        }

        byte[] SocketSetDouble(double value)
        {
            byte[] toRet = System.BitConverter.GetBytes(value);
           // if (System.BitConverter.IsLittleEndian) Array.Reverse(toRet);

            return toRet;
        }
        byte[] SocketSetString(string value)
        {
            byte[] toRet = Encoding.UTF8.GetBytes(value);

            return toRet;
        }

        Int64 SocketReceiveInt64()
        {
            SocketReceive(8);
            if (System.BitConverter.IsLittleEndian) Array.Reverse(buf, 0, 8);
            return System.BitConverter.ToInt64(buf, 0);
        }
        UInt64 SocketReceiveUInt64()
        {
            SocketReceive(8);
            if (System.BitConverter.IsLittleEndian) Array.Reverse(buf, 0, 8);
            return System.BitConverter.ToUInt64(buf, 0);
        }
        Int32 SocketReceiveInt32()
        {
            SocketReceive(4);
            if (System.BitConverter.IsLittleEndian) Array.Reverse(buf, 0, 4);
            return System.BitConverter.ToInt32(buf, 0);
        }
        UInt32 SocketReceiveUInt32()
        {
            SocketReceive(4);
            if (System.BitConverter.IsLittleEndian) Array.Reverse(buf, 0, 4);
            return System.BitConverter.ToUInt32(buf, 0);
        }
        Int16 SocketReceiveInt16()
        {
            SocketReceive(2);
            if (System.BitConverter.IsLittleEndian) Array.Reverse(buf, 0, 2);
            return System.BitConverter.ToInt16(buf, 0);
        }
        UInt16 SocketReceiveUInt16()
        {
            SocketReceive(2);
            if (System.BitConverter.IsLittleEndian) Array.Reverse(buf, 0, 2);
            return System.BitConverter.ToUInt16(buf, 0);
        }
        byte SocketReceiveByte()
        {
            SocketReceive(1);
            return buf[0];
        }

        double SocketReceiveDouble()
        {
            SocketReceive(8);
            //if (System.BitConverter.IsLittleEndian) Array.Reverse(buf, 0, 8);
            return System.BitConverter.ToDouble(buf, 0);
        }
        string SocketReceiveString(UInt32 size)
        {
            SocketReceive((int)size);
            return Encoding.UTF8.GetString(buf, 0, (int)size);
        }

       

        
    }
}
