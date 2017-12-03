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
    public partial class Menu : Form
    {
        public static Menu Current;

        public Menu()
        {
            InitializeComponent();

            if (Current != null)
            {
                Current.Close();
            }
            Current = this;
        }

        private void DifficultyRunGame(object sender, EventArgs e)
        {
            Button send = (Button)sender;
            Game.Start(send.Tag.ToDifficulty());
            this.Enabled = false;
        }
    }
}
