#include <iostream>
#include <memory>
#include <cassert>
using namespace std;

namespace cs540
{
	
	class BadFunctionCall : public std::runtime_error
    {
        public:
            BadFunctionCall(const std::string& message) 
                : std::runtime_error(message) { };
            virtual const char* what() const throw()
            {
                return "Bad Function Call";
            }
    };
	
template <typename T>
class Function;

template <typename ResultType, typename... ArgumentTypes>
bool operator==(const Function<ResultType(ArgumentTypes...)>& , std::nullptr_t);
template <typename ResultType, typename... ArgumentTypes>
bool operator==(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> &);

template <typename ResultType, typename... ArgumentTypes>
bool operator!=(const Function<ResultType(ArgumentTypes ...)> &, std::nullptr_t);
template <typename ResultType, typename... ArgumentTypes>
bool operator!=(std::nullptr_t, const Function<ResultType(ArgumentTypes ...)> &);


template <typename ReturnValue, typename... Args>
class Function<ReturnValue(Args...)> {
public:
	
	explicit operator bool() const
	{
		if(callable_ != nullptr)
			return true;
		else
			return false;
	}
	template <typename T>
	Function& operator=(T t) {
		callable_ = std::make_shared<CallableT<T>>(t);
		return *this;
	}
	
	template <typename T>
	//Taking const T& t not viable for free function types as free function names are not 
	//lvalues however can take const reference to non lvalue is it applicable function type?: yes it is tested 
	//although working fine with lambdas and functors
	// what is the type of temporary create?
	Function(const T t)
	{
		callable_ = std::make_shared<CallableT<T>>(t);
		
	}
	
	Function()
	{
		callable_ = nullptr;
	}

	ReturnValue operator()(Args... args) const {
		if(callable_ != nullptr)
		return callable_->Invoke(args...);
		else 
			throw cs540::BadFunctionCall("Bad function call");
	}
	
	friend bool operator==<ReturnValue,Args ...>(const Function &, std::nullptr_t);
	friend bool operator==<ReturnValue,Args ...>(std::nullptr_t, const Function &);
	friend bool operator!=<ReturnValue,Args ...>(const Function &f, std::nullptr_t);
	friend bool operator!=<ReturnValue,Args ...>(std::nullptr_t, const Function &f);



private:
	class ICallable {
	public:
		virtual ~ICallable() = default;
		virtual ReturnValue Invoke(Args...) = 0;
	};

	template <typename T>
	class CallableT : public ICallable {
	public:
	
	//copy construction of functor t into t_
	//since a temporary is created cannot pass it by 
	// non const reference
		CallableT(const T& t)
			: t_(t) {
		}

		~CallableT() override = default;

		ReturnValue Invoke(Args... args) override {
			return t_(args...);
		}

	private:
	//Functor operator may not be const hence cannot take const reference or a const data member, hence t_ has to be a non const value_comp
	//type. Can it be a non const reference type only in case of free standing functions it cannot? If functions name are not l values still a const reference can be 
	//can be obtained but then erro t_ invalidly declared as function type. 
		 T t_;
	};
	
	std::shared_ptr<ICallable> callable_;
};

template <typename ResultType, typename... ArgumentTypes>
bool operator==(const Function<ResultType(ArgumentTypes ...)> &f, std::nullptr_t)
{
	return !f;
}
template <typename ResultType, typename... ArgumentTypes>
bool operator==(std::nullptr_t, const Function<ResultType(ArgumentTypes ...)> &f)
{
	return !f;
}

template <typename ResultType, typename... ArgumentTypes>
bool operator!=(const Function<ResultType(ArgumentTypes ...)> &f, std::nullptr_t)
{
	return bool(f);
}

template <typename ResultType, typename... ArgumentTypes>
bool operator!=(std::nullptr_t, const Function<ResultType(ArgumentTypes ...)> &f)
{
	return bool(f);
}

}