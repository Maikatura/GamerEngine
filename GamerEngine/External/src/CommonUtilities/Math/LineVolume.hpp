#pragma once
#include <vector>
#include "Line.hpp"

namespace CommonUtilities
{
	template<typename T>
	class LineVolume
	{
	public:
		LineVolume();
		// Constructor taking a list of Line that makes up the LineVolume.
		LineVolume(const std::vector<Line<T>>& aLineList);
		// Add a Line to the LineVolume.
		void AddLine(const Line<T>& aLine);
		// Returns whether a point is inside the LineVolume: it is inside when the point is
		// on the line or on the side the normal is pointing away from for all the lines in
		// the LineVolume.
		bool IsInside(const Vector2<T>& aPosition);


	private:
		std::vector<Line<T>> myLineList;
	};

	template<typename T>
	inline LineVolume<T>::LineVolume()
	{
	}

	template<typename T>
	inline LineVolume<T>::LineVolume(const std::vector<Line<T>>& aLineList)
	{
		myLineList = aLineList;
	}

	template<typename T>
	inline void LineVolume<T>::AddLine(const Line<T>& aLine)
	{
		myLineList.push_back(aLine);
	}

	template<typename T>
	inline bool LineVolume<T>::IsInside(const Vector2<T>& aPosition)
	{
		for (int i = 0; i < myLineList.size(); i++)
		{
			if (!myLineList[i].IsInside(aPosition))
			{
				return false;
			}
		}

		return true;
	}
}