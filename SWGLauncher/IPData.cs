namespace SWGLauncher
{
    internal struct IPData
    {
        public IPData(string ip, string serverName, int port = 44453)
        {
            IP = ip;
            IPPort = port;
            ServerName = serverName;
        }

        public override string ToString() { return $"{ServerName}:{IP}:{IPPort}";}

        public static bool operator ==(IPData owner,IPData ipData) 
        {
            return owner.IP == ipData.IP &&
                   owner.IPPort == ipData.IPPort &&
                   owner.ServerName == ipData.ServerName;
        }

        public static bool operator !=(IPData owner, IPData ipData)
        {
            return owner.IP != ipData.IP ||
                   owner.IPPort != ipData.IPPort ||
                   owner.ServerName != ipData.ServerName;
        }

        public string IP { get; private set; }
        public int IPPort { get; private set; }
        public string ServerName { get; private set; }
    }
}