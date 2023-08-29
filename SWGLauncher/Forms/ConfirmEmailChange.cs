using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.ListView;

namespace SWGLauncher.Forms
{
    public partial class ConfirmEmailChange : Form
    {
        public ConfirmEmailChange()
        {
            InitializeComponent();
        }

        private void cancel_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void confirmChangeButton_Click(object sender, EventArgs e)
        {
            Program.GetFirebaseManager().SetEmail(oldEmailBox.Text);
            Program.GetFirebaseManager().SetPassword(passwordBox.Text);
            if (Program.GetFirebaseManager().Login())
            {
                Program.GetFirebaseManager().ChangeEmail(newEmailBox.Text);
                MessageBox.Show("Email Change made, please check your email for verification.", "Email Change", MessageBoxButtons.OK, MessageBoxIcon.Information);
                Program.GetFirebaseManager().SignOut();
                Program.GetFirebaseManager().SignInAnon();
                Close();
            }
        }

        private void passwordBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == '\r')
                confirmChangeButton_Click(sender, e);
        }
    }
}
