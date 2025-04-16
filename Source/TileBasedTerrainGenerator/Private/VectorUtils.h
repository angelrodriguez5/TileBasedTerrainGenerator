#pragma once
#include <vector>
#include <algorithm>

template <typename T>
const bool IsInVector(const std::vector<T>& vector, const T& element);

template <typename T>
const bool AreSameVector(const std::vector<T>& v1, const std::vector<T> v2);

template <typename T>
const void RemoveElement(std::vector<T>& vector, const T& element);

template<typename T>
inline const bool IsInVector(const std::vector<T>& vector, const T& element)
{
	for (T vi: vector)
	{
		if (vi == element)
			return true;
	}
	return false;
}

template<typename T>
inline const bool AreSameVector(const std::vector<T>& v1, const std::vector<T> v2)
{
	if (v1.size() != v2.size())
		return false;

	for (T elem_v1 : v1)
	{
		if (!IsInVector(v2, elem_v1))
			return false;
	}
	return true;
}

template<typename T>
inline const void RemoveElement(std::vector<T>& vector, const T& element)
{
	vector.erase(std::remove(vector.begin(), vector.end(), element), vector.end());
}

