/// @defgroup	MOD_MAIN "Main"
/// @{
/// 	@brief Module to implement the main function
/// @}
/// @ingroup MOD_MAIN
/// @file main.cpp
/// @brief Implementation of the main function
/// @author Matheus Pimenta

#include "simplestructures.hpp"

#include "StateManager.hpp"

/// @return Returns 0 for successful execution, or another value for an error.
/// @brief Main
int main (int argc, char** argv)
{
	try {
		StateManager state( MainArgs( argc, argv ) );
		state.run();
	}
	catch (mexception& e)
	{
		e.logerr ();
	}
	
	return 0;
}
