/*************************************************************************
 * mgen
 *************************************************************************
 * @file    Common.hpp
 * @date    28.01.15
 * @author  Hlieb Romanov <rgewebppc@gmail.com>
 * @brief   Common declarations
 ************************************************************************/
/** @addtogroup mgen Matrices generator and converter tool
 *  @{
 */
#ifndef COMMON_HPP
#define COMMON_HPP

namespace Parameters
{
    enum cmd_line_params {
        NONE,
        FILL_RANDOM = 1,
        MATRIX_ROWS = 2,
        MATRIX_COLUMNS = 4,
        INPUT_FILE = 8,
        OUTPUT_FILE = 16,
        IS_CSV_OUTPUT = 32,
        IS_TEXT_OUTPUT = 64,
        IS_BINARY_INPUT = 128
    };
}

/**
 * @brief Test specific bits in mask
 * @param mask - bit mask
 * @param flag - bits to test
 * @return True - if all bits in flag are set in mask
 */
bool testFlag(int mask, int flag);

#endif // COMMON_HPP
/** @}*/
