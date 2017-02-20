#ifndef SIMPLESTRUCTURES_HPP
#define SIMPLESTRUCTURES_HPP

#include <string>

#define callback(ref,mfunc) ((ref).*(mfunc))

class MainParam
{
private:
	int argc;
	char** argv;
public:
	MainParam (int argc, char** argv);
	
	bool find (const std::string& what) const;
};

class mexception : public std::exception
{
private:
	std::string what_;
public:
	mexception (const std::string& what) throw ();
	
	~mexception () throw ();
	
	const char* what () const throw ();
	
	void logerr () const throw ();
};

template <class T>
class aptr
{
private:
	T* ptr;
public:
	aptr (T* ptr) : ptr ( ptr )
	{
	}
	
	~aptr ()
	{
		delete ptr;
	}
	
	T* operator& ()
	{
		return ptr;
	}
	
	T* operator-> ()
	{
		return ptr;
	}
	
	T& operator* ()
	{
		return ( *ptr );
	}
};

template <class T>
class arr_aptr
{
private:
	T* ptr;
public:
	arr_aptr (T* ptr) : ptr ( ptr )
	{
	}
	
	~arr_aptr ()
	{
		delete[] ptr;
	}
	
	T* operator& ()
	{
		return ptr;
	}
	
	T& operator[] (unsigned int i)
	{
		return ptr[i];
	}
};

#endif
