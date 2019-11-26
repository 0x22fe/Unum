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
#define unum_log(...) printf("[Unum][%u ms]: ", unum_time() / 1000U); printf(__VA_ARGS__); printf("\n");
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

#define UNUM_MATH_ERROR (1e-3F)
#define UNUM_MATH_EPSILON (1e-8F)
#define UNUM_MATH_INFINITY (1e+10F)
#define UNUM_MATH_PI (3.14159265358979323846F)
#define UNUM_MATH_2_PI (UNUM_MATH_PI * 2.0F)
#define UNUM_MATH_PI_2 (UNUM_MATH_PI / 2.0F)
#define UNUM_MATH_DEG (UNUM_MATH_PI / 180.0F)
#define UNUM_MATH_RAD (180.0F / UNUM_MATH_PI)

typedef enum UnumInternalOperators
{
    UNUM_OPERATOR_COMPILER = 0,
    UNUM_OPERATOR_PASS, UNUM_OPERATOR_END, UNUM_OPERATOR_SEPARATE,
    UNUM_OPERATOR_SET_START, UNUM_OPERATOR_SET_END,
    UNUM_OPERATOR_ARRAY_START, UNUM_OPERATOR_ARRAY_END
} UnumInternalOperators;

UNUM_DEF const str UNUM_OPERATORS[] =
{
    [UNUM_OPERATOR_COMPILER] = "@",
    [UNUM_OPERATOR_PASS] = "->",
    [UNUM_OPERATOR_END] = ";",
    [UNUM_OPERATOR_SEPARATE] = ",",
    [UNUM_OPERATOR_SET_START] = "{",
    [UNUM_OPERATOR_SET_END] = "}",
    [UNUM_OPERATOR_ARRAY_START] = "[",
    [UNUM_OPERATOR_ARRAY_END] = "]"
};

typedef enum UnumInternalKeywords
{
    UNUM_KEYWORD_FUNCTION = 0,
    UNUM_KEYWORD_STRUCTURE, UNUM_KEYWORD_PARAMETERS, UNUM_KEYWORD_RESULT, UNUM_KEYWORD_BODY, UNUM_KEYWORD_RETURN
} UnumInternalKeywords;

UNUM_DEF const str UNUM_KEYWORDS[] =
{
    [UNUM_KEYWORD_FUNCTION] = "function",
    [UNUM_KEYWORD_STRUCTURE] = "function",
    [UNUM_KEYWORD_PARAMETERS] = "parameters",
    [UNUM_KEYWORD_RESULT] = "result",
    [UNUM_KEYWORD_BODY] = "body",
    [UNUM_KEYWORD_RETURN] = "return"
};

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

/****************************************************************
 * Functions
 ****************************************************************/

// Returns time in microseconds
// https://www.geeksforgeeks.org/measure-execution-time-with-high-precision-in-c-c/
UNUM_DEF size unum_time(void)
{
    return (size)(((f64)(clock()) / (f64)(CLOCKS_PER_SEC)) * 1e6);
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

UNUM_DEF bool Unum_Internal_Utility_Txtvalid(txt s)
{
    if(s == '\0' || (iscntrl(s) && s != '\n' && s != '\t')) return UNUM_FALSE;
    else return UNUM_TRUE;
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
    } else if (Unum_Internal_Utility_Strstart(begin, "->")){
        return begin + (2 * sizeof(txt));
    } else {
        return NULL;
    }
}

UNUM_DEF UnumInternalTokens Unum_Internal_Tokenize(str code)
{
    size i = 0;
    size u = 0;
    size length = strlen(code);

    UnumInternalTokens result;

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
        } else if(code[i] == '#') {
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
        } else if(!isspace(code[i])) {
            // Operator
            start = result.code + (i * sizeof(txt));
            end = Unum_Internal_Parse_Operator(start);
            val = UNUM_LANG_OPERATOR;
        } else {
            // Whitespace
            skip = UNUM_TRUE;
            while(isspace(result.code[i]) && Unum_Internal_Utility_Txtvalid(result.code[i]))
                i++;
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
            }

            i += (end - start) / sizeof(txt);
        }
    }

    return result;
}

