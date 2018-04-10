#include <exception>
#include <iostream>
#include <tuple>
#include <string.h>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <iomanip>

namespace cs540
{
	
struct WrongNumberOfArgs: std::exception
	{
		WrongNumberOfArgs() noexcept {}
		~WrongNumberOfArgs() {}
		const char* what() const noexcept
		{
			return ("Wrong number of arguments\n");
		}
	};
	
	//Taken from internet https://github.com/AdityaManjrekar/Templates---Interpolated-ostream-Output/blob/master/Interpolate.hpp
	bool flush = 0;
    std::ostream& (&ffr(std::ostream&(&f)(std::ostream&)))(std::ostream&){
        if (f == static_cast<std::ostream& (*)(std::ostream&)>(std::flush)){
            flush  = 1;
        }
        return f;
    }
/*	
//template<std::size_t index,typename index_str,typename Ts,typename Enable = void>	
not using this one moving index_str 
*/
template<std::size_t index,typename Ts,typename Enable = void> class Interpolate_helper_1;

//
template<std::size_t index,typename Ts>
class Interpolate_helper_1<index,Ts,typename std::enable_if<!(index >= std::tuple_size<Ts>::value)>::type>
{
	public:
	Ts tup;  //store the types from template parameter expansion
	std::size_t instr;
	//std::size_t ind;
	std::string stmt;  //input string to be printed
	//size_t num = index_str;
	Interpolate_helper_1( std::size_t instr_tmp,Ts tmp):instr(instr_tmp),tup(tmp),stmt(std::get<0>(tmp)){
	}//constructor

	friend std::ostream &operator<<(std::ostream &os, const Interpolate_helper_1 &tmp)
	{
		std::string str = tmp.stmt;
		std::size_t count = str.size();
		std::size_t i=tmp.instr;
		for(;i<count;i++)
		{
			if(str[i] == '%')
			{
				/*doesn't utilize the % sign encountered if theses following types are present in the tuple then they dont consume 
the % sign hence they have to be passed to the ostream object as it is.
*/				
				if((typeid(std::get<index>(tmp.tup)).name() == typeid(std::defaultfloat).name())
					||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::oct).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::setiosflags(std::ios_base::hex)).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::hex).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::noskipws).name())				
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::unitbuf).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::resetiosflags(std::ios_base::hex)).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::left).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::scientific).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::skipws).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::setfill(char())).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::noboolalpha).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::showbase).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::hexfloat).name()) 
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::dec).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::setprecision(int())).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::nounitbuf).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::nouppercase).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::uppercase).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::boolalpha).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::showpos).name()) 
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::noshowpos).name()) 
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::right).name()) 
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::internal).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::fixed).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::setbase(int())).name())
				||	(typeid(std::get<index>(tmp.tup)).name() == typeid(std::noshowbase).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::showpoint).name()) 
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::noshowpoint).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::setw(int())).name())) 
				{
					//pass these special non % consuming types as it is.
					os << std::get<index>(tmp.tup);
					/*printing the next tuple and which might have consume the % sign encountered also the % sign hasn't 
				been dealt with hence only increase the tuple index param and keep string index same until the tuple 
				consuming the % sign is encountred*/
					os<<(cs540::Interpolate_helper_1<index+1,Ts>(i,tmp.tup));
				}
				
				//fflush handling part copied from internet 
				else if(typeid(std::get<index>(tmp.tup)).name() == typeid(ffr(std::flush)).name())
				{
					os<<std::get<index>(tmp.tup);
					if(flush)
					{
						flush=0;
						os<<Interpolate_helper_1<index+1,Ts>(i,tmp.tup);
					}
					else if(i<str.size()){ //consumes the %sign
					//os << std::get<index>(tmp.tup) ;
					os<<(cs540::Interpolate_helper_1<index+1,Ts>(i+1,tmp.tup));
					}
					else
					{
						throw WrongNumberOfArgs();
					}
				}
				
				else { 
				/*any other tuple not from the list above in if condition consume the % sign hence can increase the string index (as the % is now consumed dealt with)
				also have to advance to printing the rest of string hence call to another specific templated class that would be generated 
				at compile time also advance the tuple index position as the current tuple which consumes the % has been sent to ostream*/
					os << std::get<index>(tmp.tup) ;
					os<<(cs540::Interpolate_helper_1<index+1,Ts>(i+1,tmp.tup));
				}				
				break;
			}
			//handling the printing of % using // as escape character
			else if(str[i] =='\\' && i<(count-1) && str[i+1]=='%')
			{
				os<<str[i+1];
				i++;
			}
			//print the character of string as it is 
			else
			{
				os<<str[i];
			}
		}
		
		//os<<Interpolate_helper_1<index+1,index_str,Ts>(tmp.tup); //naive call to print other tuple elements doesnt work why?
		
		/*handling the error cases generated from line 152 of test case file. Error in case there are more params than % in string
		then do simmilar checks as above i.e the string has terminated so there are no more % signs but the 
		enable if condition in template parameter for this specialization of class dectates that the tuple index is less than the 
		tuple size hence some params are left and they will the ones that do not consume the % sign*/
		if(i>=str.size()){ 
			if((typeid(std::get<index>(tmp.tup)).name() == typeid(std::defaultfloat).name())
					||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::oct).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::setiosflags(std::ios_base::hex)).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::hex).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::noskipws).name())				
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::unitbuf).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::resetiosflags(std::ios_base::hex)).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::left).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::scientific).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::skipws).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::setfill(char())).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::noboolalpha).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::showbase).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::hexfloat).name()) 
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::dec).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::setprecision(int())).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::nounitbuf).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::nouppercase).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::uppercase).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::boolalpha).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::showpos).name()) 
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::noshowpos).name()) 
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::right).name()) 
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::internal).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::fixed).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::setbase(int())).name())
				||	(typeid(std::get<index>(tmp.tup)).name() == typeid(std::noshowbase).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::showpoint).name()) 
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::noshowpoint).name())
				||(typeid(std::get<index>(tmp.tup)).name() == typeid(std::setw(int())).name()))
				{
					os << std::get<index>(tmp.tup);
					/*sending i or i+1 doesn't matter it has already exceeded the string length not printing the strings anymore
					only dealing with trainling params*/
					os<<(cs540::Interpolate_helper_1<index+1,Ts>(i+1,tmp.tup));
				}
				//handling ffr same as above taken from internet
				else if(typeid(std::get<index>(tmp.tup)).name() == typeid(ffr(std::flush)).name())
				{
					os<<std::get<index>(tmp.tup);
					if(flush)
					{
						flush=0;
						os<<Interpolate_helper_1<index+1,Ts>(i,tmp.tup);
					}
					else if(i<str.size()){ //consumes the %sign
					//os << std::get<index>(tmp.tup);
					os<<(cs540::Interpolate_helper_1<index+1,Ts>(i+1,tmp.tup));
					}
					else
					{
						throw WrongNumberOfArgs();
					}
				}
				
			else
			throw cs540::WrongNumberOfArgs();
		}
	
				
		return os;
	}
};

