using ConsoleTesterProject.Properties;
using System;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Media;
using System.Text;

namespace ConsoleTesterProject
{

    static class Program
    {
        static void Main(string[] args)
        {
            /*const int MAX = 1000000000;
            const float A = 1f / 100;
            Stopwatch t = Stopwatch.StartNew();

            for (int i = 0; i < MAX; i++)
            {
                float j = 2 * A;
            }

            long t1 = t.ElapsedTicks;
            t = Stopwatch.StartNew();

            for (int i = 0; i < MAX; i++)
            {
                float j = 2f / 100;
            }

            long t2 = t.ElapsedTicks;*/
            /*t = Stopwatch.StartNew();

            for (int i = 0; i < MAX; i++)
            {
                tmp = string.Format($"ahoj{i}sup{i}GG");
            }
            long t3 = t.ElapsedTicks;*/
            /*t.Stop();

            Console.WriteLine($"Multi: {t1}\nDivid: {t2}");*/

            /*if (!Directory.Exists(@".\Test"))
                Directory.CreateDirectory(@".\Test");

            if (Directory.Exists(@".\Test"))
            {
                if (!File.Exists(@".\Test\Test.txt"))
                    File.Create(@".\Test\Test.txt");

                if (!File.Exists(@".\Test\Test"))
                    using (FileStream f = new FileStream(@".\Test\Test", FileMode.Append))
                    {
                        byte[] Data = Encoding.ASCII.GetBytes("wat víc");
                        f.Write(Data, 0, Data.Length);
                        f.Close();
                    }
            }*/

            UnmanagedMemoryStream t = Resources.test;

            while (true)
            {
                Console.ReadKey();
                new SoundPlayer(t).Play();
            }

            Console.ReadLine();
        }
    }
}