UNUM_DEF UnumInstance* Unum_Initialize(void)
{
    p_libsys_init();

    UnumInstance* res = UNUM_MALLOC(sizeof(UnumInstance));

    res->stack.type_count = sizeof(UNUM_TYPE_PRIMITIVES) / sizeof(UnumInternalType);
    res->stack.types = UNUM_MALLOC(sizeof(UNUM_TYPE_PRIMITIVES));

    memcpy(res->stack.types, UNUM_TYPE_PRIMITIVES, sizeof(UNUM_TYPE_PRIMITIVES));

    res->stack.object_count = 0;
    res->stack.objects = UNUM_MALLOC(sizeof(UnumInternalObject) * 1);

    res->stack.function_count = 0;
    res->stack.functions = UNUM_MALLOC(sizeof(UnumInternalFunction) * 1);

    return res;
}

UNUM_DEF UnumInternalPair Unum_Internal_Find_Match(UnumInternalTokens t, size start)
{
    const UnumInternalPair def = (UnumInternalPair){.a = 0, .b = 0};

    str begin = t.tokens[start].value;
    str match;

    if(strcmp(begin, "{") == 0) match = "}";
    else if(strcmp(begin, "(") == 0) match = ")";
    else if(strcmp(begin, "[") == 0) match = "]";
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

// Takes in the starting token number (token right after function name) and tokens
// Returns an array of pairs, where each pair corresponds to the start and end token ID of the parameter
UNUM_DEF UnumInternalPair* Unum_Internal_Parse_Parameters(UnumInternalTokens t, size token_start, size* out_num_params)
{
    size i = 0;
    size current = token_start;
    UnumInternalPair* params = UNUM_MALLOC(sizeof(UnumInternalPair) * 1);
    *out_num_params = 0;

    // Handle no parameter function here
    if(strcmp(t.tokens[current].value, ";") == 0) return params;
    else if(strcmp(t.tokens[current].value, "->") == 0) return params;
    else if(strcmp(t.tokens[current].value, ",") == 0) {
        // Syntax error - extra comma
        UNUM_FREE(params);
        return NULL;
    }

    // Parse loop
    while(current < t.count)
    {
        UnumInternalToken val = t.tokens[current];
        if(strcmp(val.value, "(") == 0)
        {
            // Value isn't a literal, but an expression
            UnumInternalPair res = Unum_Internal_Find_Match(t, current);

            // unum_log("Test: \"%s\" & Results: \"%s\" and \"%s\"", val.value, t.tokens[res.a].value, t.tokens[res.b].value);

            // Syntax (?) error
            if(res.a == res.b) {
                UNUM_FREE(params);
                return NULL;
            }

            // Allocate more memory
            params = UNUM_REALLOC(params, sizeof(UnumInternalPair) * (i + 1));

            // Copy value
            params[i].a = res.a;
            params[i].b = res.b;

            // Increment current
            current = res.b + 1;
        } else if(strcmp(val.value, "[") == 0)
        {
            // Value is an array
            UnumInternalPair res = Unum_Internal_Find_Match(t, current);

            // unum_log("Test: \"%s\" & Results: \"%s\" and \"%s\"", val.value, t.tokens[res.a].value, t.tokens[res.b].value);

            // Syntax (?) error
            if(res.a == res.b) {
                UNUM_FREE(params);
                return NULL;
            }

            // Allocate more memory
            params = UNUM_REALLOC(params, sizeof(UnumInternalPair) * (i + 1));

            // Copy value
            params[i].a = res.a;
            params[i].b = res.b;

            // Increment current
            current = res.b + 1;
        } else if(strcmp(val.value, "{") == 0)
        {
            // Value is a set
            UnumInternalPair res = Unum_Internal_Find_Match(t, current);

            // unum_log("Test: \"%s\" & Results: \"%s\" and \"%s\"", val.value, t.tokens[res.a].value, t.tokens[res.b].value);

            // Syntax (?) error
            if(res.a == res.b) {
                UNUM_FREE(params);
                return NULL;
            }

            // Allocate more memory
            params = UNUM_REALLOC(params, sizeof(UnumInternalPair) * (i + 1));

            // Copy value
            params[i].a = res.a;
            params[i].b = res.b;

            // Increment current
            current = res.b + 1;
        } else {
            // Allocate more memory
            params = UNUM_REALLOC(params, sizeof(UnumInternalPair) * (i + 1));

            // Value is literal
            params[i].a = current;
            params[i].b = current;

            // Increment current
            current++;
        }

        // Increment values
        i++;
        *out_num_params = i;

        // Todo - Create all exit conditions

        // unum_log("Current %u/%u: %s", current, t.count, t.tokens[current].value);

        // Exit if token list end
        if(current == t.count) return params;
        // Statement ends
        else if(strcmp(t.tokens[current].value, ";") == 0) return params;
        // Pipe operator
        else if(strcmp(t.tokens[current].value, "->") == 0) return params;
        // Another parameter
        else if(strcmp(t.tokens[current].value, ",") == 0) { current++; }

        /*
        // Bad syntax
        else {
            UNUM_FREE(params);
            return NULL;
        }
        */
    }

    // Bad syntax
    UNUM_FREE(params);
    return NULL;
}

#define QUOTE(str) #str
#define EXPAND_AND_QUOTE(str) QUOTE(str)
#define UNUM_INTERNAL_RESULT(msg, loc, c) (UnumResult){.location = (loc), .code = (c), .message = Unum_Internal_Utility_Stradd(UNUM_CODES[(c)].message, " (Interpreter line " EXPAND_AND_QUOTE(__LINE__) "): " msg), .result = -((i32)(UNUM_CODES[(c)].level)) }

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

// Todo - Finish
// Todo - Break up into sub-functions by keyword
// Return location of error on problem, otherwise return 0
UNUM_DEF UnumResult Unum_Execute(UnumInstance* c, str code)
{
    c->program = Unum_Internal_Tokenize(code);

    unum_log("Code:");
    unum_log("\"%s\"", code);
    unum_log("Tokens:");
    for(size tmp = 0; tmp < c->program.count; tmp++)
    {
        unum_log("\t%s", c->program.tokens[tmp].value);
    }

    bool pipe = UNUM_FALSE;

    for(size i = 0; i < c->program.count; i++)
    {
        UnumInternalToken current = c->program.tokens[i];

        if(current.type == UNUM_LANG_KEYWORD)
        {
            if(strstr(current.value, UNUM_KEYWORDS[UNUM_KEYWORD_FUNCTION]) != NULL)
            {
                size num = 0;
                UnumInternalPair* params = Unum_Internal_Parse_Parameters(c->program, i + 1, &num);
                //unum_log("Keyword call \"%s\". Parameters (%u):", current.value, num);

                if(params == NULL)
                    // Error - Bad syntax
                    return UNUM_INTERNAL_RESULT("Invalid syntax", i, UNUM_CODE_SYNTAX_ERROR);
                if(num == 0)
                    // Error - Not enough arguments (need function name)
                    return UNUM_INTERNAL_RESULT("Not enough arguments (need function name)", i, UNUM_CODE_SYNTAX_ERROR);
                if(num > 1)
                    // Error - Too many arguments (only need function name)
                    return UNUM_INTERNAL_RESULT("Too many arguments (only need function name)", i, UNUM_CODE_SYNTAX_ERROR);
                if(params[0].a != params[0].b)
                    // Error - Argument is not a valid symbol
                    return UNUM_INTERNAL_RESULT("Argument is not a valid symbol", i, UNUM_CODE_SYNTAX_ERROR);

                str name = c->program.tokens[params[0].a].value;

                // Verify symbol name doesn't already exist
                for(size u = 0; u < c->stack.object_count; u++)
                {
                    if(strcmp(c->stack.objects[u].name, name) == 0)
                        // Error - Duplicate symbol name (object)
                        return UNUM_INTERNAL_RESULT("Duplicate symbol name (object)", i, UNUM_CODE_RUNTIME_ERROR);
                }
                for(size u = 0; u < c->stack.function_count; u++)
                {
                    if(strcmp(c->stack.functions[u].name, name) == 0)
                        // Error - Duplicate symbol name (function)
                        return UNUM_INTERNAL_RESULT("Duplicate symbol name (function)", i, UNUM_CODE_RUNTIME_ERROR);
                }

                UnumInternalFunction f;
                f.name = Unum_Internal_Utility_Strdup(name);
                f.data = NULL;
                f.result = (UnumInternalType) {.name = NULL, .parts = NULL};
                f.parameters = (UnumInternalType) {.name = NULL, .parts = NULL};

                // Resize array and add function
                c->stack.functions[c->stack.function_count] = f;
                c->stack.function_count++;
                c->stack.functions = UNUM_REALLOC(c->stack.functions, sizeof(UnumInternalFunction) * c->stack.function_count);

                unum_log("New Function \"%s\"", f.name);

                UNUM_FREE(params);

            } else if(strstr(current.value, UNUM_KEYWORDS[UNUM_KEYWORD_PARAMETERS]) != NULL)
            {
                // Parameter call always has to follow a function and pipe call
                // This may be changed in a future version
                // Todo - Verify piped object is a function
                if(!pipe)
                    return UNUM_INTERNAL_RESULT("@parameters must always follow a call to @function", i, UNUM_CODE_SYNTAX_ERROR);

                UnumInternalType p;
                p.name = NULL;
                p.parts_count = 0;
                p.parts = NULL;

                size num = 0;
                UnumInternalPair* params = Unum_Internal_Parse_Parameters(c->program, i + 1, &num);

                if(params == NULL)
                    return UNUM_INTERNAL_RESULT("Invalid syntax", i, UNUM_CODE_SYNTAX_ERROR);

                if(num > 0)
                {
                    p.parts_count = num;
                    p.parts = UNUM_MALLOC(sizeof(UnumInternalTypePair) * num);

                    for(size u = 0; u < num; u++)
                    {
                        // unum_log("%d, %d", params[u].b, params[u].a);
                        if(strcmp(c->program.tokens[params[u].a].value, "{") == 0
                                && params[u].b - params[u].a == 4) // Must include 5 tokens
                        {
                            size tok_symbol = params[u].a + 1;
                            size tok_itemset = params[u].a + 2;
                            size tok_typename = params[u].a + 3; // Require one-symbol typename, possible allow for expressions in the future

                            // Validate input
                            if(c->program.tokens[tok_symbol].type != UNUM_LANG_SYMBOL)
                                return UNUM_INTERNAL_RESULT("@parameters argument set expected symbol, received other type", i, UNUM_CODE_SYNTAX_ERROR);
                            if(c->program.tokens[tok_itemset].type != UNUM_LANG_OPERATOR)
                                return UNUM_INTERNAL_RESULT("@parameters argument set expected operator, received other type", i, UNUM_CODE_SYNTAX_ERROR);
                            if(strcmp(c->program.tokens[tok_itemset].value, ":") != 0)
                                return UNUM_INTERNAL_RESULT("@parameters argument set expected set equals operator (\":\"), received other operator", i, UNUM_CODE_SYNTAX_ERROR);
                            if(c->program.tokens[tok_typename].type != UNUM_LANG_SYMBOL)
                                return UNUM_INTERNAL_RESULT("@parameters argument set expected symbol, received other type", i, UNUM_CODE_SYNTAX_ERROR);

                            // Lookup typename
                            size param_type_id = Unum_Internal_Execute_Typeid(c, c->program.tokens[tok_typename].value);

                            // Todo - Compare with other parameter symbol names to ensure there are no duplicates

                            // Add to hidden type
                            p.parts[u] = (UnumInternalTypePair) {.name = Unum_Internal_Utility_Strdup(c->program.tokens[tok_symbol].value), .type_id = param_type_id};

                        } else {
                            // Error - Bad syntax
                            return UNUM_INTERNAL_RESULT("@parameters expects arguments of the format { symbol: typename }", i, UNUM_CODE_SYNTAX_ERROR);
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
                } else {
                    unum_log("No Parameters");
                }

                UNUM_FREE(params);
            } else if(strstr(current.value, UNUM_KEYWORDS[UNUM_KEYWORD_RESULT]) != NULL)
            {
                // Rsult call always has to follow a parameter and pipe call
                // This may be changed in a future version
                // Todo - Verify piped object is a function
                if(!pipe)
                    return UNUM_INTERNAL_RESULT("@result must always follow a call to @parameter", i, UNUM_CODE_SYNTAX_ERROR);

                // Possibly add support for multiple return values in the future
                size num = 0;
                UnumInternalPair* params = Unum_Internal_Parse_Parameters(c->program, i + 1, &num);

                if(params == NULL)
                    return UNUM_INTERNAL_RESULT("Invalid syntax", i, UNUM_CODE_SYNTAX_ERROR);

                if(num == 0)
                {
                    unum_log("No result");
                } else if (num == 1) {
                    if(params[0].a != params[0].b)
                        return UNUM_INTERNAL_RESULT("@return has invalid arguments", i, UNUM_CODE_SYNTAX_ERROR);
                    if(c->program.tokens[params[0].a].type != UNUM_LANG_SYMBOL)
                        return UNUM_INTERNAL_RESULT("@return expected a symbol, got an other type", i, UNUM_CODE_SYNTAX_ERROR);

                    UnumInternalType val;
                    val.name = NULL;
                    val.parts_count = 1;
                    val.parts = UNUM_MALLOC(sizeof(UnumInternalTypePair) * 1);

                    // Lookup typename
                    size param_type_id = Unum_Internal_Execute_Typeid(c, c->program.tokens[params[0].a].value);

                    val.parts[0].name = NULL;
                    val.parts[0].type_id = param_type_id;

                    // Set properties of incoming function
                    c->stack.functions[c->stack.function_count - 1].result = val;

                    unum_log("New result with type ID: %u (\"%s\")", param_type_id, c->stack.types[param_type_id].name);
                } else {
                    return UNUM_INTERNAL_RESULT("Too many return values (only 1 is allowed)", i, UNUM_CODE_SYNTAX_ERROR);
                }

                UNUM_FREE(params);
            } else {
                // Error - Invalid keyword
                // return UNUM_INTERNAL_RESULT("Invalid keyword", i, UNUM_CODE_SYNTAX_ERROR);
            }
            // Todo - Locate current.value in function table to ensure it is a function

            /*
            for(size t = 0; t < num; t++)
            {
                printf("\t");
                if(params[t].a == params[t].b)
                {
                    printf("%s", c->program.tokens[params[t].a].value);
                } else {
                    for(size pc = params[t].a; pc <= params[t].b; pc++)
                        printf("%s", c->program.tokens[pc].value);
                }
                printf("\n");
            }*/

            unum_log("End call");
        } else if (current.type == UNUM_LANG_OPERATOR) {
            if(strcmp(current.value, UNUM_OPERATORS[UNUM_OPERATOR_PASS]) == 0)
            {
                pipe = UNUM_TRUE;
            }
        } else {

        }
    }

    return UNUM_INTERNAL_RESULT("Ok", 0, UNUM_CODE_NEUTRAL);
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
