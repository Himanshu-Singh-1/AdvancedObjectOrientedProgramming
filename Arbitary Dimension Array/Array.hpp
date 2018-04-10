#include<iostream>
#include<exception>

/*Reference: https://stackoverflow.com/questions/30155608/arbitrary-dimensional-array-using-variadic-templates
Reference: https://codereview.stackexchange.com/questions/94148/generic-multidimensional-array-in-c
Last dimension major iterator taken from internet github code:
Link: https://github.com/maddouri/hyper_array/blob/master/include/hyper_array/hyper_array.hpp
*/
using namespace std;
namespace cs540
{
	struct OutOfRange: std::exception
	{
		OutOfRange() noexcept {}
		~OutOfRange() {}
		const char* what() const noexcept
		{
			return ("Out of Range\n");
		}
	};
	//template<typename T,size_t... Dims>
	//class Array
	template<typename T,size_t... Dims> class Array;
	
	template<typename T, size_t N1,size_t... Dims>
	class Array<T,N1,Dims ...>
	{
	static_assert(N1>0,"size is 0");
	Array<T,Dims...> data[N1];
		
		public:
		
		template<typename U,size_t ... var>
		friend class Array;
		
		/*Defalut ctor*/
		Array(){}
		/*copy constructor*/
		Array(const Array &tmp){
			for(int i=0;i<N1;i++)
			data[i] = tmp.data[i];
		}
		using  ValueType = T;
		/*copy ctor*/
		template<typename U>
		Array(const Array<U,N1,Dims...> &tmp){
			for(int i=0;i<N1;i++)
			data[i] = tmp.data[i];
		}
		
		/*assignment operator*/
		Array &operator=(const Array &tmp)
		{
			if(this != &tmp)
			{
				for(int i=0;i<N1;i++)
				data[i] = tmp[i];
			}
				return *this;
		}
		
		template<typename U>
		Array &operator=(const Array<U,N1,Dims ...> &tmp)
		{
			//if(this != &tmp)	
			{
			for(int i=0;i<N1;i++)
			data[i] = tmp[i];
			}
			return *this;
		}
		
		Array<T,Dims...>&operator[](size_t position)
		{
			if(position>=N1)
			throw OutOfRange();
			return data[position];
		}
		
		const Array<T,Dims...>&operator[](size_t position) const
		{
			if(position>=N1)
			throw OutOfRange();
			return data[position];
		}
		
		/*Taken from internet github code link :https://github.com/maddouri/hyper_array/blob/master/include/hyper_array/hyper_array.hpp*/
		class LastDimensionMajorIterator
		{
			public:
			Array<T, N1, Dims...> *arrayPtr;
			typename Array<T,Dims ...>::LastDimensionMajorIterator array_nxt_dimension;
			size_t position;
			//bool end;
			
			/*the position index is bieng incremented as it is. The position indexes to this particular index of next dimension array 
			this goes on until the next */
			void get_next_dimension()
			{
				array_nxt_dimension.arrayPtr = &(arrayPtr->data[position]);
				array_nxt_dimension.get_next_dimension();
				//end = array_nxt_dimension.end;
			}
			LastDimensionMajorIterator& operator++()
			{
				position++;
				if(position ==N1){
					position =0;
					//end =true;
					++array_nxt_dimension;
				}
				get_next_dimension();
				return *this;
			}
			LastDimensionMajorIterator operator++(int)
			{
				LastDimensionMajorIterator tmp =*this;
				++(*this);
				return tmp;
			}
			
			friend bool operator!=(const LastDimensionMajorIterator &tmp1,const LastDimensionMajorIterator &tmp2)
			{
				//return (tmp1.arrayPtr != tmp2.arrayPtr || tmp1.position != tmp2.position || tmp1.array_nxt_dimension != tmp2.array_nxt_dimension);
			return (!(tmp1==tmp2));
			}
			
			friend bool operator==(const LastDimensionMajorIterator &tmp1,const LastDimensionMajorIterator &tmp2)
			{
				return (tmp1.arrayPtr == tmp2.arrayPtr && tmp1.position == tmp2.position && tmp1.array_nxt_dimension == tmp2.array_nxt_dimension);
			}	
			
			T &operator*() const{
				return *array_nxt_dimension;
			}
		};
		
		LastDimensionMajorIterator lmbegin()
		{
			LastDimensionMajorIterator tmp;
			tmp.position =0;
			tmp.arrayPtr = this;
			tmp.array_nxt_dimension = data[0].lmbegin();
			return tmp;
		}
		
		LastDimensionMajorIterator lmend()
		{
			LastDimensionMajorIterator tmp;
			tmp.position =0;
			tmp.arrayPtr =this;
			tmp.array_nxt_dimension = data[0].lmend();
			return tmp;
		}
		
		class FirstDimensionMajorIterator
		{
			public:
			Array<T,N1,Dims ...> *arrayPtr;
			typename Array<T,Dims...>::FirstDimensionMajorIterator array_nxt_dimension;
			size_t position;
			size_t end;
			FirstDimensionMajorIterator &operator++()
			{
				++array_nxt_dimension;
				
				if(array_nxt_dimension.end == array_nxt_dimension.position)
				{
					position++;
					if(position != end)
					{					
						array_nxt_dimension = arrayPtr->data[position].fmbegin();
					}
				}
				return *this;				
			}
			
			FirstDimensionMajorIterator operator++(int)
			{
				FirstDimensionMajorIterator tmp = *this;
				++(*this);
				return tmp;
			}
			/*return next * operated value of next dimension array
			which returns actual data type when reaches last dimension array of T type */
			T &operator*() const
			{
				return *array_nxt_dimension;				
			}
			
