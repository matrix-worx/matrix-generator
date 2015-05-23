/*************************************************************************
 * mgen
 *************************************************************************
 * @file    main.cpp
 * @date    28.01.15
 * @author  Hlieb Romanov <rgewebppc@gmail.com>
 * @brief   Command line tool for matrices generation
 ************************************************************************/
/** @addtogroup mgen Matrices generator and converter tool
 *  @{
 */
#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include <stdlib.h>
#include <time.h>

#include "Common.hpp"
#include "CMatrix.hpp"

namespace po = boost::program_options;

// Define required options masks
uint32_t generateRandomMatrix = Parameters::MATRIX_ROWS | Parameters::MATRIX_COLUMNS | Parameters::FILL_RANDOM | Parameters::OUTPUT_FILE;
uint32_t generateMatrixFromFile = Parameters::INPUT_FILE | Parameters::OUTPUT_FILE;

po::options_description optDescr( "Allowed options" );
po::variables_map options;

/**
 * @brief Prepare program options object
 */
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

/**
 * @brief Parse command line arguments and fill passed arguments
 * @param[out] inputParametersMask - binary mask that indicates which options were set
 * @param[out] inputFile - path to the input file with matrix data
 * @param[out] outputFile - path to the file where to place generated matrix
 * @param[out] rows - rows count in new matrix
 * @param[out] cols - column count in new matrix
 * @return True - if all required options were set, false - otherwise
 */
bool parseCommandLineArguments( uint32_t& inputParametersMask,
                                std::string& inputFile,
                                std::string& outputFile,
                                uint32_t& rows,
                                uint32_t& cols )
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
/**
 * @brief Test specific bits in mask
 * @param mask - bit mask
 * @param flag - bits to test
 * @return True - if all bits in flag are set in mask
 */
bool testFlag( int mask, int flag )
{
    return ( ( mask & flag ) == flag );
}

/**
 * @brief Print help text with options description
 */
void printHelp( void )
{
    std::cout << optDescr << std::endl;
}

/**
 * @brief Create matrix with specified dimension and fill it with random numbers
 * @param rows - rows count in new matrix
 * @param cols - columns count in new matrix
 * @param[out] m - empty(invalid) matrix object to fill
 * @return True - if operation was performed successfully, false -otherwise
 */
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
/**
 * @brief The main function
 * @param argc
 * @param argv
 * @return
 */
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
/** @}*/
