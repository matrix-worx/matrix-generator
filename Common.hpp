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

bool testFlag(int mask, int flag);

#endif // COMMON_HPP
