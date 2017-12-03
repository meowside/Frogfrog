using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace VideoStop.Drawable
{
    static class Dot
    {
        public const int RADIUS = 10;
        public static Brush BRUSH { get { return Brushes.Black; } }

        public static void Draw(Graphics target, Point center)
        {
            target.FillEllipse(BRUSH, new RectangleF(center.X - RADIUS, center.Y - RADIUS, RADIUS * 2, RADIUS * 2));
        }
    }
}
