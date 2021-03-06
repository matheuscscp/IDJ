#ifndef SIMPLESTRUCTURES_HPP
#define SIMPLESTRUCTURES_HPP

#include <string>

#define CALLBACK(ref,mfunc)	((ref).*(mfunc))
#define SHOW(X)				std::cout << #X << ": " << (X) << std::endl

class MainArgs
{
private:
	int argc;
	char** argv;
public:
	MainArgs(int argc, char** argv);
	
	int find(const std::string& what) const;
	std::string get(int i) const;
	std::string get(const std::string& what) const;
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
