/// @defgroup	MOD_MAIN "Main"
/// @{
/// 	@brief Module to implement the main function
/// @}
/// @ingroup MOD_MAIN
/// @file main.cpp
/// @brief Implementation of the main function
/// @author Matheus Pimenta

#include "../include/mod/simplestructures.hpp"

#include "../include/GameManager.hpp"

/// @return Returns 0 for successful execution, or another value for an error.
/// @brief Main
int main (int argc, char** argv)
{
	try {
		GameManager gm ( MainParam ( argc, argv ) );
		gm.run ();
	}
	catch (mexception& e)
	{
		e.logerr ();
	}
	
	return 0;
}