/*template specialization the terminating case when all the tuple values have been processed hence tuple index exceeds
the tuple size but string is not completely processed the % signs and other params are processed into ostream*/
template<std::size_t index,typename Ts>
class Interpolate_helper_1<index,Ts,typename std::enable_if<index >= std::tuple_size<Ts>::value>::type>
{
	public:
	std::size_t instr;
	Ts tup;  //store the types from template parameter expansion
	//size_t num = index_str;
	std::string stmt;  //input string to be printed
	
	//constructors have to take
	Interpolate_helper_1(std::size_t instr,Ts tmp):instr(instr),tup(tmp),stmt(std::get<0>(tmp)){
	} 	
	
	friend std::ostream &operator<<(std::ostream &os, const Interpolate_helper_1 &tmp)
	{
		std::string str = tmp.stmt;
		std::size_t count = str.size();
		std::size_t i=tmp.instr;
		/* using without recursion simple print of first string
		while(i!=count)
		{
			char c = str[i];
			if(c == '%' && i!=0 && !(str[i-1]=='\\'||str[i-1]=='%'))
			{
				
				os<<std::get<index>(tmp.tup);
				//os<<Interpolate_helper_1<index+1,index_str+1,Ts>(tmp.tup);
				break;
			}
			else
			os<<c;
			i++;
		}
		*/
		
		for(;i<count;i++)
		{
			if(str[i] == '%')
			{
				throw cs540::WrongNumberOfArgs();
				break;
			}
			else if(str[i] =='\\' && i<(count-1) && str[i+1]=='%')
			{
				os<<str[i+1];
				i++;
			} 
			else
				os<<str[i];
		}
		return os;
	}	
	
};

/*taking all the input arguments within parenthesis are converted to tuples
first argument must be a string cannot have data types directly without % sign in the
starting string*/

/*cannot have partial specialization of variadic functions hence will have to use helper class
that class is not variadic it has partial specialization for handling terminating condition */
template <typename... Ts>
Interpolate_helper_1<1,std::tuple<const Ts&...>> Interpolate(const Ts&...t)
{
	std::tuple<const Ts&...> tup(t...);
	//Instantiating helper class with template parameter Ts as the e 0 is the string position
	//1 is the index for tuple element to be printed.
	Interpolate_helper_1<1,std::tuple<const Ts&...>> helperObj(0,tup);
	return helperObj;
}

/*Needed template class because cannot access the tuple with variable so need to have a compile time constant
which is bieng provided to class by a template argument inde. Taking a const int as the class member 
is possible?? how can it be done? all how to initialize the classes const int member at compile time?
test this is*/

}