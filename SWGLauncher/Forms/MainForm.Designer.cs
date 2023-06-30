namespace SWGLauncher.Forms
{
    partial class MainForm
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            tabControl1 = new TabControl();
            Login = new TabPage();
            RememberMeCheckBox = new CheckBox();
            LoginButton = new Button();
            groupBox5 = new GroupBox();
            LoginPasswordBox = new TextBox();
            groupBox4 = new GroupBox();
            LoginEmailBox = new TextBox();
            Register = new TabPage();
            groupBox6 = new GroupBox();
            RegisterDisplayNameBox = new TextBox();
            groupBox2 = new GroupBox();
            RegisterPasswordBox = new TextBox();
            RegisterAccountButton = new Button();
            groupBox1 = new GroupBox();
            RegisterEmailBox = new TextBox();
            Options = new TabPage();
            ResetPasswordButton = new Button();
            PlayMusicToggle = new CheckBox();
            pictureBox1 = new PictureBox();
            serverBox = new ComboBox();
            tabControl1.SuspendLayout();
            Login.SuspendLayout();
            groupBox5.SuspendLayout();
            groupBox4.SuspendLayout();
            Register.SuspendLayout();
            groupBox6.SuspendLayout();
            groupBox2.SuspendLayout();
            groupBox1.SuspendLayout();
            Options.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)pictureBox1).BeginInit();
            SuspendLayout();
            // 
            // tabControl1
            // 
            tabControl1.Controls.Add(Login);
            tabControl1.Controls.Add(Register);
            tabControl1.Controls.Add(Options);
            tabControl1.Location = new Point(732, 271);
            tabControl1.Name = "tabControl1";
            tabControl1.SelectedIndex = 0;
            tabControl1.Size = new Size(228, 246);
            tabControl1.TabIndex = 0;
            // 
            // Login
            // 
            Login.BackColor = SystemColors.Control;
            Login.Controls.Add(RememberMeCheckBox);
            Login.Controls.Add(LoginButton);
            Login.Controls.Add(groupBox5);
            Login.Controls.Add(groupBox4);
            Login.Location = new Point(4, 24);
            Login.Name = "Login";
            Login.Padding = new Padding(3);
            Login.Size = new Size(220, 218);
            Login.TabIndex = 0;
            Login.Text = "Login";
            // 
            // RememberMeCheckBox
            // 
            RememberMeCheckBox.AutoSize = true;
            RememberMeCheckBox.Location = new Point(8, 121);
            RememberMeCheckBox.Name = "RememberMeCheckBox";
            RememberMeCheckBox.Size = new Size(104, 19);
            RememberMeCheckBox.TabIndex = 3;
            RememberMeCheckBox.Text = "Remember Me";
            RememberMeCheckBox.UseVisualStyleBackColor = true;
            // 
            // LoginButton
            // 
            LoginButton.Location = new Point(4, 152);
            LoginButton.Name = "LoginButton";
            LoginButton.Size = new Size(213, 49);
            LoginButton.TabIndex = 2;
            LoginButton.Text = "Login";
            LoginButton.UseVisualStyleBackColor = true;
            LoginButton.Click += LoginButton_Click;
            // 
            // groupBox5
            // 
            groupBox5.BackColor = SystemColors.Control;
            groupBox5.Controls.Add(LoginPasswordBox);
            groupBox5.Location = new Point(4, 61);
            groupBox5.Name = "groupBox5";
            groupBox5.Size = new Size(213, 49);
            groupBox5.TabIndex = 1;
            groupBox5.TabStop = false;
            groupBox5.Text = "Password";
            // 
            // LoginPasswordBox
            // 
            LoginPasswordBox.Location = new Point(4, 20);
            LoginPasswordBox.Name = "LoginPasswordBox";
            LoginPasswordBox.PasswordChar = '*';
            LoginPasswordBox.Size = new Size(207, 23);
            LoginPasswordBox.TabIndex = 1;
            LoginPasswordBox.KeyPress += LoginPasswordBox_KeyPress;
            // 
            // groupBox4
            // 
            groupBox4.BackColor = SystemColors.Control;
            groupBox4.Controls.Add(LoginEmailBox);
            groupBox4.Location = new Point(4, 9);
            groupBox4.Name = "groupBox4";
            groupBox4.Size = new Size(213, 49);
            groupBox4.TabIndex = 0;
            groupBox4.TabStop = false;
            groupBox4.Text = "Email";
            // 
            // LoginEmailBox
            // 
            LoginEmailBox.BackColor = SystemColors.Window;
            LoginEmailBox.Location = new Point(4, 19);
            LoginEmailBox.Name = "LoginEmailBox";
            LoginEmailBox.Size = new Size(207, 23);
            LoginEmailBox.TabIndex = 0;
            // 
            // Register
            // 
            Register.BackColor = SystemColors.Control;
            Register.Controls.Add(groupBox6);
            Register.Controls.Add(groupBox2);
            Register.Controls.Add(RegisterAccountButton);
            Register.Controls.Add(groupBox1);
            Register.Location = new Point(4, 24);
            Register.Name = "Register";
            Register.Padding = new Padding(3);
            Register.Size = new Size(220, 218);
            Register.TabIndex = 1;
            Register.Text = "Register";
            // 
            // groupBox6
            // 
            groupBox6.BackColor = SystemColors.Control;
            groupBox6.Controls.Add(RegisterDisplayNameBox);
            groupBox6.Location = new Point(3, 61);
            groupBox6.Name = "groupBox6";
            groupBox6.Size = new Size(213, 49);
            groupBox6.TabIndex = 11;
            groupBox6.TabStop = false;
            groupBox6.Text = "Username";
            // 
            // RegisterDisplayNameBox
            // 
            RegisterDisplayNameBox.Location = new Point(3, 19);
            RegisterDisplayNameBox.Name = "RegisterDisplayNameBox";
            RegisterDisplayNameBox.Size = new Size(207, 23);
            RegisterDisplayNameBox.TabIndex = 0;
            // 
            // groupBox2
            // 
            groupBox2.BackColor = SystemColors.Control;
            groupBox2.Controls.Add(RegisterPasswordBox);
            groupBox2.Location = new Point(4, 116);
            groupBox2.Name = "groupBox2";
            groupBox2.Size = new Size(213, 49);
            groupBox2.TabIndex = 9;
            groupBox2.TabStop = false;
            groupBox2.Text = "Password";
            // 
            // RegisterPasswordBox
            // 
            RegisterPasswordBox.Location = new Point(3, 19);
            RegisterPasswordBox.Name = "RegisterPasswordBox";
            RegisterPasswordBox.PasswordChar = '*';
            RegisterPasswordBox.Size = new Size(207, 23);
            RegisterPasswordBox.TabIndex = 1;
            // 
            // RegisterAccountButton
            // 
            RegisterAccountButton.Location = new Point(3, 169);
            RegisterAccountButton.Name = "RegisterAccountButton";
            RegisterAccountButton.Size = new Size(213, 49);
            RegisterAccountButton.TabIndex = 11;
            RegisterAccountButton.Text = "Make Account";
            RegisterAccountButton.UseVisualStyleBackColor = true;
            RegisterAccountButton.Click += RegisterAccount_Click;
            // 
            // groupBox1
            // 
            groupBox1.BackColor = SystemColors.Control;
            groupBox1.Controls.Add(RegisterEmailBox);
            groupBox1.Location = new Point(3, 6);
            groupBox1.Name = "groupBox1";
            groupBox1.Size = new Size(213, 49);
            groupBox1.TabIndex = 10;
            groupBox1.TabStop = false;
            groupBox1.Text = "Email";
            // 
            // RegisterEmailBox
            // 
            RegisterEmailBox.Location = new Point(3, 19);
            RegisterEmailBox.Name = "RegisterEmailBox";
            RegisterEmailBox.Size = new Size(207, 23);
            RegisterEmailBox.TabIndex = 0;
            // 
            // Options
            // 
            Options.Controls.Add(serverBox);
            Options.Controls.Add(ResetPasswordButton);
            Options.Controls.Add(PlayMusicToggle);
            Options.Location = new Point(4, 24);
            Options.Name = "Options";
            Options.Padding = new Padding(3);
            Options.Size = new Size(220, 218);
            Options.TabIndex = 2;
            Options.Text = "Options";
            Options.UseVisualStyleBackColor = true;
            // 
            // ResetPasswordButton
            // 
            ResetPasswordButton.Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
            ResetPasswordButton.Location = new Point(55, 166);
            ResetPasswordButton.Name = "ResetPasswordButton";
            ResetPasswordButton.Size = new Size(120, 44);
            ResetPasswordButton.TabIndex = 1;
            ResetPasswordButton.Text = "Reset Password";
            ResetPasswordButton.UseVisualStyleBackColor = true;
            ResetPasswordButton.Click += ResetPasswordButton_Click;
            // 
            // PlayMusicToggle
            // 
            PlayMusicToggle.AutoSize = true;
            PlayMusicToggle.Location = new Point(8, 10);
            PlayMusicToggle.Name = "PlayMusicToggle";
            PlayMusicToggle.Size = new Size(83, 19);
            PlayMusicToggle.TabIndex = 0;
            PlayMusicToggle.Text = "Play Music";
            PlayMusicToggle.UseVisualStyleBackColor = true;
            PlayMusicToggle.CheckedChanged += PlayMusicToggle_CheckedChanged;
            // 
            // pictureBox1
            // 
            pictureBox1.BackColor = SystemColors.Control;
            pictureBox1.BackgroundImageLayout = ImageLayout.Stretch;
            pictureBox1.Dock = DockStyle.Fill;
            pictureBox1.Location = new Point(0, 0);
            pictureBox1.Name = "pictureBox1";
            pictureBox1.Size = new Size(960, 517);
            pictureBox1.SizeMode = PictureBoxSizeMode.StretchImage;
            pictureBox1.TabIndex = 1;
            pictureBox1.TabStop = false;
            // 
            // serverBox
            // 
            serverBox.FormattingEnabled = true;
            serverBox.Location = new Point(6, 35);
            serverBox.Name = "serverBox";
            serverBox.Size = new Size(206, 23);
            serverBox.TabIndex = 2;
            serverBox.SelectedIndexChanged += serverBox_SelectedIndexChanged;
            // 
            // MainForm
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            AutoSize = true;
            AutoSizeMode = AutoSizeMode.GrowAndShrink;
            BackgroundImageLayout = ImageLayout.Stretch;
            ClientSize = new Size(960, 517);
            Controls.Add(tabControl1);
            Controls.Add(pictureBox1);
            FormBorderStyle = FormBorderStyle.Fixed3D;
            Icon = (Icon)resources.GetObject("$this.Icon");
            Name = "MainForm";
            StartPosition = FormStartPosition.CenterScreen;
            Text = "Version 1.1";
            TransparencyKey = SystemColors.ActiveCaption;
            Shown += MainForm_Shown;
            tabControl1.ResumeLayout(false);
            Login.ResumeLayout(false);
            Login.PerformLayout();
            groupBox5.ResumeLayout(false);
            groupBox5.PerformLayout();
            groupBox4.ResumeLayout(false);
            groupBox4.PerformLayout();
            Register.ResumeLayout(false);
            groupBox6.ResumeLayout(false);
            groupBox6.PerformLayout();
            groupBox2.ResumeLayout(false);
            groupBox2.PerformLayout();
            groupBox1.ResumeLayout(false);
            groupBox1.PerformLayout();
            Options.ResumeLayout(false);
            Options.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)pictureBox1).EndInit();
            ResumeLayout(false);
        }

        #endregion
        private GroupBox groupBox3;
        private SplitContainer splitContainer1;
        private TabControl tabControl1;
        private TabPage Login;
        private TabPage Register;
        private GroupBox groupBox2;
        private TextBox RegisterPasswordBox;
        private Button RegisterAccountButton;
        private GroupBox groupBox1;
        private TextBox RegisterEmailBox;
        private Button LoginButton;
        private GroupBox groupBox5;
        private GroupBox groupBox4;
        private TextBox LoginPasswordBox;
        private TextBox LoginEmailBox;
        private PictureBox pictureBox1;
        private TabPage Options;
        private CheckBox PlayMusicToggle;
        private Button ResetPasswordButton;
        private GroupBox groupBox6;
        private TextBox RegisterDisplayNameBox;
        private CheckBox RememberMeCheckBox;
        private ComboBox serverBox;
    }
}