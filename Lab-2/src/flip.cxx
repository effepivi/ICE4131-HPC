/**
********************************************************************************
*
*	@file		test.cpp
*
*	@brief		BRIEF DESCRIPTION ABOUT THE CONTENT OF THE FILE.
*
*	@version	1.0
*
*	@date		DATE HERE
*
*	@author		YOUR NAME HERE
*
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#include <string>
#include <sstream>
#include <iostream>
#include <exception>

#include "Image.h"


//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
    // Return code
    int error_code(0);
    
    // Catch exceptions
    try
    {
		if (argc != 4)
		{
			throw "Invalid command line";
		}

		// Load an image
		Image input;
		input.loadASCII(argv[2]);

		Image output;

		// Flip the image
		if (std::string(argv[1]) == "-H")
		{
			output = input.flipHorizontally();
		}
		else if (std::string(argv[1]) == "-V")
		{
			output = input.flipVertically();
		}
		else
		{
			throw "Invalid option";
		}

		// Save the output
		output.saveASCII(argv[3]);
	}
    // An error occured
    catch (const std::exception& error)
    {
        error_code = 1;
        std::cerr << error.what() << std::endl;
    }
    catch (const std::string& error)
    {
        error_code = 1;
        std::cerr << error << std::endl;
    }
    catch (const char* error)
    {
        error_code = 1;
        std::cerr << error << std::endl;
    }
    catch (...)
    {
        error_code = 1;
        std::cerr << "Unknown error" << std::endl;
    }

    return (error_code);
}
