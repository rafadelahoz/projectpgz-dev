﻿using System;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Reflection;
using System.IO;
using System.Diagnostics;

namespace WorldGenGUI
{
    public partial class PGZForm : Form
    {
        private ComboBoxEx comboSelecChar = new ComboBoxEx();
        private ArrayList toolData = new ArrayList();
        private ArrayList enemyData = new ArrayList();
        private ArrayList playerData = new ArrayList();
        private ArrayList themeData = new ArrayList();

        public PGZForm()
        {
            InitializeComponent();
        }

        private void PGZForm_Load(object sender, EventArgs e)
        {
            comboSelecChar.ImageList = charImgList;
            comboSelecChar.DropDownStyle = ComboBoxStyle.DropDownList;
            comboSelecChar.Items.Add(new ComboBoxExItem("Indi", 0));
            comboSelecChar.Items.Add(new ComboBoxExItem("Skull Man", 1));
            comboSelecChar.Location = new Point(225, 40);
            tabPage1.Controls.Add(comboSelecChar);
            comboSelecChar.Height = charImgList.ImageSize.Height * charImgList.Images.Count;
            comboSelecChar.ItemHeight = charImgList.ImageSize.Height;
            comboSelecChar.SelectedIndex = 0;
            statusProgress.Visible = false;
            //butPlay.Enabled = false;
            
            comboDiff.SelectedIndex = 2;
            comboSize.SelectedIndex = 2;
            comboTheme.SelectedIndex = 0;
            comboNumZones.SelectedIndex = 1;

            
            System.Environment.CurrentDirectory = @".\bin\";

            if (System.IO.File.Exists("DataBaseContent.exe"))
            {
                Process cmd = new Process();
                cmd.StartInfo.UseShellExecute = false;
                cmd.StartInfo.FileName = "DataBaseContent.exe";
                cmd.StartInfo.CreateNoWindow = true;
                cmd.Start();
                cmd.WaitForExit();
            }

            else if (!System.IO.File.Exists("dbdata"))
            {
                MessageBox.Show("You need to have dbdata in /bin folder!",
                "dbdata not found!",
                MessageBoxButtons.OK,
                MessageBoxIcon.Exclamation,
                MessageBoxDefaultButton.Button1);
                Close();
            }
            
            /*loadDBData("dbdata");
            enemyViewer.DataSource = enemyData;
            toolViewer.DataSource = toolData;
            initDataSets();*/
            backgroundWorker1.RunWorkerAsync();
        }

        private String genName(){
            Random rand = new Random();
            string name = "";
            int namelen = rand.Next(4, 10);

            string[] letters = new string[2] { "bcdfghjklmnpqrstvwxyz", "aeiou" };
            int[] letterlen = new int[2] { letters[0].Length, letters[1].Length };

            for (int i = 0; i < namelen; i++)
                name += letters[i % 2][rand.Next() % letterlen[i % 2]];

            string capitalName = "";
            capitalName += char.ToUpper(name[0]);
            capitalName += name.Substring(1);
            return capitalName;
        }

        public void RunWithRedirect(string cmdPath)
        {
            var proc = new Process();
            proc.StartInfo.FileName = cmdPath;
            proc.StartInfo.UseShellExecute = false;

            // set up output redirection
            proc.StartInfo.RedirectStandardOutput = true;
            proc.StartInfo.RedirectStandardError = true;
            proc.EnableRaisingEvents = true;
            proc.StartInfo.CreateNoWindow = true;
            // see below for output handler
            proc.ErrorDataReceived += proc_DataReceived;
            proc.OutputDataReceived += proc_DataReceived;

            proc.Exited += new EventHandler(proc_Exited);

            proc.Start();

            proc.BeginErrorReadLine();
            proc.BeginOutputReadLine();

            //proc.WaitForExit();
        }

        public void proc_DataReceived(object sender, DataReceivedEventArgs e)
        {
            // output will be in string e.Data
            if (e.Data != null)
            {
                string[] stepInfo = e.Data.Split('#');

                if (stepInfo.Length > 1)
                {
                    this.Invoke((MethodInvoker)delegate
                    {
                        // runs on UI thread
                        //incrementar progressBar
                        statusProgress.Value += 1;
                    });

                    statusText.Text = stepInfo[1];
                    // esperamos un poco para que se pueda leer
                    System.Threading.Thread.Sleep(250);
                }
                // Filtramos basura
                else if (!e.Data.Contains("Ya existe"))
                    statusText.Text = e.Data;
            }
        }

        public void proc_Exited(object sender, System.EventArgs e)
        {
            //new System.Threading.Thread(delegate() {statusProgress.Visible = false;}).Start();
            this.Invoke((MethodInvoker)delegate
            {
                // runs on UI thread
                //statusProgress.Visible = false;
                statusText.Text = "Generation Finished!";
                copyGameData();
                butPlay.Enabled = true;
            });

            Process proc = (Process)sender;
            proc.Close();
        }

