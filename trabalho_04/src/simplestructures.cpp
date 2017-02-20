#include <iostream>
#include <fstream>
#include <ctime>

#include "simplestructures.hpp"

using std::string;
using std::cout;
using std::fstream;

MainArgs::MainArgs(int argc, char** argv) : argc(argc),argv(argv)
{
}

int MainArgs::find(const string& what) const
{
	for( int i = 0; i < argc; ++i )
	{
		if( what == argv[i] )
			return i;
	}
	
	return -1;
}

string MainArgs::get(int i) const
{
	if( i < argc )
		return string( argv[i] );
	
	return string( "" );
}

string MainArgs::get(const string& what) const
{
	int i = find( what );
	
	if( i != -1 )
		return get( i + 1 );
	
	return string( "" );
}

mexception::mexception (const string& what) throw () : what_ ( what )
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
	cout << "Logging error...\n";
	
	fstream f ( "./ErrorLog.txt", fstream::out | fstream::app );
	time_t rawtime = time ( NULL );
	
	f << "- Error: ";
	f << what_;
	f << ". Occured on ";
	f << ctime ( &rawtime );
	
	f.close ();
}
