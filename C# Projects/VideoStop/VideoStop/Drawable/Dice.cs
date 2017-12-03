using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VideoStop.Drawable
{
    static class Dice
    {
        public static void Draw1(Graphics target)
        {
            Dot.Draw(target, new Point(80, 80));
        }

        public static void Draw2(Graphics target)
        {
            Dot.Draw(target, new Point(40, 40));
            Dot.Draw(target, new Point(80, 80));
        }

        public static void Draw3(Graphics target)
        {
            Dot.Draw(target, new Point(40, 40));
            Dot.Draw(target, new Point(60, 60));
            Dot.Draw(target, new Point(80, 80));
        }

        public static void Draw4(Graphics target)
        {
            Dot.Draw(target, new Point(40, 40));
            Dot.Draw(target, new Point(40, 80));
            Dot.Draw(target, new Point(80, 40));
            Dot.Draw(target, new Point(80, 80));
        }

        public static void Draw5(Graphics target)
        {
            Dot.Draw(target, new Point(40, 40));
            Dot.Draw(target, new Point(40, 80));
            Dot.Draw(target, new Point(80, 40));
            Dot.Draw(target, new Point(80, 80));
            Dot.Draw(target, new Point(60, 60));
        }

        public static void Draw6(Graphics target)
        {
            Dot.Draw(target, new Point(40, 40));
            Dot.Draw(target, new Point(40, 60));
            Dot.Draw(target, new Point(40, 80));
            Dot.Draw(target, new Point(80, 40));
            Dot.Draw(target, new Point(80, 60));
            Dot.Draw(target, new Point(80, 80));
        }
    }
}
