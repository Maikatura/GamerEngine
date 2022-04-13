#pragma once


namespace GamerEngine
{
	class NetworkPanel
	{
	public:
		void OnImGuiRender();
	private:
		bool myIsOpenPublicly = false;

	};
}