/*
 *
 * Unum - IO Library
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

#ifndef UIO_HPP
#define UIO_HPP

/****************************************************************
 * Native Functions
 ****************************************************************/

// Safely assume types match

// Print function
static UnumInternalObject unum_lib_io_print(UnumInstance* c, UnumInternalStack* f)
{
    // Todo - Handle all data types
    f64 i = *((f64*)((UnumInternalObjSingle*)f->objects[1].data)->data);
    printf("%f", i);
    return UNUM_OBJ_SAFE;
}

/****************************************************************
 * Function Loading
 ****************************************************************/

// Function table
static UnumInternalNative UNUM_NATIVE_IO[1];

// Unum code
static const str UNUM_LIBRARY_IO =
        #include "generated/IO.h"
        ;

// Function table initialization
static bool Unum_Lib_Io_Load()
{
    size counter = 0;

    UnumInternalObject unum_lib_io_print_params[] = { [0] = UNUM_OBJECT_PRIMITIVES[UNUM_PRIMITIVE_ANY] };
    UnumInternalObject unum_lib_io_print_result = UNUM_OBJECT_PRIMITIVES[UNUM_PRIMITIVE_NULL];

    UNUM_NATIVE_IO[counter++] =
    (UnumInternalNative)
    {
        .name = "unum_io_print",
        .function = unum_lib_io_print,
        .params = unum_lib_io_print_params,
        .result = unum_lib_io_print_result
    };

    return UNUM_TRUE;
}

#endif // UIO_HPP
