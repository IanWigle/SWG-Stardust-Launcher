namespace SWGLauncher.Forms
{
    partial class PasswordResetForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PasswordResetForm));
            label1 = new Label();
            EmailTextBox = new TextBox();
            CancelButton = new Button();
            OkButton = new Button();
            SuspendLayout();
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(12, 9);
            label1.Name = "label1";
            label1.Size = new Size(231, 15);
            label1.TabIndex = 0;
            label1.Text = "Enter Email to send password reset request";
            // 
            // EmailTextBox
            // 
            EmailTextBox.Location = new Point(12, 27);
            EmailTextBox.Name = "EmailTextBox";
            EmailTextBox.Size = new Size(231, 23);
            EmailTextBox.TabIndex = 1;
            // 
            // CancelButton
            // 
            CancelButton.Location = new Point(12, 62);
            CancelButton.Name = "CancelButton";
            CancelButton.Size = new Size(75, 23);
            CancelButton.TabIndex = 2;
            CancelButton.Text = "Cancel";
            CancelButton.UseVisualStyleBackColor = true;
            CancelButton.Click += CancelButton_Click;
            // 
            // OkButton
            // 
            OkButton.Location = new Point(166, 62);
            OkButton.Name = "OkButton";
            OkButton.Size = new Size(75, 23);
            OkButton.TabIndex = 3;
            OkButton.Text = "Send";
            OkButton.UseVisualStyleBackColor = true;
            OkButton.Click += OkButton_Click;
            // 
            // PasswordResetForm
            // 
            AcceptButton = OkButton;
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            CancelButton = CancelButton;
            ClientSize = new Size(254, 96);
            Controls.Add(OkButton);
            Controls.Add(CancelButton);
            Controls.Add(EmailTextBox);
            Controls.Add(label1);
            Icon = (Icon)resources.GetObject("$this.Icon");
            Name = "PasswordResetForm";
            StartPosition = FormStartPosition.CenterScreen;
            Text = "Password Reset";
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Label label1;
        private TextBox EmailTextBox;
        private Button CancelButton;
        private Button OkButton;
    }
}