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
            Time = T;
            this.Y = Y;

            NumberOfSamples = (int)Time.TotalMilliseconds;
            ValuesInTime = new float[NumberOfSamples];

            float midpoint = NumberOfSamples / 2f;
            float start = -midpoint;
            float lastResult = 0f;

            for (float i = start; i < midpoint; i += 0.1f)
            {
                float index = i + midpoint;
                float result = -(float)(Math.Cos(Math.PI / NumberOfSamples * i) * Y);
                ValuesInTime[(int)index] = index > 0 ? result - lastResult : result;
                lastResult = result;
            }

            //Elapsed += Engine.DeltaTime;
        }

        public void Update(float miliseconds)
        {
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

            for (int i = (int)Elapsed; i < (int)MaxTime; i++)
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
            Parabola x = new Parabola(100, new TimeSpan(0, 0, 1));

            for(int i = 0; i < x.Time.TotalMilliseconds; i++)
            {
                x.Update(1f);
            }

            Console.Write($"{x.Accumulated}");

            Console.ReadLine();
        }
    }
}
