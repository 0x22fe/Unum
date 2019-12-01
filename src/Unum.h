/*
 *
 * Unum
 * Date: November 14, 2019
 *
 * MIT License
 *
 * Copyright (c) 2019 0x22fe

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

/****************************************************************
 * Standard Libraries
 ****************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <ctype.h>

/****************************************************************
 * 3rd Party Libraries
 ****************************************************************/

#include <plibsys.h>

/****************************************************************
 * Internal Libraries
 ****************************************************************/

// Coming soon!

/****************************************************************
 * Include Guards and Definitions
 ****************************************************************/

#ifndef UNUM_INCLUDE
#define UNUM_INCLUDE

// Version macro
// Refer to https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VK_MAKE_VERSION.html
#define UNUM_MAKE_VERSION(major, minor, patch) \
    (((major) << 22) | ((minor) << 12) | (patch))
#define UNUM_VERSION UNUM_MAKE_VERSION(0, 0, 1)

// Function definition type
#ifndef UNUM_DEF
#define UNUM_DEF static
#endif

// Define NULL if necessary
#ifndef NULL
#define NULL ((void*)0)
#endif

// Define non-null zero
#define SAFE ((void*)1)

// Custom true/false to avoid stdbool.h
#define UNUM_FALSE (0U)
#define UNUM_TRUE (1U)

// Log flush function
#define unum_flush() fflush(stdout)

// Debug Macro - UNUM_DEBUG

// Debug macro - Log/Assert function
#ifdef UNUM_DEBUG
#define unum_log(...) printf("[Unum][%u ms]: ", unum_time() / 1000U); printf(__VA_ARGS__); printf("\n"); unum_flush()
#else
#define unum_log(...) printf("[Unum][%u ms]: ", unum_time() / 1000U); printf(__VA_ARGS__); printf("\n")
#undef assert
#define assert ;
#endif

// NaN value
#ifndef NAN
#define UNUM_NAN 0.0F
#define UNUM_ISNAN(v) ((v) == 0.0F)
#else
#define UNUM_NAN NAN
#define UNUM_ISNAN(v) (isnan(v))
#endif

// Pointer casting protection
#ifdef __cplusplus
#define UNUM_CPROTECT(v) (v*)
#else
#define UNUM_CPROTECT(v)
#endif

// Malloc function
#ifndef UNUM_MALLOC
#define UNUM_MALLOC malloc
#endif

// Realloc function
#ifndef UNUM_REALLOC
#define UNUM_REALLOC realloc
#endif

// Free function
#ifndef UNUM_FREE
#define UNUM_FREE free
#endif

// Type definitions
typedef float_t f32;
typedef double_t f64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef u8 bool;

typedef u8 byte;
typedef char txt;
typedef txt* str;
typedef u32 size;
typedef void* any;

/****************************************************************
 * Preprocessor Constants
 ****************************************************************/

// C++ handler
#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************
 * Function Declarations
 ****************************************************************/

#ifdef UNUM_DECLARE
// Coming soon!
#endif

/****************************************************************
 * Main Code
 ****************************************************************/

/****************************************************************
 * Types
 ****************************************************************/

typedef struct UnumInternalPair
{
    size a;
    size b;
} UnumInternalPair;

typedef struct UnumInternalPairList
{
    size num;
    UnumInternalPair* pairs;
} UnumInternalPairList;

typedef struct UnumInternalTypePair
{
    str name;
    size type_id;
} UnumInternalTypePair;

typedef struct UnumInternalType
{
    str name;
    size parts_count;
    UnumInternalTypePair* parts;
} UnumInternalType;

typedef struct UnumInternalFunction
{
    str name;
    byte* data;
    UnumInternalType parameters;
    UnumInternalType result;
} UnumInternalFunction;

typedef struct UnumInternalObject
{
    str name;
    size type_id;
    byte* data;
} UnumInternalObject;

typedef struct UnumInternalStack
{
    size type_count;
    size object_count;
    size function_count;

    UnumInternalType* types;
    UnumInternalObject* objects;
    UnumInternalFunction* functions;

} UnumInternalStack;

typedef enum UnumInternalLang
{
    UNUM_LANG_BLANK = 0,
    UNUM_LANG_KEYWORD, UNUM_LANG_SYMBOL, UNUM_LANG_STRING, UNUM_LANG_NUMBER, UNUM_LANG_OPERATOR
} UnumInternalLang;

typedef struct UnumInternalStatement
{
    str name;
    UnumInternalType result;

    /*
    struct UnumInternalStatementPair
    {
        UnumInternalPair param;
        struct UnumInternalStatement* calls;
    };
    */

    UnumInternalPair* parameters;
} UnumInternalStatement;

typedef struct UnumInternalToken
{
    str value;
    UnumInternalLang type;
} UnumInternalToken;

typedef struct UnumInternalTokens
{
    size count;
    str code;
    str statement;
    UnumInternalToken* tokens;
} UnumInternalTokens;

typedef struct UnumInstance
{
    UnumInternalStack stack;
    UnumInternalTokens program;
} UnumInstance;

/****************************************************************
 * Constants
 ****************************************************************/

typedef enum UnumStatus
{
    UNUM_STATUS_OK = 0,
    UNUM_STATUS_WARNING, UNUM_STATUS_ERROR
} UnumStatus;

UNUM_DEF const str UNUM_STATUSES[] =
{
    [UNUM_STATUS_OK] = "Ok",
    [UNUM_STATUS_WARNING] = "Warning",
    [UNUM_STATUS_ERROR] = "Error"
};

typedef enum UnumCode
{
    UNUM_CODE_NEUTRAL = 0,
    UNUM_CODE_SYNTAX_ERROR, UNUM_CODE_RUNTIME_ERROR
} UnumCode;

typedef struct UnumCodeInfo
{
    str message;
    UnumStatus level;
} UnumCodeInfo;

UNUM_DEF const UnumCodeInfo UNUM_CODES[] =
{
    [UNUM_CODE_NEUTRAL] = {.message = "Ok", .level = UNUM_STATUS_OK},
    [UNUM_CODE_SYNTAX_ERROR] = {.message = "Syntax error", .level = UNUM_STATUS_ERROR},
    [UNUM_CODE_RUNTIME_ERROR] = {.message = "Runtime error", .level = UNUM_STATUS_ERROR}
};

typedef struct UnumResult
{
    size location;
    UnumCode code;
    str message;
    i32 result;
} UnumResult;

