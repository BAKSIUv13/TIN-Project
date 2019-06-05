using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;

namespace SieciowyInkScape
{
    partial class Client
    {
        void ReadShape(int IDToAppend)
        {
            string objectType = SocketReceiveString(4);

            if (objectType == "rect")
            {
                byte B_R = SocketReceiveByte();
                byte B_G = SocketReceiveByte();
                byte B_B = SocketReceiveByte();
                byte B_A = SocketReceiveByte();
                byte F_R = SocketReceiveByte();
                byte F_G = SocketReceiveByte();
                byte F_B = SocketReceiveByte();
                byte F_A = SocketReceiveByte();

                double thickness = SocketReceiveDouble();

                double xpos = SocketReceiveDouble();
                double ypos = SocketReceiveDouble();
                double width = SocketReceiveDouble();
                double height = SocketReceiveDouble();

                clientMachine.drawingArea.Access();
                clientMachine.drawingArea.objects.Add(new DrawingAreaState.RectangleObject(IDToAppend, (float)xpos, (float)ypos, (float)(width), (float)(height), (float)thickness, System.Drawing.Color.FromArgb(F_A, F_R, F_G, F_B), System.Drawing.Color.FromArgb(B_A, B_R, B_G, B_B)));
                clientMachine.drawingArea.Exit();
            }
            else if (objectType == "oval")
            {
                byte B_R = SocketReceiveByte();
                byte B_G = SocketReceiveByte();
                byte B_B = SocketReceiveByte();
                byte B_A = SocketReceiveByte();
                byte F_R = SocketReceiveByte();
                byte F_G = SocketReceiveByte();
                byte F_B = SocketReceiveByte();
                byte F_A = SocketReceiveByte();

                double thickness = SocketReceiveDouble();

                double xpos = SocketReceiveDouble();
                double ypos = SocketReceiveDouble();
                double width = SocketReceiveDouble();
                double height = SocketReceiveDouble();

                clientMachine.drawingArea.Access();
                clientMachine.drawingArea.objects.Add(new DrawingAreaState.OvalObject(IDToAppend, (float)xpos, (float)ypos, (float)(width), (float)(height), (float)thickness, System.Drawing.Color.FromArgb(F_A, F_R, F_G, F_B), System.Drawing.Color.FromArgb(B_A, B_R, B_G, B_B)));
                clientMachine.drawingArea.Exit();
            }
            else if (objectType == "path")
            {
                byte R = SocketReceiveByte();
                byte G = SocketReceiveByte();
                byte B = SocketReceiveByte();
                byte A = SocketReceiveByte();

                double thickness = SocketReceiveDouble();

                UInt32 pointCount;
                pointCount = SocketReceiveUInt32();

                double xpos = SocketReceiveDouble();
                double ypos = SocketReceiveDouble();
                pointCount--;

                List<float> anotherXposs = new List<float>();
                List<float> anotherYposs = new List<float>();

                while (pointCount-- > 0)
                {
                    anotherXposs.Add((float)SocketReceiveDouble());
                    anotherYposs.Add((float)SocketReceiveDouble());
                }
                

                clientMachine.drawingArea.Access();
                clientMachine.drawingArea.objects.Add(new DrawingAreaState.PathObject(IDToAppend, (float)xpos, (float)ypos, anotherXposs, anotherYposs, (float)thickness, System.Drawing.Color.FromArgb(A, R, G, B)));
                clientMachine.drawingArea.Exit();
            }
        }
        void SocketReceiverThread()
        {
            try
            {
                while (true)
                {

                    string header = SocketReceiveString(4);
                    if (header.Equals("OwO!")) // początek komunikatu
                    {
                        string messageType = SocketReceiveString(4);

                        if (messageType.Equals("MESG"))
                        {
                            UInt32 usernameLength;
                            string username;
                            UInt32 messageLength;
                            string message;

                            usernameLength = SocketReceiveUInt32();
                            username = SocketReceiveString(usernameLength);
                            messageLength = SocketReceiveUInt32();
                            message = SocketReceiveString(messageLength);
                            
                            mainForm.Invoke(new Action(() =>
                            {
                                MessageInboundEventArgs args = new MessageInboundEventArgs(message, username);
                                MessageInbound(this, args);
                            }
                            ));
                        }
                        else if (messageType.Equals("LSUS"))
                        {
                            List<string> userList = new List<string>();

                            UInt32 userCount;
                            userCount = SocketReceiveUInt32();

                            while (userCount-- > 0)
                            {
                                UInt32 usernameLength;
                                string username;

                                usernameLength = SocketReceiveUInt32();
                                username = SocketReceiveString(usernameLength);

                                userList.Add(username);
                            }

                            mainForm.Invoke(new Action(() =>
                            {
                                UserListInboundEventArgs args = new UserListInboundEventArgs(userList);
                                UserListInbound(this, args);
                            }
                            ));
                        }
                        else if (messageType.Equals("MAUS"))
                        {
                            double xpos = SocketReceiveDouble();
                            double ypos = SocketReceiveDouble();
                            UInt32 usernameLength = SocketReceiveUInt32();
                            string username = SocketReceiveString(usernameLength);

                            clientMachine.drawingArea.Access();
                            clientMachine.drawingArea.mousePositions[username] = new DrawingAreaState.MousePosition((float)xpos, (float)ypos, username, DateTime.Now);
                            clientMachine.drawingArea.Exit();
                        }
                        else if (messageType.Equals("LSSH"))
                        {
                            UInt32 shapeCount;
                            shapeCount = SocketReceiveUInt32();

                            while (shapeCount-- > 0)
                            {
                                int ID = SocketReceiveInt32();

                                ReadShape(ID);
                            }

                        }
                        else if (messageType.Equals("NEWW"))
                        {
                            int ID = SocketReceiveInt32();
                            UInt32 usernameLength = SocketReceiveUInt32();
                            string username = SocketReceiveString(usernameLength);

                            if (username == loggedUsername)
                            {
                                clientMachine.drawingArea.Access();
                                if (clientMachine.drawingArea.pendingObjects.Count > 0)
                                    clientMachine.drawingArea.pendingObjects.Dequeue();
                                clientMachine.drawingArea.Exit();
                            }

                            ReadShape(ID);
                        }
                        else if (messageType.Equals("CLRR"))
                        {
                            clientMachine.drawingArea.Access();
                            clientMachine.drawingArea.objects.Clear();
                            clientMachine.drawingArea.Exit();
                        }
                        else if (messageType.Equals("USER"))
                        {
                            string USER_type;
                            USER_type = SocketReceiveString(4);

                            if (USER_type == "Ulin")
                            {
                                UInt32 usernameLength;
                                string username;

                                usernameLength = SocketReceiveUInt32();
                                username = SocketReceiveString(usernameLength);
                                //  username = SocketReceiveString(6);
                                mainForm.Invoke(new Action(() =>
                                {
                                    ServerMessageInboundEventArgs args = new ServerMessageInboundEventArgs(username + " połączył się.");
                                    ServerMessageInbound(this, args);
                                }
                                ));
                            }
                            else if (USER_type == "Ulof")
                            {
                                UInt32 usernameLength;
                                string username;

                                usernameLength = SocketReceiveUInt32();
                                username = SocketReceiveString(usernameLength);

                                mainForm.Invoke(new Action(() =>
                                {
                                    ServerMessageInboundEventArgs args = new ServerMessageInboundEventArgs(username + " odłączył się.");
                                    ServerMessageInbound(this, args);
                                }
                                ));
                            }
                        }
                        else if (messageType.Equals("USER"))
                        {
                            string USER_type;
                            USER_type = SocketReceiveString(4);

                            if (USER_type == "Ulin")
                            {
                                UInt32 usernameLength;
                                string username;

                                usernameLength = SocketReceiveUInt32();
                                username = SocketReceiveString(usernameLength);

                                mainForm.Invoke(new Action(() =>
                                {
                                    ServerMessageInboundEventArgs args = new ServerMessageInboundEventArgs(username + " połączył się.");
                                    ServerMessageInbound(this, args);
                                }
                                ));
                            }
                            else if (USER_type == "Ulof")
                            {
                                UInt32 usernameLength;
                                string username;

                                 usernameLength = SocketReceiveUInt32();
                                 username = SocketReceiveString(usernameLength);

                                mainForm.Invoke(new Action(() =>
                                {
                                    ServerMessageInboundEventArgs args = new ServerMessageInboundEventArgs(username + " odłączył się.");
                                    ServerMessageInbound(this, args);
                                }
                                ));
                            }

                            clientMachine.SendUserListRequest();
                        }


                        else if (messageType.Equals("LGOK"))
                        {
                            loggedUsername = clientMachine.UsedNick;
                            loggedIn = true;

                            clientMachine.drawingArea.Access();
                            clientMachine.drawingArea.objects.Clear();
                            clientMachine.drawingArea.Exit();

                            clientMachine.SendShapeListRequest();

                            mainForm.Invoke(new Action(() =>
                            {
                                EventArgs arg = new EventArgs();
                                LoginCompleted(this, arg);
                            }
                            ));
                        }
                        else if (messageType.Equals("SIGL") || messageType.Equals("SIGH"))
                        {
                            string errorCode;
                            UInt32 messageLength;
                            string message;

                            errorCode = SocketReceiveString(4);
                            messageLength = SocketReceiveUInt32();
                            message = SocketReceiveString(messageLength);

                            loggedIn = false;

                            mainForm.Invoke(new Action(() =>
                            {
                                LoginFailedEventArgs arg = new LoginFailedEventArgs((LoginFailedEventArgs.LoginFailReasons)Enum.Parse(typeof(LoginFailedEventArgs.LoginFailReasons), errorCode), message);
                                LoginFailed(this, arg);
                            }
                            ));
                        }
                        else if (messageType.Equals("LOFF"))
                        {
                            loggedUsername = "";
                            loggedIn = false;

                            mainForm.Invoke(new Action(() =>
                            {
                                EventArgs arg = new EventArgs();
                                LogoutCompleted(this, arg);
                            }
                            ));
                        }
                        else
                        {
                            mainForm.Invoke(new Action(() =>
                            {
                                LogicErrorEventArgs arg = new LogicErrorEventArgs(LogicErrorEventArgs.Errors.BAD_DATA, true);
                                LogicErrorHappened(this, arg);
                            }
                            ));
                            Disconnect();
                            return;
                        }
                    }
                    else
                    {
                        mainForm.Invoke(new Action(() =>
                        {
                            LogicErrorEventArgs arg = new LogicErrorEventArgs(LogicErrorEventArgs.Errors.BAD_HEADER, true);
                            LogicErrorHappened(this, arg);
                        }
                        ));
                        Disconnect();
                        return;
                    }
                }
            }
            catch (SocketException sEx)
            {
                mainForm.Invoke(new Action(() =>
                {
                    ConnectionFailedEventArgs arg = new ConnectionFailedEventArgs(sEx);
                    ConnectionFailed(this, arg);
                }
                ));
                Disconnect();
            }
            catch (Exception ex)
            {
                if(connected)
                {
                    mainForm.Invoke(new Action(() =>
                    {
                        ConnectionFailedEventArgs arg = new ConnectionFailedEventArgs(ex);
                        ConnectionFailed(this, arg);
                    }
                    ));
                    Disconnect();
                }
            }
        }
    }
}
