#pragma once
#include <memory>
#include <assert.h>
namespace Engine {
	template <typename T>
	class DynamicArrayBase {
	protected:
		static const size_t DEFAULTSIZE = 1;

		std::unique_ptr<T[]> buffer;
		size_t buffer_size;

		size_t getMin(size_t a, size_t b);
		size_t size() const;

		void rezise(size_t newSize);

		//------Constructors------------
		DynamicArrayBase<T>();

		DynamicArrayBase<T>(size_t size);

		//allows setting
		T& operator [] (size_t i);

		//only for reading
		T operator [] (size_t i) const;

	public:

		void clear(T val);
		T* get();

		//copy constructor
		DynamicArrayBase<T>(const DynamicArrayBase<T>& other);

		//copy assignment
		DynamicArrayBase& operator=(const DynamicArrayBase<T>& other);

		// move constructor
		DynamicArrayBase<T>(DynamicArrayBase<T>&& other);

		// move assignment
		DynamicArrayBase<T>& operator=(DynamicArrayBase<T>&& other);
	};



	template<typename T>
	inline size_t DynamicArrayBase<T>::getMin(size_t a, size_t b)
	{
		if (a < b) {
			return a;
		}
		else {
			return b;
		}
	}

	template<typename T>
	inline size_t DynamicArrayBase<T>::size() const
	{
		return buffer_size;
	}

	template<typename T>
	inline void DynamicArrayBase<T>::rezise(size_t newSize)
	{
		assert(newSize > 0);
		T* newBuffer = new T[newSize];

		//copy values
		for (size_t i = 0; i < getMin(size(), newSize); i++) {
			newBuffer[i] = buffer[i];
		}
		//swap buffers
		this->buffer = std::unique_ptr<T[]>(newBuffer);

		this->buffer_size = newSize;
	}

	template<typename T>
	inline void DynamicArrayBase<T>::clear(T val)
	{
		for (uint64_t i = 0; i < size(); i++) {
			buffer[i] = val;
		}
	}

	template<typename T>
	inline T & DynamicArrayBase<T>::operator[](size_t i)
	{
		assert(i >= 0);
		assert(i < size());
		return buffer[i];
	}

	template<typename T>
	inline T DynamicArrayBase<T>::operator[](size_t i) const
	{
		return buffer[i];
	}

	template<typename T>
	inline T * DynamicArrayBase<T>::get()
	{
		return buffer.get();
	}

	template<typename T>
	inline DynamicArrayBase<T>::DynamicArrayBase()
	{
		buffer_size = DynamicArrayBase::DEFAULTSIZE;
		buffer = std::make_unique<T[]>(buffer_size);
	}

	template<typename T>
	inline DynamicArrayBase<T>::DynamicArrayBase(size_t size)
	{
		assert(size > 0);
		buffer_size = size;
		buffer = std::make_unique<T[]>(buffer_size);
	}

	template<typename T>
	inline DynamicArrayBase<T>::DynamicArrayBase(const DynamicArrayBase<T> & other) : DynamicArrayBase<T>(other.size())
	{
		for (size_t i = 0; i < other.size(); i++) {
			buffer[i] = other[i];
		}
	}

	template<typename T>
	inline DynamicArrayBase<T> & DynamicArrayBase<T>::operator=(const DynamicArrayBase<T> & other)
	{
		reziseBuffer(other.size());
		for (size_t i = 0; i < other.size(); i++) {
			buffer[i] = other[i];
		}

		return *this;
	}

	template<typename T>
	inline DynamicArrayBase<T>::DynamicArrayBase(DynamicArrayBase<T> && other)
	{
		for (size_t i = 0; i < other.size(); i++) {
			buffer[i] = other[i];
		}

		other.reziseBuffer(DynamicArrayBase::DEFAULTSIZE);

		other.clear();
	}

	template<typename T>
	inline DynamicArrayBase<T> & DynamicArrayBase<T>::operator=(DynamicArrayBase<T> && other)
	{
		if (this != &other) // prevent self-move
		{
			rezise(other.size());
			for (size_t i = 0; i < other.size(); i++) {
				buffer[i] = other[i];
			}

			other.rezise(DynamicArrayBase::DEFAULTSIZE);
			//other.clear();
		}
		return *this;
	}



}