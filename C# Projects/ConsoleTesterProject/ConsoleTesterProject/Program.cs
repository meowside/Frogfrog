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
    class Foo
    { }

    class Bar : Foo
    { }

    static class Program
    {
        static void Main(string[] args)
        {
            Dictionary<int, Foo> dictionar = new Dictionary<int, Foo>();

            dictionar.Add(0, (Foo)new Bar());

            Console.WriteLine(dictionar[0].GetType() == typeof(Bar));

            Console.ReadLine();
        }
    }
}
