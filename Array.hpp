//A class that allows for construction of arrays of infinite dimensions

#include <cstddef>

namespace cs540
{

class OutOfRange{};
#define FDM FirstDimensionMajorIterator
#define LDM LastDimensionMajorIterator
#define itSize sizeof...(Dims) + 1

template <typename T, std::size_t... Dims> class Array;
template <typename T, std::size_t... Dims> class LDM;

//Helper functions to get Iterator Begin and End pointers
template <typename T, std::size_t Dim>
T* getItBegin(Array<T, Dim> &arr)
{ return &(arr[0]); }

template <typename T, std::size_t Dim, std::size_t... Dims>
T* getItBegin(Array<T, Dim, Dims...> &arr)
{ return getItBegin(arr[0]); }

template <typename T, std::size_t Dim>
T* getItEnd(Array<T, Dim> &arr)
{ return &(arr[Dim - 1]); }

template <typename T, std::size_t Dim, std::size_t... Dims>
T* getItEnd(Array<T, Dim, Dims...> &arr)
{ return getItEnd(arr[Dim - 1]); }


template <typename T, std::size_t Dim, std::size_t... Dims>
struct Array<T, Dim, Dims...>
{
	//Iterators
	struct FDM;
	struct LDM;
	Array<T, Dims...> arr[Dim];
	typedef T ValueType;
	
	Array() { static_assert(Dim > 0, ""); }
	Array(const Array & a)
	{
		for(std::size_t x = 0; x < Dim; x++)
			arr[x] = a.arr[x];
	}
	template <typename U>
	Array(const Array<U, Dim, Dims...> &a)
	{
		for(std::size_t x = 0; x < Dim; x++)
			arr[x] = a.arr[x];
	}
	Array &operator= (const Array &a)
	{
		for(std::size_t x = 0; x < Dim; x++)
			arr[x] = a.arr[x];
		return *this;
	}
	template<typename U>
	Array &operator= (const Array<U, Dim, Dims...> &a)
	{
		for(std::size_t x = 0; x < Dim; x++)
			arr[x] = a.arr[x];
		return *this;
	}
	Array<T, Dims...> &operator[] (const std::size_t &index)
	{
		if(index >= Dim) throw OutOfRange();
		return arr[index];
	}
	const Array<T, Dims...> &operator[] (const std::size_t &index) const
	{
		if(index >= Dim) throw OutOfRange();
		return arr[index];
	}
	FDM fmbegin() { return getItBegin(*this); }
	//Go one over
	FDM fmend() { return getItEnd(*this) + 1; } 
	LDM lmbegin() { return getItBegin(*this); }
	LDM lmend() { return getItEnd(*this) + 1; }
};

template <typename T, std::size_t Dim>
struct Array<T, Dim>
{
	struct FDM;
	struct LDM;
	T arr[Dim];
	typedef T ValueType;
	
