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
static UnumInternalObject unum_lib_io_print(UnumInstance* c, UnumInternalObjStack* f)
{
    // Print all objects
    for(size i = 0; i < f->count; i++)
    {
        any data = f->objects[i].data;

        // Print by data type
        // Use placeholders for now
        switch (f->objects[i].type)
        {
        case UNUM_OBJ_BLANK: {
            // printf("NULL");
            break;
        }
        case UNUM_OBJ_TYPE: {
            printf("Type");
            break;
        }
        case UNUM_OBJ_STACK: {
            printf("Stack");
            break;
        }
        case UNUM_OBJ_SYMBOL: {
            UnumInternalObjSymbol* v = (UnumInternalObjSymbol*)data;
            for(size c = 0; c < v->count; c++)
            {
                if(c != v->count - 1) printf("%s.", v->names[c]);
                else printf("%s", v->names[c]);
            }
            break;
        }
        case UNUM_OBJ_SINGLE: {
            UnumInternalObjSingle* v = (UnumInternalObjSingle*)data;

            // Handle primitive types
            // Todo - Optimize using hash map
            if(strcmp(v->type.type, "null") == 0) printf("NULL");
            else if(strcmp(v->type.type, "void") == 0) printf("VOID");
            else if(strcmp(v->type.type, "any") == 0) printf("ANY");
            else if(strcmp(v->type.type, "f64") == 0) printf("%f", *((f64*)v->data));
            // ...
            else printf("UNKNOWN");
            break;
        }
        case UNUM_OBJ_ARRAY: {
            printf("Array");
            break;
        }
        case UNUM_OBJ_SET: {
            printf("Set");
            break;
        }
        case UNUM_OBJ_EXPRESSION: {
            printf("Expression");
            break;
        }
        case UNUM_OBJ_FUNCTION: {
            printf("Function");
            break;
        }
        case UNUM_OBJ_STRUCTURE: {
            printf("Structure");
            break;
        }
        case UNUM_OBJ_NAMESPACE: {
            printf("Namespace");
            break;
        }
        case UNUM_OBJ_ALIAS: {
            printf("Alias");
            break;
        }
        default: {
            printf("UNKNOWN");
            break;
        }
        }
    }

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
