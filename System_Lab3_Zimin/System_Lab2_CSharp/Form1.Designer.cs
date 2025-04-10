namespace System_Lab1_CSharp
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            StartButton = new Button();
            StopButton = new Button();
            numberThreads = new NumericUpDown();
            Threads = new Label();
            listSessions = new ListBox();
            textBox = new TextBox();
            SendButton = new Button();
            ((System.ComponentModel.ISupportInitialize)numberThreads).BeginInit();
            SuspendLayout();
            // 
            // StartButton
            // 
            StartButton.Location = new Point(366, 29);
            StartButton.Name = "StartButton";
            StartButton.Size = new Size(94, 29);
            StartButton.TabIndex = 0;
            StartButton.Text = "Start";
            StartButton.UseVisualStyleBackColor = true;
            StartButton.Click += StartButton_Click;
            // 
            // StopButton
            // 
            StopButton.Location = new Point(466, 29);
            StopButton.Name = "StopButton";
            StopButton.Size = new Size(94, 29);
            StopButton.TabIndex = 1;
            StopButton.Text = "Stop";
            StopButton.UseVisualStyleBackColor = true;
            StopButton.Click += StopButton_Click;
            // 
            // numberThreads
            // 
            numberThreads.Location = new Point(210, 29);
            numberThreads.Name = "numberThreads";
            numberThreads.Size = new Size(150, 27);
            numberThreads.TabIndex = 2;
            numberThreads.ValueChanged += numberThreads_ValueChanged;
            // 
            // Threads
            // 
            Threads.AutoSize = true;
            Threads.Location = new Point(210, 6);
            Threads.Name = "Threads";
            Threads.Size = new Size(113, 20);
            Threads.TabIndex = 3;
            Threads.Text = "Число потоков";
            // 
            // listSessions
            // 
            listSessions.FormattingEnabled = true;
            listSessions.Location = new Point(1, 2);
            listSessions.Name = "listSessions";
            listSessions.Size = new Size(203, 384);
            listSessions.TabIndex = 4;
            // 
            // textBox
            // 
            textBox.Location = new Point(210, 62);
            textBox.Name = "textBox";
            textBox.Size = new Size(150, 27);
            textBox.TabIndex = 5;
            // 
            // SendButton
            // 
            SendButton.Location = new Point(366, 64);
            SendButton.Name = "SendButton";
            SendButton.Size = new Size(94, 29);
            SendButton.TabIndex = 6;
            SendButton.Text = "Send";
            SendButton.UseVisualStyleBackColor = true;
            SendButton.Click += SendButton_Click;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(720, 400);
            Controls.Add(SendButton);
            Controls.Add(textBox);
            Controls.Add(listSessions);
            Controls.Add(Threads);
            Controls.Add(numberThreads);
            Controls.Add(StopButton);
            Controls.Add(StartButton);
            Name = "Form1";
            Text = "Form1";
            FormClosed += Form1_FormClosed;
            ((System.ComponentModel.ISupportInitialize)numberThreads).EndInit();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Button StartButton;
        private Button StopButton;
        private NumericUpDown numberThreads;
        private Label Threads;
        private ListBox listSessions;
        private TextBox textBox;
        private Button SendButton;
    }
}
