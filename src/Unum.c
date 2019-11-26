#include "Unum.h"

static const str TEST_CODE = " ## A comment ## \
        @function add2 -> \
        @parameters {yee: i32}, {bee: i32} -> \
        @result i32 -> \
        @body \
        [   \
         add yee bee -> @return;   \
        ]   \
        ";

int main(int argc, char** argv)
{
    unum_log("Unum Interpreter (Version Code %u, " __DATE__ ")", UNUM_VERSION);

    UnumInstance* ci = Unum_Initialize();
    UnumResult result = Unum_Execute(ci, TEST_CODE);
    unum_log("Result (location %u & code %d): \"%s\"\n", result.location, result.result, result.message);
    Unum_Destroy(ci);

    return (int)(result.result);
}
