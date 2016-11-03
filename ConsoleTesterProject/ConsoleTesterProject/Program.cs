using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ConsoleTesterProject
{

    class Program
    {
        static void Main(string[] args)
        {
            while (true)
            {
                char temp = Console.ReadKey(true).KeyChar;

                if (Char.IsLetterOrDigit(temp) || temp == ' ')
                    Console.WriteLine(temp);
            }
        }
    }
}