typedef enum UnumInternalKeywords
{
    UNUM_KEYWORD_FUNCTION = 0,
    UNUM_KEYWORD_STRUCTURE, UNUM_KEYWORD_PARAMETERS, UNUM_KEYWORD_RESULT, UNUM_KEYWORD_BODY, UNUM_KEYWORD_RETURN
} UnumInternalKeywords;

typedef enum UnumInternalPrimitives
{
    UNUM_PRIMITIVE_NULL = 0,
    UNUM_PRIMITIVE_PARAMETERS,
    UNUM_PRIMITIVE_ARRAY,
    UNUM_PRIMITIVE_I8, UNUM_PRIMITIVE_I16, UNUM_PRIMITIVE_I32, UNUM_PRIMITIVE_I64,
    UNUM_PRIMITIVE_U8, UNUM_PRIMITIVE_U16, UNUM_PRIMITIVE_U32, UNUM_PRIMITIVE_U64,
    UNUM_PRIMITIVE_F32, UNUM_PRIMITIVE_F64,
    UNUM_PRIMITIVE_BOOL
} UnumInternalPrimitives;

typedef enum UnumInternalOperators
{
    UNUM_OPERATOR_COMMENT_SINGLE = 0, UNUM_OPERATOR_COMMENT_MULTIPLE,
    UNUM_OPERATOR_COMPILER, UNUM_OPERATOR_STRING, UNUM_OPERATOR_MEMBER,
    UNUM_OPERATOR_PASS, UNUM_OPERATOR_END, UNUM_OPERATOR_SEPARATE,
    UNUM_OPERATOR_SET_START, UNUM_OPERATOR_SET_END,
    UNUM_OPERATOR_ARRAY_START, UNUM_OPERATOR_ARRAY_END,
    UNUM_OPERATOR_EXPRESSION_START, UNUM_OPERATOR_EXPRESSION_END
} UnumInternalOperators;

UNUM_DEF const str UNUM_OPERATORS[] =
{
    [UNUM_OPERATOR_COMMENT_SINGLE] = "#",
    [UNUM_OPERATOR_COMMENT_MULTIPLE] = "##",
    [UNUM_OPERATOR_COMPILER] = "@",
    [UNUM_OPERATOR_STRING] = "\"",
    [UNUM_OPERATOR_MEMBER] = ".",
    [UNUM_OPERATOR_PASS] = "->",
    [UNUM_OPERATOR_END] = ";",
    [UNUM_OPERATOR_SEPARATE] = ",",
    [UNUM_OPERATOR_SET_START] = "{",
    [UNUM_OPERATOR_SET_END] = "}",
    [UNUM_OPERATOR_ARRAY_START] = "[",
    [UNUM_OPERATOR_ARRAY_END] = "]",
    [UNUM_OPERATOR_EXPRESSION_START] = "(",
    [UNUM_OPERATOR_EXPRESSION_END] = ")"
};

/****************************************************************
 * Utility and Parsing
 ****************************************************************/

// Returns time in microseconds
// https://www.geeksforgeeks.org/measure-execution-time-with-high-precision-in-c-c/
UNUM_DEF size unum_time(void)
{
    return (size)(((f64)(clock()) / (f64)(CLOCKS_PER_SEC)) * 1e6);
}

UNUM_DEF bool Unum_Internal_Utility_Txtvalid(txt s)
{
    if(s == '\0' || (iscntrl(s) && s != '\n' && s != '\t')) return UNUM_FALSE;
    else return UNUM_TRUE;
}

UNUM_DEF bool Unum_Internal_Utility_Strstart(str s, str starts)
{
    if(strncmp(s, starts, strlen(starts)) == 0) return UNUM_TRUE;
    else return UNUM_FALSE;
}

UNUM_DEF str Unum_Internal_Utility_Strdup(str s)
{
    size len = strlen(s);
    str res = UNUM_MALLOC(sizeof(txt) * len);
    memcpy(res, s, len);
    return res;
}

UNUM_DEF str Unum_Internal_Utility_Stradd(str a, str b)
{
    size lena = strlen(a);
    size lenb = strlen(b);
    str res = UNUM_MALLOC(sizeof(txt) * (lena + lenb));
    memcpy(res, a, lena);
    memcpy(res + (lena * sizeof(txt)), b, lenb);
    return res;
}

UNUM_DEF str Unum_Internal_Utility_Strsub(str s, size a, size b)
{
    size len = b - a;
    str res = UNUM_MALLOC(sizeof(txt) * len);
    memcpy(res, s + (a * sizeof(txt)), len);
    return res;
}

UNUM_DEF str Unum_Internal_Utility_Strins(str s, str location, str a)
{
    size lens = strlen(s);
    size lena = strlen(a);

    if(s > location || s + (lens * sizeof(txt)) < location)
        return NULL;

    size delta = location - s;

    str res = UNUM_MALLOC(lens + lena);
    memset(res, 0, lens + lena);
    memcpy(res, s, delta);
    memcpy(res + delta, a, lena);
    memcpy(res + delta + lena, s + delta, lens - delta);

    return res;
}

UNUM_DEF UnumInternalTokens Unum_Utility_Tokensub(UnumInternalTokens s, UnumInternalPair range)
{
    UnumInternalTokens res;
    res.code = NULL;
    res.statement = NULL;
    res.count = (range.b - range.a) + 1;
    res.tokens = UNUM_MALLOC(sizeof(UnumInternalToken) * res.count);

    for(size i = 0; i < res.count; i++)
    {
        res.tokens[i] = s.tokens[range.a + i];
    }

    return res;
}

UNUM_DEF str Unum_Internal_Parse_String(str start)
{
    size i = 1;

    while(Unum_Internal_Utility_Txtvalid(start[i]))
    {
        if(start[i] == '"' && start[i - 1] != '\\') break;
        i++;
    }

    i++;

    return start + (i * sizeof(txt));
}

UNUM_DEF str Unum_Internal_Parse_Comment(str start)
{
    size i = 1;

    // Multi-line comment
    if(start[i] == '#') {
        i++;
        while(Unum_Internal_Utility_Txtvalid(start[i]))
        {
            if(start[i - 1] == '#' && start[i] == '#') break;
            i++;
        }
        i++;
    }
    // Single-line comment
    else {
        while(Unum_Internal_Utility_Txtvalid(start[i]))
        {
            if(start[i] == '\n') break;
            i++;
        }
    }

    return start + (i * sizeof(txt));
}

UNUM_DEF str Unum_Internal_Parse_Symbol(str start)
{
    size i = 1;

    while(Unum_Internal_Utility_Txtvalid(start[i]))
    {
        if(!isalnum(start[i]) && start[i] != '_') break;
        i++;
    }

    return start + (i * sizeof(char));
}

