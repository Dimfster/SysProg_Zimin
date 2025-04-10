using System.Net;
using System.Net.Sockets;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;


namespace System_Lab1_CSharp
{
    public partial class Form1 : Form
    {
        bool serverProcess = false;
        Socket s;
        int numThreads = 0;

        static void SendInt(Socket s, int num)
        {
            s.Send(BitConverter.GetBytes(num), sizeof(int), SocketFlags.None);
        }

        static int ReceiveInt(Socket s)
        {
            byte[] b = new byte[sizeof(int)];
            s.Receive(b, sizeof(int), SocketFlags.None);
            return BitConverter.ToInt32(b, 0);
        }

        static void SendString(Socket s, string str)
        {
            int n = str.Length * 2;
            SendInt(s, n);
            s.Send(Encoding.Unicode.GetBytes(str), n, SocketFlags.None);
        }

        static string ReceiveString(Socket s)
        {
            int n = ReceiveInt(s);
            byte[] b = new byte[n];
            s.Receive(b, n, SocketFlags.None);
            return Encoding.Unicode.GetString(b, 0, n);
        }

        static void SendCommand(Socket s, int command, int addres = 0, string str = "")
        {
            s.Send(BitConverter.GetBytes(command), sizeof(int), SocketFlags.None);
            if (command == 2)
            {
                s.Send(BitConverter.GetBytes(addres), sizeof(int), SocketFlags.None);
                SendString(s, str);
            }
        }

        public Form1()
        {
            InitializeComponent();
            int nPort = 5284;
            IPEndPoint endPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), nPort);
            s = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            s.Connect(endPoint);
            if (s.Connected)
            {
                serverProcess = true;
                SendCommand(s, 4);
                numThreads = ReceiveInt(s);
                if (numThreads > 0)
                {
                    listSessions.Items.Clear();
                    listSessions.Items.Add($"Все потоки");
                    listSessions.Items.Add($"Главный поток");
                }
                for (int i = 0; i < numThreads; i++)
                {
                    listSessions.Items.Add($"Поток {i}");
                }
            }
            else
            {
                Environment.Exit(1);
            }
        }

        private void UpdateThreads(int threads)
        {
            numThreads = 0;
            listSessions.Items.Clear();
            listSessions.Items.Add($"Все потоки");
            listSessions.Items.Add($"Главный поток");
            for (int i = 0; i < threads; i++)
            {
                listSessions.Items.Add($"Поток {numThreads++}");
            }
        }

        private void StartButton_Click(object sender, EventArgs e)
        {
            if (serverProcess)
            {
                if (numThreads == 0)
                {
                    listSessions.Items.Clear();
                    listSessions.Items.Add($"Все потоки");
                    listSessions.Items.Add($"Главный поток");
                }
                for (int i = 0; i < numberThreads.Value; i++)
                {
                    SendCommand(s, 0);
                    UpdateThreads(ReceiveInt(s));
                }
            }
        }

        private void StopButton_Click(object sender, EventArgs e)
        {
            if (serverProcess)
            {
                SendCommand(s, 1);
                int threads = ReceiveInt(s);
                UpdateThreads(threads);
                if (threads == -1)
                {
                    MessageBox.Show("Нет потоков!");
                }
            }

        }
        private void numberThreads_ValueChanged(object sender, EventArgs e)
        {
            //empty
        }
        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (serverProcess)
            {
                SendCommand(s, 3);
                if (ReceiveString(s) != "ok")
                {
                    MessageBox.Show("Ошибка!");
                }
            }
        }

        private void SendButton_Click(object sender, EventArgs e)
        {

            if (serverProcess)
            {
                string str = textBox.Text.ToString();
                int n = listSessions.SelectedIndex;
                switch (n)
                {
                    case 0:
                        {
                            SendCommand(s, 2, -2, str);
                            break;
                        }
                    case 1:
                        {
                            SendCommand(s, 2, -1, str);
                            break;
                        }
                    default:
                        {
                            SendCommand(s, 2, n - 2, str);
                            break;
                        }
                }
                if (ReceiveInt(s) < 0)
                {
                    MessageBox.Show("Ошибка!");
                }
            }
        }
    }
}
