#define UNUM_DEBUG

#include "Unum.h"

static const str TEST_CODE = " ## A comment ## \
        ##@test { a: \"val\", b: \"wew\", c: 3.14 } -> #(func 10 -> @return) -> #[ 232, 44, 55, 62, 77 ] -> ## \
        @function add2 -> \
        @parameters {yee: i32, bee: i32} -> \
        @result i32 -> \
        @body \
           \
         (add yee bee -> @return)   \
        ;\
        @function another ->   \
        @parameters {idk: i32} -> \
        @result i32 -> \
        @body (@return idk); \
        \
        add2 128, 23 -> io.print;";

int main(int argc, char** argv)
{
    unum_log("Unum Interpreter (Version Code %u, Built on " __DATE__ ")", UNUM_VERSION);

    UnumInstance* ci = Unum_Initialize();

    if(ci == NULL)
        return -1;

    UnumResult result = Unum_Execute(ci, TEST_CODE);
    unum_log("Result (location %u & code %d): \"%s\"\n", result.location, result.result, result.message);

    Unum_Destroy(ci);

    return (int)(result.result);
}
