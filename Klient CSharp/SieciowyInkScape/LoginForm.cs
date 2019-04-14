using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SieciowyInkScape
{
    public partial class LoginForm : Form
    {
        public string username;
        public string password;
        public string hostname;
        public int port;

        public LoginForm()
        {
            InitializeComponent();
        }

        private void LoginForm_Load(object sender, EventArgs e)
        {
            FormClosing += LoginForm_FormClosing;
        }

        private void LoginForm_FormClosing(object sender, EventArgs e)
        {
            
        }

        private void endButton_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }

        private void loginButton_Click(object sender, EventArgs e)
        {
            username = usernameEntry.Text;
            password = passwordEntry.Text;
            hostname = hostnameEntry.Text;
            port = System.Convert.ToInt32(portEntry.Text);

            DialogResult = DialogResult.OK;
        }
    }
}