        private void butGenerate_Click_1(object sender, EventArgs e)
        {
            bool allGood = true;
            if (tBoxName.TextLength == 0)
                tBoxName.Text = genName();

            if (tBoxWorldName.TextLength == 0)
                tBoxWorldName.Text = genName() + " World";
            
            if (!System.IO.File.Exists("generatorpgz.exe"))
            {
                MessageBox.Show("You need to have generatorpgz.exe in /bin folder!",
                "Generator.exe not found!",
                MessageBoxButtons.OK,
                MessageBoxIcon.Exclamation,
                MessageBoxDefaultButton.Button1);
                allGood = false;
            }
            if (!System.IO.File.Exists("projectpgz-vs2010.exe"))
            {
                MessageBox.Show("You need to have projectpgz-vs2010.exe in /bin folder!",
                "Projectpgz.exe not found!",
                MessageBoxButtons.OK,
                MessageBoxIcon.Exclamation,
                MessageBoxDefaultButton.Button1);
                allGood = false;
            }

            if (allGood)
            {
                // Save config files
                saveDecidatorData();
                saveHistoriator();
                saveInputData();

                // Launch Generator
                statusProgress.Visible = true;       
                RunWithRedirect("generatorpgz.exe");
                
                butGenerate.Enabled = false;
            }
        }

        private void butClose_Click_1(object sender, EventArgs e)
        {
            Close();
        }

        private void butPlay_Click(object sender, EventArgs e)
        {
            // Launch game
            System.Environment.CurrentDirectory = @"..\The Adventure of " + tBoxName.Text;
            Process proc = new Process();
            proc.StartInfo.UseShellExecute = false;
            proc.StartInfo.FileName = @"game.exe";
            proc.Start();
            proc.WaitForExit();
        }

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            loadDBData("dbdata");
            enemyViewer.DataSource = enemyData;
            toolViewer.DataSource = toolData;
            initDataSets();
        }

        private void loadDBData(string filename) 
        {
            StreamReader sr;
            string line;
            string[] data, values;
            sr = File.OpenText(filename);
            // Players
            line = sr.ReadLine();
            data = line.Split(';');
            for (int i = 0; i < data.Length-1; i++)
            {
                values = data[i].Split(',');
                playerData.Add(new Player(Convert.ToInt32(values[0]), values[1]));
            }
            // Enemies
            line = sr.ReadLine();
            data = line.Split(';');
            for (int i = 0; i < data.Length - 1; i++)
            {
                values = data[i].Split(',');
                enemyData.Add(new Enemy(Convert.ToInt32(values[0]), values[1], false, false));
            }
            // Themes
            line = sr.ReadLine();
            data = line.Split(';');
            for (int i = 0; i < data.Length - 1; i++)
            {
                values = data[i].Split(',');
                themeData.Add(new Theme(Convert.ToInt32(values[0]), values[1]));
            }
            // Tools
            line = sr.ReadLine();
            data = line.Split(';');
            for (int i = 0; i < data.Length - 1; i++)
            {
                values = data[i].Split(',');
                toolData.Add(new Tool(Convert.ToInt32(values[0]), values[1], false, false));
            }
            sr.Close();
        }

        private void initDataSets() {

            Tool.SortingOrder = SortOrder.Ascending;
            toolData.Sort();

            Enemy.SortingOrder = SortOrder.Ascending;
            enemyData.Sort();

            DataGridViewCellStyle style;
            style = new DataGridViewCellStyle();
            style.Alignment = System.Windows.Forms.DataGridViewContentAlignment.BottomCenter;
            style.BackColor = System.Drawing.Color.MidnightBlue;
            style.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            style.ForeColor = System.Drawing.Color.White;
            style.WrapMode = System.Windows.Forms.DataGridViewTriState.True;

            toolViewer.EnableHeadersVisualStyles = false;
            foreach (DataGridViewColumn col in toolViewer.Columns)
                col.HeaderCell.Style = style;
            toolViewer.Columns[0].HeaderText = "Tool Name";
            toolViewer.Columns[1].HeaderText = "I Want it!";
            toolViewer.Columns[2].HeaderText = "Don't Want it!";

            toolViewer.AllowUserToOrderColumns = false;
            toolViewer.AllowUserToResizeColumns = false;
            toolViewer.AllowUserToResizeRows = false;
            toolViewer.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.ColumnHeader;

            enemyViewer.EnableHeadersVisualStyles = false;
            foreach (DataGridViewColumn col in enemyViewer.Columns)
                col.HeaderCell.Style = style;
            enemyViewer.Columns[0].HeaderText = "Enemy Name";
            enemyViewer.Columns[1].HeaderText = "I Want it!";
            enemyViewer.Columns[2].HeaderText = "Don't Want it!";

            enemyViewer.AllowUserToOrderColumns = false;
            enemyViewer.AllowUserToResizeColumns = false;
            enemyViewer.AllowUserToResizeRows = false;
            enemyViewer.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.ColumnHeader;
            
        }

