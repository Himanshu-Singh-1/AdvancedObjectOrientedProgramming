#include<iostream>
#include<mutex>

/*Reference Scott Meyers papers on smart pointers
http://www.aristeia.com/Papers/C++ReportColumns/apr96.pdf
http://www.aristeia.com/Papers/C++ReportColumns/novdec96.pdf
http://www.aristeia.com/Papers/C++ReportColumns/jun96.pdf
https://www.aristeia.com/BookErrata/M29Source.html
http://www.bfilipek.com/2016/04/custom-deleters-for-c-smart-pointers.html
*/

namespace cs540 
{
	template<typename T>
	void HelperFunction_delete(void *ptr)
	{
		delete static_cast<T*>(ptr);
	}

struct Reference_manager
{
	std::mutex lockvar;
	void *ptr;
	void (*del)(void *);
	volatile std::size_t refcount;
	
	Reference_manager(void *p, void (*funcPtr)(void *)): ptr(p),del(funcPtr){refcount =1;};
	Reference_manager(const Reference_manager &tmp):ptr(tmp.ptr),del(tmp.del),refcount(tmp.refcount){}
	Reference_manager& operator=(const Reference_manager& tmp)
	{
		del = tmp.del;
		ptr = tmp.ptr;
		refcount = tmp.refcount;
		return *this;
	}
	
	~Reference_manager()
	{
		if(ptr!=nullptr)
		{

			(*del)(ptr);
			ptr = nullptr;
			del = nullptr;
		}
	}
	
	int inc(void)
	{

		return ++refcount;
	}
	
	int dec(void)
	{
		return --refcount;
		
	}
};

template<typename T>
class SharedPtr
{
	template<typename U>
	friend class SharedPtr;
	
	public:
	//default constructor non templated
	explicit SharedPtr():ref(nullptr),pointer(nullptr){}
	
	//default constructor templated will only work if (void *)p is a valid conversion
	template<typename U>
	explicit SharedPtr(U *p):ref(new Reference_manager((void *)p,&HelperFunction_delete<U>)),pointer(p){}
	
	//copy constructor non templated 
	SharedPtr(const SharedPtr &p):pointer(p.pointer)
	{
		
		if(p==nullptr)
		{
			ref = nullptr;
			return;
		}
		if(p.ref!=nullptr)
		{
			p.ref->lockvar.lock();
			ref = p.ref;
			ref->inc();
			p.ref->lockvar.unlock();
		}

	}
	
	//copy constructor templated will only work if conversion (T*)p.pointer is valid
	//:pointer((T *)p.pointer)
	template<typename U>
	SharedPtr(const SharedPtr<U> &p)
	:pointer((T *)p.pointer)
	{
		if(p==nullptr)
		{
			ref = nullptr;
			return;
		}
		
		if(p.ref!=nullptr)
		{
			p.ref->lockvar.lock();
			ref = p.ref;
			ref->inc();
			p.ref->lockvar.unlock();
		}
	}
	
	SharedPtr(SharedPtr &&p)
	{
		pointer = p.pointer;
		ref = p.ref;
		
		p.pointer = nullptr;
		p.ref = nullptr;
	}
	
	template<typename U>
	SharedPtr(SharedPtr<U> &&p)
	{
		pointer = p.pointer;
		ref = p.ref;
		
		p.pointer = nullptr;
		p.ref = nullptr;
	}
	
	SharedPtr &operator=(const SharedPtr &p)
	{
		if(*this ==p)
			return *this;
		
		//this particular shared pointer is being assigned to other hence its 
		//reference count has to be decremented, also if the ref count becomes
		//0 after decrement need to delete the pointer
		if(ref != nullptr)
		{
			ref->lockvar.lock();
			if(ref->dec()==0)
			{
				ref->lockvar.unlock();
				delete (ref);
			}
				else
					ref->lockvar.unlock();
			//the destructor of Reference_manager deletes the pointer with proper helper deleter function
		}
		
		if(p!=nullptr && p.ref !=nullptr)
		{
			pointer = p.pointer;
			//p.ref->lockvar.lock();
			p.ref->lockvar.lock();
			ref = p.ref;
			ref->inc();
			p.ref->lockvar.unlock();
		}
		else
		{
			ref = nullptr;
			pointer = nullptr;
		}
		return *this;
	}
	
	template<typename U>
	SharedPtr<T> & operator=(const SharedPtr<U> &p)
	{
		if(*this==p)
			return *this;
		
		if(ref != nullptr)
		{
			ref->lockvar.lock();
			if(ref->dec()==0)
			{
			ref->lockvar.unlock();
			delete(ref);
			}
			else
				ref->lockvar.unlock();
		}
		
		if(p!=nullptr && p.ref!=nullptr)
		{
			pointer = p.pointer;
			p.ref->lockvar.lock();
			ref = p.ref;
			ref->inc();
			p.ref->lockvar.unlock();
		}
		else
		{
			pointer = nullptr;
			ref = nullptr;
		}
		return *this;
	}
	
	SharedPtr &operator=(SharedPtr &&p)
	{

		if(ref != nullptr)
		{
			ref->lockvar.lock();
			if(ref->dec()==0)
			{
				ref->lockvar.unlock();
			delete(ref);
			}
			else
				ref->lockvar.unlock();
		}
		
		if(p!=nullptr)
		{

			pointer = p.pointer;
			ref = p.ref;
			
			p.pointer = nullptr;
			p.ref = nullptr;	
		}
			
		else
		{
			pointer = nullptr;
			ref = nullptr;
		}
		
		return *this;
	}
	
