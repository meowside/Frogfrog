using ConsoleTesterProject.Properties;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Media;
using System.Reflection;
using System.Text;

namespace ConsoleTesterProject
{
    public class Parabola
    {
        public TimeSpan Time;
        public float Y;

        public float Accumulated { get; private set; }

        public bool Enabled = false;

        float Elapsed;

        float[] ValuesInTime;
        int NumberOfSamples;
        const float SamplesInSecodnd = 1000;

        public Parabola(int Y, TimeSpan T)
        {
            Time = T; this.Y = Y;

            NumberOfSamples = (int)Time.TotalMilliseconds;
            ValuesInTime = new float[NumberOfSamples];

            double midpoint = NumberOfSamples / 2;
            double start = -midpoint;

            for (int i = (int)start; i < (int)midpoint; i++)
            {
                ValuesInTime[(int)(i + midpoint)] = (-(float)(Math.Pow(1 / midpoint * i, 2)) + Y);
                /*float result = (float)(Math.Sin(i * (Math.PI / SamplesInSecodnd)) * Y);
                ValuesInTime[i] = i > 0 ? (float)(Math.Sin(i * (Math.PI / SamplesInSecodnd)) * Y) - ValuesInTime[i - 1] : 0;*/
            }

            //Elapsed += Engine.DeltaTime;
        }

        protected void Update(float miliseconds)
        {
            Accumulated = 0;
            float MaxTime = 0;
            float LeftoverTime = (float)((Elapsed + miliseconds) - Time.TotalMilliseconds);

            if (LeftoverTime > 0)
            {
                MaxTime = (float)(Time.TotalMilliseconds - LeftoverTime);
            }
            else
            {
                MaxTime = Elapsed + miliseconds;
            }

            for (int i = (int)Elapsed; i <= (int)MaxTime; i++)
            {
                Accumulated += ValuesInTime[i];
            }

            Elapsed += miliseconds;
        }
    }

    static class Program
    {
        static void Main(string[] args)
        {
            Parabola x = new Parabola(10, new TimeSpan(0, 0, 1));

            Console.ReadLine();
        }
    }
}
