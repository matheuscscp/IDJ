#include <iostream>
#include <fstream>
#include <ctime>

#include "simplestructures.hpp"

using namespace std;

MainParam::MainParam (int argc, char** argv) : argc ( argc ), argv ( argv )
{
}

bool MainParam::find (const string& what) const
{
	for ( int i = 0; i < argc; ++i )
	{
		if ( what == argv[i] )
			return true;
	}
	
	return false;
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
