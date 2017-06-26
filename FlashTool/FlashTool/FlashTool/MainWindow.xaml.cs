using Avalonia;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;
using FlashTool.Controls;
using GreenLightApplicationDevice;
using GX;
using GxFlash;

namespace FlashTool
{
    public class MainWindow : MetroWindow
    {
        public MainWindow()
        {
            this.InitializeComponent();
            this.AttachDevTools();

            var bootloader = new GxBootloader();
            var bagSealer = new BagSealer();

            DataContext = new MainWindowViewModel(bootloader, bagSealer);

            Closed += (sender, e) =>
            {
                bootloader.Disconnect();
                bagSealer.Disconnect();
            };
        }

        private void InitializeComponent()
        {
            AvaloniaXamlLoader.Load(this);
        }
    }
}
