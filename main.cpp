#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include <stdlib.h>
#include <time.h>

#include "Common.hpp"
#include "CMatrix.hpp"

namespace po = boost::program_options;

uint32_t generateRandomMatrix = Parameters::MATRIX_ROWS | Parameters::MATRIX_COLUMNS | Parameters::FILL_RANDOM | Parameters::OUTPUT_FILE;
uint32_t generateMatrixFromFile = Parameters::INPUT_FILE | Parameters::OUTPUT_FILE;

po::options_description optDescr( "Allowed options" );
po::variables_map options;

void prepareOptionsDescription( void )
{
    optDescr.add_options()
        ( "help,h", "produce help message" )
        ( "random,r", "fill matrix with random values" )
        ( "rows,m", po::value<uint32_t>(), "rows in output matrix" )
        ( "cols,n", po::value<uint32_t>(), "columns in output matrix" )
        ( "input,i", po::value<std::string>(), "input file with matrix data" )
        ( "output,o", po::value<std::string>(), "output file" )
        ( "txt", "produce output in plain text format" )
        ( "bin", "consume input in binary format ( by default assume text format)" );
}

bool parseCommandLineArguments( uint32_t& inputParametersMask, std::string& inputFile, std::string& outputFile, uint32_t& rows, uint32_t& cols )
{

    if ( options.count( "random" ) )
    {
        inputParametersMask |= Parameters::FILL_RANDOM;
    }

    if ( options.count( "rows" ) )
    {
        rows = options["rows"].as<uint32_t>();
        inputParametersMask |= Parameters::MATRIX_ROWS;
    }

    if ( options.count( "cols" ) )
    {
        cols = options["cols"].as<uint32_t>();
        inputParametersMask |= Parameters::MATRIX_COLUMNS;
    }

    if ( options.count( "input" ) )
    {
        inputFile.append( options["input"].as<std::string>() );
        inputParametersMask |= Parameters::INPUT_FILE;
    }

    if ( options.count( "output" ) )
    {
        outputFile.append( options["output"].as<std::string>() );
        inputParametersMask |= Parameters::OUTPUT_FILE;
    }

    if ( options.count( "txt" ) )
        inputParametersMask |= Parameters::IS_TEXT_OUTPUT;

    if ( options.count( "bin" ) )
        inputParametersMask |= Parameters::IS_BINARY_INPUT;


    bool result = false;
    result |= ( ( generateRandomMatrix & inputParametersMask ) == generateRandomMatrix );
    result |= ( ( generateMatrixFromFile & inputParametersMask ) == generateMatrixFromFile );

    return result;
}

bool testFlag( int mask, int flag )
{
    return ( ( mask & flag ) == flag );
}

void printHelp( void )
{
    std::cout << optDescr << std::endl;
}

bool createRandomMatrix( uint32_t rows, uint32_t cols, matrix::CMatrix& m )
{
    srand( time( NULL ) );

    try
    {
        matrix::CMatrix mm( rows, cols );
        for ( uint32_t i = 0; i < rows; i++ )
        {
            for ( uint32_t j = 0; j < cols; j++ )
            {
                mm( i, j ) = rand();
            }
        }
        mm.swap( m );
    }
    catch( ... )
    {
        return false;
    }

    return true;
}

int main( int argc, char* argv[] )
{
    prepareOptionsDescription();

    try
    {
        po::store( po::parse_command_line( argc, argv, optDescr), options );
        po::notify( options );
    }
    catch ( ... )
    {
        std::cout << "Unrecognized options" << std::endl;
        printHelp();
        return -1;
    }

    if ( options.count( "help" ) )
    {
        printHelp();
    }

    std::string inputFile, outputFile;
    uint32_t rows = 0, cols = 0;
    uint32_t inputParametersMask = 0;

    matrix::CMatrix tempMatrix;

    bool success = parseCommandLineArguments( inputParametersMask, inputFile, outputFile, rows, cols );

    if ( success )
    {
        if ( testFlag( inputParametersMask, generateRandomMatrix ) )
        {
            success = createRandomMatrix( rows, cols, tempMatrix );
        }
        else
        {
            if ( testFlag( inputParametersMask, generateMatrixFromFile ) )
            {
                if ( testFlag( inputParametersMask, Parameters::IS_BINARY_INPUT ) )
                {
                    success = matrix::io::readFromBinFile( inputFile, tempMatrix );
                }
                else
                {
                    success = matrix::io::readFromTextFile( inputFile, tempMatrix );
                }
            }
        }
        if ( success )
        {
            if ( testFlag( inputParametersMask, Parameters::IS_TEXT_OUTPUT ) )
            {
                success = matrix::io::writeToTextFile( outputFile, tempMatrix );
            }
            else
            {
                success = matrix::io::writeToBinFile( outputFile, tempMatrix );
            }
        }
    }
    else
    {
        std::cout << "Invalid options" << std::endl;
        printHelp();
        return -1;
    }
    if ( !success )
    {
        std::cout << "Some error occurs..." << std::endl;
        return -2;
    }
    return 0;
}

