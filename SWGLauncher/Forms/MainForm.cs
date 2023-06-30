using System.Diagnostics;
using System.IO.Compression;

namespace SWGLauncher.Forms
{
    /// <summary>
    /// Main Launcher Form that all launcher logic is contained in.
    /// </summary>
    public partial class MainForm : Form
    {
        const string NEWSERVER = "New Server...";

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
            Program.GetFirebaseManager().LogLauncher("MainForm : RegisterAccount Event Started");
            if (Program.GetFirebaseManager().SignedIn() == true)
                Program.GetFirebaseManager().SignOut();

            string email = RegisterEmailBox.Text;
            string password = RegisterPasswordBox.Text;
            string displayname = RegisterDisplayNameBox.Text;

            if (email == "")
            {
                Program.GetFirebaseManager().LogLauncher("MainForm : Please provide a email address");
                MessageBox.Show("Please provide a email address", "ERROR!", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
                return;
            }
            if (password == "" || password.Length <= 4)
            {
                Program.GetFirebaseManager().LogLauncher("MainForm : Your password is too short!");
                MessageBox.Show("Your password is too short!", "ERROR!", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
                return;
            }

            Program.GetFirebaseManager().SetEmail(email);
            Program.GetFirebaseManager().SetPassword(password);
            Program.GetFirebaseManager().SetDisplayName(displayname);

            if (!Program.GetFirebaseManager().Register())
            {
                Program.GetFirebaseManager().LogLauncher($"MainForm : Firebase ERROR : {Program.GetFirebaseManager().GetLastAuthErrorString()}");
                MessageBox.Show($"{Program.GetFirebaseManager().GetLastAuthErrorString()}", "ERROR!", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
            }
            else
            {
                Program.GetFirebaseManager().LogLauncher("MainForm : Account made! Check your email to validate.");
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
                Program.GetFirebaseManager().LogLauncher("MainForm : No background images found. Not risking possible crash, closing launcher.");
                MessageBox.Show("There are no background images in the resource folder. There must be at least 1 jpg.", "ERROR!", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
                Close();
            }
            else if (Program.GetSettings().RandomizeBackgrounds)
            {
                Random random = new Random();

                string file = files[random.Next(0, files.Length)];

                pictureBox1.Image = Image.FromFile(file);

                Program.GetFirebaseManager().LogLauncher($"MainForm : Selected background image at random. Using {file}");
            }
            else if (Program.GetSettings().SpecificImage > files.Length || Program.GetSettings().SpecificImage < 0)
            {
                Program.GetFirebaseManager().LogLauncher("MainForm : Specified index to load specific image is out of possible range.");
                MessageBox.Show("Specified index to load specific image is out of possible range.", "ERROR!", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
                Close();
            }
            else
            {
                pictureBox1.Image = Image.FromFile(files[Program.GetSettings().SpecificImage]);

                Program.GetFirebaseManager().LogLauncher($"MainForm : Selected background image. Using {files[Program.GetSettings().SpecificImage]}");
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

            // Get the last logged in user.
            if (Program.GetSettings().RememberLastUser)
            {
                LoginEmailBox.Text = Program.GetSettings().LastEmail;
            }

            // Check if we are under maintenance.
            if (Program.GetFirebaseManager().GetUnderMaintenance() == true)
            {
                MessageBox.Show("Stardust is currently under maintenance. Please come back later. Check out updates on the Stardust Discord.", "Notice!", MessageBoxButtons.OK, MessageBoxIcon.Stop, MessageBoxDefaultButton.Button1);
                Close();
                return;
            }

            // Check if we have the game files. If we do not, ask to download
            if (!File.Exists(@"SwgClient_r.exe"))
            {
                switch (MessageBox.Show("We could not find the game files in the launcher directory. Download it now?", "Error!", MessageBoxButtons.YesNo, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1))
                {
                    case DialogResult.Yes:
                        {
                            Program.DownloadingGame = true;
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
            if (Program.FirstLaunch == 0)
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

            // Check firebaser for updater updates.
            else if (Program.FirebaseUpdaterVersion != Program.UpdaterVersion)
            {
                switch (MessageBox.Show("We've detected that there's a new version of the updater, can we download it?", "Update!", MessageBoxButtons.YesNo, MessageBoxIcon.Stop, MessageBoxDefaultButton.Button1))
                {
                    case DialogResult.Yes:
                        {
                            Program.GetFirebaseManager().DownloadNewUpdater();

                            if (File.Exists(@".\SWGStardustUpdater.zip"))
                            {
                                try
                                {
                                    ZipFile.ExtractToDirectory(@".\SWGStardustUpdater.zip", @".\Updater\", true);
                                    File.Delete(@".\SWGStardustUpdater.zip");
                                    Program.UpdaterVersion = Program.FirebaseUpdaterVersion;

                                    MessageBox.Show("Downloader Update finished! We will restart the launcher now.", "Finished!", MessageBoxButtons.OK, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button1);
                                    Program.SaveSettings();
                                    Application.Restart();
                                    Environment.Exit(0);
                                    return;
                                }
                                catch (Exception exception)
                                {
                                    MessageBox.Show(exception.Message, "ERROR!", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
                                    Application.Exit();
                                    Environment.Exit(0);
                                    return;
                                }
                            }

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
            else if (Program.GameVersion != Program.FirebaseGameVersion)
            {
                switch (MessageBox.Show("There is a new update to the game, would you like to download the new one?", "Update!", MessageBoxButtons.YesNo, MessageBoxIcon.Information, MessageBoxDefaultButton.Button1))
                {
                    case DialogResult.Yes:
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

            // Get all registered servers.
            FillServerBoxCombo();
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
                MessageBox.Show($"{Program.GetFirebaseManager().GetLastAuthErrorString()}", "ERROR!", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
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

                {
                    string[] loginstrings = File.ReadAllLines(@".\login.cfg");
                    loginstrings[7] = $"    loginServerPort0={Program.LastIP.IPPort}";
                    loginstrings[8] = $"    loginServerAddress0={Program.LastIP.IP}";
                    File.WriteAllLines(@".\login.cfg", loginstrings);
                }

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
                    MessageBox.Show($"{Program.GetFirebaseManager().GetLastAuthErrorString()}", "ERROR!", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
                }
            }
        }

        private void LoginPasswordBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == '\r')
                LoginButton_Click(sender, e);
        }

        private void serverBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if((serverBox.SelectedItem as string) == NEWSERVER)
            {
                AddServer addServer = new AddServer();
                addServer.ShowDialog();

                if(addServer.Successful)
                {
                    IPData newServer = new IPData(addServer.ServerIP, addServer.ServerName, addServer.ServerPort);
                    Program.IPAddresses.Add(newServer);
                    Program.LastIP = newServer;
                    FillServerBoxCombo();
                }
            }
            else
            {
                foreach(IPData server in Program.IPAddresses)
                {
                    if((serverBox.SelectedItem as string) == server.ServerName)
                    {
                        Program.LastIP = server;
                        break;
                    }
                }
            }
        }

        private void FillServerBoxCombo()
        {
            serverBox.Items.Clear();
            foreach (IPData server in Program.IPAddresses)
            {
                serverBox.Items.Add(server.ServerName);
                if(server.ServerName == Program.LastIP.ServerName)
                {
                    serverBox.SelectedIndex = serverBox.Items.IndexOf(serverBox.Items[serverBox.Items.Count-1]);
                    continue;
                }
            }
            
            serverBox.Items.Add(NEWSERVER);
        }
    }
}