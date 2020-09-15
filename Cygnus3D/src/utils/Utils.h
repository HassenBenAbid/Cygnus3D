#pragma once

namespace Cygnus3D {

	static const float PI = 2 * std::acos(0.0);

	template <typename T>
	T lerp(T currentValue, T maxValue, T d) {
		return (maxValue * d) - (currentValue * d);
	}

}