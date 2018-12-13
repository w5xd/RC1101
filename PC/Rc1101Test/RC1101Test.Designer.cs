namespace Rc1101Test
{
    partial class RC1101Test
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
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
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.buttonOK = new System.Windows.Forms.Button();
            this.listBoxAvailable = new System.Windows.Forms.ListBox();
            this.buttonStart = new System.Windows.Forms.Button();
            this.buttonStop = new System.Windows.Forms.Button();
            this.buttonK3 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // buttonOK
            // 
            this.buttonOK.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.buttonOK.Location = new System.Drawing.Point(410, 275);
            this.buttonOK.Name = "buttonOK";
            this.buttonOK.Size = new System.Drawing.Size(75, 23);
            this.buttonOK.TabIndex = 0;
            this.buttonOK.Text = "OK";
            this.buttonOK.UseVisualStyleBackColor = true;
            this.buttonOK.Click += new System.EventHandler(this.buttonOK_Click);
            // 
            // listBoxAvailable
            // 
            this.listBoxAvailable.FormattingEnabled = true;
            this.listBoxAvailable.Location = new System.Drawing.Point(13, 13);
            this.listBoxAvailable.Name = "listBoxAvailable";
            this.listBoxAvailable.Size = new System.Drawing.Size(120, 95);
            this.listBoxAvailable.TabIndex = 1;
            this.listBoxAvailable.SelectedIndexChanged += new System.EventHandler(this.listBoxAvailable_SelectedIndexChanged);
            // 
            // buttonStart
            // 
            this.buttonStart.Enabled = false;
            this.buttonStart.Location = new System.Drawing.Point(154, 13);
            this.buttonStart.Name = "buttonStart";
            this.buttonStart.Size = new System.Drawing.Size(75, 23);
            this.buttonStart.TabIndex = 2;
            this.buttonStart.Text = "Start";
            this.buttonStart.UseVisualStyleBackColor = true;
            this.buttonStart.Click += new System.EventHandler(this.buttonStart_Click);
            // 
            // buttonStop
            // 
            this.buttonStop.Enabled = false;
            this.buttonStop.Location = new System.Drawing.Point(154, 42);
            this.buttonStop.Name = "buttonStop";
            this.buttonStop.Size = new System.Drawing.Size(75, 23);
            this.buttonStop.TabIndex = 3;
            this.buttonStop.Text = "Stop";
            this.buttonStop.UseVisualStyleBackColor = true;
            this.buttonStop.Click += new System.EventHandler(this.buttonStop_Click);
            // 
            // buttonK3
            // 
            this.buttonK3.Enabled = false;
            this.buttonK3.Location = new System.Drawing.Point(260, 13);
            this.buttonK3.Name = "buttonK3";
            this.buttonK3.Size = new System.Drawing.Size(75, 23);
            this.buttonK3.TabIndex = 4;
            this.buttonK3.Text = "K3";
            this.buttonK3.UseVisualStyleBackColor = true;
            this.buttonK3.Click += new System.EventHandler(this.buttonK3_Click);
            // 
            // RC1101Test
            // 
            this.AcceptButton = this.buttonOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.buttonOK;
            this.ClientSize = new System.Drawing.Size(497, 310);
            this.Controls.Add(this.buttonK3);
            this.Controls.Add(this.buttonStop);
            this.Controls.Add(this.buttonStart);
            this.Controls.Add(this.listBoxAvailable);
            this.Controls.Add(this.buttonOK);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.Name = "RC1101Test";
            this.Text = "RC1101 Test";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.RC1101Test_FormClosing);
            this.Load += new System.EventHandler(this.RC1101Test_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button buttonOK;
        private System.Windows.Forms.ListBox listBoxAvailable;
        private System.Windows.Forms.Button buttonStart;
        private System.Windows.Forms.Button buttonStop;
        private System.Windows.Forms.Button buttonK3;
    }
}

