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

            this.Text = $"SWG Launcher : Version {Program.Launcherversion}";

            RememberMeCheckBox.Checked = Program.GetSettings().RememberLastUser;
        }

        private void RegisterAccount_Click(object sender, EventArgs e)
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
            Program.GetFirebaseManager().SetDisplayName(displayname);

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

        private void MainForm_Shown(object sender, EventArgs e)
        {
            // Setup window graphics
            string[] files = Directory.GetFiles(@".\Resources", "*.jpg");
            if (files.Length == 0)
            {
                MessageBox.Show("There are no background images in the resource folder. There must be at least 1 jpg.", "ERROR!", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
                Close();
            }
            else if (Program.GetSettings().RandomizeBackgrounds)
            {
                Random random = new Random();

                string file = files[random.Next(0, files.Length)];

                pictureBox1.Image = Image.FromFile(file);
            }
            else if (Program.GetSettings().SpecificImage > files.Length || Program.GetSettings().SpecificImage < 0)
            {
                MessageBox.Show("Specified index to load specific image is out of possible range.", "ERROR!", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
                Close();
            }
            else
            {
                pictureBox1.Image = Image.FromFile(files[Program.GetSettings().SpecificImage]);
            }

            // Setup audio
            if (Program.GetSettings().EnableSound)
            {
                Program.GetAudioManager().Play();
                PlayMusicToggle.Checked = true;
            }
            else
            {
                PlayMusicToggle.Checked = false;
            }

            if (Program.GetSettings().RememberLastUser)
            {
                LoginEmailBox.Text = Program.GetSettings().LastEmail;
            }

            // Check if we have the game files. If we do not, ask to download
            if(!File.Exists(@"SwgClient_r.exe"))
            {
                switch(MessageBox.Show("We could not find the game files in the launcher directory. Download it now?","Error!",MessageBoxButtons.YesNo, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1))
                {
                    case DialogResult.Yes:
                        {
                            Close();
                            Process process = new Process();
                            process.StartInfo.FileName = @".\Updater\SWGStardustUpdater.exe";
                            process.StartInfo.ArgumentList.Add("Game");
                            process.StartInfo.ArgumentList.Add("FromClient");
                            process.Start();
                            return;
                        }
                    default:
                        {
                            Close();
                            return;
                        }
                }
            }

            // Check if this is the user's first time opening launcher. If it is, ask if they want to use the setup exe for graphics and other settings.
            if(Program.FirstLaunch == 0)
            {
                switch (MessageBox.Show("We are assuming this is your first time opening this launcher. Do you want to first setup the games graphics settings?", "Welcome!", MessageBoxButtons.YesNo, MessageBoxIcon.Information, MessageBoxDefaultButton.Button1))
                {
                    case DialogResult.Yes:
                        {
                            Close();
                            Process process = new Process();
                            process.StartInfo.FileName = @".\SwgClientSetup_r.exe";
                            process.Start();
                            return;
                        }
                }
            }

            // Check firebase for launcher updates.
            if (Program.Launcherversion < Program.FirebaseLauncherVersion)
            {
                switch (MessageBox.Show("This launcher is out of date, would you like to download the new one?", "Update!", MessageBoxButtons.YesNo, MessageBoxIcon.Information, MessageBoxDefaultButton.Button1))
                {
                    case DialogResult.Yes:
                        {
                            Close();
                            Process process = new Process();
                            process.StartInfo.FileName = @".\Updater\SWGStardustUpdater.exe";
                            process.StartInfo.ArgumentList.Add("Launcher");
                            process.StartInfo.ArgumentList.Add("FromClient");
                            process.Start();
                            return;
                        }
                    case DialogResult.No:
                        {
                            Close();
                            return;
                        }
                }
            }
            else if (Program.Launcherversion > Program.FirebaseLauncherVersion)
            {
                MessageBox.Show("Somehow, this client is registered as newer than what is on the server. Please get a valid version.", "Hold Up", MessageBoxButtons.OK, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1);
                Close();
            }
            // Check firebase for game updates
            else if (Program.GameVersion < Program.FirebaseGameVersion)
            {
                switch(MessageBox.Show("There is a new update to the game, would you like to download the new one?", "Update!", MessageBoxButtons.YesNo, MessageBoxIcon.Information, MessageBoxDefaultButton.Button1))
                {
                    case DialogResult.Yes : 
                        {
                            Close();
                            Process process = new Process();
                            process.StartInfo.FileName = @".\Updater\SWGStardustUpdater.exe";
                            process.StartInfo.ArgumentList.Add("Update");
                            process.StartInfo.ArgumentList.Add("FromClient");
                            process.Start();
                            return;
                        }
                    case DialogResult.No:
                        {
                            Close();
                            return;
                        }
                }
            }
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
                // Save new launcher settings here before we lose any possible data.
                if (Program.GetSettings().RememberLastUser || RememberMeCheckBox.Checked == true)
                {
                    Program.GetSettings().LastEmail = email;
                    Program.GetSettings().RememberLastUser = true;
                }
                Program.GetSettings().EnableSound = PlayMusicToggle.Checked;

                string username = Program.GetFirebaseManager().GetDisplayName();
                string[] strings = File.ReadAllLines(@".\user.cfg");
                // In the user.cfg, the line we want is line 17
                if (strings[17].Contains("loginClientID="))
                {
                    string[] pieces = strings[17].Split('=');
                    if (pieces.Length == 2)
                    {
                        strings[17] = $"{pieces[0]}=";
                    }

                    strings[17] = $"{strings[17]}{username}";
                    File.WriteAllLines(@".\user.cfg", strings);
                    Process process = new Process();
                    process.StartInfo.FileName = @".\SwgClient_r.exe";
                    process.StartInfo.ArgumentList.Add("ClientSuccess");
                    process.Start();
                    Close();
                }
                // if it doesn't, we will have to add that line ourselves
                else
                {
                    MessageBox.Show("Could not find property \'loginClientID=\' at line 17 in your \'user.cfg\'. Either get a newer version of the file, or add it to your file.",
                                    "ERROR!",
                                    MessageBoxButtons.OK,
                                    MessageBoxIcon.Error,
                                    MessageBoxDefaultButton.Button1);
                    Program.GetFirebaseManager().SignOut();
                    Close();
                }
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

        private void LoginPasswordBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == '\r')
                LoginButton_Click(sender, e);
        }
    }
}