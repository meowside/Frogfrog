using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace VideoStop
{
    public partial class Game : Form
    {
        public static Game Current;
        private static readonly string CAPTION = "You {0}";
        private static readonly string TEXT = "You {0} after {1} rounds!";

        public enum Difficulties
        {
            Easy,
            Medium,
            Hard
        }

        private readonly StringBuilder FormName;

        private Difficulties _difficulty    = Difficulties.Easy;
        private int _delay                  = 0;
        private Timer _timer                = null;
        private bool _running               = false;
        private int _score                  = 0;
        private uint _rounds                 = 0;

        public Difficulties Difficulty
        {
            set
            {
                _difficulty = value;
                Delay = 1000 / (int)value;
            } 
            get { return _difficulty; }
        }
        public int Delay
        {
            set
            {
                _delay = value;
                if (this._timer != null)
                    this.Timer.Interval = value;
            }
            get { return _delay; }
        }
        public Timer Timer
        {
            set { _timer = value; }
            get { return _timer; }
        }
        public bool Running
        {
            set
            {
                _running = value;
                if (this._timer != null)
                    this._timer.Enabled = value;

                if (this.button1 != null)
                    this.button1.Text = value ? "Stop" : "Start";
            }
            get { return _running; }
        }
        public int Score
        {
            private set
            {
                _score = value;
                int len = _score.ToString().Length;
                this.FormName.Remove(FormName.Length - len, len);
                this.FormName.Append(value);
                string name = this.FormName.ToString();
                this.Name = name;
                this.Text = name;
                this.label1.Text = value.ToString();
            }
            get { return _score; }
        }
        public uint Rounds
        {
            private set
            {
                _rounds = value;
                label3.Text = value.ToString();
            }
            get { return _rounds; }
        }

        public readonly Graphics[] TargetTextureHolder;
        public readonly int[] CurrentValues;
        public readonly Random RandomNumberGenerator;

        public Game(Difficulties Difficulty)
        {
            InitializeComponent();

            if (Current != null)
            {
                Current.Close();
            }
            Current = this;

            FormName = new StringBuilder("VideoStop - Skóre: 0");
            Score = 0;

            PictureBox[] items = this.groupBox1.Controls.OfType<PictureBox>().ToArray();
            this.TargetTextureHolder = new Graphics[items.Length];
            for(int i = 0; i < items.Length; i++)
            {
                this.TargetTextureHolder[i] = items[i].CreateGraphics();
            }
            this.CurrentValues = new int[this.TargetTextureHolder.Length];
            this.RandomNumberGenerator = new Random();

            this.Timer = new Timer();
            this.Timer.Tick += this.GameLoop;
            this.Difficulty = Difficulty;
        }

        private void GameLoop(object sender, EventArgs e)
        {
            for (int i = 0; i < this.CurrentValues.Length; i++)
            {
                this.CurrentValues[i] = this.RandomNumberGenerator.Next(1, 7);
                this.TargetTextureHolder[i].Clear(Color.White);
                this.TargetTextureHolder[i].DrawDice(this.CurrentValues[i]);
            }

            Rounds++;
        }

        private void Game_FormClosing(object sender, FormClosingEventArgs e)
        {
            Running = false;
            Timer.Dispose();

            Current = null;
            VideoStop.Menu.Current.Enabled = true;
            VideoStop.Menu.Current.Focus();
        }

        

        private void Action_Click(object sender, EventArgs e)
        {
            this.Running = !this.Running;
            Dictionary<int, int> counter = new Dictionary<int, int>();

            if (!this.Running)
            {
                foreach(int value in this.CurrentValues)
                {
                    counter[value] = counter.ContainsKey(value) ? ++counter[value] : 1;
                }

                if(counter.Count == this.CurrentValues.Length)
                {
                    Score--;
                }
                else
                {
                    foreach (KeyValuePair<int, int> value in counter.Where(x => x.Value > 1))
                    {
                        Score += value.Value - 1;
                    }
                }
                
                if(Math.Abs(Score) >= 3)
                {
                    bool win = Score > 0;
                    MessageBox.Show(string.Format(TEXT, win ? "win" : "lose", Rounds),
                                    string.Format(CAPTION, win ? "WIN" : "LOSE"),
                                    MessageBoxButtons.OK,
                                    win ? MessageBoxIcon.Information : MessageBoxIcon.Error);
                    this.Close();
                }
            }
            else
            {
                GameLoop(null, null);
            }
        }

        public static void Start(Difficulties Difficulty)
        {
            new Game(Difficulty).Show();
        }

        private void Game_Shown(object sender, EventArgs e)
        {
            GameLoop(null, null);
            this.Running = true;
        }
    }
}
