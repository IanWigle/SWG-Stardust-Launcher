namespace SWGLauncher
{
    partial class Form1
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            tabControl1 = new TabControl();
            Login = new TabPage();
            LoginButton = new Button();
            groupBox5 = new GroupBox();
            textBox2 = new TextBox();
            groupBox4 = new GroupBox();
            textBox1 = new TextBox();
            tabPage2 = new TabPage();
            groupBox2 = new GroupBox();
            RegisterPasswordBox = new TextBox();
            RegisterAccountButton = new Button();
            groupBox1 = new GroupBox();
            RegisterEmailBox = new TextBox();
            pictureBox1 = new PictureBox();
            tabControl1.SuspendLayout();
            Login.SuspendLayout();
            groupBox5.SuspendLayout();
            groupBox4.SuspendLayout();
            tabPage2.SuspendLayout();
            groupBox2.SuspendLayout();
            groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)pictureBox1).BeginInit();
            SuspendLayout();
            // 
            // tabControl1
            // 
            tabControl1.Controls.Add(Login);
            tabControl1.Controls.Add(tabPage2);
            tabControl1.Location = new Point(732, 320);
            tabControl1.Name = "tabControl1";
            tabControl1.SelectedIndex = 0;
            tabControl1.Size = new Size(228, 197);
            tabControl1.TabIndex = 0;
            // 
            // Login
            // 
            Login.BackColor = SystemColors.Control;
            Login.Controls.Add(LoginButton);
            Login.Controls.Add(groupBox5);
            Login.Controls.Add(groupBox4);
            Login.Location = new Point(4, 24);
            Login.Name = "Login";
            Login.Padding = new Padding(3);
            Login.Size = new Size(220, 169);
            Login.TabIndex = 0;
            Login.Text = "Login";
            // 
            // LoginButton
            // 
            LoginButton.Location = new Point(4, 116);
            LoginButton.Name = "LoginButton";
            LoginButton.Size = new Size(213, 49);
            LoginButton.TabIndex = 2;
            LoginButton.Text = "Login";
            LoginButton.UseVisualStyleBackColor = true;
            // 
            // groupBox5
            // 
            groupBox5.BackColor = SystemColors.Control;
            groupBox5.Controls.Add(textBox2);
            groupBox5.Location = new Point(4, 61);
            groupBox5.Name = "groupBox5";
            groupBox5.Size = new Size(213, 49);
            groupBox5.TabIndex = 1;
            groupBox5.TabStop = false;
            groupBox5.Text = "Password";
            // 
            // textBox2
            // 
            textBox2.Location = new Point(4, 20);
            textBox2.Name = "textBox2";
            textBox2.Size = new Size(207, 23);
            textBox2.TabIndex = 1;
            // 
            // groupBox4
            // 
            groupBox4.BackColor = SystemColors.Control;
            groupBox4.Controls.Add(textBox1);
            groupBox4.Location = new Point(4, 9);
            groupBox4.Name = "groupBox4";
            groupBox4.Size = new Size(213, 49);
            groupBox4.TabIndex = 0;
            groupBox4.TabStop = false;
            groupBox4.Text = "Email";
            // 
            // textBox1
            // 
            textBox1.BackColor = SystemColors.Window;
            textBox1.Location = new Point(4, 19);
            textBox1.Name = "textBox1";
            textBox1.Size = new Size(207, 23);
            textBox1.TabIndex = 0;
            // 
            // tabPage2
            // 
            tabPage2.BackColor = SystemColors.Control;
            tabPage2.Controls.Add(groupBox2);
            tabPage2.Controls.Add(RegisterAccountButton);
            tabPage2.Controls.Add(groupBox1);
            tabPage2.Location = new Point(4, 24);
            tabPage2.Name = "tabPage2";
            tabPage2.Padding = new Padding(3);
            tabPage2.Size = new Size(220, 190);
            tabPage2.TabIndex = 1;
            tabPage2.Text = "Register";
            // 
            // groupBox2
            // 
            groupBox2.BackColor = SystemColors.Control;
            groupBox2.Controls.Add(RegisterPasswordBox);
            groupBox2.Location = new Point(4, 61);
            groupBox2.Name = "groupBox2";
            groupBox2.Size = new Size(213, 48);
            groupBox2.TabIndex = 9;
            groupBox2.TabStop = false;
            groupBox2.Text = "Password";
            // 
            // RegisterPasswordBox
            // 
            RegisterPasswordBox.Location = new Point(3, 19);
            RegisterPasswordBox.Name = "RegisterPasswordBox";
            RegisterPasswordBox.Size = new Size(207, 23);
            RegisterPasswordBox.TabIndex = 1;
            // 
            // RegisterAccountButton
            // 
            RegisterAccountButton.Location = new Point(4, 115);
            RegisterAccountButton.Name = "RegisterAccountButton";
            RegisterAccountButton.Size = new Size(213, 49);
            RegisterAccountButton.TabIndex = 11;
            RegisterAccountButton.Text = "Make Account";
            RegisterAccountButton.UseVisualStyleBackColor = true;
            RegisterAccountButton.Click += button1_Click;
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
            // Form1
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
            Name = "Form1";
            StartPosition = FormStartPosition.CenterScreen;
            Text = "Version 1";
            TransparencyKey = SystemColors.ActiveCaption;
            Shown += Form1_Shown;
            tabControl1.ResumeLayout(false);
            Login.ResumeLayout(false);
            groupBox5.ResumeLayout(false);
            groupBox5.PerformLayout();
            groupBox4.ResumeLayout(false);
            groupBox4.PerformLayout();
            tabPage2.ResumeLayout(false);
            groupBox2.ResumeLayout(false);
            groupBox2.PerformLayout();
            groupBox1.ResumeLayout(false);
            groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)pictureBox1).EndInit();
            ResumeLayout(false);
        }

        #endregion
        private GroupBox groupBox3;
        private SplitContainer splitContainer1;
        private TabControl tabControl1;
        private TabPage Login;
        private TabPage tabPage2;
        private GroupBox groupBox2;
        private TextBox RegisterPasswordBox;
        private Button RegisterAccountButton;
        private GroupBox groupBox1;
        private TextBox RegisterEmailBox;
        private Button LoginButton;
        private GroupBox groupBox5;
        private GroupBox groupBox4;
        private TextBox textBox2;
        private TextBox textBox1;
        private PictureBox pictureBox1;
    }
}