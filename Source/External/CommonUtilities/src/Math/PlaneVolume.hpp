#pragma once
#include <vector>
#include "Plane.hpp"


namespace CommonUtilities
{


	template<typename T>
	class PlaneVolume
	{
	public:
		PlaneVolume();
		PlaneVolume(const std::vector<Plane<T>>& aPlaneList);

		void AddPlane(const Plane<T>& aPlane);
		bool IsInside(const Vector3<T>& aPosition);

	private:
		std::vector<Plane<T>> myPlaneList;
	};

	template<typename T>
	inline PlaneVolume<T>::PlaneVolume()
	{
	}

	template<typename T>
	inline PlaneVolume<T>::PlaneVolume(const std::vector<Plane<T>>& aPlaneList)
	{
		myPlaneList = aPlaneList;
	}

	template<typename T>
	inline void PlaneVolume<T>::AddPlane(const Plane<T>& aPlane)
	{
		myPlaneList.push_back(aPlane);
	}

	template<typename T>
	inline bool PlaneVolume<T>::IsInside(const Vector3<T>& aPosition)
	{
		for (int i = 0; i < myPlaneList.size(); i++)
		{
			if (!myPlaneList[i].IsInside(aPosition))
			{
				return false;
			}
		}

		return true;
	}
}
