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
#define UNUM_DEF extern
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
#define unum_log(...) printf("[Unum][%u ms]: ", Unum_Time() / 1000U); printf(__VA_ARGS__); printf("\n"); unum_flush()
#else
#define unum_log(...) printf("[Unum][%u ms]: ", Unum_Time() / 1000U); printf(__VA_ARGS__); printf("\n")
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

/****************************************************************
 * Preprocessor Constants
 ****************************************************************/

// C++ handler
#ifdef __cplusplus
extern "C" {
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
    UNUM_OBJ_DATA, UNUM_OBJ_TYPE, UNUM_OBJ_STACK, UNUM_OBJ_SYMBOL,
    UNUM_OBJ_SINGLE, UNUM_OBJ_ARRAY, UNUM_OBJ_SET, UNUM_OBJ_EXPRESSION,
    UNUM_OBJ_FUNCTION, UNUM_OBJ_STRUCTURE, UNUM_OBJ_NAMESPACE, UNUM_OBJ_ALIAS
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
    size count;
    UnumInternalObject* data;
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

typedef struct UnumInternalObjAlias
{
    UnumInternalObjSymbol* original;
    UnumInternalObjSymbol* alias;
} UnumInternalObjAlias;

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
    UnumInternalObject* params;
    UnumInternalObject result; // Possibly allow for multiple return types in the future
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

typedef struct UnumInternalKeyword
{
    str name;
    UnumInternalObject (*func)(UnumInstance*, UnumInternalStack*);
} UnumInternalKeyword;

/****************************************************************
 * Other types
 ****************************************************************/

typedef enum UnumStatus
{
    UNUM_STATUS_OK = 0,
    UNUM_STATUS_DEBUG,
    UNUM_STATUS_WARNING, UNUM_STATUS_ERROR
} UnumStatus;

typedef enum UnumCode
{
    UNUM_CODE_NEUTRAL = 0,
    UNUM_CODE_SYNTAX_ERROR, UNUM_CODE_RUNTIME_ERROR
} UnumCode;

typedef enum UnumInternalKeywords
{
    UNUM_KEYWORD_BLANK = 0,
    UNUM_KEYWORD_NAMESPACE, UNUM_KEYWORD_ALIAS, UNUM_KEYWORD_FUNCTION, UNUM_KEYWORD_STRUCTURE,
    UNUM_KEYWORD_PARAMETERS, UNUM_KEYWORD_RESULT, UNUM_KEYWORD_BODY, UNUM_KEYWORD_RETURN
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

typedef struct UnumCodeInfo
{
    str message;
    UnumStatus level;
} UnumCodeInfo;

typedef struct UnumResult
{
    size location;
    UnumCode code;
    str message;
    i32 result;
} UnumResult;

/****************************************************************
 * Declarations
 ****************************************************************/

UNUM_DEF size Unum_Time(void);
UNUM_DEF bool Unum_Internal_Utility_Txtvalid(txt s);
UNUM_DEF bool Unum_Internal_Utility_Strstart(str s, str starts);
UNUM_DEF str Unum_Internal_Utility_Strdup(str s);
UNUM_DEF str Unum_Internal_Utility_Stradd(str a, str b);
UNUM_DEF str Unum_Internal_Utility_Strsub(str s, size a, size b);
UNUM_DEF str Unum_Internal_Utility_Strins(str s, str location, str a);
UNUM_DEF UnumInternalTokens Unum_Internal_Utility_Tokensub(UnumInternalTokens s, UnumInternalPair range);
UNUM_DEF str Unum_Internal_Parse_Char(str start);
UNUM_DEF str Unum_Internal_Parse_String(str start);
UNUM_DEF str Unum_Internal_Parse_Comment(str start);
UNUM_DEF str Unum_Internal_Parse_Symbol(str start);
UNUM_DEF str Unum_Internal_Parse_Number(str start);
UNUM_DEF str Unum_Internal_Parse_Operator(str start);
UNUM_DEF UnumInternalTokens Unum_Internal_Tokenize(str code);
UNUM_DEF UnumInternalPair Unum_Internal_Parse_Match(UnumInternalTokens t, size start);
UNUM_DEF UnumInternalPairList Unum_Internal_Parse_Separate(UnumInternalTokens t, UnumInternalPair range, str sep);
UNUM_DEF UnumInternalPair* Unum_Internal_Parse_Arguments(UnumInternalTokens t, size start, size* out_num);
UNUM_DEF void Unum_Debug_Pair(UnumInternalTokens s, UnumInternalPairList p);
UNUM_DEF void Unum_Debug_Object(UnumInternalTokens s, UnumInternalObject p);
UNUM_DEF bool Unum_Internal_Execute_Obj_Safe(UnumInternalObject o);
UNUM_DEF bool Unum_Internal_Execute_Obj_Null(UnumInternalObject o);
UNUM_DEF void Unum_Internal_Execute_Obj_Clear(UnumInternalObject o);
UNUM_DEF bool Unum_Internal_Execute_Same_Type(UnumInternalObject a, UnumInternalObject b);
UNUM_DEF UnumInternalPair Unum_Internal_Execute_Id(UnumInstance* c, str name);
UNUM_DEF size Unum_Internal_Execute_Stacklevel(UnumInstance* c);
UNUM_DEF UnumInternalStack* Unum_Internal_Execute_Stack(UnumInstance* c);
UNUM_DEF UnumInternalStack* Unum_Internal_Execute_Level(UnumInstance* c, size lvl);
UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Function(UnumInstance* c, UnumInternalStack* params);
UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Parameters(UnumInstance* c, UnumInternalStack* params);
UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Result(UnumInstance* c, UnumInternalStack* params);
UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Namespace(UnumInstance* c, UnumInternalStack* params);
UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Alias(UnumInstance* c, UnumInternalStack* params);
UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Structure(UnumInstance* c, UnumInternalStack* params);
UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Body(UnumInstance* c, UnumInternalStack* params);
UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Return(UnumInstance* c, UnumInternalStack* params);
UNUM_DEF UnumInternalObject Unum_Internal_Execute_Data(UnumInstance* c, UnumInternalStack* ns, UnumInternalPair range);
UNUM_DEF UnumInternalObject Unum_Internal_Execute_Expressions(UnumInstance* c, UnumInternalPair p);
UNUM_DEF UnumResult Unum_Execute(UnumInstance* c, str code);
UNUM_DEF UnumInstance* Unum_Initialize(void);
UNUM_DEF void Unum_Destroy(UnumInstance* c);

/****************************************************************
 * Constants
 ****************************************************************/

static const str UNUM_STATUSES[] =
{
    [UNUM_STATUS_OK] = "Ok",
    [UNUM_STATUS_DEBUG] = "Debug",
    [UNUM_STATUS_WARNING] = "Warning",
    [UNUM_STATUS_ERROR] = "Error"
};

static const UnumCodeInfo UNUM_CODES[] =
{
    [UNUM_CODE_NEUTRAL] = {.message = "Ok", .level = UNUM_STATUS_OK},
    [UNUM_CODE_SYNTAX_ERROR] = {.message = "Syntax error", .level = UNUM_STATUS_ERROR},
    [UNUM_CODE_RUNTIME_ERROR] = {.message = "Runtime error", .level = UNUM_STATUS_ERROR}
};

static const str UNUM_OPERATORS[] =
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

static UnumInternalKeyword UNUM_KEYWORDS[] =
{
    [UNUM_KEYWORD_BLANK] = {.name = "", .func = NULL},
    [UNUM_KEYWORD_NAMESPACE] = {.name = "namespace", .func = Unum_Internal_Keyword_Namespace},
    [UNUM_KEYWORD_ALIAS] = {.name = "alias", .func = Unum_Internal_Keyword_Alias},
    [UNUM_KEYWORD_FUNCTION] = {.name = "function", .func = Unum_Internal_Keyword_Function},
    [UNUM_KEYWORD_STRUCTURE] = {.name = "structure", .func = Unum_Internal_Keyword_Structure},
    [UNUM_KEYWORD_PARAMETERS] = {.name = "parameters", .func = Unum_Internal_Keyword_Parameters},
    [UNUM_KEYWORD_RESULT] = {.name = "result", .func = Unum_Internal_Keyword_Result},
    [UNUM_KEYWORD_BODY] = {.name = "body", .func = Unum_Internal_Keyword_Body},
    [UNUM_KEYWORD_RETURN] = {.name = "return", .func = Unum_Internal_Keyword_Return}
};

// Treat as if constant
static UnumInternalObjType UNUM_PRIMITIVES[] =
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
static const UnumInternalObject UNUM_OBJECT_PRIMITIVES[] =
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

// Object defaults
static const UnumInternalObject UNUM_OBJ_DEF = (UnumInternalObject) {.name = NULL, .data = NULL, .type = UNUM_OBJ_BLANK};
static const UnumInternalObject UNUM_OBJ_SAFE = (UnumInternalObject) {.name = SAFE, .data = SAFE, .type = UNUM_OBJ_BLANK};

#ifdef __cplusplus
}
#endif // C++ guard

#endif // UNUM_INCLUDE