UNUM_DEF str Unum_Internal_Parse_Number(str start)
{
    size i = 1;

    while(Unum_Internal_Utility_Txtvalid(start[i]))
    {
        if(!isdigit(start[i]))
        {
            if(start[i] == '.')
            {
                if(!isdigit(start[i + 1])) break;
            } else {
                break;
            }
        }

        i++;
    }

    return start + (i * sizeof(char));
}

UNUM_DEF str Unum_Internal_Parse_Operator(str start)
{
    /*
        if(isalnum(start[i]) || isspace(start[i]) ||
                start[i] == '@' || start[i] == '#' || start[i] == '"'
                || start[i] == ',' || start[i] == '\''|| start[i] == ';'
                || start[i] == '[' || start[i] == ']') break;
             */
    /*
        if(start[i] != '{' || start[i] != '}' || start[i] != '(' || start[i] != ')' ||
                start[i] != '[' || start[i] != ']') break;
                */

    str begin = start;

    // Todo - Optimize
    if(Unum_Internal_Utility_Strstart(begin, "{") ||
            Unum_Internal_Utility_Strstart(begin, "}") ||
            Unum_Internal_Utility_Strstart(begin, "(") ||
            Unum_Internal_Utility_Strstart(begin, ")") ||
            Unum_Internal_Utility_Strstart(begin, "[") ||
            Unum_Internal_Utility_Strstart(begin, "]") ||
            Unum_Internal_Utility_Strstart(begin, ",") ||
            Unum_Internal_Utility_Strstart(begin, ".") ||
            Unum_Internal_Utility_Strstart(begin, ":") ||
            Unum_Internal_Utility_Strstart(begin, ";")) {
        return begin + (1 * sizeof(txt));
    } else if (Unum_Internal_Utility_Strstart(begin, "->")) {
        return begin + (2 * sizeof(txt));
    } else {
        return NULL;
    }
}

UNUM_DEF UnumInternalTokens Unum_Internal_Tokenize(str code)
{
    size i = 0;
    size u = 0;
    size lines = 0;
    size length = strlen(code);

    UnumInternalTokens result;

    // Add () to start and end of program
    /*
    str res_first = Unum_Internal_Utility_Strins(code, code, "(");
    result.code = Unum_Internal_Utility_Strins(res_first, res_first + strlen(res_first), ")");
    //UNUM_FREE(res_first);
    length = strlen(result.code);
    // unum_log("\"%s\"", result.code);
    */

    result.count = 0;
    result.code = UNUM_MALLOC(length * sizeof(txt));
    result.statement = UNUM_MALLOC(2 * length * sizeof(txt)); // Arbitrary size
    result.tokens = UNUM_MALLOC(sizeof(UnumInternalToken) * length); // Likely exceeds actual count

    memcpy(result.code, code, length * sizeof(txt));
    memset(result.statement, ' ', 2 * length * sizeof(txt));
    memset(result.tokens, 0, sizeof(UnumInternalToken) * length);

    // Parse loop
    while(i < length)
    {
        str start = NULL;
        str end = NULL;

        UnumInternalLang val = UNUM_LANG_BLANK;
        bool skip = UNUM_FALSE;

        if(result.code[i] == '"')
        {
            // String
            start = result.code + (i * sizeof(txt));
            end = Unum_Internal_Parse_String(start);
            val = UNUM_LANG_STRING;
        } else if(result.code[i] == '#') {
            // Comment
            start = result.code + (i * sizeof(txt));
            end = Unum_Internal_Parse_Comment(start);
            skip = UNUM_TRUE;
        } else if(result.code[i] == '@') {
            // Keyword
            start = result.code + (i * sizeof(txt));
            end = Unum_Internal_Parse_Symbol(start);
            val = UNUM_LANG_KEYWORD;
        } else if(isalpha(result.code[i]) || result.code[i] == '_') {
            // Symbol
            start = result.code + (i * sizeof(txt));
            end = Unum_Internal_Parse_Symbol(start);
            val = UNUM_LANG_SYMBOL;
        } else if(isdigit(result.code[i]) || (result.code[i] == '.' && isdigit(result.code[i + 1]))) {
            // Number
            start = result.code + (i * sizeof(txt));
            end = Unum_Internal_Parse_Number(start);
            val = UNUM_LANG_NUMBER;
        } else if(!isspace(result.code[i])) {
            // Operator
            start = result.code + (i * sizeof(txt));
            end = Unum_Internal_Parse_Operator(start);
            val = UNUM_LANG_OPERATOR;
        } else {
            // Whitespace
            skip = UNUM_TRUE;
            while(isspace(result.code[i]) && Unum_Internal_Utility_Txtvalid(result.code[i]))
            {
                if(result.code[i] == '\n') lines++;
                i++;
            }
        }

        // Add to tokens
        if(start != NULL)
        {
            if(end == NULL) /* Error */ return result;

            if(!skip)
            {
                str s = result.statement + (u * sizeof(txt));

                memcpy(s, start, end - start);

                result.tokens[result.count].type = val;
                result.tokens[result.count].value = s;

                u += (end - start) / sizeof(txt);
                result.statement[u] = '\0';
                u++;

                result.count++;
            } else {
                lines++;
            }

            i += (end - start) / sizeof(txt);
        }
    }

    return result;
}

UNUM_DEF UnumInternalPair Unum_Internal_Parse_Match(UnumInternalTokens t, size start)
{
    const UnumInternalPair def = (UnumInternalPair){.a = 0, .b = 0};

    str begin = t.tokens[start].value;
    str match;

    if(strcmp(begin, "{") == 0) match = "}"; // Set operator
    else if(strcmp(begin, "(") == 0) match = ")"; // Expression operator
    else if(strcmp(begin, "[") == 0) match = "]"; // Array operator
    else return def;

    size open = 1;
    for(size i = start + 1; i < t.count; i++)
    {
        if(strcmp(t.tokens[i].value, match) == 0) open--;
        else if (strcmp(t.tokens[i].value, begin) == 0) open++;
        if(open == 0) return (UnumInternalPair){.a = start, .b = i};
    }

    return def;
}

