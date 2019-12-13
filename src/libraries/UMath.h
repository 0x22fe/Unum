/*
 *
 * Unum - Math Library
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

#ifndef UMATH_HPP
#define UMATH_HPP

/****************************************************************
 * Constants
 ****************************************************************/

#define UNUM_LIBRARY_MATH_ERROR (1e-4F)
#define UNUM_LIBRARY_MATH_EPSILON (1e-8F)
#define UNUM_LIBRARY_MATH_INFINITY (1e+10F)
#define UNUM_LIBRARY_MATH_PI (3.14159265358979323846F)
#define UNUM_LIBRARY_MATH_2_PI (UNUM_MATH_PI * 2.0F)
#define UNUM_LIBRARY_MATH_PI_2 (UNUM_MATH_PI / 2.0F)
#define UNUM_LIBRARY_MATH_DEG (UNUM_MATH_PI / 180.0F)
#define UNUM_LIBRARY_MATH_RAD (180.0F / UNUM_MATH_PI)

UNUM_DEF f64 unum_math_sin(f64 f)
{
    return (f64)sin((f64)f);
}

UNUM_DEF const UnumInternalNative UNUM_NATIVE_MATH[] =
{
    [0] = { .name = "unum_math_sin", .function = (any) unum_math_sin,
            .params = (UnumInternalObject**){&UNUM_OBJECT_PRIMITIVES[UNUM_PRIMITIVE_F64],},
            .result = (UnumInternalObject**){&UNUM_OBJECT_PRIMITIVES[UNUM_PRIMITIVE_F64],} }
};

UNUM_DEF const str UNUM_LIBRARY_MATH =
        #include "generated/Math.h"
        ;


#endif // UMATH_HPP
