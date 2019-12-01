#define UNUM_DEBUG

#include "Unum.h"

static const str TEST_CODE = " ## A comment ## \
        ##@test { a: \"val\", b: \"wew\", c: 3.14 } -> #(func 10 -> @return) -> #[ 232, 44, 55, 62, 77 ] -> ## \
        @function add2 -> \
        @parameters {yee: i32}, {bee: i32} -> \
        @result i32 -> \
        @body \
           \
         (add yee bee -> @return)   \
        ;\
        @function another ->   \
        @parameters {idk: i32} -> \
        @result i32 -> \
        @body (@return idk); \
        ";

int main(int argc, char** argv)
{
    unum_log("Unum Interpreter (Version Code %u, Built on " __DATE__ ")", UNUM_VERSION);

    UnumInstance* ci = Unum_Initialize();
    // UnumResult result = Unum_Execute(ci, TEST_CODE);
    // unum_log("Result (location %u & code %d): \"%s\"\n", result.location, result.result, result.message);

        /*
    size t = 0;
    UnumInternalPairList pp = Unum_Internal_Parse_Separate(ci->program, ";", 0);
    UnumInternalTokens n = Unum_Internal_Tokens_Sub(ci->program, pp.pairs[0]);
    for(size i = 0; i < n.count; i++)
    {
        unum_log("%s", n.tokens[i].value);
    }
    UnumInternalPairList nn = Unum_Internal_Parse_Separate(n, "->", 0);

            Unum_Debug_Print_Pair_New(ci->program, pp);
            Unum_Debug_Print_Pair_New(n, nn);

            unum_log("Done printing new pair");

    */

    Unum_Destroy(ci);

/*
                str beep = "kachow";
                str res = Unum_Internal_Utility_Strins(beep, beep, "( ");
                str res2 = Unum_Internal_Utility_Strins(res, res + strlen(res), " )");
                printf("Original: \"%s\", After: \"%s\"\n", beep, res2);
                //return 0;
 */

        return 0;
    //return (int)(result.result);
}
