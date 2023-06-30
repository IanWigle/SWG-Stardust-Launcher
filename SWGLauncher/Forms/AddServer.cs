namespace SWGLauncher.Forms
{
    public partial class AddServer : Form
    {
        public string ServerName { get; private set; }
        public int ServerPort { get; private set; }
        public string ServerIP { get; private set; }
        public bool Successful { get; private set; }

        public AddServer()
        {
            InitializeComponent();
            Successful = false;
        }

        private void AddServer_FormClosing(object sender, FormClosingEventArgs e)
        {
            ServerName = serverNameBox.Text;
            ServerPort = (serverPortBox.Text == "") ? 44453 : int.Parse(serverPortBox.Text);
            ServerIP = serverIPBox.Text;
        }

        private void OkButton_Click(object sender, EventArgs e)
        {
            if (serverNameBox.Text == "" || serverIPBox.Text == "")
            {
                MessageBox.Show("You are missing either the server name or ip.", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
                return;
            }
            else
            {
                Successful = true;
                Close();
            }
        }

        private void CancelButton_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
