namespace SWGLauncher
{
    internal class ProgramSettings
    {
        // Audio
        public bool EnableSound { get; set; }
        // Graphics
        public bool RandomizeBackgrounds { get; set; }
        public int SpecificImage { get; set; }
        // Login
        public bool RememberLastUser { get; set; }
        public string LastEmail { get; set; }

        public bool EnableDebugString { get; set; }

        public ProgramSettings(bool enableSound, bool randomizeBackgrounds, int specificImage, bool rememberLastUser, string lastEmail, bool enableDebugString)
        {
            EnableSound = enableSound;
            RandomizeBackgrounds = randomizeBackgrounds;
            SpecificImage = specificImage;
            RememberLastUser = rememberLastUser;
            LastEmail = lastEmail;
            EnableDebugString = enableDebugString;
        }

        public ProgramSettings()
        {
            EnableSound = true;
            RandomizeBackgrounds = true;
            SpecificImage = 0;
            RememberLastUser = false;
            LastEmail = "";
            EnableDebugString = false;
        }
    }
}
