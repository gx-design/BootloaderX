using Avalonia;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;
using GX;
using GxFlash;

namespace FlashTool
{
    public class MainWindow : Window
    {
        public MainWindow()
        {
            this.InitializeComponent();
            this.AttachDevTools();

            DataContext = new MainWindowViewModel();
        }

        private void InitializeComponent()
        {
            AvaloniaXamlLoader.Load(this);
        }
    }
}
