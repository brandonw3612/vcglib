#ifndef _ATOMIC_INT_APPLE_H

#define _ATOMIC_INT_APPLE_H

#include <libkern/OSAtomic.h>


//http://developer.apple.com/library/mac/#documentation/Darwin/Reference/KernelIOKitFramework/OSAtomic_h/index.html

namespace mt{

class atomicInt
{
public:
	atomicInt()
	{
		value = 0;
	}
	atomicInt( int value )
	{
		value = value;
	}

	// atomic API

	/**
	Reads the current value of this QAtomicInt and then adds valueToAdd
	to the current value, returning the original value.

	Unfortunately, MacOSX does not provide with fetch-and-add functions,
	only add-and-fetch. Therefore, we have to simulate them.

	Implementation based on SDL:
	//http://lists.libsdl.org/pipermail/commits-libsdl.org/2011-January/003568.html
	*/
	inline int fetchAndAddAcquire( int valueToAdd )
	{
		 //T *originalValue = currentValue;
		 //currentValue += valueToAdd;
		 //return originalValue;

		int originalValue;
		do { 
			originalValue = value;
		} while (!OSAtomicCompareAndSwap32Barrier(originalValue, originalValue+valueToAdd, &value));
		return originalValue;
	}

	/**
	Atomically increments the value of this atomicInt. 
	Returns true if the new value is non-zero, false otherwise.*/
	inline bool ref()
	{
		return OSAtomicIncrement32Barrier(&value) != 0;
	}

	/*
	Atomically decrements the value of this QAtomicInt. 
	Returns true if the new value is non-zero, false otherwise.*/
	inline bool deref()
	{
		return OSAtomicDecrement32Barrier(&value) != 0;
	}

	inline bool testAndSetOrdered(int expectedValue, int newValue)
	{
		//if (currentValue == expectedValue) {
		//	 currentValue = newValue;
		//	 return true;
		// }
		//return false;

        return OSAtomicCompareAndSwap32Barrier(expectedValue, newValue, &value);
	}

    // Non-atomic API
    inline bool operator==(int value) const
    {
        return value == value;
    }

    inline bool operator!=(int value) const
    {
        return value != value;
    }

    inline bool operator!() const
    {
        return value == 0;
    }

    inline operator int() const
    {
        return value;
    }
	   
	inline atomicInt &operator=(int value)
    {
        value = value;
        return *this;
    }

	inline bool operator>(int value) const
	{
		return value > value;
	}

	inline bool operator<(int value) const
	{
		return value < value;
	}

private:
	volatile int value;

};

}//namespace

#endif
