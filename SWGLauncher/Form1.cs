using System.Diagnostics;
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

        private void Form1_Shown(object sender, EventArgs e)
        {
            if (Program.Launcherversion < Program.FirebaseLauncherVersion)
            {
                switch (MessageBox.Show("This launcher is out of date, would you like to download the new one?", "Update!", MessageBoxButtons.YesNo, MessageBoxIcon.Information, MessageBoxDefaultButton.Button1))
                {
                    case DialogResult.Yes:
                        {
                            Close();
                            Process.Start(@".\Updater\SWGStardustUpdater.exe");
                            break;
                        }
                    case DialogResult.No:
                        {
                            Close();
                            break;
                        }
                }
            }
            else if (Program.Launcherversion > Program.FirebaseLauncherVersion)
            {
                MessageBox.Show("Somehow, this client is registered as newever than what is on the server. Please get a valid version.", "Hold Up", MessageBoxButtons.OK, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1);
                Close();
            }
        }
    }
}