// If range.a is not equal to range.b, the range values are used
// If range.a is equal to range.b, it is ignored
UNUM_DEF UnumInternalPairList Unum_Internal_Parse_Separate(UnumInternalTokens t, UnumInternalPair range, str sep)
{
    const UnumInternalPairList def = (UnumInternalPairList){.num = 0, .pairs = NULL};
    UnumInternalPair p;

    if(range.a != range.b)
    {
        // Parse in range
        p = range;
    } else {
        p = Unum_Internal_Parse_Match(t, range.a);
        // Ensure match has been found
        if(p.a == p.b)
            return def;
        // Handle empty value
        else if(p.b - p.a == 1)
            return (UnumInternalPairList){.num = 0, .pairs = SAFE};
    }

    UnumInternalPairList res;
    res.num = 0;
    res.pairs = UNUM_MALLOC(sizeof(UnumInternalPair) * 1);

    size i = p.a == 0 ? 0 : p.a + 1;
    size last = i;
    while(i < p.b)
    {
        if(strcmp(t.tokens[i].value, "{") == 0 || strcmp(t.tokens[i].value, "(") == 0 || strcmp(t.tokens[i].value, "[") == 0)
        {
            UnumInternalPair tmp = Unum_Internal_Parse_Match(t, i);
            if(tmp.a == tmp.b || tmp.b > p.b)
            {
                // Error - Incorrect start/end operators (syntax)
                UNUM_FREE(res.pairs);
                return def;
            }
            i = tmp.b;
        }

        if(strcmp(t.tokens[i].value, sep) == 0)
        {
            /*
            if(last >= i - 1) {
                res.num++;
                res.pairs = UNUM_REALLOC(res.pairs, sizeof(UnumInternalPair) * res.num);
                res.pairs[res.num - 1] = (UnumInternalPair) {.a = i - 1, .b = i - 1};
            }
            */
            res.num++;
            res.pairs = UNUM_REALLOC(res.pairs, sizeof(UnumInternalPair) * res.num);
            res.pairs[res.num - 1] = (UnumInternalPair) {.a = last, .b = i - 1};
            last = i + 1;
        }

        i++;
    }

    if(last == i + 1) {
        // Warning - Stray separator at end of array
        // Ignore, as this is helpful in cases where the array/set/expression is generated by an external program or script
    } else {
        // Add last element
        res.num++;
        res.pairs = UNUM_REALLOC(res.pairs, sizeof(UnumInternalPair) * res.num);
        res.pairs[res.num - 1] = (UnumInternalPair) {.a = last, .b = p.b - (strlen(sep) > 1 ? 0 : 1)};
    }

    return res;
}

UNUM_DEF UnumInternalPair* Unum_Internal_Parse_Arguments(UnumInternalTokens t, size start, size* out_num)
{
    UnumInternalPair p = Unum_Internal_Parse_Match(t, start);
    UnumInternalPair* res = UNUM_MALLOC(sizeof(UnumInternalPair) * 1);

    *out_num = 0;

    if(p.a == p.b)
    {
        UNUM_FREE(res);
        return NULL;
    } else if(p.b - p.a == 1) {
        UNUM_FREE(res);
        return res; // Empty result
    }

    // Array
    // Parses into each element of the array
    if(strcmp(t.tokens[p.a].value, "[") == 0)
    {
        size i = start + 1;
        size last = i;
        while(i < p.b)
        {
            if(strcmp(t.tokens[i].value, "{") == 0 || strcmp(t.tokens[i].value, "(") == 0 || strcmp(t.tokens[i].value, "[") == 0)
            {
                UnumInternalPair tmp = Unum_Internal_Parse_Match(t, i);
                if(tmp.a == tmp.b || tmp.b > p.b)
                {
                    // Error - Incorrect start/end operators (syntax)
                    UNUM_FREE(res);
                    return NULL;
                }
                i = tmp.b;
            }

            i++;

            if(strcmp(t.tokens[i].value, ",") == 0)
            {
                *out_num += 1;
                res = UNUM_REALLOC(res, sizeof(UnumInternalPair) * (*out_num));
                res[(*out_num) - 1] = (UnumInternalPair) {.a = last, .b = i - 1};
                last = i + 1;
            }
        }

        if(last == i) {
            // Warning - Stray comma at end of array
            // Ignore, as this is helpful in cases where the array is generated by an external program or script
        } else {
            // Add last element
            *out_num += 1;
            res = UNUM_REALLOC(res, sizeof(UnumInternalPair) * (*out_num));
            res[(*out_num) - 1] = (UnumInternalPair) {.a = last, .b = p.b - 1};
        }

        // Set
        // Parses into all the key-pair values in the set
    } else if(strcmp(t.tokens[p.a].value, "{") == 0)
    {
        bool next_is_value = UNUM_FALSE;
        size i = start + 1;
        size last = i;
        while(i < p.b)
        {
            if(strcmp(t.tokens[i].value, "{") == 0 || strcmp(t.tokens[i].value, "(") == 0 || strcmp(t.tokens[i].value, "[") == 0)
            {
                UnumInternalPair tmp = Unum_Internal_Parse_Match(t, i);
                if(tmp.a == tmp.b || tmp.b > p.b)
                {
                    // Error - Incorrect start/end operators (syntax)
                    UNUM_FREE(res);
                    return NULL;
                }
                i = tmp.b;
            }

            i++;

            if(strcmp(t.tokens[i].value, ":") == 0)
            {
                if(next_is_value == UNUM_TRUE) {
                    // Error - Symbol out of place (syntax)
                    UNUM_FREE(res);
                    return NULL;
                }
                next_is_value = UNUM_TRUE;
            } else if(strcmp(t.tokens[i].value, ",") == 0)
            {
                if(next_is_value == UNUM_FALSE) {
                    // Error - Value out of place (syntax)
                    UNUM_FREE(res);
                    return NULL;
                }
                next_is_value = UNUM_FALSE;
                *out_num += 1;
                res = UNUM_REALLOC(res, sizeof(UnumInternalPair) * (*out_num));
                res[(*out_num) - 1] = (UnumInternalPair) {.a = last, .b = i - 1};
                last = i + 1;
            }
        }

        if(last == i) {
            // Warning - Stray comma at end of array
            // Ignore, as this is helpful in cases where the array is generated by an external program or script
        } else {
            // Add last element
            if(next_is_value == UNUM_FALSE) {
                // Error - Value out of place (syntax)
                UNUM_FREE(res);
                return NULL;
            }
            next_is_value = UNUM_FALSE;
            *out_num += 1;
            res = UNUM_REALLOC(res, sizeof(UnumInternalPair) * (*out_num));
            res[(*out_num) - 1] = (UnumInternalPair) {.a = last, .b = i - 1};
            last = i + 1;
        }

        // Expression
        // Parses into each function call in the expression
    } else if(strcmp(t.tokens[p.a].value, "(") == 0) {
        size i = start + 1;
        size last = i;
        while(i < p.b)
        {
            if(strcmp(t.tokens[i].value, "{") == 0 || strcmp(t.tokens[i].value, "(") == 0 || strcmp(t.tokens[i].value, "[") == 0)
            {
                UnumInternalPair tmp = Unum_Internal_Parse_Match(t, i);
                if(tmp.a == tmp.b || tmp.b > p.b)
                {
                    // Error - Incorrect start/end operators (syntax)
                    UNUM_FREE(res);
                    return NULL;
                }
                i = tmp.b;
            }

            i++;

            if(strcmp(t.tokens[i].value, "->") == 0 || strcmp(t.tokens[i].value, ";") == 0)
            {
                *out_num += 1;
                res = UNUM_REALLOC(res, sizeof(UnumInternalPair) * (*out_num));
                res[(*out_num) - 1] = (UnumInternalPair) {.a = last, .b = i - 1};
                last = i + 1;
            } else if(strcmp(t.tokens[i].value, ";") == 0)
            {
                *out_num += 1;
                res = UNUM_REALLOC(res, sizeof(UnumInternalPair) * (*out_num));
                res[(*out_num) - 1] = (UnumInternalPair) {.a = last, .b = i - 1};
                last = i + 1;
                break;
            }
        }

        if(last == i && strcmp(t.tokens[last - 1].value, ";") != 0) {
            // Error - Stray pass operator at end of array
            UNUM_FREE(res);
            return NULL;
        } else if(strcmp(t.tokens[last - 1].value, ";") != 0) {
            // Add last element
            *out_num += 1;
            res = UNUM_REALLOC(res, sizeof(UnumInternalPair) * (*out_num));
            res[(*out_num) - 1] = (UnumInternalPair) {.a = last, .b = p.b - 1};
        }
    }

    return res;
}

