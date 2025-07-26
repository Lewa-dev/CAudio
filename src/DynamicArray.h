#pragma once
#include <memory>
#include <assert.h>
#include "DynamicArrayBase.h"
namespace Engine {
	template <typename T>
	class DynamicArray : public DynamicArrayBase<T> {

	public:
		DynamicArray<T>();
		DynamicArray<T>(unsigned int size);

		//allows setting
		T& operator [] (size_t i);

		//only for reading
		T operator [] (size_t i) const;

		size_t size() const;
		void rezise(size_t newSize);
	};



	template<typename T>
	inline DynamicArray<T>::DynamicArray() : DynamicArrayBase<T>()
	{
	}

	template<typename T>
	inline DynamicArray<T>::DynamicArray(unsigned int size) : DynamicArrayBase<T>(size)
	{
	}
	template<typename T>
	inline T & DynamicArray<T>::operator[](size_t i)
	{
		return DynamicArrayBase<T>::operator[](i);
	}
	template<typename T>
	inline T DynamicArray<T>::operator[](size_t i) const
	{
		return DynamicArrayBase<T>::operator[](i);
	}
	template<typename T>
	inline size_t DynamicArray<T>::size() const
	{
		return DynamicArrayBase<T>::size();
	}
	template<typename T>
	inline void DynamicArray<T>::rezise(size_t newSize)
	{
		DynamicArrayBase<T>::rezise(newSize);
	}
}