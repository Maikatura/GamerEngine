#pragma once
#include "Math/MathTypes.hpp"
#include "Scene/UUID.h"


namespace Network
{
	class NetworkComponent
	{
		bool myIsServer;
		bool myShouldSmooth;
		bool myFirstStart = true;
		GamerEngine::UUID myServerId;

		Vector3f myNewTranslation;


	public:
		NetworkComponent();
		NetworkComponent(GamerEngine::UUID aUUID);

		GamerEngine::UUID GetID();
		void SetID(GamerEngine::UUID aUUID2);

		bool IsServer();
		void SetServer(bool aIsServer);
		


		void SetNewPosition(Vector3f aNewPosition);

		bool ShouldSmooth();
		void SetShouldSmooth(bool shouldSmooth);
	};
}