UNUM_DEF void Unum_Debug_Print_Pair(UnumInternalTokens s, UnumInternalPairList p)
{
    unum_log("Pair (%u elements):", p.num);
    for(size t = 0; t < p.num; t++)
    {
        printf("\t\t");
        if(p.pairs[t].a == p.pairs[t].b)
        {
            printf("%s", s.tokens[p.pairs[t].a].value);
        } else {
            for(size c = p.pairs[t].a; c <= p.pairs[t].b; c++)
                printf("%s", s.tokens[c].value);
        }
        printf("\n");
    }
    unum_log("End Pair");
}

/****************************************************************
 * Language core functions
 ****************************************************************/

// Refer https://stackoverflow.com/questions/3419332/c-preprocessor-stringify-the-result-of-a-macro
#define UNUM_QUOTE(str) #str
#define UNUM_EXPAND_AND_QUOTE(str) UNUM_QUOTE(str)
#define UNUM_INTERNAL_RESULT(msg, loc, c) (UnumResult){.location = (loc), .code = (c), .message = Unum_Internal_Utility_Stradd(UNUM_CODES[(c)].message, " (Interpreter line " UNUM_EXPAND_AND_QUOTE(__LINE__) "): " msg), .result = -((i32)(UNUM_CODES[(c)].level)) }
#define UNUM_INTERNAL_RESULT_OK (UnumResult){.location = 0, .code = (UNUM_CODE_NEUTRAL), .message = Unum_Internal_Utility_Stradd(UNUM_CODES[(UNUM_CODE_NEUTRAL)].message, " (Interpreter line " UNUM_EXPAND_AND_QUOTE(__LINE__) "): " "Ok"), .result = -((i32)(UNUM_CODES[(UNUM_CODE_NEUTRAL)].level)) }

// Resolves type name to ID
UNUM_DEF size Unum_Internal_Execute_Typeid(UnumInstance* c, str typename)
{
    // Lookup type name
    // Todo - Optimize by using a hash table
    size res = UNUM_PRIMITIVE_NULL;
    for(size u = 0; u < c->stack.type_count; u++)
    {
        if(strcmp(typename, c->stack.types[u].name) == 0)
        {
            res = u;
            break;
        }
    }

    return res;
}

// Resolves function name to ID
UNUM_DEF size Unum_Internal_Execute_Functionid(UnumInstance* c, str typename)
{
    // Lookup function name
    // Todo - Optimize by using a hash table
    size res = 0;
    for(size i = 0; i < c->stack.function_count; i++)
    {
        if(strcmp(typename, c->stack.functions[i].name) == 0)
        {
            res = i;
            break;
        }
    }

    return res;
}

// Resolves object name to ID
UNUM_DEF size Unum_Internal_Execute_Objectid(UnumInstance* c, str typename)
{
    // Lookup object name
    // Todo - Optimize by using a hash table
    size res = 0;
    for(size i = 0; i < c->stack.object_count; i++)
    {
        if(strcmp(typename, c->stack.objects[i].name) == 0)
        {
            res = i;
            break;
        }
    }

    return res;
}

UNUM_DEF UnumResult Unum_Internal_Keyword_Function(UnumInstance* c, UnumInternalPairList params)
{
    if(params.num == 0)
        // Error - Not enough arguments (need function name)
        return UNUM_INTERNAL_RESULT("Not enough arguments (need function name)", 0, UNUM_CODE_SYNTAX_ERROR);
    if(params.num > 1)
        // Error - Too many arguments (only need function name)
        return UNUM_INTERNAL_RESULT("Too many arguments (only need function name)", 0, UNUM_CODE_SYNTAX_ERROR);
    if(params.pairs[0].a != params.pairs[0].b)
        // Error - Argument is not a valid symbol
        return UNUM_INTERNAL_RESULT("Argument is not a valid symbol", 0, UNUM_CODE_SYNTAX_ERROR);

    str name = c->program.tokens[params.pairs[0].a].value;

    // Verify symbol name doesn't already exist
    for(size u = 0; u < c->stack.object_count; u++)
    {
        if(strcmp(c->stack.objects[u].name, name) == 0)
            // Error - Duplicate symbol name (object)
            return UNUM_INTERNAL_RESULT("Duplicate symbol name (object)", 0, UNUM_CODE_RUNTIME_ERROR);
    }
    for(size u = 0; u < c->stack.function_count; u++)
    {
        if(strcmp(c->stack.functions[u].name, name) == 0)
            // Error - Duplicate symbol name (function)
            return UNUM_INTERNAL_RESULT("Duplicate symbol name (function)", 0, UNUM_CODE_RUNTIME_ERROR);
    }

    UnumInternalFunction f;
    f.name = Unum_Internal_Utility_Strdup(name);
    f.data = NULL;
    f.result = (UnumInternalType) {.name = NULL, .parts = NULL};
    f.parameters = (UnumInternalType) {.name = NULL, .parts = NULL};

    // Resize array and add function
    c->stack.function_count++;
    c->stack.functions = UNUM_REALLOC(c->stack.functions, sizeof(UnumInternalFunction) * c->stack.function_count);
    c->stack.functions[c->stack.function_count - 1] = f;

    unum_log("New Function \"%s\"", f.name);
    return UNUM_INTERNAL_RESULT_OK;
}

