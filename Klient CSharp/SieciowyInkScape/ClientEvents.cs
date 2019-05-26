using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SieciowyInkScape
{
    partial class Client
    {
        public class LogicErrorEventArgs : EventArgs
        {
            public enum Errors
            {
                BAD_HEADER, BAD_DATA, SEND_TIMEDOUT, PACKET_TOO_LONG, RECEIVE_REQUEST_TOO_LARGE, RECEIVED_TOO_MUCH, SOCKET_CLOSED, SEND_FAILURE
            }

            public LogicErrorEventArgs(Errors error, bool critical)
            {
                this.error = error;
                this.critical = critical;
            }
            public Errors error;
            public bool critical;
        }
        public delegate void OnLogicError(object sender, LogicErrorEventArgs e);
        public event OnLogicError LogicErrorHappened;

        public class ConnectionFailedEventArgs : EventArgs
        {
            public enum ConnectionFailReasons
            {
                socketException, otherException, socketNotCreated, alreadyConnected, alreadyConnecting, criticalLogicErrorHappened
            }

            public ConnectionFailedEventArgs(System.Net.Sockets.SocketException socketException)
            {
                this.reason = ConnectionFailReasons.socketException;
                this.socketException = socketException;
            }
            public ConnectionFailedEventArgs(Exception exception)
            {
                this.reason = ConnectionFailReasons.otherException;
                this.exception = exception;
            }
            public ConnectionFailedEventArgs(ConnectionFailReasons reason)
            {
                if (reason == ConnectionFailReasons.socketException) throw new Exception("Należy podać socketException zamiast ustawiać ręcznie reason na socketException!");
                this.reason = reason;
            }
            public System.Net.Sockets.SocketException socketException;
            public Exception exception;
            public ConnectionFailReasons reason;
        }
        public delegate void OnConnectionFail(object sender, ConnectionFailedEventArgs e);
        public event OnConnectionFail ConnectionFailed;

        public delegate void OnConnectionSucceeded(object sender, EventArgs e);
        public event OnConnectionSucceeded ConnectionSucceeded;

        public delegate void OnLogoutCompleted(object sender, EventArgs e);
        public event OnLogoutCompleted LogoutCompleted;

        public delegate void OnLoginComplete(object sender, EventArgs e);
        public event OnLoginComplete LoginCompleted;

        public class LoginFailedEventArgs : EventArgs
        {
            public enum LoginFailReasons
            {
                badl, band, wasl, acoc, loer, nlog, LNun, LNpw
            }
            public LoginFailedEventArgs(LoginFailReasons reason, string serverMessage)
            {
                this.reason = reason;
                this.serverMessage = serverMessage;
            }
            public LoginFailReasons reason;
            public string serverMessage;
        }
        public delegate void OnLoginFail(object sender, LoginFailedEventArgs e);
        public event OnLoginFail LoginFailed;

        public class MessageInboundEventArgs : EventArgs
        {
            public MessageInboundEventArgs(string message, string username)
            {
                this.message = message;
                this.username = username;
            }
            public string username;
            public string message;
        }
        public delegate void OnMessageInbound(object sender, MessageInboundEventArgs e);
        public event OnMessageInbound MessageInbound;
    }
}
