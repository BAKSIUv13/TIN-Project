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

        DrawingAreaState drawingArea;

        int framesCounted = 0;
        int FPS = 0;

        public MainForm()
        {
            InitializeComponent();

            messageCreator = new MessageCreator(this);
        }

        public static void SetDoubleBuffered(System.Windows.Forms.Control c)
        {
            //Taxes: Remote Desktop Connection and painting
            //http://blogs.msdn.com/oldnewthing/archive/2006/01/03/508694.aspx
            if (System.Windows.Forms.SystemInformation.TerminalServerSession)
                return;

            System.Reflection.PropertyInfo aProp =
                  typeof(System.Windows.Forms.Control).GetProperty(
                        "DoubleBuffered",
                        System.Reflection.BindingFlags.NonPublic |
                        System.Reflection.BindingFlags.Instance);

            aProp.SetValue(c, true, null);
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            Show();
            SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
            SetDoubleBuffered(drawing);

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

        bool Connect(string server, int port)
        {
            try
            {
                buf = new byte[4096];
                socket = ConnectSocket(server, port);
                if (socket == null) return false;

                //socket.ReceiveTimeout = 1000;
                //socket.SendTimeout = 1000;

                drawingArea = new DrawingAreaState(new Point(drawing.Width, drawing.Height));
                drawingArea.mousePositions["nkpkiller"] = new DrawingAreaState.MousePosition(0.4f, 0.3f, "nkpkiller");
                drawingArea.mousePositions["LubiePierogi"] = new DrawingAreaState.MousePosition(0.7f, 0.4f, "LubiePierogi");
                drawingArea.mousePositions["Baksiu"] = new DrawingAreaState.MousePosition(0.1f, 0.8f, "Baksiu");


                socketThread = new Thread(new ThreadStart(SocketThread));
                socketThread.Start();

                return true;
            }
            catch
            {
                return false;
            }
           
        }

        void SocketReceive(int size)
        {
            if(size > 1024*1024) MakeError(ErrorCodes.PACKET_TOO_LONG);

            while (size > buf.Length) buf = new byte[buf.Length * 2];

            int received = 0;

            while(received != size)
            {
                if(received > size) MakeError(ErrorCodes.RECEIVED_TOO_MUCH);

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

        void SocketSend(byte[] message)
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

        void SocketThread()
        {
            while(true)
            {
                
                UInt32 header = SocketReceiveUInt32();
                if(header == Codes.OwO) // początek komunikatu
                {
                    UInt32 messageType = SocketReceiveUInt32();

                    if(messageType == Codes.MessageInbound)
                    {
                        UInt32 usernameLength;
                        string username;
                        UInt32 messageLength;
                        string message;

                        usernameLength = SocketReceiveUInt32();
                        username = SocketReceiveString(usernameLength);
                        messageLength = SocketReceiveUInt32();
                        message = SocketReceiveString(messageLength);

                        chatBox.Invoke(new MethodInvoker(delegate ()
                        {
                            chatBox.AppendText(username + ": " + message + "\n");
                        }));

                        break;
                    }
                    if(messageType == Codes.MouseInbound)
                    {
                        UInt32 xpos = SocketReceiveUInt32();
                        UInt32 ypos = SocketReceiveUInt32();
                        UInt32 usernameLength = SocketReceiveUInt32();
                        string username = SocketReceiveString(usernameLength);

                        drawingArea.Access();
                        drawingArea.mousePositions[username] = new DrawingAreaState.MousePosition(xpos, ypos, username);
                        drawingArea.Exit();
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

        void DrawObject(DrawingAreaState state, DrawingAreaState.DrawingObject obj, Graphics gr)
        {
            switch(obj.objectType)
            {
                case DrawingAreaState.DrawingObject.ObjectType.LINE:
                    DrawingAreaState.LineObject line = (DrawingAreaState.LineObject)obj;
                    gr.DrawLine(new Pen(new SolidBrush(line.color), line.thickness),
                        line.xpos * state.areaSize.X, line.ypos * state.areaSize.Y,
                        line.xpos2 * state.areaSize.X, line.ypos2 * state.areaSize.Y);
                    break;
                case DrawingAreaState.DrawingObject.ObjectType.RECTANGLE:
                    DrawingAreaState.RectangleObject rect = (DrawingAreaState.RectangleObject)obj;
                    gr.DrawRectangle(new Pen(new SolidBrush(rect.color), rect.thickness),
                        ((rect.xpos2 - rect.xpos) > 0 ? (rect.xpos) : (rect.xpos2)) * state.areaSize.X,
                        ((rect.ypos2 - rect.ypos) > 0 ? (rect.ypos) : (rect.ypos2)) * state.areaSize.Y,
                        ((rect.xpos2 - rect.xpos) > 0 ? (rect.xpos2 - rect.xpos) : (rect.xpos - rect.xpos2)) * state.areaSize.X,
                        ((rect.ypos2 - rect.ypos) > 0 ? (rect.ypos2 - rect.ypos) : (rect.ypos - rect.ypos2)) * state.areaSize.Y);
                    break;
            }
        }

        private void drawing_Paint(object sender, PaintEventArgs e)
        {
           
            if(!(drawingArea is null))
            {
                framesCounted++;

                drawingArea.Access();

                Graphics graphics = e.Graphics;
                graphics.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.HighQuality;
                graphics.Clear(Color.White);
                

                foreach(DrawingAreaState.DrawingObject obj in drawingArea.objects)
                {
                    DrawObject(drawingArea, obj, graphics);
                }

                drawingArea.CheckPendingObjects();
                foreach (DrawingAreaState.PendingObject pobj in drawingArea.pendingObjects)
                {
                    DrawObject(drawingArea, pobj.obj, graphics);
                }

                foreach (KeyValuePair<string, DrawingAreaState.MousePosition> entry in drawingArea.mousePositions)
                {
                    // do something with entry.Value or entry.Key
                    DrawingAreaState.MousePosition pos = entry.Value;

                    graphics.DrawLine(new Pen(Brushes.Black),
                        pos.xpos * drawingArea.areaSize.X - 5, pos.ypos * drawingArea.areaSize.Y - 5,
                        pos.xpos * drawingArea.areaSize.X + 5, pos.ypos * drawingArea.areaSize.Y + 5);
                    graphics.DrawLine(new Pen(Brushes.Black),
                        pos.xpos * drawingArea.areaSize.X + 5, pos.ypos * drawingArea.areaSize.Y - 5,
                        pos.xpos * drawingArea.areaSize.X - 5, pos.ypos * drawingArea.areaSize.Y + 5);

                    graphics.DrawString(pos.username, new Font("Courier New", 12.0f), Brushes.Black, pos.xpos * drawingArea.areaSize.X, pos.ypos * drawingArea.areaSize.Y - 25.0f);
                }

                if(!(drawingArea.tempObject is null))
                {
                    DrawObject(drawingArea, drawingArea.tempObject, graphics);
                }

                graphics.DrawString("FPS: " + FPS.ToString(), new Font("Courier New", 12.0f), Brushes.Black, 5, 5);


                drawingArea.Exit();
            }
        }

        private void drawing_MouseDown(object sender, MouseEventArgs e)
        {
            if(drawingArea.state == DrawingAreaState.State.IDLE)
            {
                drawingArea.state = DrawingAreaState.State.DRAWING;
                switch (drawingArea.selectedTool)
                {
                    case DrawingAreaState.Tools.LINE:
                        drawingArea.tempObject = new DrawingAreaState.LineObject(
                        (float)(e.X) / (float)drawingArea.areaSize.X,
                        (float)(e.Y) / (float)drawingArea.areaSize.Y,
                        (float)(e.X) / (float)drawingArea.areaSize.X,
                        (float)(e.Y) / (float)drawingArea.areaSize.Y, 1, Color.Black);
                        break;
                    case DrawingAreaState.Tools.RECTANGLE:
                        drawingArea.tempObject = new DrawingAreaState.RectangleObject(
                        (float)(e.X) / (float)drawingArea.areaSize.X,
                        (float)(e.Y) / (float)drawingArea.areaSize.Y,
                        (float)(e.X) / (float)drawingArea.areaSize.X,
                        (float)(e.Y) / (float)drawingArea.areaSize.Y, 1, Color.Black);
                        break;
                }
            }

            drawing.Refresh();
        }

        private void drawing_MouseMove(object sender, MouseEventArgs e)
        {
            if (drawingArea.state == DrawingAreaState.State.DRAWING)
            {
                DrawingAreaState.DrawingObject obj = drawingArea.tempObject;
                switch (obj.objectType)
                {
                    case DrawingAreaState.DrawingObject.ObjectType.LINE:
                        DrawingAreaState.LineObject line = (DrawingAreaState.LineObject)obj;
                        line.xpos2 = (float)(e.X) / (float)drawingArea.areaSize.X;
                        line.ypos2 = (float)(e.Y) / (float)drawingArea.areaSize.Y;

                        break;
                    case DrawingAreaState.DrawingObject.ObjectType.RECTANGLE:
                        DrawingAreaState.RectangleObject rect = (DrawingAreaState.RectangleObject)obj;
                        rect.xpos2 = (float)(e.X) / (float)drawingArea.areaSize.X;
                        rect.ypos2 = (float)(e.Y) / (float)drawingArea.areaSize.Y;

                        break;
                }
               
            }

            drawing.Refresh();
        }

        private void drawing_MouseUp(object sender, MouseEventArgs e)
        {
            if (drawingArea.state == DrawingAreaState.State.DRAWING)
            {
                DrawingAreaState.DrawingObject obj = drawingArea.tempObject;
               
                drawingArea.FinalizeObject(this, drawingArea.tempObject);

                drawingArea.tempObject = null;

                drawingArea.state = DrawingAreaState.State.IDLE;
            }

            drawing.Refresh();
        }

        private void RefreshTimer_Tick(object sender, EventArgs e)
        {
            if (!(drawingArea is null))
            {
                drawingArea.CheckPendingObjects();
            }
            drawing.Refresh();
        }

        private void FPSTimer_Tick(object sender, EventArgs e)
        {
            FPS = framesCounted;
            framesCounted = 0;
        }

        private void rectangleButton_Click(object sender, EventArgs e)
        {
            drawingArea.selectedTool = DrawingAreaState.Tools.RECTANGLE;
        }

        private void lineButton_Click(object sender, EventArgs e)
        {
            drawingArea.selectedTool = DrawingAreaState.Tools.LINE;
        }
    }
}