UNUM_DEF UnumResult Unum_Internal_Keyword_Parameters(UnumInstance* c, UnumInternalPairList params)
{
    UnumInternalType p;
    p.name = NULL;
    p.parts_count = 0;
    p.parts = NULL;

    if(params.num > 0)
    {
        p.parts_count = params.num;
        p.parts = UNUM_MALLOC(sizeof(UnumInternalTypePair) * params.num);

        for(size u = 0; u < params.num; u++)
        {
            // unum_log("%d, %d", params[u].b, params[u].a);
            if(strcmp(c->program.tokens[params.pairs[u].a].value, "{") == 0
                    && params.pairs[u].b - params.pairs[u].a == 4) // Must include 5 tokens
            {
                size tok_symbol = params.pairs[u].a + 1;
                size tok_itemset = params.pairs[u].a + 2;
                size tok_typename = params.pairs[u].a + 3; // Require one-symbol typename, possible allow for expressions in the future

                // Validate input
                if(c->program.tokens[tok_symbol].type != UNUM_LANG_SYMBOL)
                    return UNUM_INTERNAL_RESULT("@parameters argument set expected symbol, received other type", 0, UNUM_CODE_SYNTAX_ERROR);
                if(c->program.tokens[tok_itemset].type != UNUM_LANG_OPERATOR)
                    return UNUM_INTERNAL_RESULT("@parameters argument set expected operator, received other type", 0, UNUM_CODE_SYNTAX_ERROR);
                if(strcmp(c->program.tokens[tok_itemset].value, ":") != 0)
                    return UNUM_INTERNAL_RESULT("@parameters argument set expected set equals operator (\":\"), received other operator", 0, UNUM_CODE_SYNTAX_ERROR);
                if(c->program.tokens[tok_typename].type != UNUM_LANG_SYMBOL)
                    return UNUM_INTERNAL_RESULT("@parameters argument set expected symbol, received other type", 0, UNUM_CODE_SYNTAX_ERROR);

                // Lookup typename
                size param_type_id = Unum_Internal_Execute_Typeid(c, c->program.tokens[tok_typename].value);

                // Todo - Compare with other parameter symbol names to ensure there are no duplicates

                // Verify types exist
                if(param_type_id == UNUM_PRIMITIVE_NULL)
                    return UNUM_INTERNAL_RESULT("@parameters argument set has unknown type", 0, UNUM_CODE_RUNTIME_ERROR);

                // Add to hidden type
                p.parts[u] = (UnumInternalTypePair) {.name = Unum_Internal_Utility_Strdup(c->program.tokens[tok_symbol].value), .type_id = param_type_id};

            } else {
                // Error - Bad syntax
                return UNUM_INTERNAL_RESULT("@parameters expects arguments of the format { symbol: typename }", 0, UNUM_CODE_SYNTAX_ERROR);
            }
        }

        // Type is function-specific, so don't add to global table
        /*
        // Add type to type table
        c->stack.types[c->stack.type_count] = p;
        c->stack.type_count++;
        */

        // Set properties of incoming function
        c->stack.functions[c->stack.function_count - 1].parameters = p;

        unum_log("New Parameters (%u)", p.parts_count);
        for(size v = 0; v < p.parts_count; v++)
        {
            unum_log("\"%s\" of type ID #%u (\"%s\")", p.parts[v].name, p.parts[v].type_id, c->stack.types[p.parts[v].type_id].name);
        }
        return UNUM_INTERNAL_RESULT_OK;
    } else {
        // Todo - Return empty parameter object
        unum_log("No Parameters");
        return UNUM_INTERNAL_RESULT_OK;
    }
}

UNUM_DEF UnumResult Unum_Internal_Keyword_Result(UnumInstance* c, UnumInternalPairList params)
{
    if(params.num == 0)
    {
        // Todo - Return empty result object
        unum_log("No result");
        return UNUM_INTERNAL_RESULT_OK;
    } else if (params.num == 1) {
        if(params.pairs[0].a != params.pairs[0].b)
            return UNUM_INTERNAL_RESULT("@return has invalid arguments", 0, UNUM_CODE_SYNTAX_ERROR);
        if(c->program.tokens[params.pairs[0].a].type != UNUM_LANG_SYMBOL)
            return UNUM_INTERNAL_RESULT("@return expected a symbol, got an other type", 0, UNUM_CODE_SYNTAX_ERROR);

        UnumInternalType val;
        val.name = NULL;
        val.parts_count = 1;
        val.parts = UNUM_MALLOC(sizeof(UnumInternalTypePair) * 1);

        // Lookup typename
        size param_type_id = Unum_Internal_Execute_Typeid(c, c->program.tokens[params.pairs[0].a].value);

        val.parts[0].name = NULL;
        val.parts[0].type_id = param_type_id;

        // Set properties of incoming function
        c->stack.functions[c->stack.function_count - 1].result = val;

        unum_log("New result with type ID #%u (\"%s\")", param_type_id, c->stack.types[param_type_id].name);
        return UNUM_INTERNAL_RESULT_OK;
    } else {
        return UNUM_INTERNAL_RESULT("Too many return values (only 1 is allowed)", 0, UNUM_CODE_SYNTAX_ERROR);
    }
}

UNUM_DEF UnumResult Unum_Internal_Keyword_Structure(UnumInstance* c, UnumInternalPairList params)
{
    return UNUM_INTERNAL_RESULT_OK;
}

UNUM_DEF UnumResult Unum_Internal_Keyword_Body(UnumInstance* c, UnumInternalPairList params)
{
    return UNUM_INTERNAL_RESULT_OK;
}

/****************************************************************
 * Language execution
 ****************************************************************/

typedef struct UnumInternalKeyword
{
    str name;
    UnumResult (*func)(UnumInstance*, UnumInternalPairList);
} UnumInternalKeyword;

