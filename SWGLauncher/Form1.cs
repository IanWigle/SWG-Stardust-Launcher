using System.Text;

namespace SWGLauncher
{
    /// <summary>
    /// Main Launcher Form that all launcher logic is contained in.
    /// </summary>
    public partial class Form1 : Form
    {
        /// <summary>
        /// Constructor.
        /// </summary>
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (Program.GetFirebaseManager().SignedIn() == true)
                Program.GetFirebaseManager().SignOut();

            string email = UserTextBox.Text;
            string password = PasswordTextBox.Text;

            Program.GetFirebaseManager().SetEmail(email);
            Program.GetFirebaseManager().SetPassword(password);

            string getpassword = "";
            string getusername = "";

            getpassword = Program.GetFirebaseManager().GetPassword();
            getusername = Program.GetFirebaseManager().GetEmail();

            Program.GetFirebaseManager().Register();
            System.Threading.Thread.Sleep(2000);
        }
    }
}