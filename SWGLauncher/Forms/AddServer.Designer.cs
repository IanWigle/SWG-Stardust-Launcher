namespace SWGLauncher.Forms
{
    partial class AddServer
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AddServer));
            OkButton = new Button();
            CancelButton = new Button();
            serverNameBox = new TextBox();
            serverIPBox = new TextBox();
            serverPortBox = new TextBox();
            label1 = new Label();
            label2 = new Label();
            label3 = new Label();
            SuspendLayout();
            // 
            // OkButton
            // 
            OkButton.Location = new Point(10, 220);
            OkButton.Name = "OkButton";
            OkButton.Size = new Size(75, 23);
            OkButton.TabIndex = 0;
            OkButton.Text = "Ok";
            OkButton.UseVisualStyleBackColor = true;
            OkButton.Click += OkButton_Click;
            // 
            // CancelButton
            // 
            CancelButton.Location = new Point(116, 220);
            CancelButton.Name = "CancelButton";
            CancelButton.Size = new Size(75, 23);
            CancelButton.TabIndex = 1;
            CancelButton.Text = "Cancel";
            CancelButton.UseVisualStyleBackColor = true;
            CancelButton.Click += CancelButton_Click;
            // 
            // serverNameBox
            // 
            serverNameBox.Location = new Point(12, 32);
            serverNameBox.Name = "serverNameBox";
            serverNameBox.Size = new Size(179, 23);
            serverNameBox.TabIndex = 2;
            // 
            // serverIPBox
            // 
            serverIPBox.Location = new Point(12, 110);
            serverIPBox.Name = "serverIPBox";
            serverIPBox.Size = new Size(179, 23);
            serverIPBox.TabIndex = 3;
            // 
            // serverPortBox
            // 
            serverPortBox.Location = new Point(12, 182);
            serverPortBox.Name = "serverPortBox";
            serverPortBox.Size = new Size(179, 23);
            serverPortBox.TabIndex = 4;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(12, 14);
            label1.Name = "label1";
            label1.Size = new Size(132, 15);
            label1.TabIndex = 5;
            label1.Text = "Server Name (Required)";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(12, 92);
            label2.Name = "label2";
            label2.Size = new Size(110, 15);
            label2.TabIndex = 6;
            label2.Text = "Server IP (Required)";
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(12, 164);
            label3.Name = "label3";
            label3.Size = new Size(121, 15);
            label3.TabIndex = 7;
            label3.Text = "Server Port (Optional)\r\n";
            // 
            // AddServer
            // 
            AcceptButton = OkButton;
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(203, 253);
            Controls.Add(label3);
            Controls.Add(label2);
            Controls.Add(label1);
            Controls.Add(serverPortBox);
            Controls.Add(serverIPBox);
            Controls.Add(serverNameBox);
            Controls.Add(CancelButton);
            Controls.Add(OkButton);
            Icon = (Icon)resources.GetObject("$this.Icon");
            Name = "AddServer";
            StartPosition = FormStartPosition.CenterParent;
            Text = "Add Server";
            FormClosing += AddServer_FormClosing;
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Button OkButton;
        private Button CancelButton;
        private TextBox serverNameBox;
        private TextBox serverIPBox;
        private TextBox serverPortBox;
        private Label label1;
        private Label label2;
        private Label label3;
    }
}