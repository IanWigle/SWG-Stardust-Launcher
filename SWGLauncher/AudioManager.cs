using NAudio.Wave;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SWGLauncher
{
    internal class AudioManager
    {
        private WaveOutEvent outputDevice;
        private AudioFileReader audioFile;
        private bool loop = true;

        public AudioManager(string url)
        {
            try
            {
                outputDevice = new WaveOutEvent();
                audioFile = new AudioFileReader(url);
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
            if(loop && outputDevice.PlaybackState == PlaybackState.Stopped && outputDevice.DeviceNumber != -1)
            {
                outputDevice.Play();
            }
        }
    }
}
