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
		UUID2 myServerId;

		Vector3f myNewTranslation;


	public:
		NetworkComponent();
		NetworkComponent(UUID2 aUUID);

		UUID2 GetID();
		void SetID(UUID2 aUUID2);

		bool IsServer();
		void SetServer(bool aIsServer);
		


		void SetNewPosition(Vector3f aNewPosition);

		bool ShouldSmooth();
		void SetShouldSmooth(bool shouldSmooth);
	};
}
