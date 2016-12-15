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

// The User Control item template is documented at http://go.microsoft.com/fwlink/?LinkId=234236

namespace WTF
{
    public sealed partial class PointOfInterestHolder : UserControl
    {
        public static readonly DependencyProperty ImageHolder = DependencyProperty.Register("ImageSource", typeof(Uri), typeof(PointOfInterestHolder), null);

        public Uri ImageSource
        {
            get { return (Uri)GetValue(ImageHolder); }
            set { SetValue(ImageHolder, value); }
        }

        public static readonly DependencyProperty PoINameProperty = DependencyProperty.Register("PoIName", typeof(string), typeof(PointOfInterestHolder), null);

        public string PoIName
        {
            get { return (string)GetValue(PoINameProperty); }
            set { SetValue(PoINameProperty, value); }
        }

        public static readonly DependencyProperty VisitTimeProperty = DependencyProperty.Register("VisitTime", typeof(string), typeof(PointOfInterestHolder), null);

        public string VisitTime
        {
            get { return (string)GetValue(VisitTimeProperty); }
            set { SetValue(VisitTimeProperty, value); }
        }

        public static readonly DependencyProperty LatitudeProperty = DependencyProperty.Register("Latitude", typeof(string), typeof(PointOfInterestHolder), null);

        public string Latitude
        {
            get { return (string)GetValue(LatitudeProperty); }
            set { SetValue(LatitudeProperty, value); }
        }

        public static readonly DependencyProperty LongtitudeProperty = DependencyProperty.Register("Longtitude", typeof(string), typeof(PointOfInterestHolder), null);

        public string Longtitude
        {
            get { return (string)GetValue(LongtitudeProperty); }
            set { SetValue(LongtitudeProperty, value); }
        }

        public PointOfInterestHolder()
        {
            this.InitializeComponent();
            this.DataContext = this;
        }
    }
}
