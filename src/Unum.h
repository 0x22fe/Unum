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
#define unum_log(...) { printf("[Unum][%u ms]: ", unum_time() / 1000U); printf(__VA_ARGS__); printf("\n"); unum_flush(); }
#else
#define unum_log(...) { printf("[Unum][%u ms]: ", unum_time() / 1000U); printf(__VA_ARGS__); printf("\n"); }
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

typedef enum UnumInternalObjectType
{
    UNUM_OBJ_BLANK = 0,
    UNUM_OBJ_TYPE, UNUM_OBJ_STACK, UNUM_OBJ_SYMBOL,
    UNUM_OBJ_SINGLE, UNUM_OBJ_ARRAY, UNUM_OBJ_SET, UNUM_OBJ_EXPRESSION,
    UNUM_OBJ_FUNCTION, UNUM_OBJ_STRUCTURE, UNUM_OBJ_NAMESPACE
} UnumInternalObjectType;

typedef struct UnumInternalObject
{
    str name;
    any data;
    UnumInternalObjectType type;
} UnumInternalObject;

typedef struct UnumInternalObjType
{
    str type;
    size count;
    struct UnumInternalObjType* parts;
} UnumInternalObjType;

typedef struct UnumInternalObjSymbol
{
    str name;
} UnumInternalObjSymbol;

typedef struct UnumInternalObjSingle
{
    any data;
    UnumInternalObjType* type; // Pointer as type is not object specific
} UnumInternalObjSingle;

typedef struct UnumInternalObjArray
{
    any data;
    UnumInternalObjType* type; // Pointer as type is not object specific
} UnumInternalObjArray;

typedef struct UnumInternalObjSet
{
    size count;
    str* keys;
    UnumInternalObject* values;
} UnumInternalObjSet;

typedef struct UnumInternalObjExpression
{
    UnumInternalPair pair;
} UnumInternalObjExpression;

typedef struct UnumInternalObjNamespace
{
    UnumInternalObject* values;
} UnumInternalObjNamespace;

typedef struct UnumInternalObjFunction
{
    UnumInternalObjExpression* body;
    UnumInternalObjSet parameters;
    UnumInternalObjType result; // May switch to set to allow multiple return types in the future
} UnumInternalObjFunction;

typedef struct UnumInternalObjStructure
{
    UnumInternalObjType body;
    UnumInternalObjFunction* functions;
} UnumInternalObjStructure;

typedef struct UnumInternalStack
{
    size count;
    UnumInternalObject* objects;
} UnumInternalStack;

typedef struct UnumInternalNative
{
    str name;
    any function;
    UnumInternalObject** params;
    UnumInternalObject** result; // Array as multiple return types may be allowed in the future
} UnumInternalNative;

typedef enum UnumInternalLang
{
    UNUM_LANG_BLANK = 0,
    UNUM_LANG_KEYWORD, UNUM_LANG_SYMBOL, UNUM_LANG_CHAR, UNUM_LANG_STRING, UNUM_LANG_NUMBER, UNUM_LANG_OPERATOR
} UnumInternalLang;

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
    UnumInternalNative* native;
} UnumInstance;

/****************************************************************
 * Constants
 ****************************************************************/

typedef enum UnumStatus
{
    UNUM_STATUS_OK = 0,
    UNUM_STATUS_DEBUG,
    UNUM_STATUS_WARNING, UNUM_STATUS_ERROR
} UnumStatus;

UNUM_DEF const str UNUM_STATUSES[] =
{
    [UNUM_STATUS_OK] = "Ok",
    [UNUM_STATUS_DEBUG] = "Debug",
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
    UNUM_KEYWORD_BLANK = 0,
    UNUM_KEYWORD_NAMESPACE, UNUM_KEYWORD_FUNCTION, UNUM_KEYWORD_STRUCTURE, UNUM_KEYWORD_PARAMETERS,
    UNUM_KEYWORD_RESULT, UNUM_KEYWORD_BODY, UNUM_KEYWORD_RETURN
} UnumInternalKeywords;

typedef enum UnumInternalPrimitives
{
    UNUM_PRIMITIVE_NULL = 0,
    UNUM_PRIMITIVE_I8, UNUM_PRIMITIVE_I16, UNUM_PRIMITIVE_I32, UNUM_PRIMITIVE_I64,
    UNUM_PRIMITIVE_U8, UNUM_PRIMITIVE_U16, UNUM_PRIMITIVE_U32, UNUM_PRIMITIVE_U64,
    UNUM_PRIMITIVE_F32, UNUM_PRIMITIVE_F64,
    UNUM_PRIMITIVE_BOOL
} UnumInternalPrimitives;

