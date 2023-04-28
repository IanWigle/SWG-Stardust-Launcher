using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SWGLauncher
{
    public partial class PasswordResetForm : Form
    {
        bool Canceled = true;

        public PasswordResetForm()
        {
            InitializeComponent();
        }

        public string GetEmail()
        {
            return EmailTextBox.Text;
        }

        public bool WasCanceled()
        {
            return Canceled;
        }

        private void OkButton_Click(object sender, EventArgs e)
        {
            Canceled = false;
            Close();
        }

        private void CancelButton_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
