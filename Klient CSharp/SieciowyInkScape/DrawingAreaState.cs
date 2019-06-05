using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace SieciowyInkScape
{
    public class DrawingAreaState
    {
        Semaphore semaphore;
        public State state;
        

        
        public int drawingAckTime_ms = 250;
        public int mousePositionTimeOut_s = 30;


        public enum State
        {
            IDLE, DRAWING, EDITING
        }
        public enum Tools
        {
            NOTHING, MODIFY, PENCIL, LINE, RECTANGLE, OVAL
        }



        
        public DrawingAreaState(Point areaSize)
        {
            semaphore = new Semaphore(1, 1);
            this.areaSize = areaSize;
            this.state = State.IDLE;
            selectedTool = Tools.RECTANGLE;
            objects = new List<DrawingObject>();
            pendingObjects = new Queue<PendingObject>();
        }
        public void Access()
        {
            semaphore.WaitOne();
        }
        public void Exit()
        {
            semaphore.Release();
        }

        public void ChangeAreaSize(Point areaSize)
        {
            this.areaSize = areaSize;
        }
        public void ChangeAreaSize(Size areaSize)
        {
            this.areaSize = new Point(areaSize.Width, areaSize.Height);
        }

        public void FinalizeObject(MainForm main, DrawingObject obj)
        {
            // TODO - send object
            PendingObject pobj = new PendingObject(DateTime.Now, obj);
            pendingObjects.Enqueue(pobj);
        }


        public void CheckPendingObjects()
        {
            while (pendingObjects.Count > 0 && pendingObjects.Peek().timeStarted + new TimeSpan(0, 0, 0, 0, drawingAckTime_ms) < DateTime.Now)
            {
                pendingObjects.Dequeue();
            }
        }
        public void CheckPendingMousePositions()
        {
            bool modified = true;
            while(modified)
            {
                modified = false;
                foreach (MousePosition position in mousePositions.Values)
                {
                    if (position.timeReceived + new TimeSpan(0, 0, mousePositionTimeOut_s) < DateTime.Now)
                    {
                        modified = true;
                        mousePositions.Remove(position.username);
                        break;
                    }
                }
            }
            
        }



        public class PendingObject
        {
            public DateTime timeStarted;
            public DrawingObject obj;

            public PendingObject(DateTime timeStarted, DrawingObject obj)
            {
                this.timeStarted = timeStarted;
                this.obj = obj;
            }
        }

        public abstract class DrawingObject
        {
            public enum ObjectType
            {
                RECTANGLE, PATH, OVAL
            }

            public ObjectType objectType;

            public Color color;

            public Int32 ID;

            public float xpos;
            public float ypos;
        }
        public class PathObject : DrawingObject
        {
            public List<float> anotherXposs = new List<float>();
            public List<float> anotherYposs = new List<float>();

            public float thickness;

            public PathObject(Int32 ID, float xpos, float ypos, List<float> anotherXposs, List<float> anotherYposs, float thickness, Color color)
            {
                this.objectType = ObjectType.PATH;
                this.xpos = xpos;
                this.ypos = ypos;
                this.anotherXposs = anotherXposs;
                this.anotherYposs = anotherYposs;
                this.thickness = thickness;
                this.color = color;
            }
            public PathObject()
            {
            }
        }
        public class RectangleObject : DrawingObject
        {
            public float width;
            public float height;

            public float thickness;

            public Color BGColor;

            public RectangleObject(Int32 ID, float xpos, float ypos, float width, float height, float thickness, Color FGcolor, Color BGColor)
            {
                this.objectType = ObjectType.RECTANGLE;
                this.xpos = xpos;
                this.ypos = ypos;
                this.width = width;
                this.height = height;
                this.thickness = thickness;
                this.color = FGcolor;
                this.BGColor = BGColor;
            }
            public RectangleObject()
            {
            }
        }
        public class OvalObject : DrawingObject
        {
            public float width;
            public float height;

            public float thickness;

            public Color BGColor;

            public OvalObject(Int32 ID, float xpos, float ypos, float width, float height, float thickness, Color FGcolor, Color BGColor)
            {
                this.objectType = ObjectType.OVAL;
                this.xpos = xpos;
                this.ypos = ypos;
                this.width = width;
                this.height = height;
                this.thickness = thickness;
                this.color = FGcolor;
                this.BGColor = BGColor;
            }
            public OvalObject()
            {
            }
        }


        public class MousePosition
        {
            public float xpos;
            public float ypos;
            public string username;

            public DateTime timeReceived;

            public MousePosition(float xpos, float ypos, string username, DateTime timeReceived)
            {
                this.xpos = xpos;
                this.ypos = ypos;
                this.username = username;
                this.timeReceived = timeReceived;
            }
        }

        public Dictionary<string, MousePosition> mousePositions = new Dictionary<string, MousePosition>();
        public PointF mousePosition = new PointF(0.0f, 0.0f);

        public Point areaSize;

        public List<DrawingObject> objects;
        public Queue<PendingObject> pendingObjects;
        public DrawingObject tempObject;
        public Point mousepos_start;
        public Point mousepos_now;
        public Tools selectedTool;

        public Color ForegroundColor = Color.Black;
        public Color BackgroundColor = Color.FromArgb(0, Color.White);
        public float thickness = 0.001f;

    }
}
