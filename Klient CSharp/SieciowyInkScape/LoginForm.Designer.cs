namespace SieciowyInkScape
{
    partial class LoginForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.hostnameEntry = new System.Windows.Forms.TextBox();
            this.portEntry = new System.Windows.Forms.TextBox();
            this.usernameEntry = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.passwordEntry = new System.Windows.Forms.TextBox();
            this.loginButton = new System.Windows.Forms.Button();
            this.endButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(52, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(66, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Adres hosta:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(89, 39);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(29, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "Port:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(13, 65);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(105, 13);
            this.label3.TabIndex = 2;
            this.label3.Text = "Nazwa użytkownika:";
            // 
            // hostnameEntry
            // 
            this.hostnameEntry.Location = new System.Drawing.Point(124, 10);
            this.hostnameEntry.Name = "hostnameEntry";
            this.hostnameEntry.Size = new System.Drawing.Size(162, 20);
            this.hostnameEntry.TabIndex = 3;
            this.hostnameEntry.Text = "localhost";
            // 
            // portEntry
            // 
            this.portEntry.Location = new System.Drawing.Point(124, 36);
            this.portEntry.Name = "portEntry";
            this.portEntry.Size = new System.Drawing.Size(75, 20);
            this.portEntry.TabIndex = 4;
            this.portEntry.Text = "1234";
            // 
            // usernameEntry
            // 
            this.usernameEntry.Location = new System.Drawing.Point(124, 62);
            this.usernameEntry.Name = "usernameEntry";
            this.usernameEntry.Size = new System.Drawing.Size(162, 20);
            this.usernameEntry.TabIndex = 5;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(79, 91);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(39, 13);
            this.label4.TabIndex = 6;
            this.label4.Text = "Hasło:";
            // 
            // passwordEntry
            // 
            this.passwordEntry.Location = new System.Drawing.Point(124, 88);
            this.passwordEntry.Name = "passwordEntry";
            this.passwordEntry.PasswordChar = '*';
            this.passwordEntry.Size = new System.Drawing.Size(162, 20);
            this.passwordEntry.TabIndex = 7;
            // 
            // loginButton
            // 
            this.loginButton.Location = new System.Drawing.Point(275, 149);
            this.loginButton.Name = "loginButton";
            this.loginButton.Size = new System.Drawing.Size(75, 23);
            this.loginButton.TabIndex = 8;
            this.loginButton.Text = "Zaloguj";
            this.loginButton.UseVisualStyleBackColor = true;
            this.loginButton.Click += new System.EventHandler(this.loginButton_Click);
            // 
            // endButton
            // 
            this.endButton.Location = new System.Drawing.Point(12, 149);
            this.endButton.Name = "endButton";
            this.endButton.Size = new System.Drawing.Size(75, 23);
            this.endButton.TabIndex = 9;
            this.endButton.Text = "Zakończ";
            this.endButton.UseVisualStyleBackColor = true;
            this.endButton.Click += new System.EventHandler(this.endButton_Click);
            // 
            // LoginForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(362, 184);
            this.Controls.Add(this.endButton);
            this.Controls.Add(this.loginButton);
            this.Controls.Add(this.passwordEntry);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.usernameEntry);
            this.Controls.Add(this.portEntry);
            this.Controls.Add(this.hostnameEntry);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "LoginForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Nawiązywanie połączenia";
            this.Load += new System.EventHandler(this.LoginForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox hostnameEntry;
        private System.Windows.Forms.TextBox portEntry;
        private System.Windows.Forms.TextBox usernameEntry;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox passwordEntry;
        private System.Windows.Forms.Button loginButton;
        private System.Windows.Forms.Button endButton;
    }
}