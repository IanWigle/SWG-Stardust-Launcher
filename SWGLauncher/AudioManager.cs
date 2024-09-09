using NAudio.Wave;

namespace SWGLauncher
{
    internal class AudioManager
    {
        private WaveOutEvent outputDevice;
        private AudioFileReader audioFile;
        private bool loop = true;

        public AudioManager(string url)
        {
            outputDevice = new WaveOutEvent();
            audioFile = new AudioFileReader(url);
            try
            {
                outputDevice.Init(audioFile);
                outputDevice.PlaybackStopped += OnPlaybackStopped;
                outputDevice.Volume = 0.1f;
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        public void Pause()
        {
            if (!IsPlaying()) return;

            outputDevice.Pause();
        }

        public void Play()
        {
            if (IsPlaying()) return;

            outputDevice.Play();
        }

        public bool IsPlaying()
        {
            return outputDevice.PlaybackState == PlaybackState.Playing;
        }

        private void OnPlaybackStopped(object sender, StoppedEventArgs eventArgs)
        {
            if(loop /*&& outputDevice.PlaybackState == PlaybackState.Stopped && outputDevice.DeviceNumber != -1*/)
            {
                outputDevice.Dispose();
                outputDevice = null;
                outputDevice = new WaveOutEvent();
                try
                {
                    outputDevice.Init(audioFile);
                    outputDevice.PlaybackStopped += OnPlaybackStopped;
                    outputDevice.Volume = 0.1f;
                    outputDevice.Play();
                }
                catch (Exception e)
                {
                    MessageBox.Show(e.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }
    }
}
