#pragma once
namespace GamerEngine
{
	class NavigationPanel
	{
	public:
		void OnImGuiRender();
	private:

		void FilePanel();
		void EditPanel();
		void ComponentsPanel();
		void WindowPanel();
		void CreditsPanel();
		void HelpPanel();

		void CenterText(std::string text);

		bool myCreditsIsOpen = false;
	};
}