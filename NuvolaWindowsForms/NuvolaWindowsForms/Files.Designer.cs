namespace NuvolaWindowsForms
{
    partial class Files
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
            this.latestLbl = new System.Windows.Forms.Label();
            this.allFilesLbl = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // latestLbl
            // 
            this.latestLbl.AutoSize = true;
            this.latestLbl.Font = new System.Drawing.Font("Century Gothic", 21.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.latestLbl.Location = new System.Drawing.Point(12, 9);
            this.latestLbl.Name = "latestLbl";
            this.latestLbl.Size = new System.Drawing.Size(98, 36);
            this.latestLbl.TabIndex = 3;
            this.latestLbl.Text = "Latest";
            // 
            // allFilesLbl
            // 
            this.allFilesLbl.AutoSize = true;
            this.allFilesLbl.Font = new System.Drawing.Font("Century Gothic", 21.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.allFilesLbl.Location = new System.Drawing.Point(12, 342);
            this.allFilesLbl.Name = "allFilesLbl";
            this.allFilesLbl.Size = new System.Drawing.Size(113, 36);
            this.allFilesLbl.TabIndex = 4;
            this.allFilesLbl.Text = "All Files";
            // 
            // Files
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(783, 396);
            this.Controls.Add(this.allFilesLbl);
            this.Controls.Add(this.latestLbl);
            this.Name = "Files";
            this.Text = "Files";
            this.Load += new System.EventHandler(this.Files_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label latestLbl;
        private System.Windows.Forms.Label allFilesLbl;
    }
}