UNUM_DEF UnumInternalKeyword UNUM_KEYWORDS[] =
{
    [UNUM_KEYWORD_FUNCTION] = {.name = "function", .func = Unum_Internal_Keyword_Function},
    [UNUM_KEYWORD_STRUCTURE] = {.name = "structure", .func = Unum_Internal_Keyword_Structure},
    [UNUM_KEYWORD_PARAMETERS] = {.name = "parameters", .func = Unum_Internal_Keyword_Parameters},
    [UNUM_KEYWORD_RESULT] = {.name = "result", .func = Unum_Internal_Keyword_Result},
    [UNUM_KEYWORD_BODY] = {.name = "body", .func = Unum_Internal_Keyword_Body},
    [UNUM_KEYWORD_RETURN] = {.name = "return", .func = Unum_Internal_Keyword_Function}
};

UNUM_DEF const UnumInternalType UNUM_TYPE_PRIMITIVES[] =
{
    [UNUM_PRIMITIVE_NULL] = { .name = "null", .parts = NULL },
    [UNUM_PRIMITIVE_PARAMETERS] = { .name = "parameters", .parts = NULL },
    [UNUM_PRIMITIVE_ARRAY] = { .name = "array", .parts = NULL },
    [UNUM_PRIMITIVE_I8] = { .name = "i8", .parts = NULL },
    [UNUM_PRIMITIVE_I16] = { .name = "i16", .parts = NULL },
    [UNUM_PRIMITIVE_I32] = { .name = "i32", .parts = NULL },
    [UNUM_PRIMITIVE_I64] = { .name = "i64", .parts = NULL },
    [UNUM_PRIMITIVE_U8] = { .name = "u8", .parts = NULL },
    [UNUM_PRIMITIVE_U16] = { .name = "u16", .parts = NULL },
    [UNUM_PRIMITIVE_U32] = { .name = "u32", .parts = NULL },
    [UNUM_PRIMITIVE_U64] = { .name = "u64", .parts = NULL },
    [UNUM_PRIMITIVE_F32] = { .name = "f32", .parts = NULL },
    [UNUM_PRIMITIVE_F64] = { .name = "f64", .parts = NULL },
    [UNUM_PRIMITIVE_BOOL] = { .name = "bool", .parts = NULL }
};

// Todo - Finish
// Return location of error on problem, otherwise return 0
UNUM_DEF UnumResult Unum_Internal_Execute(UnumInstance* c, UnumInternalPair p)
{
    /*
    unum_log("Code:");
    unum_log("\"%s\"", c->program.code);
    unum_log("Tokens:");
    for(size tmp = 0; tmp < c->program.count; tmp++)
    {
        unum_log("\t%s", c->program.tokens[tmp].value);
    }
    */

    UnumInternalPairList top = Unum_Internal_Parse_Separate(c->program, (UnumInternalPair){.a = p.a, .b = p.a}, ";"); // Unum_Internal_Parse_Chain(c->program, last, &out_num);
    Unum_Debug_Print_Pair(c->program, top);

    // Verify pairs
    if(top.pairs == NULL)
    {
        unum_log("Invalid chain");
        return UNUM_INTERNAL_RESULT("Bad syntax", 0, UNUM_CODE_SYNTAX_ERROR);
    } else if(top.num == 0 && top.pairs == SAFE){
        unum_log("No statement chain");
        return UNUM_INTERNAL_RESULT_OK;
    }

    // Iterate over expressions
    for(size i = 0; i < top.num; i++)
    {
        UnumInternalPairList expressions = Unum_Internal_Parse_Separate(c->program, top.pairs[i], "->");

        // Verify pairs
        if(expressions.num == 0 || expressions.pairs == NULL)
        {
            UNUM_FREE(top.pairs);
            UNUM_FREE(expressions.pairs);
            return UNUM_INTERNAL_RESULT("Bad syntax", 0, UNUM_CODE_SYNTAX_ERROR);
        }

        // Iterate over statements
        for(size u = 0; u < expressions.num; u++)
        {
            // First token should be function/keyword name of current statement
            size current = expressions.pairs[u].a;
            UnumInternalToken tok = c->program.tokens[current];

            // Get arguments

            // Calculate range
            UnumInternalPair arg_range;
            // Starts with function name
            arg_range.a = expressions.pairs[u].a;
            // End with pass operator/end operator
            arg_range.b = u == expressions.num - 1 ? expressions.pairs[u].b : expressions.pairs[u + 1].a - 1;

            // Parse arguments
            UnumInternalPairList args = Unum_Internal_Parse_Separate(c->program, arg_range, ",");

            // Verify valid arguments
            if(args.pairs == NULL)
            {
                UNUM_FREE(top.pairs);
                UNUM_FREE(expressions.pairs);
                UNUM_FREE(args.pairs);
                return UNUM_INTERNAL_RESULT("Invalid syntax", current, UNUM_CODE_SYNTAX_ERROR);
            }

            // Try keyword matching
            if(tok.type == UNUM_LANG_KEYWORD)
            {
                // Todo - Optimize with hash table
                // Remember, the keyword at index 0 is reserved
                size id = 0;
                for(size v = 0; v < sizeof(UNUM_KEYWORDS) / sizeof(UnumInternalKeyword); v++)
                {
                    if(strcmp(UNUM_KEYWORDS[v].name, tok.value) == 0)
                    {
                        id = v;
                        break;
                    }
                }

                // Verify keyword exists
                if(id == 0)
                {
                    UNUM_FREE(top.pairs);
                    UNUM_FREE(expressions.pairs);
                    UNUM_FREE(args.pairs);
                    return UNUM_INTERNAL_RESULT("Unknown/non-existent keyword", current, UNUM_CODE_RUNTIME_ERROR);
                }

                // Call keyword

                UnumResult rs = UNUM_KEYWORDS[id].func(c, args);

                if(rs.code != UNUM_CODE_NEUTRAL)
                {
                    UNUM_FREE(top.pairs);
                    UNUM_FREE(expressions.pairs);
                    UNUM_FREE(args.pairs);
                    return rs;
                }

                // Try function/object matching
            } else if(tok.type == UNUM_LANG_SYMBOL) {
                // Todo - Match with object name as well (to warn for incorrect names)
                // Remember, functions[0] and objects[0] are supposed to be null
                size id = Unum_Internal_Execute_Functionid(c, tok.value);
                // Verify function exists
                if(id == 0)
                {
                    UNUM_FREE(top.pairs);
                    UNUM_FREE(expressions.pairs);
                    UNUM_FREE(args.pairs);
                    return UNUM_INTERNAL_RESULT("Unknown/non-existent function name", current, UNUM_CODE_RUNTIME_ERROR);
                }

                // Todo - Call function

                // Bad syntax
            } else {
                UNUM_FREE(top.pairs);
                UNUM_FREE(expressions.pairs);
                UNUM_FREE(args.pairs);
                return UNUM_INTERNAL_RESULT("Expected symbol/keyword for function", current, UNUM_CODE_SYNTAX_ERROR);
            }
        }

        UNUM_FREE(expressions.pairs);

        // END

        /*
        // Iterate over statements
        for(size i = 0; i < top.num; i++)
        {
            // First token should be function/keyword name
            size current = top.pairs[i].a;
            str fname = c->program.tokens[current].value;
            // Check if keyword
            if(c->program.tokens[current].type == UNUM_LANG_KEYWORD)
            {
                // Check which keyword

                // Function
                if(strstr(fname, UNUM_KEYWORDS[UNUM_KEYWORD_FUNCTION].name) != NULL)
                {
                    size num;
                    UnumInternalPair* params = Unum_Internal_Parse_Parameters(c->program, current + 1, &num);
                    UnumResult u = Unum_Internal_Keyword_Function(c, params, current);
                    if(u.code != UNUM_CODE_NEUTRAL)
                        return u;

                    // Parameters
                } else if(strstr(fname, UNUM_KEYWORDS[UNUM_KEYWORD_PARAMETERS].name) != NULL)
                {
                    // Ensure call is after a pass
                    if(i == 0)
                        return UNUM_INTERNAL_RESULT("@parameters keyword must always follow a pass operator", current, UNUM_CODE_SYNTAX_ERROR);

                    size num;
                    UnumInternalPair* params = Unum_Internal_Parse_Parameters(c->program, current + 1, &num);
                    UnumResult u = Unum_Internal_Keyword_Parameters(c, params, current);
                    if(u.code != UNUM_CODE_NEUTRAL)
                        return u;

                    // Result
                } else if(strstr(fname, UNUM_KEYWORDS[UNUM_KEYWORD_RESULT].name) != NULL)
                {
                    // Ensure call is after a pass
                    if(i == 0)
                        return UNUM_INTERNAL_RESULT("@result keyword must always follow a pass operator", current, UNUM_CODE_SYNTAX_ERROR);

                    size num;
                    UnumInternalPair* params = Unum_Internal_Parse_Parameters(c->program, current + 1, &num);
                    UnumResult u = Unum_Internal_Keyword_Result(c, params, current);
                    if(u.code != UNUM_CODE_NEUTRAL)
                        return u;

                    // Body
                } else if(strstr(fname, UNUM_KEYWORDS[UNUM_KEYWORD_BODY].name) != NULL)
                {
                    // Ensure call is after a pass
                    if(i == 0)
                        return UNUM_INTERNAL_RESULT("@body keyword must always follow a pass operator", current, UNUM_CODE_SYNTAX_ERROR);

                    // Unknown keyword
                } else {
                    return UNUM_INTERNAL_RESULT("Unknown keyword", current, UNUM_CODE_SYNTAX_ERROR);
                }

                // Check if function
            } else if(c->program.tokens[current].type == UNUM_LANG_SYMBOL) {
                // Try function matching
                // Todo - Optimize with hash table
                // Remember, functions[0] and objects[0] are supposed to be null
                size id = 0;
                for(size u = 0; u < c->stack.function_count; u++)
                {
                    if(strcmp(c->stack.functions[u].name, fname) == 0)
                    {
                        id = u;
                        break;
                    }
                }

                // Verify function exists
                if(id == 0)
                    return UNUM_INTERNAL_RESULT("Unknown/non-existent function name", current, UNUM_CODE_RUNTIME_ERROR);

                // Todo - Call function

                // No matches, return error
            } else {
                UNUM_FREE(top);
                return UNUM_INTERNAL_RESULT("Expected symbol/keyword for function", current, UNUM_CODE_SYNTAX_ERROR);
            }
        }
            */
    }

    UNUM_FREE(top.pairs);

    return UNUM_INTERNAL_RESULT_OK;
}