        private void toolViewer_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            if (e.RowIndex >= 0 && e.ColumnIndex >= 0)
            {
                if (e.ColumnIndex == 1)
                    if ((bool)toolViewer[e.ColumnIndex, e.RowIndex].Value)
                    {
                        toolViewer[e.ColumnIndex + 1, e.RowIndex].Value = false;
                        toolViewer[e.ColumnIndex, e.RowIndex].Style.BackColor = Color.Green;
                    }
                    else
                        toolViewer[e.ColumnIndex, e.RowIndex].Style.BackColor = Color.White;
                else
                    if ((bool)toolViewer[e.ColumnIndex, e.RowIndex].Value)
                    {
                        toolViewer[e.ColumnIndex - 1, e.RowIndex].Value = false;
                        toolViewer[e.ColumnIndex, e.RowIndex].Style.BackColor = Color.Red;
                    }
                    else
                        toolViewer[e.ColumnIndex, e.RowIndex].Style.BackColor = Color.White;
            }
        }

        private void enemyViewer_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            if (e.RowIndex >= 0 && e.ColumnIndex >= 0)
            {
                if (e.ColumnIndex == 1)
                    if ((bool)enemyViewer[e.ColumnIndex, e.RowIndex].Value)
                    {
                        enemyViewer[e.ColumnIndex + 1, e.RowIndex].Value = false;
                        enemyViewer[e.ColumnIndex, e.RowIndex].Style.BackColor = Color.Green;
                    }
                    else
                        enemyViewer[e.ColumnIndex, e.RowIndex].Style.BackColor = Color.White;
                else
                    if ((bool)enemyViewer[e.ColumnIndex, e.RowIndex].Value)
                    {
                        enemyViewer[e.ColumnIndex - 1, e.RowIndex].Value = false;
                        enemyViewer[e.ColumnIndex, e.RowIndex].Style.BackColor = Color.Red;
                    }
                    else
                        enemyViewer[e.ColumnIndex, e.RowIndex].Style.BackColor = Color.White;
            }
        }

        private void saveDecidatorData() 
        {
            StreamWriter sw = new StreamWriter("decidator.dat");
            int i = 0;
            foreach (DataGridViewRow row in enemyViewer.Rows)
            {
                sw.Write(enemyViewer["Name", i].Value + " ");
                if ((bool)enemyViewer["Want", i].Value)
                    sw.Write(1 + " ");
                else sw.Write(0 + " ");
                if ((bool)enemyViewer["NoWant", i].Value)
                    sw.WriteLine(1);
                else
                    sw.WriteLine(0);
                i++;
            }
            sw.Close();
        }

        private void saveInputData()
        {
            StreamWriter sw = new StreamWriter("input.dat");
            sw.WriteLine("thematic=" + comboTheme.SelectedItem.ToString());
            sw.WriteLine("playerName=" + tBoxName.Text);
            sw.WriteLine("player=" + comboSelecChar.SelectedIndex);
            sw.WriteLine("difficulty=" + comboDiff.SelectedIndex);
            sw.WriteLine("worldSize=" + comboSize.SelectedIndex);
            sw.WriteLine("worldName=" + tBoxWorldName.Text);
            sw.WriteLine("ratio=" + barRatio.Value);

            sw.WriteLine("toolsSet=" + "1,2"); //FIXME hardcode
            sw.WriteLine("enemiesSet=" + "1,2"); //FIXME hardcode
            sw.WriteLine("zonesSet=" + "1,2"); //FIXME hardcode
            sw.Close();
        }

        private void saveHistoriator()
        {
            StreamWriter sw = new StreamWriter("historiator.dat");
            sw.WriteLine(prologFormat("playerName", tBoxName.Text));
            sw.WriteLine(prologFormat("thematic", comboTheme.SelectedItem.ToString()));
            sw.WriteLine(prologFormat("worldSize", comboSize.SelectedItem.ToString()));
            sw.Close();
        }

        private string prologFormat(string name, string value) 
        {
            return name + "('" + value + "')."; 
        }

        private void copyGameData() 
        {
            string destPath = @"..\The Adventure of " + tBoxName.Text + "\\";
            System.IO.Directory.CreateDirectory(destPath);
            CopyFolder("data", destPath + @"data");
            System.IO.File.Copy("projectpgz-vs2010.exe", destPath + "game.exe", true);
            System.IO.File.Copy("openal32.dll", destPath + "openal32.dll", true);
            System.IO.File.Copy("libsndfile-1.dll", destPath + "libsndfile-1.dll", true);
            System.IO.File.Copy("prologue.txt", destPath + "prologue.txt", true);
            
        }

        static public void CopyFolder(string sourceFolder, string destFolder)
        {
            if (Directory.Exists(destFolder))
                Directory.Delete(destFolder, true);
            Directory.CreateDirectory(destFolder);
            string[] files = Directory.GetFiles(sourceFolder);
            foreach (string file in files)
            {
                string name = Path.GetFileName(file);
                string dest = Path.Combine(destFolder, name);
                File.Copy(file, dest);
            }
            string[] folders = Directory.GetDirectories(sourceFolder);
            foreach (string folder in folders)
            {
                string name = Path.GetFileName(folder);
                string dest = Path.Combine(destFolder, name);
                CopyFolder(folder, dest);
            }
        }

    }
}
