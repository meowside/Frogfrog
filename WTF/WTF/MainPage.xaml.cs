﻿using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace WTF
{
    public sealed partial class MainPage : Page
    {

        ObservableCollection<PointOfInterestHolder> Source = new ObservableCollection<PointOfInterestHolder>();

        public MainPage()
        {
            this.InitializeComponent();

            cancer.ItemsSource = Source;
            
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Source.Add(new PointOfInterestHolder() { PoIName = "cancer", ImageSource = new Uri(@"ms-aapx:Assets/test.jpeg"), Latitude = "100", Longtitude = "100", VisitTime = "Bullshit" });
            
        }
    }
}
