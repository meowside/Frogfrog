using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ConsoleTesterProject
{

    class Program
    {
        static void Main(string[] args)
        {
            Image test = Image.FromFile(@"giphy-downsized-large.gif");

            FrameDimension fd = new FrameDimension(test.FrameDimensionsList[0]);

            int count = test.GetFrameCount(fd);

            AnimatedGif 

            Console.ReadLine();
        }
    }
}
