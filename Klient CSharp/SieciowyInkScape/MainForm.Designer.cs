namespace SieciowyInkScape
{
    partial class MainForm
    {
        /// <summary>
        /// Wymagana zmienna projektanta.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Wyczyść wszystkie używane zasoby.
        /// </summary>
        /// <param name="disposing">prawda, jeżeli zarządzane zasoby powinny zostać zlikwidowane; Fałsz w przeciwnym wypadku.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Kod generowany przez Projektanta formularzy systemu Windows

        /// <summary>
        /// Metoda wymagana do obsługi projektanta — nie należy modyfikować
        /// jej zawartości w edytorze kodu.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.chatBox = new System.Windows.Forms.TextBox();
            this.chatPanel = new System.Windows.Forms.Panel();
            this.label7 = new System.Windows.Forms.Label();
            this.userListBox = new System.Windows.Forms.TextBox();
            this.messageBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.ThicknessLabel = new System.Windows.Forms.Label();
            this.ThicknessBar = new System.Windows.Forms.TrackBar();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.label9 = new System.Windows.Forms.Label();
            this.BGAlphaLabel = new System.Windows.Forms.Label();
            this.BGColorButton = new System.Windows.Forms.Button();
            this.BGAlphaBar = new System.Windows.Forms.TrackBar();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label8 = new System.Windows.Forms.Label();
            this.FGAlphaLabel = new System.Windows.Forms.Label();
            this.FGColorButton = new System.Windows.Forms.Button();
            this.FGAlphaBar = new System.Windows.Forms.TrackBar();
            this.lineButton = new System.Windows.Forms.Button();
            this.drawing = new System.Windows.Forms.Panel();
            this.RefreshTimer = new System.Windows.Forms.Timer(this.components);
            this.FPSTimer = new System.Windows.Forms.Timer(this.components);
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.textBoxPort = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.textBoxHostname = new System.Windows.Forms.TextBox();
            this.buttonDisconnect = new System.Windows.Forms.Button();
            this.buttonConnect = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.buttonLogout = new System.Windows.Forms.Button();
            this.buttonLogin = new System.Windows.Forms.Button();
            this.textBoxPassword = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.textBoxUsername = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.FGColor = new System.Windows.Forms.ColorDialog();
            this.BGColor = new System.Windows.Forms.ColorDialog();
            this.panel2 = new System.Windows.Forms.Panel();
            this.penButton = new System.Windows.Forms.Button();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.elipseButton = new System.Windows.Forms.Button();
            this.rectangleButton = new System.Windows.Forms.Button();
            this.chatPanel.SuspendLayout();
            this.groupBox5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ThicknessBar)).BeginInit();
            this.groupBox4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.BGAlphaBar)).BeginInit();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.FGAlphaBar)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // chatBox
            // 
            this.chatBox.BackColor = System.Drawing.Color.White;
            this.chatBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.chatBox.Cursor = System.Windows.Forms.Cursors.Arrow;
            this.chatBox.ForeColor = System.Drawing.Color.Black;
            this.chatBox.Location = new System.Drawing.Point(3, 301);
            this.chatBox.Multiline = true;
            this.chatBox.Name = "chatBox";
            this.chatBox.ReadOnly = true;
            this.chatBox.Size = new System.Drawing.Size(295, 339);
            this.chatBox.TabIndex = 0;
            // 
            // chatPanel
            // 
            this.chatPanel.Controls.Add(this.label7);
            this.chatPanel.Controls.Add(this.userListBox);
            this.chatPanel.Controls.Add(this.messageBox);
            this.chatPanel.Controls.Add(this.label2);
            this.chatPanel.Controls.Add(this.label1);
            this.chatPanel.Controls.Add(this.chatBox);
            this.chatPanel.Dock = System.Windows.Forms.DockStyle.Right;
            this.chatPanel.Location = new System.Drawing.Point(963, 0);
            this.chatPanel.Name = "chatPanel";
            this.chatPanel.Size = new System.Drawing.Size(301, 682);
            this.chatPanel.TabIndex = 1;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(3, 3);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(94, 13);
            this.label7.TabIndex = 5;
            this.label7.Text = "Lista użytkoników:";
            // 
            // userListBox
            // 
            this.userListBox.BackColor = System.Drawing.Color.White;
            this.userListBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.userListBox.Cursor = System.Windows.Forms.Cursors.Arrow;
            this.userListBox.ForeColor = System.Drawing.Color.Black;
            this.userListBox.Location = new System.Drawing.Point(3, 19);
            this.userListBox.Multiline = true;
            this.userListBox.Name = "userListBox";
            this.userListBox.ReadOnly = true;
            this.userListBox.Size = new System.Drawing.Size(295, 244);
            this.userListBox.TabIndex = 4;
            // 
            // messageBox
            // 
            this.messageBox.Location = new System.Drawing.Point(3, 659);
            this.messageBox.Name = "messageBox";
            this.messageBox.Size = new System.Drawing.Size(295, 20);
            this.messageBox.TabIndex = 3;
            this.messageBox.TextChanged += new System.EventHandler(this.messageBox_TextChanged);
            this.messageBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.messageBox_KeyPressed);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 643);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(98, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Napisz wiadomość:";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 285);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(32, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Chat:";
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.ThicknessLabel);
            this.groupBox5.Controls.Add(this.ThicknessBar);
            this.groupBox5.Location = new System.Drawing.Point(3, 206);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(92, 125);
            this.groupBox5.TabIndex = 12;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Parametry";
            // 
            // ThicknessLabel
            // 
            this.ThicknessLabel.AutoSize = true;
            this.ThicknessLabel.Location = new System.Drawing.Point(6, 16);
            this.ThicknessLabel.Name = "ThicknessLabel";
            this.ThicknessLabel.Size = new System.Drawing.Size(80, 13);
            this.ThicknessLabel.TabIndex = 9;
            this.ThicknessLabel.Text = "Grubość: 0,001";
            // 
            // ThicknessBar
            // 
            this.ThicknessBar.LargeChange = 10;
            this.ThicknessBar.Location = new System.Drawing.Point(9, 32);
            this.ThicknessBar.Maximum = 25;
            this.ThicknessBar.Minimum = 1;
            this.ThicknessBar.Name = "ThicknessBar";
            this.ThicknessBar.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.ThicknessBar.Size = new System.Drawing.Size(45, 78);
            this.ThicknessBar.TabIndex = 7;
            this.ThicknessBar.TickStyle = System.Windows.Forms.TickStyle.None;
            this.ThicknessBar.Value = 1;
            this.ThicknessBar.Scroll += new System.EventHandler(this.ThicknessBar_Scroll);
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.label9);
            this.groupBox4.Controls.Add(this.BGAlphaLabel);
            this.groupBox4.Controls.Add(this.BGColorButton);
            this.groupBox4.Controls.Add(this.BGAlphaBar);
            this.groupBox4.Location = new System.Drawing.Point(143, 75);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(134, 125);
            this.groupBox4.TabIndex = 11;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Kolor tła";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(69, 16);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(48, 13);
            this.label9.TabIndex = 10;
            this.label9.Text = "Wybierz:";
            // 
            // BGAlphaLabel
            // 
            this.BGAlphaLabel.AutoSize = true;
            this.BGAlphaLabel.Location = new System.Drawing.Point(6, 16);
            this.BGAlphaLabel.Name = "BGAlphaLabel";
            this.BGAlphaLabel.Size = new System.Drawing.Size(46, 13);
            this.BGAlphaLabel.TabIndex = 9;
            this.BGAlphaLabel.Text = "Alpha: 0";
            // 
            // BGColorButton
            // 
            this.BGColorButton.BackColor = System.Drawing.Color.White;
            this.BGColorButton.Location = new System.Drawing.Point(72, 32);
            this.BGColorButton.Name = "BGColorButton";
            this.BGColorButton.Size = new System.Drawing.Size(45, 23);
            this.BGColorButton.TabIndex = 8;
            this.BGColorButton.UseVisualStyleBackColor = false;
            this.BGColorButton.Click += new System.EventHandler(this.BGColorButton_Click);
            // 
            // BGAlphaBar
            // 
            this.BGAlphaBar.LargeChange = 25;
            this.BGAlphaBar.Location = new System.Drawing.Point(9, 32);
            this.BGAlphaBar.Maximum = 255;
            this.BGAlphaBar.Name = "BGAlphaBar";
            this.BGAlphaBar.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.BGAlphaBar.Size = new System.Drawing.Size(45, 78);
            this.BGAlphaBar.TabIndex = 7;
            this.BGAlphaBar.TickStyle = System.Windows.Forms.TickStyle.None;
            this.BGAlphaBar.Scroll += new System.EventHandler(this.BGAlphaBar_Scroll);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label8);
            this.groupBox3.Controls.Add(this.FGAlphaLabel);
            this.groupBox3.Controls.Add(this.FGColorButton);
            this.groupBox3.Controls.Add(this.FGAlphaBar);
            this.groupBox3.Location = new System.Drawing.Point(3, 75);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(134, 125);
            this.groupBox3.TabIndex = 8;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Kolor pierwszego planu";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(69, 16);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(48, 13);
            this.label8.TabIndex = 10;
            this.label8.Text = "Wybierz:";
            // 
            // FGAlphaLabel
            // 
            this.FGAlphaLabel.AutoSize = true;
            this.FGAlphaLabel.Location = new System.Drawing.Point(6, 16);
            this.FGAlphaLabel.Name = "FGAlphaLabel";
            this.FGAlphaLabel.Size = new System.Drawing.Size(58, 13);
            this.FGAlphaLabel.TabIndex = 9;
            this.FGAlphaLabel.Text = "Alpha: 255";
            // 
            // FGColorButton
            // 
            this.FGColorButton.BackColor = System.Drawing.Color.Black;
            this.FGColorButton.Location = new System.Drawing.Point(72, 32);
            this.FGColorButton.Name = "FGColorButton";
            this.FGColorButton.Size = new System.Drawing.Size(45, 23);
            this.FGColorButton.TabIndex = 8;
            this.FGColorButton.Text = "Wybierz...";
            this.FGColorButton.UseVisualStyleBackColor = false;
            this.FGColorButton.Click += new System.EventHandler(this.FGColorButton_Click);
            // 
            // FGAlphaBar
            // 
            this.FGAlphaBar.LargeChange = 25;
            this.FGAlphaBar.Location = new System.Drawing.Point(9, 32);
            this.FGAlphaBar.Maximum = 255;
            this.FGAlphaBar.Name = "FGAlphaBar";
            this.FGAlphaBar.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.FGAlphaBar.Size = new System.Drawing.Size(45, 78);
            this.FGAlphaBar.TabIndex = 7;
            this.FGAlphaBar.TickStyle = System.Windows.Forms.TickStyle.None;
            this.FGAlphaBar.Value = 255;
            this.FGAlphaBar.Scroll += new System.EventHandler(this.FGAlphaBar_Scroll);
            // 
            // lineButton
            // 
            this.lineButton.Location = new System.Drawing.Point(3, 32);
            this.lineButton.Name = "lineButton";
            this.lineButton.Size = new System.Drawing.Size(75, 23);
            this.lineButton.TabIndex = 3;
            this.lineButton.Text = "Linia";
            this.lineButton.UseVisualStyleBackColor = true;
            this.lineButton.Click += new System.EventHandler(this.lineButton_Click);
            // 
            // drawing
            // 
            this.drawing.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.drawing.AutoSize = true;
            this.drawing.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.drawing.Location = new System.Drawing.Point(6, 85);
            this.drawing.Name = "drawing";
            this.drawing.Size = new System.Drawing.Size(660, 590);
            this.drawing.TabIndex = 2;
            this.drawing.Paint += new System.Windows.Forms.PaintEventHandler(this.drawing_Paint);
            this.drawing.MouseDown += new System.Windows.Forms.MouseEventHandler(this.drawing_MouseDown);
            this.drawing.MouseMove += new System.Windows.Forms.MouseEventHandler(this.drawing_MouseMove);
            this.drawing.MouseUp += new System.Windows.Forms.MouseEventHandler(this.drawing_MouseUp);
            this.drawing.Resize += new System.EventHandler(this.drawing_Resize);
            // 
            // RefreshTimer
            // 
            this.RefreshTimer.Enabled = true;
            this.RefreshTimer.Tick += new System.EventHandler(this.RefreshTimer_Tick);
            // 
            // FPSTimer
            // 
            this.FPSTimer.Enabled = true;
            this.FPSTimer.Interval = 1000;
            this.FPSTimer.Tick += new System.EventHandler(this.FPSTimer_Tick);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.textBoxPort);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.textBoxHostname);
            this.groupBox1.Controls.Add(this.buttonDisconnect);
            this.groupBox1.Controls.Add(this.buttonConnect);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Location = new System.Drawing.Point(3, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(365, 75);
            this.groupBox1.TabIndex = 7;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Połączenie";
            // 
            // textBoxPort
            // 
            this.textBoxPort.Location = new System.Drawing.Point(285, 17);
            this.textBoxPort.Name = "textBoxPort";
            this.textBoxPort.Size = new System.Drawing.Size(74, 20);
            this.textBoxPort.TabIndex = 5;
            this.textBoxPort.Text = "59898";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(250, 21);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(29, 13);
            this.label4.TabIndex = 4;
            this.label4.Text = "Port:";
            // 
            // textBoxHostname
            // 
            this.textBoxHostname.Location = new System.Drawing.Point(45, 17);
            this.textBoxHostname.Name = "textBoxHostname";
            this.textBoxHostname.Size = new System.Drawing.Size(178, 20);
            this.textBoxHostname.TabIndex = 3;
            this.textBoxHostname.Text = "192.168.0.52";
            // 
            // buttonDisconnect
            // 
            this.buttonDisconnect.Enabled = false;
            this.buttonDisconnect.Location = new System.Drawing.Point(204, 46);
            this.buttonDisconnect.Name = "buttonDisconnect";
            this.buttonDisconnect.Size = new System.Drawing.Size(75, 23);
            this.buttonDisconnect.TabIndex = 2;
            this.buttonDisconnect.Text = "Rozłącz";
            this.buttonDisconnect.UseVisualStyleBackColor = true;
            this.buttonDisconnect.Click += new System.EventHandler(this.buttonDisconnect_Click);
            // 
            // buttonConnect
            // 
            this.buttonConnect.Location = new System.Drawing.Point(285, 46);
            this.buttonConnect.Name = "buttonConnect";
            this.buttonConnect.Size = new System.Drawing.Size(75, 23);
            this.buttonConnect.TabIndex = 1;
            this.buttonConnect.Text = "Połącz";
            this.buttonConnect.UseVisualStyleBackColor = true;
            this.buttonConnect.Click += new System.EventHandler(this.buttonConnect_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(7, 20);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(32, 13);
            this.label3.TabIndex = 0;
            this.label3.Text = "Host:";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.buttonLogout);
            this.groupBox2.Controls.Add(this.buttonLogin);
            this.groupBox2.Controls.Add(this.textBoxPassword);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.textBoxUsername);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Location = new System.Drawing.Point(374, 3);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(474, 75);
            this.groupBox2.TabIndex = 8;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Logowanie";
            this.groupBox2.Enter += new System.EventHandler(this.groupBox2_Enter);
            // 
            // buttonLogout
            // 
            this.buttonLogout.Enabled = false;
            this.buttonLogout.Location = new System.Drawing.Point(312, 46);
            this.buttonLogout.Name = "buttonLogout";
            this.buttonLogout.Size = new System.Drawing.Size(75, 23);
            this.buttonLogout.TabIndex = 5;
            this.buttonLogout.Text = "Wyloguj";
            this.buttonLogout.UseVisualStyleBackColor = true;
            this.buttonLogout.Click += new System.EventHandler(this.buttonLogout_Click);
            // 
            // buttonLogin
            // 
            this.buttonLogin.Enabled = false;
            this.buttonLogin.Location = new System.Drawing.Point(393, 46);
            this.buttonLogin.Name = "buttonLogin";
            this.buttonLogin.Size = new System.Drawing.Size(75, 23);
            this.buttonLogin.TabIndex = 4;
            this.buttonLogin.Text = "Zaloguj";
            this.buttonLogin.UseVisualStyleBackColor = true;
            this.buttonLogin.Click += new System.EventHandler(this.buttonLogin_Click);
            // 
            // textBoxPassword
            // 
            this.textBoxPassword.Location = new System.Drawing.Point(287, 17);
            this.textBoxPassword.Name = "textBoxPassword";
            this.textBoxPassword.PasswordChar = '*';
            this.textBoxPassword.Size = new System.Drawing.Size(181, 20);
            this.textBoxPassword.TabIndex = 3;
            this.textBoxPassword.Text = "norbert1";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(242, 20);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(39, 13);
            this.label6.TabIndex = 2;
            this.label6.Text = "Hasło:";
            // 
            // textBoxUsername
            // 
            this.textBoxUsername.Location = new System.Drawing.Point(78, 17);
            this.textBoxUsername.Name = "textBoxUsername";
            this.textBoxUsername.Size = new System.Drawing.Size(136, 20);
            this.textBoxUsername.TabIndex = 1;
            this.textBoxUsername.Text = "nkp123";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 20);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(65, 13);
            this.label5.TabIndex = 0;
            this.label5.Text = "Użytkownik:";
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.groupBox1);
            this.panel1.Controls.Add(this.groupBox2);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(963, 79);
            this.panel1.TabIndex = 9;
            this.panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint);
            // 
            // BGColor
            // 
            this.BGColor.Color = System.Drawing.Color.White;
            // 
            // panel2
            // 
            this.panel2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.panel2.Controls.Add(this.penButton);
            this.panel2.Controls.Add(this.checkBox1);
            this.panel2.Controls.Add(this.elipseButton);
            this.panel2.Controls.Add(this.groupBox3);
            this.panel2.Controls.Add(this.groupBox4);
            this.panel2.Controls.Add(this.groupBox5);
            this.panel2.Controls.Add(this.lineButton);
            this.panel2.Controls.Add(this.rectangleButton);
            this.panel2.Location = new System.Drawing.Point(672, 85);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(285, 590);
            this.panel2.TabIndex = 10;
            // 
            // penButton
            // 
            this.penButton.Location = new System.Drawing.Point(84, 32);
            this.penButton.Name = "penButton";
            this.penButton.Size = new System.Drawing.Size(75, 23);
            this.penButton.TabIndex = 15;
            this.penButton.Text = "Ołówek";
            this.penButton.UseVisualStyleBackColor = true;
            this.penButton.Click += new System.EventHandler(this.penButton_Click);
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Location = new System.Drawing.Point(120, 259);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(140, 17);
            this.checkBox1.TabIndex = 14;
            this.checkBox1.Text = "Spamuj updejtami myszy";
            this.checkBox1.UseVisualStyleBackColor = true;
            // 
            // elipseButton
            // 
            this.elipseButton.Location = new System.Drawing.Point(84, 3);
            this.elipseButton.Name = "elipseButton";
            this.elipseButton.Size = new System.Drawing.Size(75, 23);
            this.elipseButton.TabIndex = 13;
            this.elipseButton.Text = "Elipsa";
            this.elipseButton.UseVisualStyleBackColor = true;
            this.elipseButton.Click += new System.EventHandler(this.elipseButton_Click);
            // 
            // rectangleButton
            // 
            this.rectangleButton.Location = new System.Drawing.Point(3, 3);
            this.rectangleButton.Name = "rectangleButton";
            this.rectangleButton.Size = new System.Drawing.Size(75, 23);
            this.rectangleButton.TabIndex = 6;
            this.rectangleButton.Text = "Prostokąt";
            this.rectangleButton.UseVisualStyleBackColor = true;
            this.rectangleButton.Click += new System.EventHandler(this.rectangleButton_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1264, 682);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.drawing);
            this.Controls.Add(this.chatPanel);
            this.DoubleBuffered = true;
            this.MinimumSize = new System.Drawing.Size(1280, 720);
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Sieciowy InkScape - Norbert Pietrucha-Kacprowicz";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.chatPanel.ResumeLayout(false);
            this.chatPanel.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ThicknessBar)).EndInit();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.BGAlphaBar)).EndInit();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.FGAlphaBar)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Panel chatPanel;
        private System.Windows.Forms.TextBox messageBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Panel drawing;
        private System.Windows.Forms.Button lineButton;
        private System.Windows.Forms.Timer RefreshTimer;
        private System.Windows.Forms.Timer FPSTimer;
        public System.Windows.Forms.TextBox chatBox;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TextBox textBoxPort;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBoxHostname;
        private System.Windows.Forms.Button buttonDisconnect;
        private System.Windows.Forms.Button buttonConnect;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button buttonLogout;
        private System.Windows.Forms.Button buttonLogin;
        private System.Windows.Forms.TextBox textBoxPassword;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox textBoxUsername;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TrackBar FGAlphaBar;
        private System.Windows.Forms.ColorDialog FGColor;
        private System.Windows.Forms.ColorDialog BGColor;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label BGAlphaLabel;
        private System.Windows.Forms.Button BGColorButton;
        private System.Windows.Forms.TrackBar BGAlphaBar;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label FGAlphaLabel;
        private System.Windows.Forms.Button FGColorButton;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.Label ThicknessLabel;
        private System.Windows.Forms.TrackBar ThicknessBar;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Label label7;
        public System.Windows.Forms.TextBox userListBox;
        private System.Windows.Forms.Button elipseButton;
        private System.Windows.Forms.Button rectangleButton;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.Button penButton;

    }
}

