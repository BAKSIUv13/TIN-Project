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
                        else if (messageType.Equals("MAUS"))
                        {
                            double xpos = SocketReceiveDouble();
                            double ypos = SocketReceiveDouble();
                            UInt32 usernameLength = SocketReceiveUInt32();
                            string username = SocketReceiveString(usernameLength);

                            clientMachine.drawingArea.Access();
                            clientMachine.drawingArea.mousePositions[username] = new DrawingAreaState.MousePosition((float)xpos, (float)ypos, username);
                            clientMachine.drawingArea.Exit();
                        }
                        else if (messageType.Equals("LGOK"))
                        {
                            loggedUsername = clientMachine.UsedNick;
                            loggedIn = true;

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
                                LogicErrorEventArgs arg = new LogicErrorEventArgs(LogicErrorEventArgs.Errors.BAD_DATA, false);
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
                            LogicErrorEventArgs arg = new LogicErrorEventArgs(LogicErrorEventArgs.Errors.BAD_HEADER, false);
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
