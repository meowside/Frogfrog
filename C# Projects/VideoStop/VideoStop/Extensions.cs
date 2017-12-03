using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using VideoStop.Drawable;

namespace VideoStop
{
    static class Extensions
    {
        public static Game.Difficulties ToDifficulty(this object obj)
        {
            return (Game.Difficulties)Convert.ToInt32(obj);
        }

        public static void DrawDice(this Graphics target, int dice)
        {
            typeof(Dice).GetMethod($"Draw{dice}").Invoke(null, new object[] { target });
        }
    }
}