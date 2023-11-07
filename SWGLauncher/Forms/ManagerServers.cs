using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SWGLauncher.Forms
{
    public partial class ManagerServers : Form
    {
        string selectedServer;

        public ManagerServers()
        {
            InitializeComponent();
        }

        private void ManagerServers_Shown(object sender, EventArgs e)
        {
            FillList();
        }

        private void addServerButton_Click(object sender, EventArgs e)
        {
            AddServer addServer = new AddServer();
            addServer.ShowDialog();

            if (addServer.Successful)
            {
                IPData newServer = new IPData(addServer.ServerIP, addServer.ServerName, addServer.ServerPort);
                Program.IPAddresses.Add(newServer);
                Program.LastIP = newServer;
                FillList();
            }
        }

        private void FillList()
        {
            serverView.Items.Clear();

            List<IPData> iPDatas = Program.IPAddresses;

            foreach (IPData data in iPDatas)
            {
                string[] param = { data.ServerName, data.IP, data.IPPort.ToString() };
                ListViewItem item = new ListViewItem(param);
                serverView.Items.Add(item);
            }
        }

        private void removeServerButton_Click(object sender, EventArgs e)
        {
            foreach (IPData data in Program.IPAddresses)
            {
                if (data.ServerName == selectedServer)
                {
                    Program.IPAddresses.Remove(data);
                    break;
                }
            }

            FillList();
        }

        private void serverView_SelectedIndexChanged(object sender, EventArgs e)
        {
            selectedServer = serverView.SelectedItems[0].Text;
            Debug.WriteLine(selectedServer);
        }
    }
}
