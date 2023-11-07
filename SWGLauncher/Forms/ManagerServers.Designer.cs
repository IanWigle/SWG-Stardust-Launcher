namespace SWGLauncher.Forms
{
    partial class ManagerServers
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ManagerServers));
            serverView = new ListView();
            columnHeader1 = new ColumnHeader();
            columnHeader2 = new ColumnHeader();
            columnHeader3 = new ColumnHeader();
            addServerButton = new Button();
            removeServerButton = new Button();
            SuspendLayout();
            // 
            // serverView
            // 
            serverView.Columns.AddRange(new ColumnHeader[] { columnHeader1, columnHeader2, columnHeader3 });
            serverView.Location = new Point(12, 12);
            serverView.Name = "serverView";
            serverView.Size = new Size(554, 426);
            serverView.TabIndex = 0;
            serverView.UseCompatibleStateImageBehavior = false;
            serverView.View = View.Details;
            serverView.SelectedIndexChanged += serverView_SelectedIndexChanged;
            // 
            // columnHeader1
            // 
            columnHeader1.Text = "Name";
            columnHeader1.Width = 300;
            // 
            // columnHeader2
            // 
            columnHeader2.Text = "IP";
            columnHeader2.Width = 150;
            // 
            // columnHeader3
            // 
            columnHeader3.Text = "Port";
            columnHeader3.Width = 100;
            // 
            // addServerButton
            // 
            addServerButton.Location = new Point(591, 12);
            addServerButton.Name = "addServerButton";
            addServerButton.Size = new Size(122, 23);
            addServerButton.TabIndex = 1;
            addServerButton.Text = "Add Server";
            addServerButton.UseVisualStyleBackColor = true;
            addServerButton.Click += addServerButton_Click;
            // 
            // removeServerButton
            // 
            removeServerButton.Location = new Point(591, 55);
            removeServerButton.Name = "removeServerButton";
            removeServerButton.Size = new Size(122, 23);
            removeServerButton.TabIndex = 2;
            removeServerButton.Text = "Remove Server";
            removeServerButton.UseVisualStyleBackColor = true;
            removeServerButton.Click += removeServerButton_Click;
            // 
            // ManagerServers
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(725, 450);
            Controls.Add(removeServerButton);
            Controls.Add(addServerButton);
            Controls.Add(serverView);
            Icon = (Icon)resources.GetObject("$this.Icon");
            Name = "ManagerServers";
            Text = "ManagerServers";
            Shown += ManagerServers_Shown;
            ResumeLayout(false);
        }

        #endregion

        private ListView serverView;
        private ColumnHeader columnHeader1;
        private ColumnHeader columnHeader2;
        private ColumnHeader columnHeader3;
        private Button addServerButton;
        private Button removeServerButton;
    }
}