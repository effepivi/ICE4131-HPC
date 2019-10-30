/**
********************************************************************************
*
*   @file       log.cxx
*
*   @brief      Appy a log transformation to all the pixels of the input image.
*
*   @version    2.0
*
*   @date       30/10/2019
*
*   @author     Franck Vidal
*   @author     YOUR NAME
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
#include <exception>
#include <cctype>
#include <algorithm>
#include <locale>         // locale, toupper
#include <chrono>   // To measure durations

#include <getopt.h>
#include <unistd.h>

#include "Image.h"
#include "PthreadImage.h"
#include "OpenMPImage.h"


//******************************************************************************
//  Namespace
//******************************************************************************
using namespace std;


//******************************************************************************
//  Typedef
//******************************************************************************
typedef std::chrono::time_point<std::chrono::high_resolution_clock> timestamp;


//******************************************************************************
//  Global variables
//******************************************************************************
string input_file;
string output_file;
string implementation;
int number_of_threads = 0;


void parseCommandLine(int& argc, char** argv);
void printHelp();
void checkInputParameters();
string toUpper(const string& aString);


//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
    // Return code
    int error_code(0);

    // Catch exceptions
    try
    {
        parseCommandLine(argc, argv);
        checkInputParameters();

        /*cout << "Input file: " << input_file << endl;
        cout << "Output file: " << output_file << endl;
        cout << "Implementation: " << implementation << endl;
        cout << "Number of processes/threads: " << number_of_threads << endl;*/

        // Output image
        Image output;

        // Timestamps
        timestamp start;
        timestamp end;

        if (toUpper(implementation) == "SERIAL" || number_of_threads == 0)
        {
            // Declaration
            Image input;

            // Load the image
            input.loadASCII(input_file);

            // Filter the image
            start = chrono::high_resolution_clock::now();
            output = input.logFilter();
            end = chrono::high_resolution_clock::now();
        }
        else if (toUpper(implementation) == "PTHREAD" ||
                 toUpper(implementation) == "PTHREADS")
        {
            // Declaration
            PthreadImage input(number_of_threads);

            // Load the image
            input.loadASCII(input_file);

            // Filter the image
            start = chrono::high_resolution_clock::now();
            output = input.logFilter();
            end = chrono::high_resolution_clock::now();
        }
        else if (toUpper(implementation) == "OPENMP" ||
                 toUpper(implementation) == "OMP")
        {
            // Declaration
            OpenMPImage input(number_of_threads);

            // Load the image
            input.loadASCII(input_file);

            // Filter the image
            start = chrono::high_resolution_clock::now();
            output = input.logFilter();
            end = chrono::high_resolution_clock::now();
        }
        /*else if (toUpper(implementation) == "CUDA")
        {
            // Declaration
            CUDAImage input;

            // Load the image
            input.loadASCII(input_file);

            // Filter the image
            start = chrono::high_resolution_clock::now();
            output = input.logFilter();
            end = chrono::high_resolution_clock::now();
        }
        else if (toUpper(implementation) == "MPI")
        {
            // Declaration
            MPIImage input;

            // Load the image
            input.loadASCII(input_file);

            // Filter the image
            start = chrono::high_resolution_clock::now();
            output = input.logFilter();
            end = chrono::high_resolution_clock::now();
        }*/

        cout << "Log_filter," <<
            "\"" << input_file << "\"" << "," <<
            "\"" << output_file << "\"" << "," <<
            implementation << "," <<
            number_of_threads << "," <<
            chrono::duration<double>(end - start).count() << endl;

        // Save the output
        if (output_file.size())
        {
            output.saveASCII(output_file);
        }
    }
    // An error occured
    catch (const exception& error)
    {
        error_code = 1;
        cerr << error.what() << endl;
    }
    catch (const string& error)
    {
        error_code = 1;
        cerr << error << endl;
    }
    catch (const char* error)
    {
        error_code = 1;
        cerr << error << endl;
    }
    catch (...)
    {
        error_code = 1;
        cerr << "Unknown error" << endl;
    }

    return (error_code);
}


//-------------------------------------------
void parseCommandLine(int& argc, char** argv)
//-------------------------------------------
{
    int c;

    while (1)
    {
        static struct option long_options[] =
        {
            {"num",             required_argument, &number_of_threads, 'n'},
            {"implementation",  required_argument, nullptr,            'c'},
            {"inputFile",       required_argument, nullptr,            'i'},
            {"outputFile",      required_argument, nullptr,            'o'},
            {"help",            no_argument,       nullptr,            'h'},
            {nullptr,           no_argument,       nullptr,            0}
        };

        // getopt_long stores the option index here.
        int option_index = 0;

        c = getopt_long (argc, argv, "n:c:i:o:h",
                         long_options, &option_index);

        // Detect the end of the options.
        if (c == -1)
            break;

        switch (c)
        {
        case 0:
            // If this option set a flag, do nothing else now.
            if (long_options[option_index].flag != 0)
                break;

            printf ("option %s", long_options[option_index].name);
            if (optarg)
                printf (" with arg %s", optarg);
                printf ("\n");
                break;

        case 'n':
            number_of_threads = atoi(optarg);
            break;

        case 'c':
            implementation = optarg;
            break;

        case 'i':
            input_file = optarg;
            break;

        case 'o':
            output_file = optarg;
            break;

        case 'h':
            printHelp();
            break;

        case '?':
            // getopt_long already printed an error message.
            break;

        default:
            abort ();
        }
    }
}


//--------------
void printHelp()
//--------------
{
    cout <<
            "--num <n>:                  Number of threads/processes\n"
            "--implementation <string>:  serial|pthread|openmp|cuda|mpi\n"
            "--inputFile <fname>:        File to write to\n"
            "--outputFile <fname>:       File to write to\n"
            "--help:                     Show help\n";
    exit(1);
}


//-------------------------
void checkInputParameters()
//-------------------------
{
    if (!input_file.size())
    {
        throw "No input file to process.";
    }

    if (!output_file.size())
    {
        cerr << "No output file to save the result." << endl;
    }

    if (!implementation.size())
    {
        implementation = "serial";
    }
    else
    {
        if (toUpper(implementation) != "SERIAL")
        if (toUpper(implementation) != "PTHREAD")
        if (toUpper(implementation) != "PTHREADS")
        if (toUpper(implementation) != "OPENMP")
        if (toUpper(implementation) != "OMP")
        if (toUpper(implementation) != "CUDA")
        if (toUpper(implementation) != "MPI")
            throw "Invalid implementation. Valid options are serial, pthread, openmp, cuda, or mpi.";

        if (toUpper(implementation) == "CUDA")
            throw "CUDA implementation not supported as yet.";

        if (toUpper(implementation) == "MPI")
            throw "MPI implementation not supported as yet.";
    }
}


//---------------------------------------------
string toUpper(const string& aString)
//---------------------------------------------
{
    string temp = aString;
    locale loc;

    for (auto ite = temp.begin(); ite != temp.end(); ++ite)
    {
        *ite = toupper(*ite, loc);
    }

    return temp;
}
