using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ConsoleTesterProject
{
    class Foo
    {
        public int X { get; set; } = 0;
    }

    class Program
    {

        static void Main(string[] args)
        {
            /*List<Foo> t = new List<Foo>();
            Action x = null;

            for(int i = 0; i < 3000; i++)
            {
                t.Add(new Foo(i));
                x += t[i].Print;
            }

            Stopwatch time = Stopwatch.StartNew();

            x.Invoke();

            //t?.Invoke();

            time.Stop();

            Console.WriteLine($"{time.ElapsedTicks} {time.Elapsed} FPS:{1000 / time.ElapsedMilliseconds}");

            time = Stopwatch.StartNew();

            Parallel.ForEach(x.GetInvocationList(), (a, b, c) => a.DynamicInvoke());

            //t.BeginInvoke(Foo, )

            //Parallel.ForEach(t.GetInvocationList(), () => );

            //t?.Invoke();

            time.Stop();

            Console.WriteLine($"{time.ElapsedTicks} {time.Elapsed} FPS:{1000 / time.ElapsedMilliseconds}");
            */

            Thread.Sleep(10000);

            if (Console.KeyAvailable)
            {
                ConsoleKeyInfo k = Console.ReadKey();
                Console.Write("");
            }

            /*while (true)
            {
                char x = Console.ReadKey(true).KeyChar;
                char y = Console.ReadKey(true).KeyChar;
                continue;
            }*/
                

            Console.ReadLine();
        }
    }
}
