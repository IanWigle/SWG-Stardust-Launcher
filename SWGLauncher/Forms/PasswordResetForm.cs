namespace SWGLauncher.Forms
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
