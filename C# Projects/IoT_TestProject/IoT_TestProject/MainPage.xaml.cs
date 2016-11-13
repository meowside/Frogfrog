using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using Windows.Devices.Gpio;
using System.Threading.Tasks;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace IoT_TestProject
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();

            bool[] letter = new bool[]
            {
                false, true, false, false, false, false, false, true
            };

            const int D1 = 17;
            const int D2 = 18;
            const int D3 = 27;
            const int D4 = 22;
            const int D5 = 23;
            const int D6 = 10;
            const int D7 = 9;
            const int D8 = 25;

            GpioController GC = GpioController.GetDefault();

            GpioPin t = GC.OpenPin(26);
            t.SetDriveMode(GpioPinDriveMode.Input);
            t.Write(GpioPinValue.High);

            Parallel.Invoke(

                //
                // Data 1
                //

                () =>
                {
                    GpioPin output = GC.OpenPin(D1);

                    output.SetDriveMode(GpioPinDriveMode.Output);
                    output.Write(letter[0] ? GpioPinValue.High : GpioPinValue.Low);
                },

                //
                // Data 2
                //

                () =>
                {
                    GpioPin output = GC.OpenPin(D2);

                    output.SetDriveMode(GpioPinDriveMode.Output);
                    output.Write(letter[0] ? GpioPinValue.High : GpioPinValue.Low);
                },

                //
                // Data 3
                //

                () =>
                {
                    GpioPin output = GC.OpenPin(D3);

                    output.SetDriveMode(GpioPinDriveMode.Output);
                    output.Write(letter[0] ? GpioPinValue.High : GpioPinValue.Low);
                },

                //
                // Data 4
                //

                () =>
                {
                    GpioPin output = GC.OpenPin(D4);

                    output.SetDriveMode(GpioPinDriveMode.Output);
                    output.Write(letter[0] ? GpioPinValue.High : GpioPinValue.Low);
                },

                //
                // Data 5
                //

                () =>
                {
                    GpioPin output = GC.OpenPin(D5);

                    output.SetDriveMode(GpioPinDriveMode.Output);
                    output.Write(letter[0] ? GpioPinValue.High : GpioPinValue.Low);
                },

                //
                // Data 6
                //

                () =>
                {
                    GpioPin output = GC.OpenPin(D6);

                    output.SetDriveMode(GpioPinDriveMode.Output);
                    output.Write(letter[0] ? GpioPinValue.High : GpioPinValue.Low);
                },

                //
                // Data 7
                //

                () =>
                {
                    GpioPin output = GC.OpenPin(D7);

                    output.SetDriveMode(GpioPinDriveMode.Output);
                    output.Write(letter[0] ? GpioPinValue.High : GpioPinValue.Low);
                },

                //
                // Data 8
                //

                () =>
                {
                    GpioPin output = GC.OpenPin(D8);

                    output.SetDriveMode(GpioPinDriveMode.Output);
                    output.Write(letter[0] ? GpioPinValue.High : GpioPinValue.Low);
                }

                );
        }
    }
}