/****************************************************************
 * Front end
 ****************************************************************/

// Unum standard libraries
#include "libraries/UMath.h"

UNUM_DEF UnumResult Unum_Execute(UnumInstance* c, str code)
{
    c->program = Unum_Internal_Tokenize(code);
    return Unum_Internal_Execute(c, (UnumInternalPair){.a = 0, .b = c->program.count});
}

UNUM_DEF UnumInstance* Unum_Initialize(void)
{
    p_libsys_init();

    UnumInstance* res = UNUM_MALLOC(sizeof(UnumInstance));

    res->stack.type_count = sizeof(UNUM_TYPE_PRIMITIVES) / sizeof(UnumInternalType);
    res->stack.types = UNUM_MALLOC(sizeof(UNUM_TYPE_PRIMITIVES));

    memcpy(res->stack.types, UNUM_TYPE_PRIMITIVES, sizeof(UNUM_TYPE_PRIMITIVES));

    // First object is reserved
    res->stack.object_count = 1;
    res->stack.objects = UNUM_MALLOC(sizeof(UnumInternalObject) * 1);

    // First function is reserved
    res->stack.function_count = 1;
    res->stack.functions = UNUM_MALLOC(sizeof(UnumInternalFunction) * 1);

    // Load standard libraries
    // Unum_Execute(res, UNUM_LIBRARY_MATH);

    return res;
}

UNUM_DEF void Unum_Destroy(UnumInstance* c)
{
    for(size i = 0; i < c->stack.object_count; i++)
    {
        UNUM_FREE(c->stack.types[i].name);
        UNUM_FREE(c->stack.types[i].parts);
    }

    for(size i = 0; i < c->stack.object_count; i++)
    {
        UNUM_FREE(c->stack.objects[i].name);
        UNUM_FREE(c->stack.objects[i].data);
    }

    for(size i = 0; i < c->stack.function_count; i++)
    {
        UNUM_FREE(c->stack.functions[i].name);
        UNUM_FREE(c->stack.functions[i].data);
    }

    UNUM_FREE(c->stack.types);
    UNUM_FREE(c->stack.objects);
    UNUM_FREE(c->stack.functions);

    UNUM_FREE(c->program.code);
    UNUM_FREE(c->program.statement);
    UNUM_FREE(c->program.tokens);

    UNUM_FREE(c);

    p_libsys_shutdown();
}

#ifdef __cplusplus
}
#endif // C++ guard

#endif // UNUM_INCLUDE
