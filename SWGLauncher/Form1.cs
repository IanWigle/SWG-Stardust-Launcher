using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
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

            string[] files = Directory.GetFiles($"{Directory.GetCurrentDirectory()}\\Resources", "*.jpg");
            Random random = new Random();

            string file = files[random.Next(0, files.Length)];

            pictureBox1.Image = Image.FromFile(file);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (Program.GetFirebaseManager().SignedIn() == true)
                Program.GetFirebaseManager().SignOut();

            string email = RegisterEmailBox.Text;
            string password = RegisterPasswordBox.Text;

            Program.GetFirebaseManager().SetEmail(email);
            Program.GetFirebaseManager().SetPassword(password);

            string getpassword = "";
            string getusername = "";

            // debug checks
            getpassword = Program.GetFirebaseManager().GetPassword();
            getusername = Program.GetFirebaseManager().GetEmail();

            if (!Program.GetFirebaseManager().Register())
            {
                MessageBox.Show($"{Program.GetFirebaseManager().GetLastError()}", "ERROR!", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
            }
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
                MessageBox.Show("Somehow, this client is registered as newer than what is on the server. Please get a valid version.", "Hold Up", MessageBoxButtons.OK, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1);
                Close();
            }
        }

        private void pictureBox2_Paint(object sender, PaintEventArgs e)
        {
            // e.Graphics.DrawImage(Image.FromFile($"{Directory.GetCurrentDirectory()}\\Resources\\Project_Stardust.png"), 0, 0);
        }
    }
}