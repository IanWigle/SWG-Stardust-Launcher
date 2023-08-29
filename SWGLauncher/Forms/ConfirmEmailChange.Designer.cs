namespace SWGLauncher.Forms
{
    partial class ConfirmEmailChange
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ConfirmEmailChange));
            label1 = new Label();
            oldEmailBox = new TextBox();
            label2 = new Label();
            newEmailBox = new TextBox();
            label3 = new Label();
            passwordBox = new TextBox();
            confirmChangeButton = new Button();
            cancel = new Button();
            SuspendLayout();
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(12, 9);
            label1.Name = "label1";
            label1.Size = new Size(58, 15);
            label1.TabIndex = 0;
            label1.Text = "Old Email";
            // 
            // oldEmailBox
            // 
            oldEmailBox.Location = new Point(12, 27);
            oldEmailBox.Name = "oldEmailBox";
            oldEmailBox.Size = new Size(100, 23);
            oldEmailBox.TabIndex = 1;
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(12, 70);
            label2.Name = "label2";
            label2.Size = new Size(63, 15);
            label2.TabIndex = 2;
            label2.Text = "New Email";
            // 
            // newEmailBox
            // 
            newEmailBox.Location = new Point(12, 88);
            newEmailBox.Name = "newEmailBox";
            newEmailBox.Size = new Size(100, 23);
            newEmailBox.TabIndex = 3;
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(12, 136);
            label3.Name = "label3";
            label3.Size = new Size(57, 15);
            label3.TabIndex = 4;
            label3.Text = "Password";
            // 
            // passwordBox
            // 
            passwordBox.Location = new Point(12, 154);
            passwordBox.Name = "passwordBox";
            passwordBox.Size = new Size(100, 23);
            passwordBox.TabIndex = 5;
            passwordBox.UseSystemPasswordChar = true;
            passwordBox.KeyPress += passwordBox_KeyPress;
            // 
            // confirmChangeButton
            // 
            confirmChangeButton.Location = new Point(12, 216);
            confirmChangeButton.Name = "confirmChangeButton";
            confirmChangeButton.Size = new Size(75, 23);
            confirmChangeButton.TabIndex = 6;
            confirmChangeButton.Text = "Confirm";
            confirmChangeButton.UseVisualStyleBackColor = true;
            confirmChangeButton.Click += confirmChangeButton_Click;
            // 
            // cancel
            // 
            cancel.Location = new Point(120, 216);
            cancel.Name = "cancel";
            cancel.Size = new Size(75, 23);
            cancel.TabIndex = 7;
            cancel.Text = "Cancel";
            cancel.UseVisualStyleBackColor = true;
            cancel.Click += cancel_Click;
            // 
            // ConfirmEmailChange
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(207, 251);
            Controls.Add(cancel);
            Controls.Add(confirmChangeButton);
            Controls.Add(passwordBox);
            Controls.Add(label3);
            Controls.Add(newEmailBox);
            Controls.Add(label2);
            Controls.Add(oldEmailBox);
            Controls.Add(label1);
            Icon = (Icon)resources.GetObject("$this.Icon");
            Name = "ConfirmEmailChange";
            Text = "Email Change";
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Label label1;
        private TextBox oldEmailBox;
        private Label label2;
        private TextBox newEmailBox;
        private Label label3;
        private TextBox passwordBox;
        private Button confirmChangeButton;
        private Button cancel;
    }
}