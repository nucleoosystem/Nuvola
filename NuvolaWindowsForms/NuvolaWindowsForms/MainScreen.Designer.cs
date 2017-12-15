namespace NuvolaWindowsForms
{
    partial class MainScreen
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
            System.Windows.Forms.Panel headerPanel;
            this.searchCombo = new System.Windows.Forms.ComboBox();
            this.searchBox = new System.Windows.Forms.TextBox();
            this.logoPanel = new System.Windows.Forms.Panel();
            this.logoLbl = new System.Windows.Forms.Label();
            this.sidePanel = new System.Windows.Forms.Panel();
            this.sideDevicesBtn = new System.Windows.Forms.Button();
            this.sideExitBtn = new System.Windows.Forms.Button();
            this.sideSettingsBtn = new System.Windows.Forms.Button();
            this.sideGroupsBtn = new System.Windows.Forms.Button();
            this.sideFilesBtn = new System.Windows.Forms.Button();
            this.mainPanel = new System.Windows.Forms.Panel();
            this.sideUploadBtn = new System.Windows.Forms.Button();
            headerPanel = new System.Windows.Forms.Panel();
            headerPanel.SuspendLayout();
            this.logoPanel.SuspendLayout();
            this.sidePanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // headerPanel
            // 
            headerPanel.BackColor = System.Drawing.Color.WhiteSmoke;
            headerPanel.Controls.Add(this.searchCombo);
            headerPanel.Controls.Add(this.searchBox);
            headerPanel.Controls.Add(this.logoPanel);
            headerPanel.Dock = System.Windows.Forms.DockStyle.Top;
            headerPanel.Location = new System.Drawing.Point(0, 0);
            headerPanel.Name = "headerPanel";
            headerPanel.Size = new System.Drawing.Size(866, 67);
            headerPanel.TabIndex = 0;
            // 
            // searchCombo
            // 
            this.searchCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.searchCombo.Font = new System.Drawing.Font("Century Gothic", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.searchCombo.FormattingEnabled = true;
            this.searchCombo.Items.AddRange(new object[] {
            "All",
            "Groups",
            "Users",
            "Files"});
            this.searchCombo.Location = new System.Drawing.Point(577, 21);
            this.searchCombo.Name = "searchCombo";
            this.searchCombo.Size = new System.Drawing.Size(248, 30);
            this.searchCombo.TabIndex = 6;
            // 
            // searchBox
            // 
            this.searchBox.Font = new System.Drawing.Font("Century Gothic", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.searchBox.Location = new System.Drawing.Point(216, 21);
            this.searchBox.Name = "searchBox";
            this.searchBox.Size = new System.Drawing.Size(325, 31);
            this.searchBox.TabIndex = 5;
            this.searchBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.searchBox_KeyPress);
            // 
            // logoPanel
            // 
            this.logoPanel.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(0)))));
            this.logoPanel.Controls.Add(this.logoLbl);
            this.logoPanel.Location = new System.Drawing.Point(0, 0);
            this.logoPanel.Name = "logoPanel";
            this.logoPanel.Size = new System.Drawing.Size(200, 67);
            this.logoPanel.TabIndex = 0;
            // 
            // logoLbl
            // 
            this.logoLbl.AutoSize = true;
            this.logoLbl.Font = new System.Drawing.Font("Century Gothic", 21.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.logoLbl.Location = new System.Drawing.Point(38, 16);
            this.logoLbl.Name = "logoLbl";
            this.logoLbl.Size = new System.Drawing.Size(116, 36);
            this.logoLbl.TabIndex = 1;
            this.logoLbl.Text = "Nuvola";
            // 
            // sidePanel
            // 
            this.sidePanel.BackColor = System.Drawing.Color.Gray;
            this.sidePanel.Controls.Add(this.sideUploadBtn);
            this.sidePanel.Controls.Add(this.sideDevicesBtn);
            this.sidePanel.Controls.Add(this.sideExitBtn);
            this.sidePanel.Controls.Add(this.sideSettingsBtn);
            this.sidePanel.Controls.Add(this.sideGroupsBtn);
            this.sidePanel.Controls.Add(this.sideFilesBtn);
            this.sidePanel.Dock = System.Windows.Forms.DockStyle.Left;
            this.sidePanel.Location = new System.Drawing.Point(0, 67);
            this.sidePanel.Name = "sidePanel";
            this.sidePanel.Size = new System.Drawing.Size(200, 276);
            this.sidePanel.TabIndex = 1;
            // 
            // sideDevicesBtn
            // 
            this.sideDevicesBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.sideDevicesBtn.Font = new System.Drawing.Font("Century Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.sideDevicesBtn.Location = new System.Drawing.Point(0, 129);
            this.sideDevicesBtn.Name = "sideDevicesBtn";
            this.sideDevicesBtn.Size = new System.Drawing.Size(200, 37);
            this.sideDevicesBtn.TabIndex = 11;
            this.sideDevicesBtn.Text = "Devices";
            this.sideDevicesBtn.UseVisualStyleBackColor = true;
            this.sideDevicesBtn.Click += new System.EventHandler(this.sideDevicesBtn_Click);
            // 
            // sideExitBtn
            // 
            this.sideExitBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.sideExitBtn.Font = new System.Drawing.Font("Century Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.sideExitBtn.Location = new System.Drawing.Point(0, 236);
            this.sideExitBtn.Name = "sideExitBtn";
            this.sideExitBtn.Size = new System.Drawing.Size(200, 37);
            this.sideExitBtn.TabIndex = 10;
            this.sideExitBtn.Text = "Exit";
            this.sideExitBtn.UseVisualStyleBackColor = true;
            this.sideExitBtn.Click += new System.EventHandler(this.sideExitBtn_Click);
            // 
            // sideSettingsBtn
            // 
            this.sideSettingsBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.sideSettingsBtn.Font = new System.Drawing.Font("Century Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.sideSettingsBtn.Location = new System.Drawing.Point(0, 172);
            this.sideSettingsBtn.Name = "sideSettingsBtn";
            this.sideSettingsBtn.Size = new System.Drawing.Size(200, 37);
            this.sideSettingsBtn.TabIndex = 9;
            this.sideSettingsBtn.Text = "Settings";
            this.sideSettingsBtn.UseVisualStyleBackColor = true;
            // 
            // sideGroupsBtn
            // 
            this.sideGroupsBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.sideGroupsBtn.Font = new System.Drawing.Font("Century Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.sideGroupsBtn.Location = new System.Drawing.Point(0, 43);
            this.sideGroupsBtn.Name = "sideGroupsBtn";
            this.sideGroupsBtn.Size = new System.Drawing.Size(200, 37);
            this.sideGroupsBtn.TabIndex = 8;
            this.sideGroupsBtn.Text = "Groups";
            this.sideGroupsBtn.UseVisualStyleBackColor = true;
            // 
            // sideFilesBtn
            // 
            this.sideFilesBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.sideFilesBtn.Font = new System.Drawing.Font("Century Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.sideFilesBtn.Location = new System.Drawing.Point(0, 0);
            this.sideFilesBtn.Name = "sideFilesBtn";
            this.sideFilesBtn.Size = new System.Drawing.Size(200, 37);
            this.sideFilesBtn.TabIndex = 7;
            this.sideFilesBtn.Text = "Files";
            this.sideFilesBtn.UseVisualStyleBackColor = true;
            this.sideFilesBtn.Click += new System.EventHandler(this.sideFilesBtn_Click);
            // 
            // mainPanel
            // 
            this.mainPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainPanel.Location = new System.Drawing.Point(200, 67);
            this.mainPanel.Name = "mainPanel";
            this.mainPanel.Size = new System.Drawing.Size(666, 276);
            this.mainPanel.TabIndex = 2;
            // 
            // sideUploadBtn
            // 
            this.sideUploadBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.sideUploadBtn.Font = new System.Drawing.Font("Century Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.sideUploadBtn.Location = new System.Drawing.Point(0, 86);
            this.sideUploadBtn.Name = "sideUploadBtn";
            this.sideUploadBtn.Size = new System.Drawing.Size(200, 37);
            this.sideUploadBtn.TabIndex = 12;
            this.sideUploadBtn.Text = "Upload";
            this.sideUploadBtn.UseVisualStyleBackColor = true;
            this.sideUploadBtn.Click += new System.EventHandler(this.sideUploadBtn_Click);
            // 
            // MainScreen
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(866, 343);
            this.Controls.Add(this.mainPanel);
            this.Controls.Add(this.sidePanel);
            this.Controls.Add(headerPanel);
            this.Name = "MainScreen";
            this.Text = "MainScreen";
            this.Load += new System.EventHandler(this.MainScreen_Load);
            headerPanel.ResumeLayout(false);
            headerPanel.PerformLayout();
            this.logoPanel.ResumeLayout(false);
            this.logoPanel.PerformLayout();
            this.sidePanel.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel logoPanel;
        private System.Windows.Forms.Panel sidePanel;
        private System.Windows.Forms.Label logoLbl;
        private System.Windows.Forms.TextBox searchBox;
        private System.Windows.Forms.ComboBox searchCombo;
        private System.Windows.Forms.Button sideFilesBtn;
        private System.Windows.Forms.Button sideExitBtn;
        private System.Windows.Forms.Button sideSettingsBtn;
        private System.Windows.Forms.Button sideGroupsBtn;
        private System.Windows.Forms.Button sideDevicesBtn;
        private System.Windows.Forms.Panel mainPanel;
        private System.Windows.Forms.Button sideUploadBtn;

    }
}