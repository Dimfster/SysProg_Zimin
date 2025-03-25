using System.Diagnostics;

namespace System_Lab1_CSharp
{
    public partial class Form1 : Form
    {
        Process childProcess = null;
        System.Threading.EventWaitHandle stopEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "StopEvent");
        System.Threading.EventWaitHandle startEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "StartEvent");
        System.Threading.EventWaitHandle exitEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "ExitEvent");
        System.Threading.EventWaitHandle confirmEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "ConfirmEvent");

        int numThreads = 0;
        public Form1()
        {
            InitializeComponent();
        }

        private void StartButton_Click(object sender, EventArgs e)
        {
            if (childProcess == null || childProcess.HasExited)
            {
                childProcess = Process.Start("System_Lab1.exe");
                childProcess.EnableRaisingEvents = true;

                // Подписка на событие завершения процесса
                childProcess.Exited += ChildProcess_Exited;

                listSessions.Items.Add($"Все потоки");
                listSessions.Items.Add($"Главный поток");
            }
            else
            {
                for (int i = 0; i < numberThreads.Value; i++)
                {
                    startEvent.Set();
                    confirmEvent.WaitOne();
                    listSessions.Items.Add($"Поток {++numThreads}");
                }
            }
        }

        private void StopButton_Click(object sender, EventArgs e)
        {
            if (!(childProcess == null || childProcess.HasExited))
            {
                stopEvent.Set();
                confirmEvent.WaitOne();
                if (listSessions.Items.Count > 0)
                {
                    listSessions.Items.RemoveAt(listSessions.Items.Count - 1);
                    numThreads--;
                }
            }
        }

        private void numberThreads_ValueChanged(object sender, EventArgs e)
        {
            //empty
        }
        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            if(childProcess != null)
            {
                exitEvent.Set();
                confirmEvent.WaitOne();
            }
        }
        private void ChildProcess_Exited(object sender, EventArgs e)
        {
            // Так как UI обновляется из другого потока, используем Invoke
            this.Invoke((MethodInvoker)delegate {
                listSessions.Items.Clear();
                numThreads = 0;
            });
        }
    }
}
