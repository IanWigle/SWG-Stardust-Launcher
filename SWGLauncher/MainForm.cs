using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.Text;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.ListView;

namespace SWGLauncher
{
    /// <summary>
    /// Main Launcher Form that all launcher logic is contained in.
    /// </summary>
    public partial class MainForm : Form
    {
        /// <summary>
        /// Constructor.
        /// </summary>
        public MainForm()
        {
            InitializeComponent();

            string[] files = Directory.GetFiles(@".\Resources", "*.jpg");
            Random random = new Random();

            string file = files[random.Next(0, files.Length)];

            pictureBox1.Image = Image.FromFile(file);

            this.Text = $"SWG Launcher : Version {Program.Launcherversion}";

            PlayMusicToggle.Checked = Program.GetAudioManager().IsPlaying();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (Program.GetFirebaseManager().SignedIn() == true)
                Program.GetFirebaseManager().SignOut();

            string email = RegisterEmailBox.Text;
            string password = RegisterPasswordBox.Text;
            string displayname = RegisterDisplayNameBox.Text;

            if (email == "")
            {
                MessageBox.Show("Please provide a email address", "ERROR!", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
                return;
            }
            if (password == "" || password.Length <= 4)
            {
                MessageBox.Show("Your password is too short!", "ERROR!", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
                return;
            }
            if (displayname == "" || displayname.Length <= 5)
            {
                MessageBox.Show("Your display name is too short!", "ERROR!", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
                return;
            }

            Program.GetFirebaseManager().SetEmail(email);
            Program.GetFirebaseManager().SetPassword(password);

            if (!Program.GetFirebaseManager().Register())
            {
                MessageBox.Show($"{Program.GetFirebaseManager().GetLastError()}", "ERROR!", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
            }
            else
            {
                MessageBox.Show("Account made! Check your email to validate.", "Almost there...", MessageBoxButtons.OK, MessageBoxIcon.Information, MessageBoxDefaultButton.Button1);
            }
            System.Threading.Thread.Sleep(1000);
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

        private void LoginButton_Click(object sender, EventArgs e)
        {
            string email = LoginEmailBox.Text;
            string password = LoginPasswordBox.Text;

            if (email == "")
            {
                MessageBox.Show("Please provide a email address", "ERROR!", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
                return;
            }
            if (password == "" || password.Length <= 4)
            {
                MessageBox.Show("Your password is too short!", "ERROR!", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
                return;
            }

            // Prevent people from making accounts while logging in.
            // Just to prevent spam firebase requests.
            RegisterAccountButton.Enabled = false;

            Program.GetFirebaseManager().SetEmail(email);
            Program.GetFirebaseManager().SetPassword(password);

            if (!Program.GetFirebaseManager().Login())
            {
                int authError = Program.GetFirebaseManager().GetLastAuthError();
                MessageBox.Show($"{Program.GetFirebaseManager().GetLastError()}", "ERROR!", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
            }
            else
            {
                MessageBox.Show("Signed In!", "Success!", MessageBoxButtons.OK, MessageBoxIcon.Information, MessageBoxDefaultButton.Button1);
            }

            RegisterAccountButton.Enabled = true;
        }

        private void PlayMusicToggle_CheckedChanged(object sender, EventArgs e)
        {
            if (PlayMusicToggle.Checked)
            {
                Program.GetAudioManager().Play();
            }
            else
            {
                Program.GetAudioManager().Pause();
            }
        }

        private void ResetPasswordButton_Click(object sender, EventArgs e)
        {
            PasswordResetForm passwordResetForm = new PasswordResetForm();
            passwordResetForm.ShowDialog();

            if (!passwordResetForm.WasCanceled())
            {
                Program.GetFirebaseManager().SendPasswordReset(passwordResetForm.GetEmail());

                // If we succeeded
                if (Program.GetFirebaseManager().GetLastAuthError() == 0)
                {
                    MessageBox.Show("Email sent to reset password.", "Email Sent", MessageBoxButtons.OK, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button1);
                }
                else
                {
                    MessageBox.Show($"{Program.GetFirebaseManager().GetLastError()}", "ERROR!", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
                }
            }
        }
    }
}