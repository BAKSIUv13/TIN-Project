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

        Client client;


        

        int framesCounted = 0;
        int FPS = 0;

        public MainForm()
        {
            InitializeComponent();

            
            client = new Client(this, drawing.Size);

           
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
                    if (client.Connect(loginForm.hostname, loginForm.port) == false)
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



       

        private void messageBox_KeyPressed(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == '\r')
            {
                client.clientMachine.SendChatMessage(messageBox.Text);
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
            DrawingAreaState drawingArea = client.clientMachine.drawingArea;

            if (!(drawingArea is null))
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
            DrawingAreaState drawingArea = client.clientMachine.drawingArea;

            if (drawingArea.state == DrawingAreaState.State.IDLE)
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
            DrawingAreaState drawingArea = client.clientMachine.drawingArea;

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

            /*
            drawingArea.Access();
            drawingArea.mousePositions["nkpkiller"] = new DrawingAreaState.MousePosition((float)(e.X) / (float)drawingArea.areaSize.X, (float)(e.Y) / (float)drawingArea.areaSize.Y, "nkpkiller");
            drawingArea.Exit();
            */

            drawing.Refresh();
        }

        private void drawing_MouseUp(object sender, MouseEventArgs e)
        {
            DrawingAreaState drawingArea = client.clientMachine.drawingArea;

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
            DrawingAreaState drawingArea = client.clientMachine.drawingArea;

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
            DrawingAreaState drawingArea = client.clientMachine.drawingArea;

            drawingArea.selectedTool = DrawingAreaState.Tools.RECTANGLE;
        }

        private void lineButton_Click(object sender, EventArgs e)
        {
            DrawingAreaState drawingArea = client.clientMachine.drawingArea;

            drawingArea.selectedTool = DrawingAreaState.Tools.LINE;
        }
    }
}
