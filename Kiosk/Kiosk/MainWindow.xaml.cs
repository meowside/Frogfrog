using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Runtime.InteropServices;

namespace Kiosk
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// 
    
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        [DllImport("user32.dll")]
        private static extern void SetForegroundWindow(IntPtr ipHwnd);

        private void Browser_open(object sender, RoutedEventArgs e)
        {
            var path = new FileInfo(@"chrome.lnk");

            bool wat = false;
            Process[] procs = Process.GetProcesses();
            foreach (Process proc in procs)
            {

                if (proc.ProcessName == "chrome")
                {
                    wat = true;

                    if (proc.MainWindowTitle != "")
                    {
                        //IntPtr ipHwnd = proc.MainWindowHandle;
                        SetForegroundWindow(proc.MainWindowHandle);

                        return;
                    }
                }
            }

            if(!wat) Process.Start(path.FullName);
        }

        

        private void Weather_open(object sender, RoutedEventArgs e)
        {
            var path = new FileInfo(@"weather.lnk");
            Process[] procs = Process.GetProcesses();
            foreach (Process proc in procs)
            {

                if (proc.MainWindowTitle == "Počasí")
                {
                    SetForegroundWindow(proc.MainWindowHandle);

                    return;
                }
            }

            Process.Start(path.FullName);
        }

        private void Calc_open(object sender, RoutedEventArgs e)
        {
            var path = new FileInfo(@"calc.lnk");
            Process[] procs = Process.GetProcesses();
            foreach (Process proc in procs)
            {

                if (proc.MainWindowTitle == "Kalkulačka")
                {
                    SetForegroundWindow(proc.MainWindowHandle);

                    return;
                }
            }

            Process.Start(path.FullName);
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            Process.Start(@"GUI.bat");
        }
    }
}
