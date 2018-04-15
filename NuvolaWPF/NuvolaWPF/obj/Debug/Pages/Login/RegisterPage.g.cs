﻿#pragma checksum "..\..\..\..\Pages\Login\RegisterPage.xaml" "{ff1816ec-aa5e-4d10-87f7-6f4963833460}" "A5ED2516160144A38D939B3056A9427BAA9173E5"
//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:4.0.30319.42000
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

using FirstFloor.ModernUI.Presentation;
using FirstFloor.ModernUI.Windows;
using FirstFloor.ModernUI.Windows.Controls;
using FirstFloor.ModernUI.Windows.Converters;
using FirstFloor.ModernUI.Windows.Navigation;
using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Automation;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Markup;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Effects;
using System.Windows.Media.Imaging;
using System.Windows.Media.Media3D;
using System.Windows.Media.TextFormatting;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Shell;


namespace NuvolaWPF.Pages.Login {
    
    
    /// <summary>
    /// RegisterPage
    /// </summary>
    public partial class RegisterPage : FirstFloor.ModernUI.Windows.Controls.ModernWindow, System.Windows.Markup.IComponentConnector {
        
        
        #line 17 "..\..\..\..\Pages\Login\RegisterPage.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.TextBox usernameBox;
        
        #line default
        #line hidden
        
        
        #line 19 "..\..\..\..\Pages\Login\RegisterPage.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.PasswordBox passwordBox;
        
        #line default
        #line hidden
        
        
        #line 21 "..\..\..\..\Pages\Login\RegisterPage.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.TextBox emailBox;
        
        #line default
        #line hidden
        
        
        #line 23 "..\..\..\..\Pages\Login\RegisterPage.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.TextBox sizeBox;
        
        #line default
        #line hidden
        
        
        #line 24 "..\..\..\..\Pages\Login\RegisterPage.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button registerBtn;
        
        #line default
        #line hidden
        
        
        #line 25 "..\..\..\..\Pages\Login\RegisterPage.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button exitToLoginBtn;
        
        #line default
        #line hidden
        
        private bool _contentLoaded;
        
        /// <summary>
        /// InitializeComponent
        /// </summary>
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "4.0.0.0")]
        public void InitializeComponent() {
            if (_contentLoaded) {
                return;
            }
            _contentLoaded = true;
            System.Uri resourceLocater = new System.Uri("/NuvolaWPF;component/pages/login/registerpage.xaml", System.UriKind.Relative);
            
            #line 1 "..\..\..\..\Pages\Login\RegisterPage.xaml"
            System.Windows.Application.LoadComponent(this, resourceLocater);
            
            #line default
            #line hidden
        }
        
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "4.0.0.0")]
        [System.ComponentModel.EditorBrowsableAttribute(System.ComponentModel.EditorBrowsableState.Never)]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Design", "CA1033:InterfaceMethodsShouldBeCallableByChildTypes")]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Maintainability", "CA1502:AvoidExcessiveComplexity")]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1800:DoNotCastUnnecessarily")]
        void System.Windows.Markup.IComponentConnector.Connect(int connectionId, object target) {
            switch (connectionId)
            {
            case 1:
            
            #line 12 "..\..\..\..\Pages\Login\RegisterPage.xaml"
            ((NuvolaWPF.Pages.Login.RegisterPage)(target)).Closed += new System.EventHandler(this.ModernWindow_Closed);
            
            #line default
            #line hidden
            
            #line 12 "..\..\..\..\Pages\Login\RegisterPage.xaml"
            ((NuvolaWPF.Pages.Login.RegisterPage)(target)).Loaded += new System.Windows.RoutedEventHandler(this.ModernWindow_Loaded);
            
            #line default
            #line hidden
            return;
            case 2:
            this.usernameBox = ((System.Windows.Controls.TextBox)(target));
            return;
            case 3:
            this.passwordBox = ((System.Windows.Controls.PasswordBox)(target));
            return;
            case 4:
            this.emailBox = ((System.Windows.Controls.TextBox)(target));
            return;
            case 5:
            this.sizeBox = ((System.Windows.Controls.TextBox)(target));
            return;
            case 6:
            this.registerBtn = ((System.Windows.Controls.Button)(target));
            
            #line 24 "..\..\..\..\Pages\Login\RegisterPage.xaml"
            this.registerBtn.Click += new System.Windows.RoutedEventHandler(this.registerBtn_Click);
            
            #line default
            #line hidden
            return;
            case 7:
            this.exitToLoginBtn = ((System.Windows.Controls.Button)(target));
            
            #line 25 "..\..\..\..\Pages\Login\RegisterPage.xaml"
            this.exitToLoginBtn.Click += new System.Windows.RoutedEventHandler(this.exitToLoginBtn_Click);
            
            #line default
            #line hidden
            return;
            }
            this._contentLoaded = true;
        }
    }
}

