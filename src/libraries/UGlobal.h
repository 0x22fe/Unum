/*
 *
 * Unum - Global Library
 * Date: November 30, 2019
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

// Include this file in Unum.h (in library loading section)

// Include here, although this is technically unnecessary
#include "../Unum.h"

#ifndef UGLOBAL_HPP
#define UGLOBAL_HPP

/****************************************************************
 * Native Functions
 ****************************************************************/

// Safely assume types match

// Version code function
static UnumInternalObject unum_lib_global_version(UnumInstance* c, UnumInternalObjStack* f)
{
    u64* v = UNUM_MALLOC(sizeof(u64) * 1); \
    *v = UNUM_VERSION; \
    UnumInternalObjSingle* res = UNUM_MALLOC(sizeof(UnumInternalObjSingle) * 1); \
    res->data = v; \
    UnumInternalPair p = Unum_Internal_Execute_Id(c, "u64"); \
    res->type = *((UnumInternalObjType*)(Unum_Internal_Execute_Level(c, p.a)->objects[p.b].data)); \
    return (UnumInternalObject) {.name = SAFE, .type = UNUM_OBJ_SINGLE, .data = res};
}

/****************************************************************
 * Function Loading
 ****************************************************************/

// Function table
static UnumInternalNative UNUM_NATIVE_GLOBAL[1];

// Unum code
static const str UNUM_LIBRARY_GLOBAL =
        #include "generated/Global.h"
        ;

// Function table initialization
static bool Unum_Lib_Global_Load()
{
    size counter = 0;

    UnumInternalObject unum_lib_global_version_params[] = { [0] = UNUM_OBJECT_PRIMITIVES[UNUM_PRIMITIVE_VOID] };
    UnumInternalObject unum_lib_global_version_result = UNUM_OBJECT_PRIMITIVES[UNUM_PRIMITIVE_U64];

    UNUM_NATIVE_GLOBAL[counter++] =
    (UnumInternalNative)
    {
        .name = "unum_global_version",
        .function = unum_lib_global_version,
        .params = unum_lib_global_version_params,
        .result = unum_lib_global_version_result
    };

    return UNUM_TRUE;
}

#endif // UGLOBAL_HPP