	Array()	{ static_assert(Dim > 0, ""); }
	Array(const Array & a)
	{
		for(std::size_t x = 0; x < Dim; x++)
			arr[x] = a.arr[x];
	}
	template <typename U>
	Array(const Array<U, Dim> &a)
	{
		for(std::size_t x = 0; x < Dim; x++)
			arr[x] = a.arr[x];
	}
	Array &operator= (const Array &a)
	{
		for(std::size_t x = 0; x < Dim; x++)
			arr[x] = a.arr[x];
		return *this;
	}
	template<typename U>
	Array &operator= (const Array<U, Dim> &a)
	{
		for(std::size_t x = 0; x < Dim; x++)
			arr[x] = a.arr[x];
		return *this;
	}
	T &operator[] (const std::size_t &index)
	{
		if(index >= Dim) throw OutOfRange();
		return arr[index];
	}
	const T &operator[] (const std::size_t & index) const
	{
		if(index >= Dim) throw OutOfRange();
		return arr[index];
	}
	FDM fmbegin() { return &(arr[0]); }
	FDM fmend()	{ return &(arr[Dim - 1]) + 1; }
	LDM lmbegin() { return &(arr[0]); }
	LDM lmend()	{ return &(arr[Dim - 1]) + 1;}
};

//These are helper functions required for LDM
template <typename Dim>
void getDimensions (void *s, const std::size_t& index, 
						const Dim& d)
{
	std::size_t *sz = (std::size_t *) s;
	sz[index] = d - 1;
}

template <typename Dim, typename... Dims>
void getDimensions (void *s, const std::size_t& index, 
						const Dim& d, const Dims&... dims)
{
	std::size_t *sz = (std::size_t *) s;
	sz[index] = d - 1;
	for(std::size_t x = 0; x < sizeof...(dims); x++)
		getDimensions((void *)sz, index + 1, dims...);
}

//Here we compute the factorial of Dims...
template <typename Dim>
std::size_t getMultiples (void *s, const std::size_t& index,
							const Dim& d)
{
	std::size_t *sz = (std::size_t *) s;
	sz[index] = 1;
	return d;
}

template <typename Dim, typename... Dims>
std::size_t getMultiples (void *s, const std::size_t& index, 
							const Dim& d, const Dims&... dims)
{
	std::size_t *sz = (std::size_t *) s;
	sz[index] = getMultiples(sz, index + 1, dims...);
	return d * sz[index];
}

//Here we implement our iterators
//The general idea is to use ptr++
//Since the arrays are statically allocated on the stack
template <typename T, std::size_t Dim>
struct Array<T, Dim>::FDM
{
	T *ptr;
	FDM(){}
	FDM(T *p) : ptr(p) {}
	FDM(const FDM &f) : ptr(f.ptr) {}
	FDM &operator=(const FDM &f) 
	{ 
		ptr = f.ptr; 
		return *this;
	}
	FDM &operator++()
	{
		ptr++;
		return *this;
	}
	FDM operator++(int)
	{
		FDM rv(*this);
		ptr++;
		return rv;
	}
	T &operator*() const { return *ptr; }
	bool operator==(const FDM &f) {return ptr == f.ptr;}
	bool operator!=(const FDM &f) {return ptr != f.ptr;}
};

template <typename T, std::size_t Dim, std::size_t... Dims>
struct Array<T, Dim, Dims...>::FDM
{
	T *ptr;
	FDM(){}
	FDM(T *p) : ptr(p) {}
	FDM(const FDM &f) : ptr(f.ptr) {}
	FDM &operator=(const FDM &f) 
	{ 
		ptr = f.ptr; 
		return *this;
	}
	FDM &operator++()
	{
		ptr++;
		return *this;
	}
	FDM operator++(int)
	{
		FDM rv(*this);
		ptr++;
		return rv;
	}
	T &operator*() const { return *ptr; }
	bool operator==(const FDM &f) {return ptr == f.ptr;}
	bool operator!=(const FDM &f) {return ptr != f.ptr;}
};

//Since we have only one dimension, LDM = FDM
template <typename T, std::size_t Dim>
struct Array<T, Dim>::LDM
{
	T *ptr;
	LDM(){}
	LDM(T *p) : ptr(p) {}
	LDM(const LDM &f) : ptr(f.ptr) {}
	LDM &operator=(const LDM &f) 
	{ 
		ptr = f.ptr; 
		return *this;
	}
	LDM &operator++()
	{
		ptr++;
		return *this;
	}
	LDM operator++(int)
	{
		LDM rv(*this);
		ptr++;
		return rv;
	}
	T &operator*() const { return *ptr; }
	bool operator==(const LDM &f) {return ptr == f.ptr;}
	bool operator!=(const LDM &f) {return ptr != f.ptr;}
};
		
//The idea here is to calculate the offset by which to jump
//By getting the factorial of Dims...
//And then jumping back and forth by adding and subtracting to ptr
template <typename T, std::size_t Dim, std::size_t... Dims>
struct Array<T, Dim, Dims...>::LDM
{
	T *ptr;
	//Variables to keep track of where we are
	std::size_t end; //Once end reaches endSize, we terminate by returning endPtr
	T *endPtr;
	std::size_t endSize;
	std::size_t index[itSize];
	std::size_t count[itSize];
	std::size_t multiples[itSize];
	LDM()
	{
		end = 0;
		getDimensions((void*)&index, 0, Dim, Dims...);
		endSize = getMultiples((void*)&multiples, 0, Dim, Dims...);
		for(std::size_t x = 0; x < itSize; x++) count[x] = 0;
	}
	LDM(T *p) : ptr(p) 
	{
		end = 0;
		getDimensions((void*)&index, 0, Dim, Dims...);
		endSize = getMultiples((void*)&multiples,  0, Dim, Dims...);
		for(std::size_t x = 0; x < itSize; x++) count[x] = 0;
		endPtr = ptr + endSize; //Will be incorrect for lmend()
	}
	LDM(const LDM &f) : ptr(f.ptr), end(f.end), endPtr(f.endPtr), endSize(f.endSize)
	{
		for(std::size_t x = 0; x < itSize; x++)
		{
			index[x] = f.index[x];
			count[x] = f.count[x];
			multiples[x] = f.multiples[x];
		}
	}
	LDM &operator=(const LDM &f) 
	{ 
		ptr = f.ptr; 
		end = f.end;
		endSize = f.endSize;
		endPtr = ptr + endSize;
		for(std::size_t x = 0; x < itSize; x++)
		{
			index[x] = f.index[x];
			count[x] = f.count[x];
			multiples[x] = f.multiples[x];
		}
		return *this;
	}
	LDM &operator++()
	{
		increment();		
		if(end >= endSize)	ptr = endPtr;
		return *this;
	}
	LDM operator++(int)
	{
		LDM rv(*this);
		increment();
		if(end >= endSize) 
		{
			ptr = endPtr;
			return *this;
		//	rv.ptr = endPtr; //Not Working
		}
		return rv;
	}
	T &operator*() const { return *ptr; }
	bool operator==(const LDM &f) {return ptr == f.ptr;}
	bool operator!=(const LDM &f) {return ptr != f.ptr;}
	
	private:
		void increment()
		{
			for(std::size_t x = 0; x < itSize; x++)
			{
				//next column (in 2D)
				if(count[x] < index[x])
				{
					count[x]++;
					ptr += multiples[x];
					break;
				}
				//next row (in 2D)
				else
				{
					count[x] = 0;
					ptr -= multiples[x] * index[x];
				}
			}
			end++;
		}
};

}
