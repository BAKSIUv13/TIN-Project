using NAudio.Wave;
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
        DateTime mousePositionLastSentTime = new DateTime(0);
        int mousePositionSendInterval_ms = 100;
        DateTime pencilLastSentTime = new DateTime(0);
        int pencilSendInterval_ms = 33;

        Client client;


        int framesCounted = 0;
        int FPS = 0;

        WaveOutEvent waveOut;

        public MainForm()
        {
            InitializeComponent();
            
            client = new Client(this, drawing.Size);
            client.MessageInbound += OnMessageInbound;
            client.ServerMessageInbound += OnServerMessageInbound;
            client.ConnectionFailed += OnConnectionFailed;
            client.ConnectionSucceeded += OnConnectionSucceeded;
            client.LoginCompleted += OnLoginSucceeded;
            client.LoginFailed += OnLoginFailed;
            client.LogoutCompleted += OnLogout;
            client.LogicErrorHappened += OnLogicError;
            client.UserListInbound += OnUserListInbound;
            Show();
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
            client.clientMachine.drawingArea.areaSize.X = drawing.Size.Width;
            client.clientMachine.drawingArea.areaSize.Y = drawing.Size.Height;

        }

        void OnLogout(object sender, EventArgs e)
        {
            ChatBoxWriteLine("Wylogowano");

            buttonConnect.Enabled = false;
            buttonLogin.Enabled = true;
            buttonLogout.Enabled = false;
            buttonDisconnect.Enabled = true;

            textBoxHostname.Enabled = false;
            textBoxPort.Enabled = false;
            textBoxUsername.Enabled = true;
            textBoxPassword.Enabled = true;
        }
        void OnLoginSucceeded(object sender, EventArgs e)
        {
            ChatBoxWriteLine("Logowanie udane!");

            buttonConnect.Enabled = false;
            buttonLogin.Enabled = false;
            buttonLogout.Enabled = true;
            buttonDisconnect.Enabled = true;

            textBoxHostname.Enabled = false;
            textBoxPort.Enabled = false;
            textBoxUsername.Enabled = false;
            textBoxPassword.Enabled = false;
        }
        void OnLoginFailed(object sender, Client.LoginFailedEventArgs e)
        {
            ChatBoxWriteLine("Logowanie nieudane. | Kod: " + e.reason.ToString("g") + " | Wiadomość: " + e.serverMessage);

            buttonConnect.Enabled = false;
            buttonLogin.Enabled = true;
            buttonLogout.Enabled = false;
            buttonDisconnect.Enabled = true;

            textBoxHostname.Enabled = false;
            textBoxPort.Enabled = false;
            textBoxUsername.Enabled = true;
            textBoxPassword.Enabled = true;
        }
        void OnConnectionSucceeded(object sender, EventArgs e)
        {
            ChatBoxWriteLine("Połączenie udane!");

            buttonConnect.Enabled = false;
            buttonLogin.Enabled = true;
            buttonLogout.Enabled = false;
            buttonDisconnect.Enabled = true;

            textBoxHostname.Enabled = false;
            textBoxPort.Enabled = false;
            textBoxUsername.Enabled = true;
            textBoxPassword.Enabled = true;
        }
        void OnConnectionFailed(object sender, Client.ConnectionFailedEventArgs e)
        {
            if (e.reason == Client.ConnectionFailedEventArgs.ConnectionFailReasons.alreadyConnected)
            {
                ChatBoxWriteLine("Połączenie już wcześniej zostało nawiązane!");
                return;
            }
            else if (e.reason == Client.ConnectionFailedEventArgs.ConnectionFailReasons.alreadyConnecting)
            {
                ChatBoxWriteLine("Proces łączenia już trwa!");
                return;
            }
            else if (e.reason == Client.ConnectionFailedEventArgs.ConnectionFailReasons.socketException)
            {
                ChatBoxWriteLine("Połączenie przerwane. Status socketa: " + e.socketException.Message);
            }
            else
            {
                ChatBoxWriteLine("Połączenie przerwane. Powód: " + e.reason.ToString("g"));
            }

            buttonConnect.Enabled = true;
            buttonLogin.Enabled = false;
            buttonLogout.Enabled = false;
            buttonDisconnect.Enabled = false;

            textBoxHostname.Enabled = true;
            textBoxPort.Enabled = true;
            textBoxUsername.Enabled = true;
            textBoxPassword.Enabled = true;
        }
        void OnLogicError(object sender, Client.LogicErrorEventArgs e)
        {
            if(e.critical)
            {
                ChatBoxWriteLine("Krytyczny błąd logiki programu: " + e.error.ToString("g"));

                buttonConnect.Enabled = true;
                buttonLogin.Enabled = false;
                buttonLogout.Enabled = false;
                buttonDisconnect.Enabled = false;

                textBoxHostname.Enabled = true;
                textBoxPort.Enabled = true;
                textBoxUsername.Enabled = true;
                textBoxPassword.Enabled = true;

            }
            else ChatBoxWriteLine("Błąd logiki programu: " + e.error.ToString("g"));
        }
        void OnMessageInbound(object sender, Client.MessageInboundEventArgs e)
        {
            ChatBoxWriteLine(e.username + ": " + e.message);

            if(e.message.ToUpper().Contains("CARAMEL"))
            {
                if (!(waveOut is null)) waveOut.Dispose();
                waveOut = new WaveOutEvent();
                NAudio.Vorbis.VorbisWaveReader ogg = new NAudio.Vorbis.VorbisWaveReader(new System.IO.MemoryStream(Properties.Resources.CaramelHeaven));
                waveOut.Init(ogg);
                waveOut.Play();
            }
            if (e.message.ToUpper().Contains("MIAU") || e.message.ToUpper().Contains("MEOW"))
            {
                if (!(waveOut is null)) waveOut.Dispose();
                waveOut = new WaveOutEvent();
                NAudio.Vorbis.VorbisWaveReader ogg = new NAudio.Vorbis.VorbisWaveReader(new System.IO.MemoryStream(Properties.Resources.Meow));
                waveOut.Init(ogg);
                waveOut.Play();
            }
            if (e.message.ToUpper().Contains("MONOSUGOI"))
            {
                if (!(waveOut is null)) waveOut.Dispose();
                waveOut = new WaveOutEvent();
                NAudio.Vorbis.VorbisWaveReader ogg = new NAudio.Vorbis.VorbisWaveReader(new System.IO.MemoryStream(Properties.Resources.Monosugoi_Space_Shuttle_de_Koishi_ga_Monosugoi_uta));
                waveOut.Init(ogg);
                waveOut.Play();
            }
            if (e.message.ToUpper().Contains("STOP"))
            {
                if (!(waveOut is null)) waveOut.Dispose();
            }
        }
        void OnServerMessageInbound(object sender, Client.ServerMessageInboundEventArgs e)
        {
            ChatBoxWriteLine("> SERWER: " + e.message);
        }
        void OnUserListInbound(object sender, Client.UserListInboundEventArgs e)
        {
            string output = "";
            for(int x = 0; x < e.userList.Count; ++x)
            {
                output += e.userList[x] + "\n";
            }

            UserListWrite(output);
        }

        void ChatBoxWriteLine(string text)
        {
            chatBox.Invoke(new System.Windows.Forms.MethodInvoker(delegate ()
            {
                //chatBox.Text += text + Environment.NewLine;
                chatBox.AppendText(text + Environment.NewLine);
            }));
        }

        void UserListWrite(string text)
        {
            chatBox.Invoke(new System.Windows.Forms.MethodInvoker(delegate ()
            {
                userListBox.Text = text;
            }));
        }

        private void messageBox_KeyPressed(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == '\r')
            {
                if(client.loggedIn)
                {
                    if(messageBox.Text != "MEOOW")
                    {
                        client.clientMachine.SendChatMessage(messageBox.Text);
                        messageBox.Clear();
                    }
                    else
                    {
                        Bitmap cat = Properties.Resources.CAT;

                        for(int y = 0; y < cat.Height; y += 3)
                        {
                            for (int x = 0; x < cat.Width; x += 3)
                            {
                                client.clientMachine.SendRectangle(new DrawingAreaState.RectangleObject(-1,
                                    (float)x / 600.0f + 0.300f,
                                    (float)y / 520.0f + 0.125f,
                                    4.0f / 600.0f,
                                    4.0f / 520.0f,
                                    0.000001f,
                                    Color.FromArgb(255, cat.GetPixel(x, y)),
                                    Color.FromArgb(255, cat.GetPixel(x, y))
                                    ));
                            }
                            Application.DoEvents();
                        }
                    }
                }
                
            }
        }

        void DrawObject(DrawingAreaState state, DrawingAreaState.DrawingObject obj, Graphics gr)
        {
            switch(obj.objectType)
            {
                case DrawingAreaState.DrawingObject.ObjectType.PATH:
                    DrawingAreaState.PathObject path = (DrawingAreaState.PathObject)obj;

                    List<PointF> points = new List<PointF>();
                    List<byte> pointsType = new List<byte>();

                    points.Add(new PointF(path.xpos * state.areaSize.X, path.ypos * state.areaSize.Y));
                    pointsType.Add((byte)System.Drawing.Drawing2D.PathPointType.Start);

                    for (int i = 0; i < path.anotherXposs.Count(); ++i)
                    {
                        points.Add(new PointF(path.anotherXposs[i] * state.areaSize.X, path.anotherYposs[i] * state.areaSize.Y));
                        pointsType.Add((byte)System.Drawing.Drawing2D.PathPointType.Line);
                    }

                    gr.DrawPath(new Pen(new SolidBrush(path.color), path.thickness), new System.Drawing.Drawing2D.GraphicsPath(points.ToArray(), pointsType.ToArray()));

                    break;

                case DrawingAreaState.DrawingObject.ObjectType.RECTANGLE:
                    DrawingAreaState.RectangleObject rect = (DrawingAreaState.RectangleObject)obj;

                    if(rect.BGColor.A > 0)
                    {
                        /*gr.FillRectangle(new SolidBrush(rect.BGColor),
                            (rect.xpos + rect.thickness / 2.0f) * state.areaSize.X, (rect.ypos + rect.thickness / 2.0f) * state.areaSize.Y,
                            (rect.width - rect.thickness) * state.areaSize.X, (rect.height - rect.thickness) * state.areaSize.Y); */

                        gr.FillRectangle(new SolidBrush(rect.BGColor),
                            (rect.xpos) * state.areaSize.X, (rect.ypos) * state.areaSize.Y,
                            (rect.width) * state.areaSize.X, (rect.height) * state.areaSize.Y);
                    }
                    gr.DrawRectangle(new Pen(new SolidBrush(rect.color), (float)(rect.thickness * (double)state.areaSize.X)),
                        rect.xpos * state.areaSize.X, rect.ypos * state.areaSize.Y,
                        rect.width * state.areaSize.X, rect.height * state.areaSize.Y);
                    break;

                case DrawingAreaState.DrawingObject.ObjectType.OVAL:
                    DrawingAreaState.OvalObject oval = (DrawingAreaState.OvalObject)obj;
                    if (oval.BGColor.A > 0)
                    {
                        gr.FillEllipse(new SolidBrush(oval.BGColor),
                            (oval.xpos) * state.areaSize.X, (oval.ypos) * state.areaSize.Y,
                            (oval.width) * state.areaSize.X, (oval.height) * state.areaSize.Y);
                    }
                    gr.DrawEllipse(new Pen(new SolidBrush(oval.color), (float)(oval.thickness * (double)state.areaSize.X)),
                        oval.xpos * state.areaSize.X, oval.ypos * state.areaSize.Y,
                        oval.width * state.areaSize.X, oval.height * state.areaSize.Y);
                    break;
            }
        }

        private void drawing_Paint(object sender, PaintEventArgs e)
        {
            DrawingAreaState drawingArea = client.clientMachine.drawingArea;

            if (!(drawingArea is null))

            {
                framesCounted++;

                drawingArea.Access();

                Graphics graphics = e.Graphics;
                graphics.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.HighSpeed;
               // graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.Default;
               // graphics.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.HighSpeed;
               // graphics.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.Low;

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
            DrawingAreaState drawingArea = client.clientMachine.drawingArea;

            drawingArea.Access();
            if (drawingArea.state == DrawingAreaState.State.IDLE)

            {
                drawingArea.state = DrawingAreaState.State.DRAWING;
                drawingArea.mousepos_start = new Point(e.X, e.Y);
                drawingArea.mousepos_now = new Point(e.X, e.Y);
                switch (drawingArea.selectedTool)
                {
                    case DrawingAreaState.Tools.LINE:
                        drawingArea.tempObject = new DrawingAreaState.PathObject(-1,
                        (float)(e.X) / (float)drawingArea.areaSize.X,
                        (float)(e.Y) / (float)drawingArea.areaSize.Y,
                        new List<float>(),
                        new List<float>(), drawingArea.thickness, drawingArea.ForegroundColor);
                        break;

                    case DrawingAreaState.Tools.PENCIL:
                        drawingArea.tempObject = new DrawingAreaState.PathObject(-1,
                        (float)(e.X) / (float)drawingArea.areaSize.X,
                        (float)(e.Y) / (float)drawingArea.areaSize.Y,
                        new List<float>(),
                        new List<float>(), drawingArea.thickness, drawingArea.ForegroundColor);
                        break;

                    case DrawingAreaState.Tools.RECTANGLE:
                        drawingArea.tempObject = new DrawingAreaState.RectangleObject(-1,
                        (float)(e.X) / (float)drawingArea.areaSize.X,
                        (float)(e.Y) / (float)drawingArea.areaSize.Y,
                        (float)0,
                        (float)0, drawingArea.thickness, drawingArea.ForegroundColor, drawingArea.BackgroundColor);
                        break;


                    case DrawingAreaState.Tools.OVAL:
                        drawingArea.tempObject = new DrawingAreaState.OvalObject(-1,
                        (float)(e.X) / (float)drawingArea.areaSize.X,
                        (float)(e.Y) / (float)drawingArea.areaSize.Y,
                        (float)0,
                        (float)0, drawingArea.thickness, drawingArea.ForegroundColor, drawingArea.BackgroundColor);
                        break;
                }
            }
            drawingArea.Exit();

            drawing.Refresh();
          //  RefreshTimer.Interval = 33;
        }

        private void drawing_MouseMove(object sender, MouseEventArgs e)
        {
            DrawingAreaState drawingArea = client.clientMachine.drawingArea;

            drawingArea.Access();
            drawingArea.mousePosition.X = (float)e.Location.X / (float)drawingArea.areaSize.X;
            drawingArea.mousePosition.Y = (float)e.Location.Y / (float)drawingArea.areaSize.Y;

            if(client.loggedIn)
            {
                if (mousePositionLastSentTime + new TimeSpan(0, 0, 0, 0, mousePositionSendInterval_ms) < DateTime.Now)
                {
                    mousePositionLastSentTime = DateTime.Now;

                    client.clientMachine.SendMousePosition(drawingArea.mousePosition);
                }
            }
            

            DrawingAreaState.State drawingAreaState = drawingArea.state;
            if (drawingArea.state == DrawingAreaState.State.DRAWING)
            {
                drawingArea.mousepos_now = new Point(e.X, e.Y);
                Point ms = drawingArea.mousepos_start;
                Point mn = drawingArea.mousepos_now;

                DrawingAreaState.DrawingObject obj = drawingArea.tempObject;
                switch (drawingArea.selectedTool)
                {
                    case DrawingAreaState.Tools.LINE:
                        DrawingAreaState.PathObject line = (DrawingAreaState.PathObject)obj;
                        line.anotherXposs.Clear();
                        line.anotherYposs.Clear();
                        line.anotherXposs.Add((float)(mn.X) / (float)drawingArea.areaSize.X);
                        line.anotherYposs.Add((float)(mn.Y) / (float)drawingArea.areaSize.Y);
                        break;

                    case DrawingAreaState.Tools.PENCIL:
                        if (pencilLastSentTime + new TimeSpan(0, 0, 0, 0, pencilSendInterval_ms) < DateTime.Now)
                        {
                            pencilLastSentTime = DateTime.Now;

                            DrawingAreaState.PathObject path = (DrawingAreaState.PathObject)obj;
                            path.anotherXposs.Add((float)(mn.X) / (float)drawingArea.areaSize.X);
                            path.anotherYposs.Add((float)(mn.Y) / (float)drawingArea.areaSize.Y);
                        }


                        break;

                    case DrawingAreaState.Tools.RECTANGLE:
                        DrawingAreaState.RectangleObject rect = (DrawingAreaState.RectangleObject)obj;
                        rect.xpos = (mn.X - ms.X) > 0 ? ((float)ms.X / (float)drawingArea.areaSize.X) : ((float)mn.X / (float)drawingArea.areaSize.X);
                        rect.ypos = (mn.Y - ms.Y) > 0 ? ((float)ms.Y / (float)drawingArea.areaSize.Y) : ((float)mn.Y / (float)drawingArea.areaSize.Y);
                        rect.width = (mn.X - ms.X) > 0 ? ((float)(mn.X - ms.X) / (float)drawingArea.areaSize.X) : ((float)(ms.X - mn.X) / (float)drawingArea.areaSize.X);
                        rect.height = (mn.Y - ms.Y) > 0 ? ((float)(mn.Y - ms.Y) / (float)drawingArea.areaSize.Y) : ((float)(ms.Y - mn.Y) / (float)drawingArea.areaSize.Y);
                        break;

                    case DrawingAreaState.Tools.OVAL:
                        DrawingAreaState.OvalObject oval = (DrawingAreaState.OvalObject)obj;
                        oval.xpos = (mn.X - ms.X) > 0 ? ((float)ms.X / (float)drawingArea.areaSize.X) : ((float)mn.X / (float)drawingArea.areaSize.X);
                        oval.ypos = (mn.Y - ms.Y) > 0 ? ((float)ms.Y / (float)drawingArea.areaSize.Y) : ((float)mn.Y / (float)drawingArea.areaSize.Y);
                        oval.width = (mn.X - ms.X) > 0 ? ((float)(mn.X - ms.X) / (float)drawingArea.areaSize.X) : ((float)(ms.X - mn.X) / (float)drawingArea.areaSize.X);
                        oval.height = (mn.Y - ms.Y) > 0 ? ((float)(mn.Y - ms.Y) / (float)drawingArea.areaSize.Y) : ((float)(ms.Y - mn.Y) / (float)drawingArea.areaSize.Y);
                        break;
                }
            }
            drawingArea.Exit();

            if (drawingAreaState == DrawingAreaState.State.DRAWING || drawingAreaState == DrawingAreaState.State.EDITING) drawing.Refresh();

            /*
            drawingArea.Access();
            drawingArea.mousePositions["nkpkiller"] = new DrawingAreaState.MousePosition((float)(e.X) / (float)drawingArea.areaSize.X, (float)(e.Y) / (float)drawingArea.areaSize.Y, "nkpkiller");
            drawingArea.Exit();
            */

            Application.DoEvents();
            // drawing.Refresh();
        }

        private void drawing_MouseUp(object sender, MouseEventArgs e)
        {
            DrawingAreaState drawingArea = client.clientMachine.drawingArea;

            drawingArea.Access();
            if (drawingArea.state == DrawingAreaState.State.DRAWING)
            {
                DrawingAreaState.DrawingObject obj = drawingArea.tempObject;

                if (client.loggedIn)
                {
                    drawingArea.FinalizeObject(this, drawingArea.tempObject);

                    switch (drawingArea.selectedTool)
                    {
                        case DrawingAreaState.Tools.RECTANGLE:
                            client.clientMachine.SendRectangle((DrawingAreaState.RectangleObject)obj);
                            break;
                        case DrawingAreaState.Tools.OVAL:
                            client.clientMachine.SendOval((DrawingAreaState.OvalObject)obj);
                            break;
                        case DrawingAreaState.Tools.PENCIL:
                            // add one more point to end
                            Point ms = drawingArea.mousepos_start;
                            Point mn = drawingArea.mousepos_now;

                            DrawingAreaState.PathObject path = (DrawingAreaState.PathObject)obj;
                            path.anotherXposs.Add((float)(mn.X) / (float)drawingArea.areaSize.X);
                            path.anotherYposs.Add((float)(mn.Y) / (float)drawingArea.areaSize.Y);

                            client.clientMachine.SendPath((DrawingAreaState.PathObject)obj);
                            break;
                        case DrawingAreaState.Tools.LINE:
                            client.clientMachine.SendPath((DrawingAreaState.PathObject)obj);
                            break;
                    }
                }

                drawingArea.tempObject = null;

                drawingArea.state = DrawingAreaState.State.IDLE;
            }
            drawingArea.Exit();



            drawing.Refresh();
           // RefreshTimer.Interval = 100;
        }

        private void RefreshTimer_Tick(object sender, EventArgs e)
        {
           

            drawing.Refresh();
        }

        private void FPSTimer_Tick(object sender, EventArgs e)
        {
            FPS = framesCounted;
            framesCounted = 0;
        }

        private void rectangleButton_Click(object sender, EventArgs e)
        {
            DrawingAreaState drawingArea = client.clientMachine.drawingArea;

            drawingArea.Access();
            drawingArea.selectedTool = DrawingAreaState.Tools.RECTANGLE;
            drawingArea.Exit();
        }

        private void lineButton_Click(object sender, EventArgs e)
        {
            DrawingAreaState drawingArea = client.clientMachine.drawingArea;

            drawingArea.Access();
            drawingArea.selectedTool = DrawingAreaState.Tools.LINE;
            drawingArea.Exit();
        }

        private void buttonDisconnect_Click(object sender, EventArgs e)
        {
            buttonDisconnect.Enabled = false;
            client.Disconnect();
        }

        private void buttonConnect_Click(object sender, EventArgs e)
        {
            int port;
            if(Int32.TryParse(textBoxPort.Text, out port) == false)
            {
                ChatBoxWriteLine("Numer portu musi być liczbą");
                return;
            }
            textBoxHostname.Enabled = false;
            textBoxPort.Enabled = false;
            buttonConnect.Enabled = false;
            client.Connect(textBoxHostname.Text, port);
        }

        private void buttonLogout_Click(object sender, EventArgs e)
        {
            buttonLogout.Enabled = false;
            client.clientMachine.Logout();
        }

        private void buttonLogin_Click(object sender, EventArgs e)
        {
            textBoxUsername.Enabled = false;
            textBoxPassword.Enabled = false;
            buttonLogin.Enabled = false;
            client.clientMachine.Login(textBoxUsername.Text, textBoxPassword.Text);
        }

        private void drawing_Resize(object sender, EventArgs e)
        {
            client.clientMachine.drawingArea.ChangeAreaSize(drawing.Size);
        }

        private void FGColorButton_Click(object sender, EventArgs e)
        {
            FGColor.ShowDialog();

            DrawingAreaState drawingArea = client.clientMachine.drawingArea;

            drawingArea.Access();
            drawingArea.ForegroundColor = Color.FromArgb(drawingArea.ForegroundColor.A, FGColor.Color);
            drawingArea.Exit();

            FGColorButton.BackColor = FGColor.Color;
        }

        private void BGColorButton_Click(object sender, EventArgs e)
        {
            BGColor.ShowDialog();

            DrawingAreaState drawingArea = client.clientMachine.drawingArea;

            drawingArea.Access();
            drawingArea.BackgroundColor = Color.FromArgb(drawingArea.BackgroundColor.A, BGColor.Color);
            drawingArea.Exit();

            BGColorButton.BackColor = BGColor.Color;
        }

        private void FGAlphaBar_Scroll(object sender, EventArgs e)
        {
            DrawingAreaState drawingArea = client.clientMachine.drawingArea;
            drawingArea.ForegroundColor = Color.FromArgb(FGAlphaBar.Value, drawingArea.ForegroundColor);
            FGAlphaLabel.Text = "Alpha: " + FGAlphaBar.Value.ToString();
        }

        private void BGAlphaBar_Scroll(object sender, EventArgs e)
        {
            DrawingAreaState drawingArea = client.clientMachine.drawingArea;
            drawingArea.BackgroundColor = Color.FromArgb(BGAlphaBar.Value, drawingArea.BackgroundColor);
            BGAlphaLabel.Text = "Alpha: " + BGAlphaBar.Value.ToString();
        }

        private void ThicknessBar_Scroll(object sender, EventArgs e)
        {
            DrawingAreaState drawingArea = client.clientMachine.drawingArea;

            drawingArea.Access();
            drawingArea.thickness = (float)ThicknessBar.Value / 1000.0f;
            drawingArea.Exit();

            ThicknessLabel.Text = "Grubość: " + drawingArea.thickness.ToString("0.000");
        }

        private void messageBox_TextChanged(object sender, EventArgs e)
        {

        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
           if(client.connected) client.Disconnect();
        }

        private void elipseButton_Click(object sender, EventArgs e)
        {
            DrawingAreaState drawingArea = client.clientMachine.drawingArea;

            drawingArea.Access();
            drawingArea.selectedTool = DrawingAreaState.Tools.OVAL;
            drawingArea.Exit();
        }

        private void penButton_Click(object sender, EventArgs e)
        {
            DrawingAreaState drawingArea = client.clientMachine.drawingArea;

            drawingArea.Access();
            drawingArea.selectedTool = DrawingAreaState.Tools.PENCIL;
            drawingArea.Exit();
        }
    }
}