			friend bool operator == (const FirstDimensionMajorIterator &tmp1, const FirstDimensionMajorIterator tmp2)
			{
			return (tmp1.arrayPtr == tmp2.arrayPtr && tmp1.position == tmp2.position && tmp1.array_nxt_dimension==tmp2.array_nxt_dimension);
			}
			friend bool operator!=(const FirstDimensionMajorIterator &tmp1, const FirstDimensionMajorIterator &tmp2)
			{
			//return (tmp1.arrayPtr!=tmp2.arrayPtr || tmp1.position!=tmp2.position);		
			return(!(tmp1==tmp2));
			}
			
		};
		
		FirstDimensionMajorIterator fmbegin()
		{
			FirstDimensionMajorIterator tmp;
			tmp.end = N1;
			tmp.arrayPtr = this;
			/*next dimension iterator stored if traversal exceeds N1
			it points to the iterator of next dimension array
			*/
			tmp.array_nxt_dimension = data[0].fmbegin();
			tmp.position = 0;
			return tmp;
		}
		
		FirstDimensionMajorIterator fmend()
		{
			FirstDimensionMajorIterator tmp;
			tmp.end = N1;
			tmp.arrayPtr = this;
			tmp.array_nxt_dimension = data[N1-1].fmend();
			tmp.position = N1;
			return tmp;
		}
		
	};

template<typename T,size_t N>
class Array<T,N>
{
	static_assert(N>0,"size is 0");
	T data[N];
	
	public:
	using  ValueType = T;
	template<typename U, size_t ... var>
	friend class Array;
	Array(){}
	//~Array(){}
	
	Array(const Array &tmp){
		for(size_t i=0;i<N;i++)
			data[i] = tmp[i];
	}
	template<typename U>
	Array(const Array<U,N> &tmp){
			for(size_t i=0;i<N;i++)
			data[i] = tmp[i];
	}
	
	Array &operator=(const Array&tmp)
	{
		for(int i=0;i<N;i++)
			data[i] = tmp.data[i];
		return *this;
	}
	
	template<typename U>
	Array &operator=(const Array<U,N> &tmp)
	{
		for(int i=0;i<N;i++)
		data[i] = tmp.data[i];
	return *this;
	}
	
	T&operator[](std::size_t position)
	{
		if(position>=N)
			throw cs540::OutOfRange();
		
		return data[position];
	}
	
	const T&operator[](std::size_t position) const
	{
		if(position>=N)
			throw cs540::OutOfRange();
		
		return data[position];
	}
	
	
	
	class LastDimensionMajorIterator
		{
			public:
			Array<T,N> *arrayPtr;
			T* array_nxt_dimension;
			size_t position;
			bool end;
			
			void get_next_dimension()
			{
			array_nxt_dimension = &(arrayPtr->data[position]);
			}
			LastDimensionMajorIterator& operator++()
			{
				position++;
				if(position == N)
					end=true;
				get_next_dimension();
				return *this;
			}
			
			LastDimensionMajorIterator operator++(int)
			{
				LastDimensionMajorIterator tmp = *this;
				++(*this);
				return tmp;
			}
			T&operator*() const
			{
				return *array_nxt_dimension;
			}
			friend bool operator==(const LastDimensionMajorIterator &tmp1, const LastDimensionMajorIterator &tmp2)
			{
				return (tmp1.arrayPtr == tmp2.arrayPtr && tmp1.position == tmp2.position);
			}
			friend bool operator!=(const LastDimensionMajorIterator &tmp1, const LastDimensionMajorIterator &tmp2)
			{
				//return (tmp1.arrayPtr != tmp2.arrayPtr || tmp1.position != tmp2.position);
				return(!(tmp1==tmp2));

				}
			
		};
		LastDimensionMajorIterator lmbegin()
		{
			LastDimensionMajorIterator tmp;
			tmp.arrayPtr = this;
			tmp.end =0;
			tmp.position =0;
			tmp.array_nxt_dimension =data;
			return tmp;
		}
		LastDimensionMajorIterator lmend()
		{
			LastDimensionMajorIterator tmp;
			tmp.arrayPtr = this;
			tmp.end =1;
			tmp.position = N;
			tmp.array_nxt_dimension = data;
			return tmp;
		}
	
	
	class FirstDimensionMajorIterator
	{
		public:
		Array<T,N> *arrayPtr;
		T *array_nxt_dimension;
		size_t position;
		size_t end;
			FirstDimensionMajorIterator &operator++()
			{
				
				position++;
				return *this;				
			}
			
			FirstDimensionMajorIterator operator++(int)
			{
				FirstDimensionMajorIterator tmp = *this;
				++(*this);
				return tmp;
			}
			
			T &operator*() const
			{
				return array_nxt_dimension[position];				
			}
			friend bool operator == (const FirstDimensionMajorIterator &tmp1, const FirstDimensionMajorIterator tmp2)
			{
			return (tmp1.arrayPtr == tmp2.arrayPtr && tmp1.position == tmp2.position);
			}
			friend bool operator!=(const FirstDimensionMajorIterator &tmp1, const FirstDimensionMajorIterator &tmp2)
			{
			//return (tmp1.arrayPtr!=tmp2.arrayPtr || tmp1.position!=tmp2.position);		
			return (!(tmp1==tmp2));
			}

	};
	
		FirstDimensionMajorIterator fmbegin()
		{
			FirstDimensionMajorIterator tmp;
			tmp.arrayPtr = this;
			tmp.position = 0;
			tmp.end = N;
			tmp.array_nxt_dimension = data;
			return tmp;
		}
		
		FirstDimensionMajorIterator fmend()
		{
			FirstDimensionMajorIterator tmp;
			tmp.arrayPtr = this;
			tmp.position = N;
			tmp.end = N;
			tmp.array_nxt_dimension = data;
			return tmp;
		}
		
		
};

}