	template<typename U>
	SharedPtr &operator=(SharedPtr<U> &&p)
	{
		if(ref != nullptr)
		{
			ref->lockvar.lock();
			if(ref->dec()==0)
			{
				ref->lockvar.unlock();
			delete(ref);
			}
			else
				ref->lockvar.unlock();

		}
		
		if(p!=nullptr)
		{
			pointer = p.pointer;
			ref = p.ref;
			
			p.pointer = nullptr;
			p.ref = nullptr;
		}
		
		else
		{
			pointer = nullptr;
			ref = nullptr;
		}
		return *this;
	}
	
	~SharedPtr()
	{
		//static int callno;

		if(this->ref != nullptr)
		{
			ref->lockvar.lock();
			if(ref->dec()==0)
			{
			ref->lockvar.unlock();
			delete(ref);
			}
			else
				ref->lockvar.unlock();
		}
		ref = nullptr;
		pointer = nullptr;
		//callno++;
		//std::cout<<"\nDESTRUCTOR CALLED"<<callno<<std::endl;
	}
	
	void reset()
	{
		if(this->ref != nullptr)
		{
			ref->lockvar.lock();
			if(ref->dec()==0)
			{
			ref->lockvar.unlock();	
			delete(ref);
			}
			else
				ref->lockvar.unlock();
		}
			ref = nullptr;
			pointer = nullptr;
	}
	
	template<typename U>
	void reset(U *p)
	{

		if(p == nullptr)
		{
			reset();
		}
		else
		{
			if(this->ref != nullptr)
		{
			ref->lockvar.lock();
			if(ref->dec()==0)
			{
			ref->lockvar.unlock();
			delete(ref);
			}
			else
				ref->lockvar.unlock();
		}
			ref = new Reference_manager(static_cast<void *>(p), &HelperFunction_delete<U>);
			pointer = p;
		}
	}
	
	T* get() const
	{
		return pointer;
	}
	 
	T &operator*() const
	{
		return *pointer;
	}
	
	T* operator->() const
	{
		return pointer;
	}
	
	explicit operator bool() const
	{
		if(pointer != nullptr)
			return true;
		else
			return false;
	}
	
	template <typename T1, typename T2>
    friend bool operator==(const SharedPtr<T1> &, const SharedPtr<T2> &);
    template <typename TT>
    friend bool operator==(const SharedPtr<TT> &, std::nullptr_t);
    template <typename TT>
    friend bool operator==(std::nullptr_t, const SharedPtr<T> &);
    template <typename T1, typename T2>
    friend bool operator!=(const SharedPtr<T1> &, const SharedPtr<T2> &);
    template <typename TT>
    friend bool operator!=(const SharedPtr<TT> &, std::nullptr_t);
    template <typename TT>
    friend bool operator!=(std::nullptr_t, const SharedPtr<TT> &rhs);
    template <typename TT, typename U>
    friend SharedPtr<TT> static_pointer_cast(const SharedPtr<U> &sp);
    template <typename TT, typename U>
    friend SharedPtr<TT> dynamic_pointer_cast(const SharedPtr<U> &sp);
	
	private:
	T* pointer;
	struct Reference_manager *ref;
};

template <typename T1, typename T2>
    bool operator==(const SharedPtr<T1> &p1, const SharedPtr<T2> &p2) {
        return (p1.ref == p2.ref);
    }
    template <typename TT>
    bool operator==(const SharedPtr<TT> &p1, std::nullptr_t p2) {
        return (p1.ref == p2);
    }
    template <typename TT>
    bool operator==(std::nullptr_t p1, const SharedPtr<TT> &p2) {
        return(p1 == p2.ref);
    }
    template <typename T1, typename T2>
    bool operator!=(const SharedPtr<T1>& p1, const SharedPtr<T2>& p2) {
        return !(p1 == p2);
    }
    template <typename TT>
    bool operator!=(const SharedPtr<TT> &p1, std::nullptr_t p2) {
        return !(p1 == p2);
    }
    template <typename TT>
    bool operator!=(std::nullptr_t p1, const SharedPtr<TT> &p2) {
        return !(p1 == p2);
    }
	
	/*
	Use dynamic_cast when casting from a base class type to a derived class type. It checks that the object being cast is 
	actually of the derived class type and returns a null pointer if the object is not of the desired type (unless you're 
	casting to a reference type -- then it throws a bad_cast exception).

	Use static_cast if this extra check is not necessary. Since dynamic_cast performs the extra check, it requires 
	RTTI information and thus has a greater runtime overhead, whereas static_cast is performed at compile-time.
	*/
	
    template <typename T, typename U>
    SharedPtr<T> static_pointer_cast(const SharedPtr<U> &p){
		SharedPtr<T> tmp(p);
		tmp.pointer = static_cast<T*>(p.pointer);
		return tmp;		
    }
    template <typename T, typename U>
    SharedPtr<T> dynamic_pointer_cast(const SharedPtr<U> &p){
		SharedPtr<T> tmp(p);
		tmp.pointer = dynamic_cast<T*>(p.pointer);
		return tmp;
    }


}