typedef enum UnumInternalOperators
{
    UNUM_OPERATOR_COMMENT_SINGLE = 0, UNUM_OPERATOR_COMMENT_MULTIPLE,
    UNUM_OPERATOR_COMPILER,
    UNUM_OPERATOR_CHAR_START, UNUM_OPERATOR_CHAR_END,
    UNUM_OPERATOR_STRING_START, UNUM_OPERATOR_STRING_END,
    UNUM_OPERATOR_MEMBER,
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
    [UNUM_OPERATOR_CHAR_START] = "'",
    [UNUM_OPERATOR_CHAR_END] = "'",
    [UNUM_OPERATOR_STRING_START] = "\"",
    [UNUM_OPERATOR_STRING_END] = "\"",
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

UNUM_DEF UnumInternalTokens Unum_Internal_Utility_Tokensub(UnumInternalTokens s, UnumInternalPair range)
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

UNUM_DEF str Unum_Internal_Parse_Char(str start)
{
    size i = 1;

    while(Unum_Internal_Utility_Txtvalid(start[i]))
    {
        if(start[i] == '\'' && start[i - 1] != '\\') break;
        i++;
    }

    i++;

    return start + (i * sizeof(txt));
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

        if(result.code[i] == '\'')
        {
            // Char
            start = result.code + (i * sizeof(txt));
            end = Unum_Internal_Parse_Char(start);
            val = UNUM_LANG_CHAR;
        } else if(result.code[i] == '"')
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
        } else if(isdigit(result.code[i]) || (result.code[i] == '.' && isdigit(result.code[i + 1]))
                  || (result.code[i] == '-' && ((result.code[i + 1] == '.' && isdigit(result.code[i + 2]))
                                                || isdigit(result.code[i + 1])))) {
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

UNUM_DEF void Unum_Debug_Pair(UnumInternalTokens s, UnumInternalPairList p)
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

// Refer to https://stackoverflow.com/questions/3419332/c-preprocessor-stringify-the-result-of-a-macro
#define UNUM_QUOTE(str) #str
#define UNUM_EXPAND_AND_QUOTE(str) UNUM_QUOTE(str)
#define UNUM_INTERNAL_RESULT(msg, loc, c) (UnumResult){.location = (loc), .code = (c), .message = Unum_Internal_Utility_Stradd(UNUM_CODES[(c)].message, " (Interpreter line " UNUM_EXPAND_AND_QUOTE(__LINE__) "): " msg), .result = -((i32)(UNUM_CODES[(c)].level)) }
#define UNUM_INTERNAL_RESULT_OK (UnumResult){.location = 0, .code = (UNUM_CODE_NEUTRAL), .message = Unum_Internal_Utility_Stradd(UNUM_CODES[(UNUM_CODE_NEUTRAL)].message, " (Interpreter line " UNUM_EXPAND_AND_QUOTE(__LINE__) "): " "Ok"), .result = -((i32)(UNUM_CODES[(UNUM_CODE_NEUTRAL)].level)) }

UNUM_DEF const UnumInternalObject UNUM_OBJ_DEF = (UnumInternalObject) {.name = NULL, .data = NULL, .type = UNUM_OBJ_BLANK};
UNUM_DEF const UnumInternalObject UNUM_OBJ_SAFE = (UnumInternalObject) {.name = SAFE, .data = SAFE, .type = UNUM_OBJ_BLANK};

UNUM_DEF bool Unum_Internal_Execute_Safe(UnumInternalObject o)
{
    if(o.name == SAFE && o.data == SAFE && o.type == UNUM_OBJ_BLANK)
        return UNUM_TRUE;
    else
        return UNUM_FALSE;
}

UNUM_DEF bool Unum_Internal_Execute_Null(UnumInternalObject o)
{
    if(o.name == NULL && o.data == NULL && o.type == UNUM_OBJ_BLANK)
        return UNUM_TRUE;
    else
        return UNUM_FALSE;
}

// Resolves object name to stack level and ID
UNUM_DEF UnumInternalPair Unum_Internal_Execute_Id(UnumInstance* c, str name)
{
    // Lookup object name in all stack levels
    // Todo - Optimize by using a hash table

    UnumInternalPair res;
    res.a = 0; // Stack level
    res.b = 0; // Object ID

    UnumInternalStack* last = &c->stack;
    while(last->objects != NULL)
    {
        for(size i = 1; i < last->count; i++)
        {
            if(strcmp(name, last->objects[i].name) == 0)
            {
                res.b = i;
                break;
            }
        }

        if(res.b != 0)
            break;

        res.a++;
        last = (UnumInternalStack*)(&last->objects[0]);
    }

    return res;
}

// Returns highest stack level
UNUM_DEF size Unum_Internal_Execute_Stacklevel(UnumInstance* c)
{
    size level = 0;
    UnumInternalStack* last = &c->stack;
    while(last->objects != NULL)
    {
        last = (UnumInternalStack*)(&last->objects[0]);
        level++;
    }
    return level;
}

// Returns pointer to highest level stack
UNUM_DEF UnumInternalStack* Unum_Internal_Execute_Stack(UnumInstance* c)
{
    UnumInternalStack* last = &c->stack;
    while(last->objects != NULL)
        last = (UnumInternalStack*)(&last->objects[0]);
    return last;
}

// Returns pointer to given level stack
UNUM_DEF UnumInternalStack* Unum_Internal_Execute_Level(UnumInstance* c, size lvl)
{
    size i = 0;
    UnumInternalStack* last = &c->stack;
    while(last->objects != NULL && i < lvl)
    {
        i++;
        last = (UnumInternalStack*)(&last->objects[0]);
    }
    return last;
}

UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Function(UnumInstance* c, UnumInternalStack* params)
{
    if(params->count == 0)
        return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("Not enough arguments (need function name)", 0, UNUM_CODE_SYNTAX_ERROR);
    if(params->count > 1)
        return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("Too many arguments (only need function name)", 0, UNUM_CODE_SYNTAX_ERROR);
    if(params->objects[0].type != UNUM_OBJ_SYMBOL)
        return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("Argument is not a symbol", 0, UNUM_CODE_SYNTAX_ERROR);

    // Verify symbol name doesn't already exist
    str name = ((UnumInternalObjSymbol*)params->objects[0].data)->name;
    UnumInternalPair id_res = Unum_Internal_Execute_Id(c, name);

    if(id_res.b != 0)
        return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("Duplicate symbol name (object)", 0, UNUM_CODE_RUNTIME_ERROR);

    // Create function object

    UnumInternalObjFunction* f = UNUM_MALLOC(sizeof(UnumInternalObjFunction) * 1);
    f->body = NULL;
    f->result = (UnumInternalObjType) { .type = UNUM_PRIMITIVE_NULL, .count = 0, .parts = NULL};
    f->parameters = (UnumInternalObjSet) { .count = 0, .keys = NULL, .values = NULL};

    UnumInternalObject o;
    o.data = f;
    o.name = Unum_Internal_Utility_Strdup(name);
    o.type = UNUM_OBJ_FUNCTION;

    unum_log("New Function \"%s\"", o.name);
    return o; // UNUM_INTERNAL_RESULT_OK;
}

UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Parameters(UnumInstance* c, UnumInternalStack* params)
{
    // Remember, 0 is reserved in the stack
    if(params->count < 2 || params->count > 3)
        return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("@parameters needs to be called after a @function pass", 0, UNUM_CODE_RUNTIME_ERROR);
    if(params->objects[1].type != UNUM_OBJ_FUNCTION)
        return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("@parameters called with pass not from @function", 0, UNUM_CODE_RUNTIME_ERROR);

    // No argument function
    if(params->count == 2)
    {
        // Maybe Todo - Return empty parameter object
        unum_log("No Parameters");
        return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT_OK;
    }

    // Function with argument(s)
    if(params->objects[2].type != UNUM_OBJ_SET)
        return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("@parameters expected a set as a second argument", 0, UNUM_CODE_SYNTAX_ERROR);

    // Add parameters to function object
    UnumInternalObjSet* args = (UnumInternalObjSet*)params->objects[1].data;
    ((UnumInternalObjFunction*)params->objects[1].data)->parameters = *args;

    // Verify set is in correct format
    for(size u = 0; u < args->count; u++)
    {
        // Get set properties
        str current_name = args->keys[u];
        UnumInternalObject current_obj = args->values[u];

        // Verify object is of "type" type
        if(current_obj.type != UNUM_OBJ_TYPE)
            return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("@parameters argument set expected type name", 0, UNUM_CODE_SYNTAX_ERROR);

        // Lookup typename
        // Todo - Support more than the "Single" type
        UnumInternalPair param_type_id = Unum_Internal_Execute_Id(c, ((UnumInternalObjType*)current_obj.data)->type);

        // Verify type exists
        if(param_type_id.b == 0)
            return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("@parameters argument set has unknown type", 0, UNUM_CODE_RUNTIME_ERROR);

        // Compare with other parameter symbol names to ensure there are no duplicates
        for(size v = 0; v < u; v++)
        {
            if(strcmp(current_name, args->keys[v]))
            {
                return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("@parameters argument set has duplicate key", 0, UNUM_CODE_SYNTAX_ERROR);
            }
        }
    }

    unum_log("New Parameters (%u)", args->count);

    /*
        for(size v = 0; v < p.count; v++)
        {
            unum_log("\"%s\" of type ID #%u (\"%s\")", p.parts[v].name, p.parts[v].type_id, c->stack.types[p.parts[v].type_id].name);
        }
    */

    // Return function object
    return params->objects[1];
}

UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Result(UnumInstance* c, UnumInternalStack* params)
{
    // Remember, 0 is reserved in the stack
    if(params->count < 2)
        return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("@result needs to be called after a @function pass", 0, UNUM_CODE_RUNTIME_ERROR);
    if(params->objects[1].type != UNUM_OBJ_FUNCTION)
        return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("@result called with pass not from @function", 0, UNUM_CODE_RUNTIME_ERROR);

    // No return value
    if(params->count == 2)
    {
        // Todo - Return empty result object
        unum_log("No result");
        return params->objects[1];
        // Has a return value
    } else if (params->count == 3) {
        // Todo - Allow returning custom set instead of a single type
        if(params->objects[2].type != UNUM_OBJ_SYMBOL)
            return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("@result expected a symbol, got an other type", 0, UNUM_CODE_SYNTAX_ERROR);

        // Lookup typename
        UnumInternalPair param_type_id = Unum_Internal_Execute_Id(c, ((UnumInternalObjSymbol*)params->objects[2].data)->name);

        // Verify type exists
        if(param_type_id.b == 0)
            return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("@result argument has unknown type", 0, UNUM_CODE_RUNTIME_ERROR);

        // Lookup symbol name in global stack
        UnumInternalObject* ob = &Unum_Internal_Execute_Level(c, param_type_id.a)->objects[param_type_id.b];

        // Verify symbol name is a type
        if(ob->type != UNUM_OBJ_TYPE)
            return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("@result argument expected a type name", 0, UNUM_CODE_RUNTIME_ERROR);

        // Create type (one value for now, possibly multiple in the future via a set)
        UnumInternalObjType s = *((UnumInternalObjType*)(ob->data));

        // Add result type to function object
        ((UnumInternalObjFunction*)params->objects[1].data)->result = s;

        unum_log("New result with stack level #%u, type ID #%u (\"%s\")", param_type_id.a, param_type_id.b, ((UnumInternalObjSymbol*)params->objects[2].data)->name);

        // Return function object
        return params->objects[1];
    } else {
        // Too many return values, error
        return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("@result has too many return values (only 1 is allowed)", 0, UNUM_CODE_SYNTAX_ERROR);
    }
}

UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Namespace(UnumInstance* c, UnumInternalStack* params)
{
    return UNUM_OBJ_DEF;
}

UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Structure(UnumInstance* c, UnumInternalStack* params)
{
    return UNUM_OBJ_DEF;
}

UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Body(UnumInstance* c, UnumInternalStack* params)
{
    return UNUM_OBJ_DEF;
}

UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Return(UnumInstance* c, UnumInternalStack* params)
{
    return UNUM_OBJ_DEF;
}

/****************************************************************
 * Language execution
 ****************************************************************/

typedef struct UnumInternalKeyword
{
    str name;
    UnumInternalObject (*func)(UnumInstance*, UnumInternalStack*);
} UnumInternalKeyword;

UNUM_DEF UnumInternalKeyword UNUM_KEYWORDS[] =
{
    [UNUM_KEYWORD_BLANK] = {.name = "", .func = NULL},
    [UNUM_KEYWORD_NAMESPACE] = {.name = "namespace", .func = Unum_Internal_Keyword_Namespace},
    [UNUM_KEYWORD_FUNCTION] = {.name = "function", .func = Unum_Internal_Keyword_Function},
    [UNUM_KEYWORD_STRUCTURE] = {.name = "structure", .func = Unum_Internal_Keyword_Structure},
    [UNUM_KEYWORD_PARAMETERS] = {.name = "parameters", .func = Unum_Internal_Keyword_Parameters},
    [UNUM_KEYWORD_RESULT] = {.name = "result", .func = Unum_Internal_Keyword_Result},
    [UNUM_KEYWORD_BODY] = {.name = "body", .func = Unum_Internal_Keyword_Body},
    [UNUM_KEYWORD_RETURN] = {.name = "return", .func = Unum_Internal_Keyword_Return}
};

// Treat as if constant
UNUM_DEF UnumInternalObjType UNUM_PRIMITIVES[] =
{
    [UNUM_PRIMITIVE_NULL] = { .type = "null", .count = 0, .parts = NULL },
    [UNUM_PRIMITIVE_I8] = { .type = "i8", .count = 0, .parts = NULL },
    [UNUM_PRIMITIVE_I16] = { .type = "i16", .count = 0, .parts = NULL },
    [UNUM_PRIMITIVE_I32] = { .type = "i32", .count = 0, .parts = NULL },
    [UNUM_PRIMITIVE_I64] = { .type = "i64", .count = 0, .parts = NULL },
    [UNUM_PRIMITIVE_U8] = { .type = "u8", .count = 0, .parts = NULL },
    [UNUM_PRIMITIVE_U16] = { .type = "u16", .count = 0, .parts = NULL },
    [UNUM_PRIMITIVE_U32] = { .type = "u32", .count = 0, .parts = NULL },
    [UNUM_PRIMITIVE_U64] = { .type = "u64", .count = 0, .parts = NULL },
    [UNUM_PRIMITIVE_F32] = { .type = "f32", .count = 0, .parts = NULL },
    [UNUM_PRIMITIVE_F64] = { .type = "f64", .count = 0, .parts = NULL },
    [UNUM_PRIMITIVE_BOOL] = { .type = "bool", .count = 0, .parts = NULL }
};

// Treat as if constant
UNUM_DEF UnumInternalObject UNUM_OBJECT_PRIMITIVES[] =
{
    [UNUM_PRIMITIVE_NULL] = { .name = "null", .data = NULL, .type = UNUM_OBJ_BLANK },
    [UNUM_PRIMITIVE_I8] = { .name = "i8", .data = &UNUM_PRIMITIVES[UNUM_PRIMITIVE_I8], .type = UNUM_OBJ_SINGLE },
    [UNUM_PRIMITIVE_I16] = { .name = "i16", .data = &UNUM_PRIMITIVES[UNUM_PRIMITIVE_I16], .type = UNUM_OBJ_SINGLE },
    [UNUM_PRIMITIVE_I32] = { .name = "i32", .data = &UNUM_PRIMITIVES[UNUM_PRIMITIVE_I32], .type = UNUM_OBJ_SINGLE },
    [UNUM_PRIMITIVE_I64] = { .name = "i64", .data = &UNUM_PRIMITIVES[UNUM_PRIMITIVE_I64], .type = UNUM_OBJ_SINGLE },
    [UNUM_PRIMITIVE_U8] = { .name = "u8", .data = &UNUM_PRIMITIVES[UNUM_PRIMITIVE_U8], .type = UNUM_OBJ_SINGLE },
    [UNUM_PRIMITIVE_U16] = { .name = "u16", .data = &UNUM_PRIMITIVES[UNUM_PRIMITIVE_U16], .type = UNUM_OBJ_SINGLE },
    [UNUM_PRIMITIVE_U32] = { .name = "u32", .data = &UNUM_PRIMITIVES[UNUM_PRIMITIVE_U32], .type = UNUM_OBJ_SINGLE },
    [UNUM_PRIMITIVE_U64] = { .name = "u64", .data = &UNUM_PRIMITIVES[UNUM_PRIMITIVE_U64], .type = UNUM_OBJ_SINGLE },
    [UNUM_PRIMITIVE_F32] = { .name = "f32", .data = &UNUM_PRIMITIVES[UNUM_PRIMITIVE_F32], .type = UNUM_OBJ_SINGLE },
    [UNUM_PRIMITIVE_F64] = { .name = "f64", .data = &UNUM_PRIMITIVES[UNUM_PRIMITIVE_F64], .type = UNUM_OBJ_SINGLE },
    [UNUM_PRIMITIVE_BOOL] = { .name = "bool", .data = &UNUM_PRIMITIVES[UNUM_PRIMITIVE_BOOL], .type = UNUM_OBJ_SINGLE }
};

// Declare execution functions in advance
// Todo - Move all declarations to dedicated section
UNUM_DEF UnumInternalObject Unum_Internal_Execute_Expressions(UnumInstance* c, UnumInternalPair p);
UNUM_DEF UnumInternalObject Unum_Internal_Execute_Data(UnumInstance* c, UnumInternalStack* ns, UnumInternalPair p);

// Todo - Finish
// Return location of error on problem, otherwise return 0
UNUM_DEF UnumInternalObject Unum_Internal_Execute_Data(UnumInstance* c, UnumInternalStack* ns, UnumInternalPair range)
{
    // Get current location
    size current = range.a;

    // Get first token
    UnumInternalToken active = c->program.tokens[range.a];

    // Handle expressions
    if(strcmp(active.value, "(") == 0)
    {
        // Recursively execute expressions
        Unum_Internal_Execute_Expressions(c, range);
        // Todo - Store result

        // Handle sets
    } else if(strcmp(active.value, "{") == 0)
    {
        // Create set object
        UnumInternalObjSet* set = UNUM_MALLOC(sizeof(UnumInternalObjSet) * 1);

        // Get set data
        UnumInternalPairList set_data = Unum_Internal_Parse_Separate(c->program, range, ",");

        // Verify valid arguments
        if(set_data.pairs == NULL)
        {
            UNUM_FREE(set);
            return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("Invalid syntax", current, UNUM_CODE_SYNTAX_ERROR);
        }

        // Set set count
        set->count = set_data.num;

        // Handle empty set
        if(set_data.num == 0)
        {
            set->keys = NULL;
            set->values = NULL;
        } else {
            set->keys = UNUM_MALLOC(sizeof(str) * set_data.num);
            set->values = UNUM_MALLOC(sizeof(UnumInternalObject) * set_data.num);
        }

        // Iterate over key/pair values
        for(size sc = 0; sc < set_data.num; sc++)
        {
            // Get key/pair value of element in set
            UnumInternalPairList tmp_args = Unum_Internal_Parse_Separate(c->program, set_data.pairs[sc], ":");

            // Verify valid set (must contain 1 key/pair value, 2 elements total)
            if(tmp_args.pairs == NULL || tmp_args.num != 2)
            {
                UNUM_FREE(set_data.pairs);
                return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("Invalid syntax", current, UNUM_CODE_SYNTAX_ERROR);
            }

            // Verify first element is syntactically valid
            if(tmp_args.pairs[0].a != tmp_args.pairs[0].b || c->program.tokens[tmp_args.pairs[0].a].type != UNUM_LANG_SYMBOL)
            {
                UNUM_FREE(set_data.pairs);
                UNUM_FREE(tmp_args.pairs);
                return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("Invalid syntax", current, UNUM_CODE_SYNTAX_ERROR);
            }

            // Get first element
            // Should be a key and therefore a single symbol value (possibly allow for strings in the future)
            set->keys[sc] = Unum_Internal_Utility_Strdup(c->program.tokens[tmp_args.pairs[0].a].value);

            // Get second element
            UnumInternalObject pair_value = Unum_Internal_Execute_Data(c, ns, tmp_args.pairs[1]);

            // Verify object exists
            if(Unum_Internal_Execute_Null(pair_value) || Unum_Internal_Execute_Safe(pair_value))
            {
                return UNUM_OBJ_DEF;
            } else {
                set->values[sc] = pair_value;
            }
        }

        // Store in stack
        ns->objects[ns->count].name = NULL;
        ns->objects[ns->count].data = set;
        ns->objects[ns->count].type = UNUM_OBJ_SET;
        ns->count++;

        // Handle char constants
    } else if(active.type == UNUM_LANG_CHAR) {
        // Assumes char is ASCII (unsigned 8 bit characters)
        // Todo - Process char to handle escape sequences
        UnumInternalObjSingle* val = UNUM_MALLOC(sizeof(UnumInternalObjSingle) * 1);
        val->data = UNUM_MALLOC(sizeof(u8) * 1);
        val->type = &UNUM_PRIMITIVES[UNUM_PRIMITIVE_U8];
        val->data = active.value[1]; // Copy first character inside quotes, ignore rest (for now)

        // Store in stack
        ns->objects[ns->count].name = NULL;
        ns->objects[ns->count].data = val;
        ns->objects[ns->count].type = UNUM_OBJ_SINGLE;
        ns->count++;

        // Handle string constants
    } else if(active.type == UNUM_LANG_STRING) {
        // Assumes string is ASCII (unsigned 8 bit characters)
        UnumInternalObjArray* arr = UNUM_MALLOC(sizeof(UnumInternalObjArray) * 1);
        arr->data = Unum_Internal_Utility_Strdup(active.value);
        arr->type = &UNUM_PRIMITIVES[UNUM_PRIMITIVE_U8];

        // Todo - Process string to handle escape sequences
        // Todo - Make keyword table and lookup function arguments/names from there

        // Store in stack
        ns->objects[ns->count].name = NULL;
        ns->objects[ns->count].data = arr;
        ns->objects[ns->count].type = UNUM_OBJ_ARRAY;
        ns->count++;

        // Handle numeric constants
    } else if(active.type == UNUM_LANG_NUMBER) {
        // Create number object
        UnumInternalObjSingle* dat = UNUM_MALLOC(sizeof(UnumInternalObjSingle) * 1);

        // Check if number is floating point
        // Assumes number is in base 10 format
        if(strchr(active.value, '.') != NULL)
        {
            // Use default type f64
            f64* v = UNUM_MALLOC(sizeof(f64) * 1);
            *v = strtod(active.value, NULL);
            dat->data = v;
            dat->type = &UNUM_PRIMITIVES[UNUM_PRIMITIVE_F64];
        } else {
            // Check if negative
            if(strchr(active.value, '-') != NULL)
            {
                // Use default type i64
                i64* v = UNUM_MALLOC(sizeof(i64) * 1);
                *v = strtol(active.value, NULL, 10);
                dat->data = v;
                dat->type = &UNUM_PRIMITIVES[UNUM_PRIMITIVE_I64];
            } else {
                // Use default type u64
                u64* v = UNUM_MALLOC(sizeof(u64) * 1);
                *v = strtoul(active.value, NULL, 10);
                dat->data = v;
                dat->type = &UNUM_PRIMITIVES[UNUM_PRIMITIVE_U64];
            }
        }

        // Todo - Make keyword table and lookup function arguments/names from there

        // Store in stack
        ns->objects[ns->count].name = NULL;
        ns->objects[ns->count].data = dat;
        ns->objects[ns->count].type = UNUM_OBJ_SINGLE;
        ns->count++;
    } else {
        // Todo - Handle other types of values
        return UNUM_OBJ_DEF;
    }

    return ns->objects[ns->count - 1];
}

// Todo - Finish
// Return location of error on problem, otherwise return 0
UNUM_DEF UnumInternalObject Unum_Internal_Execute_Expressions(UnumInstance* c, UnumInternalPair p)
{
    // unum_log("Code:");
    // unum_log("\"%s\"", c->program.code);
    unum_log("Tokens:");
    for(size tmp = 0; tmp < c->program.count; tmp++)
    {
        unum_log("\t%s", c->program.tokens[tmp].value);
    }

    // Split into expressions
    UnumInternalPairList top = Unum_Internal_Parse_Separate(c->program, (UnumInternalPair){.a = p.a, .b = p.b}, ";"); // Unum_Internal_Parse_Chain(c->program, last, &out_num);
    Unum_Debug_Pair(c->program, top);

    // Verify pairs
    if(top.pairs == NULL)
    {
        unum_log("Invalid chain");
        return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("Bad syntax", 0, UNUM_CODE_SYNTAX_ERROR);
    } else if(top.num == 0 && top.pairs == SAFE){
        unum_log("No statement chain");
        return UNUM_OBJ_SAFE; // UNUM_INTERNAL_RESULT_OK;
    }

    // Get highest stack level
    UnumInternalStack* last_ns = Unum_Internal_Execute_Stack(c);

    // Iterate over expressions
    for(size i = 0; i < top.num; i++)
    {
        // Split into sub-expressions (statements)
        UnumInternalPairList expressions = Unum_Internal_Parse_Separate(c->program, top.pairs[i], "->");
        Unum_Debug_Pair(c->program, expressions);

        // Verify pairs
        if(expressions.num == 0 || expressions.pairs == NULL)
        {
            UNUM_FREE(top.pairs);
            return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("Bad syntax", 0, UNUM_CODE_SYNTAX_ERROR);
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
            Unum_Debug_Pair(c->program, args);

            // Verify valid arguments
            if(args.pairs == NULL)
            {
                UNUM_FREE(top.pairs);
                UNUM_FREE(expressions.pairs);
                return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("Invalid syntax", current, UNUM_CODE_SYNTAX_ERROR);
            }

            // Try keyword matching
            if(tok.type == UNUM_LANG_KEYWORD)
            {
                // Todo - Optimize with hash table
                // Remember, the keyword at index 0 is reserved
                size id = 0;
                for(size v = 0; v < sizeof(UNUM_KEYWORDS) / sizeof(UnumInternalKeyword); v++)
                {
                    // Compare keywords, add 1 to skip operator "@"
                    if(strcmp(UNUM_KEYWORDS[v].name, tok.value + (1 * sizeof(txt))) == 0)
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
                    return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("Unknown/non-existent keyword", current, UNUM_CODE_RUNTIME_ERROR);
                }

                // Create stack for argument values
                UnumInternalStack* ns = UNUM_MALLOC(sizeof(UnumInternalStack) * 1);

                // Add latest object to current stack if pass operator was used
                // Remember, location 0 is reserved, so add 1 to the argument stack anyways
                if(u > 0)
                {
                    ns->count = args.num + 2;
                    ns->objects = UNUM_MALLOC(sizeof(UnumInternalObject) * (ns->count));

                    ns->objects[1] = last_ns->objects[last_ns->count - 1];
                } else {
                    ns->count = args.num + 1;
                    ns->objects = UNUM_MALLOC(sizeof(UnumInternalObject) * (ns->count));
                }

                // Clear element at location 0
                ns->objects[0] = UNUM_OBJ_DEF;

                // Add stack to global stack
                last_ns->objects[0].name = SAFE;
                last_ns->objects[0].data = ns;
                last_ns->objects[0].type = UNUM_OBJ_STACK;

                // Evaluate arguments
                for(size e = 0; e < args.num; e++)
                {
                    // Handle data
                    // Remember to skip passed object if necessary
                    if(u > 0)
                        ns->objects[e + 2] = Unum_Internal_Execute_Data(c, ns, args.pairs[e]);
                    else
                        ns->objects[e + 1] = Unum_Internal_Execute_Data(c, ns, args.pairs[e]);
                }

                // Call keyword
                UnumInternalObject rs = UNUM_KEYWORDS[id].func(c, ns);
                unum_log("Keyword call: \"%s\"", UNUM_KEYWORDS[id].name);

                // Verify successful call
                if(Unum_Internal_Execute_Null(rs)) // rs.code != UNUM_CODE_NEUTRAL)
                {
                    UNUM_FREE(ns);
                    UNUM_FREE(top.pairs);
                    UNUM_FREE(expressions.pairs);
                    UNUM_FREE(args.pairs);
                    return UNUM_OBJ_DEF; // rs;
                }

                // Copy modified stack values back to source if applicable
                if(u > 0)
                    last_ns->objects[last_ns->count - 1] = ns->objects[1];

                // Start value depends on whether there was a pass
                for(size si = u > 0 ? 2 : 1; si < ns->count; si++)
                {
                    // Todo - Check if values are constant

                    // Check if object exists on a lower level stack
                    UnumInternalPair obj_stack_id = Unum_Internal_Execute_Id(c, ns->objects[si].name);

                    // Object is not on any stack (might be technically unnecessary)
                    if(obj_stack_id.b == 0)
                        continue;
                    // Object is on lower level, meaning modifying its value is meaningful
                    if(obj_stack_id.a < Unum_Internal_Execute_Stacklevel(c))
                        Unum_Internal_Execute_Level(c, obj_stack_id.a)->objects[obj_stack_id.b] = ns->objects[si];
                }

                // Delete stack
                UNUM_FREE(ns);
                last_ns->objects[0].data = NULL;

                // Add to current level stack if not empty object
                if(!Unum_Internal_Execute_Safe(rs))
                {
                    last_ns->objects = UNUM_REALLOC(last_ns->objects, sizeof(UnumInternalObject) * (last_ns->count + 1));
                    last_ns->objects[last_ns->count] = rs;
                    last_ns->count++;
                }

                // Try function/object matching
            } else if(tok.type == UNUM_LANG_SYMBOL) {
                // Todo - Match with object name as well (to warn for incorrect names)
                // Remember, functions[0] and objects[0] are supposed to be null
                UnumInternalPair res_id = Unum_Internal_Execute_Id(c, tok.value);

                // Verify object exists
                if(res_id.b == 0)
                {
                    UNUM_FREE(top.pairs);
                    UNUM_FREE(expressions.pairs);
                    UNUM_FREE(args.pairs);
                    return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("Unknown/non-existent function name", current, UNUM_CODE_RUNTIME_ERROR);
                }

                // Verify it is a function
                // Todo - Change to handle object members in the future
                UnumInternalObject res_obj = Unum_Internal_Execute_Level(c, res_id.a)->objects[res_id.b];
                if(res_obj.type != UNUM_OBJ_FUNCTION)
                {
                    UNUM_FREE(top.pairs);
                    UNUM_FREE(expressions.pairs);
                    UNUM_FREE(args.pairs);
                    return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("Expected function name", current, UNUM_CODE_RUNTIME_ERROR);
                }

                // Todo - Call function
                unum_log("Function call here...");

                // Bad syntax
            } else {
                UNUM_FREE(top.pairs);
                UNUM_FREE(expressions.pairs);
                UNUM_FREE(args.pairs);
                return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("Expected symbol/keyword for function", current, UNUM_CODE_SYNTAX_ERROR);
            }
        }

        // Free expression ranges
        UNUM_FREE(expressions.pairs);

        // Check if last expression
        if(i == top.num - 1)
        {
        }

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

    // Free overall expression ranges
    UNUM_FREE(top.pairs);

    // Return object on the top of the stack if not empty
    if(last_ns->count > 1)
    {
        return last_ns->objects[last_ns->count - 1];
    } else {
        // Otherwise return empty object
        return UNUM_OBJ_SAFE;
    }
}

/****************************************************************
 * Front end
 ****************************************************************/

// Unum standard libraries
#include "libraries/UMath.h"

UNUM_DEF UnumResult Unum_Execute(UnumInstance* c, str code)
{
    c->program = Unum_Internal_Tokenize(code);
    UnumInternalObject ob = Unum_Internal_Execute_Expressions(c, (UnumInternalPair){.a = 0, .b = c->program.count});

    if(Unum_Internal_Execute_Null(ob))
    {
        return UNUM_INTERNAL_RESULT("Error", 0, UNUM_CODE_RUNTIME_ERROR);
    } else {
        c->stack.objects = UNUM_REALLOC(c->stack.objects, sizeof(UnumInternalObject) * (c->stack.count + 1));
        c->stack.objects[c->stack.count] = ob;
        c->stack.count++;

        return UNUM_INTERNAL_RESULT_OK;
    }
}

UNUM_DEF UnumInstance* Unum_Initialize(void)
{
    p_libsys_init();

    UnumInstance* res = UNUM_MALLOC(sizeof(UnumInstance));

    res->stack.count = sizeof(UNUM_OBJECT_PRIMITIVES) / sizeof(UnumInternalObject);
    res->stack.objects = UNUM_MALLOC(sizeof(UnumInternalObject) * res->stack.count);

    memcpy(res->stack.objects, UNUM_OBJECT_PRIMITIVES, sizeof(UNUM_OBJECT_PRIMITIVES));

    // First object is reserved
    res->stack.count = 1;
    res->stack.objects = UNUM_MALLOC(sizeof(UnumInternalObject) * 1);

    // Load native functions
    res->native = UNUM_MALLOC(sizeof(UNUM_NATIVE_MATH));
    memcpy(res->native, UNUM_NATIVE_MATH, sizeof(UNUM_NATIVE_MATH));

    // Load standard libraries
    UnumResult r = Unum_Execute(res, UNUM_LIBRARY_MATH);
    if(r.code != UNUM_CODE_NEUTRAL)
    {
        UNUM_FREE(res);
        return NULL;
    }

    return res;
}

UNUM_DEF void Unum_Destroy(UnumInstance* c)
{
    // Todo - Improve cleanup by using memory pool
    // Note - The current cleanup method may leak memory in some cases

    UnumInternalStack* s = &c->stack;

    while(s != NULL)
    {
        for(size i = 0; i < s->count; i++)
        {
            UNUM_FREE(s->objects[i].name);
            UNUM_FREE(s->objects[i].data);
        }

        if(s->count > 0)
            s = (UnumInternalStack*)(s->objects[s->count - 1].data);
        else
            s = NULL;
    }

    UNUM_FREE(c->stack.objects);

    UNUM_FREE(c->program.code);
    UNUM_FREE(c->program.statement);
    UNUM_FREE(c->program.tokens);

    UNUM_FREE(c->native);

    UNUM_FREE(c);

    p_libsys_shutdown();
}

#ifdef __cplusplus
}
#endif // C++ guard

#endif // UNUM_INCLUDE
