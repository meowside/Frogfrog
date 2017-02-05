using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleTesterProject
{
    struct ColorEx
    {
        public readonly byte A;
        public readonly byte R;
        public readonly byte G;
        public readonly byte B;

        public ColorEx(byte A, byte R, byte G, byte B)
        {
            this.A = A;
            this.R = R;
            this.B = B;
            this.G = G;
        }
    
        public static ColorEx FromARGB(byte A, byte R, byte G, byte B)
        {
            return new ColorEx(A, R, G, B);
        }
    }
}
