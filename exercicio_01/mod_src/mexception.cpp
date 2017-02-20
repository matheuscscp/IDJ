/// @ingroup MOD_MEXCEP
/// @file mexception.cpp
/// @brief Implementation of the new exception class methods
/// @author Matheus Pimenta

#include "mexception.hpp"

mexception::mexception (const std::string& what) throw () : what_ ( what )
{
}

mexception::~mexception () throw ()
{
}

const char* mexception::what () const throw ()
{
	return what_.c_str ();
}

void mexception::logerr () const throw ()
{
	std::cout << "Logging error...\n";
	
	std::fstream f (	"./ErrorLog.txt",
				std::fstream::out | std::fstream::app	);
	time_t rawtime = time ( NULL );
	
	f << "- Error: ";
	f << what_;
	f << ". Occured on ";
	f << ctime ( &rawtime );
	
	f.close ();
}
