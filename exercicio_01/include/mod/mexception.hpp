/// @defgroup	MOD_MEXCEP "mexception"
/// @{
/// 	@brief Module to define and implement a new exception class
/// @}
/// @ingroup MOD_MEXCEP
/// @file mexception.hpp
/// @brief New exception class and its methods
/// @author Matheus Pimenta

#ifndef MEXCEPTION_HPP
#define MEXCEPTION_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

/// Class to handle exceptions with a string to describe it.
/// @brief Exception class with descriptive string
class mexception : public std::exception
{
private:
	/// @brief Descriptive string
	std::string what_;
public:
	/// @param what Descriptive string to be assigned.
	/// @brief Assignment constructor
	mexception (const std::string& what) throw ();
	
	/// @brief Empty destructor
	~mexception () throw ();
	
	/// @return The descriptive string.
	/// @brief Access method to the descriptive string
	const char* what () const throw ();
	
	/// @brief Append the description in a file named "ErrorLog.txt"
	void logerr () const throw ();
};

#endif
