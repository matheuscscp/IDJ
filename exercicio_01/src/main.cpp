/// @defgroup	MOD_MAIN "Main"
/// @{
/// 	@brief Module to implement the main function
/// @}
/// @ingroup MOD_MAIN
/// @file main.cpp
/// @brief Implementation of the main function
/// @author Matheus Pimenta

#include "../include/mod/mexception.hpp"

#include "../include/GameManager.hpp"

/// @return Returns 0 for successful execution, or another value for an error.
/// @brief Main
int main ()
{
	try {
		GameManager gm;
		gm.run ();
	}
	catch (mexception& e)
	{
		e.logerr ();
	}
	
	return 0;
}
