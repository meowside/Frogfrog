using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hra
{
    class TOTO

    {
        static void Main(string[] args)
        {
            char[] str = new char[10];
            char[] str2 = new char[str.Length];
            int zivoty = 8;
            char klavesa;
            int spatne;
            int uhadnuto = 0;

            do
            {
                Console.Write("Napis slovo o maximalne 10 pismen: ");
                str = Console.ReadLine().ToCharArray();
            } while (str.Length > 10);

            for (int i = 0; i < str.Length; i++)
            {
                str2[i] = '_';
            }
            Console.Clear();
            while (true)
            {
                Console.SetCursorPosition(10, 10);
                for (int i = 0; i < str2.Length; i++)
                {
                    Console.Write("{0} ", str2[i]);
                }

                Console.SetCursorPosition(68, 0);
                Console.Write("{0} zivoty ", zivoty);

                Console.SetCursorPosition(35, 12);
                Console.Write("Hadej pismeno:  \b");
                spatne = 0;
                klavesa = (char)Console.ReadLine()[0];
                for (int i = 0; i < str.Length; i++)
                {
                    if (klavesa == str[i])
                    {
                        if (str[i] != str2[i])
                        {
                            str2[i] = klavesa;
                            uhadnuto++;
                        }

                        spatne = 1;
                    }
                }
                if (spatne == 0)
                {
                    zivoty--;
                }

                if (uhadnuto == str.Length)
                {
                    Console.Clear();
                    Console.Write("Uspesne jsi dokoncil jsi druhe kolo");
                    Console.ReadLine();
                    Console.ReadLine();
                    break;
                }
                if (zivoty == 0)
                {
                    Console.Clear();
                    Console.Write("Konec druheho kola");
                    Console.ReadLine();
                    Console.ReadLine();
                    break;
                }
            }
        }
    }
}
