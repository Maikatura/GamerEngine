#pragma once
#include "TurHeader.h"

namespace TurNet
{
	/**
	 * \brief Compare if 2 clients are the same.
	 * \param aClientOne Is the first client to compare.
	 * \param aClientTwo Is the second client to compare.
	 * \return It return true if the two clients are the same. false if they are not.
	 */
	inline bool CompareClients(ClientAddress& aClientOne, ClientAddress& aClientTwo)
	{
		return ((aClientOne.Address == aClientTwo.Address) && (aClientOne.Port == aClientTwo.Port));
	}
}
