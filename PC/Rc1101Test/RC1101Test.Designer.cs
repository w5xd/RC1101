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
            this.listBoxFT232H = new System.Windows.Forms.ListBox();
            this.buttonResetFT283 = new System.Windows.Forms.Button();
            this.buttonReload = new System.Windows.Forms.Button();
            this.buttonResetFP = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.buttonTestFP = new System.Windows.Forms.Button();
            this.buttonTS590 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // buttonOK
            // 
            this.buttonOK.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.buttonOK.Location = new System.Drawing.Point(410, 275);
            this.buttonOK.Name = "buttonOK";
            this.buttonOK.Size = new System.Drawing.Size(75, 23);
            this.buttonOK.TabIndex = 10;
            this.buttonOK.Text = "OK";
            this.buttonOK.UseVisualStyleBackColor = true;
            this.buttonOK.Click += new System.EventHandler(this.buttonOK_Click);
            // 
            // listBoxAvailable
            // 
            this.listBoxAvailable.FormattingEnabled = true;
            this.listBoxAvailable.Location = new System.Drawing.Point(13, 65);
            this.listBoxAvailable.Name = "listBoxAvailable";
            this.listBoxAvailable.Size = new System.Drawing.Size(120, 95);
            this.listBoxAvailable.TabIndex = 2;
            this.listBoxAvailable.SelectedIndexChanged += new System.EventHandler(this.listBoxAvailable_SelectedIndexChanged);
            // 
            // buttonStart
            // 
            this.buttonStart.Enabled = false;
            this.buttonStart.Location = new System.Drawing.Point(154, 65);
            this.buttonStart.Name = "buttonStart";
            this.buttonStart.Size = new System.Drawing.Size(75, 23);
            this.buttonStart.TabIndex = 3;
            this.buttonStart.Text = "Start";
            this.buttonStart.UseVisualStyleBackColor = true;
            this.buttonStart.Click += new System.EventHandler(this.buttonStart_Click);
            // 
            // buttonStop
            // 
            this.buttonStop.Enabled = false;
            this.buttonStop.Location = new System.Drawing.Point(154, 94);
            this.buttonStop.Name = "buttonStop";
            this.buttonStop.Size = new System.Drawing.Size(75, 23);
            this.buttonStop.TabIndex = 4;
            this.buttonStop.Text = "Stop";
            this.buttonStop.UseVisualStyleBackColor = true;
            this.buttonStop.Click += new System.EventHandler(this.buttonStop_Click);
            // 
            // buttonK3
            // 
            this.buttonK3.Enabled = false;
            this.buttonK3.Location = new System.Drawing.Point(260, 65);
            this.buttonK3.Name = "buttonK3";
            this.buttonK3.Size = new System.Drawing.Size(75, 23);
            this.buttonK3.TabIndex = 6;
            this.buttonK3.Text = "K3";
            this.buttonK3.UseVisualStyleBackColor = true;
            this.buttonK3.Click += new System.EventHandler(this.buttonK3_Click);
            // 
            // listBoxFT232H
            // 
            this.listBoxFT232H.FormattingEnabled = true;
            this.listBoxFT232H.Location = new System.Drawing.Point(13, 203);
            this.listBoxFT232H.Name = "listBoxFT232H";
            this.listBoxFT232H.Size = new System.Drawing.Size(120, 95);
            this.listBoxFT232H.TabIndex = 8;
            this.listBoxFT232H.SelectedIndexChanged += new System.EventHandler(this.listBoxFT232H_SelectedIndexChanged);
            // 
            // buttonResetFT283
            // 
            this.buttonResetFT283.Enabled = false;
            this.buttonResetFT283.Location = new System.Drawing.Point(154, 274);
            this.buttonResetFT283.Name = "buttonResetFT283";
            this.buttonResetFT283.Size = new System.Drawing.Size(75, 23);
            this.buttonResetFT283.TabIndex = 9;
            this.buttonResetFT283.Text = "Reboot";
            this.buttonResetFT283.UseVisualStyleBackColor = true;
            this.buttonResetFT283.Click += new System.EventHandler(this.buttonResetFT283_Click);
            // 
            // buttonReload
            // 
            this.buttonReload.Location = new System.Drawing.Point(13, 13);
            this.buttonReload.Name = "buttonReload";
            this.buttonReload.Size = new System.Drawing.Size(75, 23);
            this.buttonReload.TabIndex = 0;
            this.buttonReload.Text = "Rescan USB";
            this.buttonReload.UseVisualStyleBackColor = true;
            this.buttonReload.Click += new System.EventHandler(this.buttonReload_Click);
            // 
            // buttonResetFP
            // 
            this.buttonResetFP.Enabled = false;
            this.buttonResetFP.Location = new System.Drawing.Point(154, 137);
            this.buttonResetFP.Name = "buttonResetFP";
            this.buttonResetFP.Size = new System.Drawing.Size(75, 23);
            this.buttonResetFP.TabIndex = 5;
            this.buttonResetFP.Text = "Reboot";
            this.buttonResetFP.UseVisualStyleBackColor = true;
            this.buttonResetFP.Click += new System.EventHandler(this.buttonResetFP_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 46);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(100, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "RC1101 responses:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(10, 187);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(97, 13);
            this.label2.TabIndex = 7;
            this.label2.Text = "FT232H responses";
            // 
            // buttonTestFP
            // 
            this.buttonTestFP.Location = new System.Drawing.Point(260, 136);
            this.buttonTestFP.Name = "buttonTestFP";
            this.buttonTestFP.Size = new System.Drawing.Size(75, 23);
            this.buttonTestFP.TabIndex = 11;
            this.buttonTestFP.Text = "Test";
            this.buttonTestFP.UseVisualStyleBackColor = true;
            this.buttonTestFP.Click += new System.EventHandler(this.buttonTestFP_Click);
            // 
            // buttonTS590
            // 
            this.buttonTS590.Enabled = false;
            this.buttonTS590.Location = new System.Drawing.Point(352, 65);
            this.buttonTS590.Name = "buttonTS590";
            this.buttonTS590.Size = new System.Drawing.Size(75, 23);
            this.buttonTS590.TabIndex = 12;
            this.buttonTS590.Text = "TS590";
            this.buttonTS590.UseVisualStyleBackColor = true;
            this.buttonTS590.Click += new System.EventHandler(this.buttonTS590_Click);
            // 
            // RC1101Test
            // 
            this.AcceptButton = this.buttonOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.buttonOK;
            this.ClientSize = new System.Drawing.Size(497, 310);
            this.Controls.Add(this.buttonTS590);
            this.Controls.Add(this.buttonTestFP);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.buttonResetFP);
            this.Controls.Add(this.buttonReload);
            this.Controls.Add(this.buttonResetFT283);
            this.Controls.Add(this.listBoxFT232H);
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
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonOK;
        private System.Windows.Forms.ListBox listBoxAvailable;
        private System.Windows.Forms.Button buttonStart;
        private System.Windows.Forms.Button buttonStop;
        private System.Windows.Forms.Button buttonK3;
        private System.Windows.Forms.ListBox listBoxFT232H;
        private System.Windows.Forms.Button buttonResetFT283;
        private System.Windows.Forms.Button buttonReload;
        private System.Windows.Forms.Button buttonResetFP;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button buttonTestFP;
        private System.Windows.Forms.Button buttonTS590;
    }
}

