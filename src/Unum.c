#include "Unum.h"

static const str TEST_CODE = " ## A comment ## \
        @function m -> \
        @parameters \"wew\", {String}, {int} -> \
        @result int -> \
        @body \
        { \
        ioprint \"Hello, World!\"}, \
        {ioprint sss }, \
        {return 0 \
        }; \
        ";

int main(int argc, char** argv)
{
    UnumInstance* ci = Unum_Initialize();
    UnumResult result = Unum_Execute(ci, TEST_CODE);
    printf("Result (location %u & code %u): \"%s\"\n", result.location, result.result, result.message);
    Unum_Destroy(ci);

    return (int)(result.result);
}
