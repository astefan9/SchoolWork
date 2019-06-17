//This class allows using printf style code with a stream

#include <string>
#include <sstream>
#include <tuple>


auto ffr(std::ostream&(*f)(std::ostream&))
	-> decltype(*f) 
{
	return *f;
}

namespace cs540
{

//Our Error class	
class WrongNumberOfArgs
{
	std::string what() const throw() { return "Wrong Number of Args\n"; }
};

//Forward Declarations
template<typename Arg, typename... Args>
void Interpolater(std::string str, std::ostream *stream, const Arg& arg, const Args&... args);
template<typename Arg>
void Interpolater(std::string str, std::ostream *stream, const Arg& arg);
void Interpolater(std::string str, std::ostream *stream);

//These structs parse the variadic tuple
//Obtained from stackoverflow
template<int ...>
struct seq { };

template<int N, int ...S>
struct gens : gens<N-1, N-1, S...> { };

template<int ...S>
struct gens<0, S...> {
  typedef seq<S...> type;
};

//Helper is the return type of Interpolate so that we have somewhere to overload on ostream<<
template <typename... Args>
struct Helper
{
	Helper(const std::string& s, const std::tuple<const Args&...> t) 
		: str(s), tup(t) {}
	friend std::ostream& operator<<(std::ostream &os, const Helper &help)
	{
		callFunc(os, help, typename gens<sizeof...(Args)>::type());
		return os;
	}
	//callFunc parses our variadic tuple
	template<typename Helper, int ...S>
	static void callFunc(std::ostream &os, const Helper &help, seq<S...>)
	{
		Interpolater(help.str, &os, std::get<S>(help.tup)...);
	}
	
	std::string str;
	std::tuple<const Args&...> tup;
};

//for Comparing arg to std::endl, std::ends
static std::ostringstream check2, check3;

//Interpolate Creates a Helper which calls Interpolater
template <typename... Args>
Helper<Args...> Interpolate(std::string str, const Args&... args)
{
	//Initialize checks for checking whether a % is consumed
	check2 << ffr(std::endl);
	check3 << ffr(std::ends);
	
	std::tuple<const Args&...> tup = std::tuple<const Args&...>(args...);
	return Helper<Args...>(str, tup);
}

//Does the actual work on the ostream
void Interpolater(std::string str, std::ostream *stream)
{
	if(str.find("\\%") != std::string::npos)
	{
		*stream << str.substr(0, str.find("\\%"));
		*stream << "%";
	}
	else if(str.find("%") != std::string::npos)
		throw WrongNumberOfArgs();
	else
		*stream << str;
}
template<typename Arg>
void Interpolater(std::string str, std::ostream *stream, const Arg& arg)
{
	bool redo = false;
	std::ostringstream check;
	check << arg;
	
	//Check for argument numbers
	if(str.length() <= 0)
	{
		//Allow if arg is a manipulator
		if(check.str().length() == 0)
		{
			*stream << arg;
			return;
		}
		throw WrongNumberOfArgs();
	}
		
	if(str.find("%") != str.find("\\%") + 1 
		|| str.find("\\%") == std::string::npos)
	{
		if(str[0] != '%' && str.find("%") != std::string::npos)
			*stream << str.substr(0, str.find("%"));
		*stream << arg;
	}
	else
	{
		*stream << str.substr(0, str.find("\\%"));
		*stream << "%";
		redo = true;
	}
	
	str = str.substr(str.find("%") + 1);

	if(redo)	Interpolater(str, stream, arg);
	else if(str.find("%") != std::string::npos)
				throw WrongNumberOfArgs();
	else		*stream << str;
}

template<typename Arg, typename... Args>
void Interpolater(std::string str, std::ostream *stream, const Arg& arg, const Args&... args)
{
	bool redo = false;
	
	//We use checks to see if we need to consume a percent sign
	//if arg has length 0, we don't consume
	std::ostringstream check;//, check2, check3;
	check << arg;
	
	//Check for argument numbers
	if(str.length() <= 0)
	{
		//Allow if arg is a manipulator
		if(check.str().length() == 0)
		{
			*stream << arg;
			Interpolater(str, stream, args...);
			return;
		}
		throw WrongNumberOfArgs();
	}
	
	if(str.find("%") != str.find("\\%") + 1 
		|| str.find("\\%") == std::string::npos)
	{	
		if(str[0] != '%')
			*stream << str.substr(0, str.find("%"));
		*stream << arg;
	}
	else
	{
		*stream << str.substr(0, str.find("\\%"));
		*stream << "%";
		redo = true;
	}
	
	if(!redo && check.str().length() == 0 
		&& check.str() != check2.str()
		&& check.str() != check3.str())
		str = str.substr(str.find("%"));
	else
		str = str.substr(str.find("%") + 1);
	
	if(redo)	Interpolater(str, stream, arg, args...);
	else		Interpolater(str, stream, args...);
}


}