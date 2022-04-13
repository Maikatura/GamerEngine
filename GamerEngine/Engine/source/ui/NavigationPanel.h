#pragma once
 namespace GamerEngine
 {
	 class NavigationPanel
	 {
	 public:
		 void OnImGuiRender();
	 private:

		 void CenterText(std::string text);

		 bool myCreditsIsOpen = false;
	 };
 }