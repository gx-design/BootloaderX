using Avalonia.Markup.Xaml;
using Avalonia.Styling;

namespace UiUtils.Controls
{
	public class MetroWindowTheme : Styles
	{
		public MetroWindowTheme()
		{
			AvaloniaXamlLoader.Load(this);
		}
	}
}