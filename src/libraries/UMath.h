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

// Refer to https://en.wikipedia.org/wiki/Inverse_trigonometric_functions

#define UNUM_LIBRARY_MATH_ERROR (1e-4F)
#define UNUM_LIBRARY_MATH_EPSILON (1e-8F)
#define UNUM_LIBRARY_MATH_INFINITY (1e+10F)
#define UNUM_LIBRARY_MATH_PI (3.14159265358979323846F)
#define UNUM_LIBRARY_MATH_2_PI (UNUM_LIBRARY_MATH_PI * 2.0F)
#define UNUM_LIBRARY_MATH_PI_2 (UNUM_LIBRARY_MATH_PI / 2.0F)
#define UNUM_LIBRARY_MATH_DEG (UNUM_LIBRARY_MATH_PI / 180.0F)
#define UNUM_LIBRARY_MATH_RAD (180.0F / UNUM_LIBRARY_MATH_PI)

// Convenience macros
#define ULIB_INAT(idx, ty, f) *((ty*)((UnumInternalObjSingle*)f->objects[idx].data)->data)
#define ULIB_IN1(ty, f) ULIB_INAT(1, ty, f)
#define ULIB_OUT1(ty, f, val) \
    ty* v = UNUM_MALLOC(sizeof(ty) * 1); \
    *v = val; \
    UnumInternalObjSingle* res = UNUM_MALLOC(sizeof(UnumInternalObjSingle) * 1); \
    res->data = v; \
    UnumInternalPair p = Unum_Internal_Execute_Id(c, #ty ); \
    res->type = (UnumInternalObjType*)(Unum_Internal_Execute_Level(c, p.a)->objects[p.b].data); \
    return (UnumInternalObject) {.name = SAFE, .type = UNUM_OBJ_SINGLE, .data = res}

/****************************************************************
 * Native Functions
 ****************************************************************/

// Safely assume types match

// Addition function
static UnumInternalObject unum_lib_math_add(UnumInstance* c, UnumInternalStack* f)
{
    f64 a = ULIB_INAT(1, f64, f);
    f64 b = ULIB_INAT(2, f64, f);
    f64 o = a + b;
    ULIB_OUT1(f64, f, o);
}

// Subtraction function
static UnumInternalObject unum_lib_math_sub(UnumInstance* c, UnumInternalStack* f)
{
    f64 a = ULIB_INAT(1, f64, f);
    f64 b = ULIB_INAT(2, f64, f);
    f64 o = a - b;
    ULIB_OUT1(f64, f, o);
}

// Multiplication function
static UnumInternalObject unum_lib_math_mul(UnumInstance* c, UnumInternalStack* f)
{
    f64 a = ULIB_INAT(1, f64, f);
    f64 b = ULIB_INAT(2, f64, f);
    f64 o = a * b;
    ULIB_OUT1(f64, f, o);
}

// Division function
static UnumInternalObject unum_lib_math_div(UnumInstance* c, UnumInternalStack* f)
{
    f64 a = ULIB_INAT(1, f64, f);
    f64 b = ULIB_INAT(2, f64, f);
    f64 o = a / b;
    ULIB_OUT1(f64, f, o);
}

// Logb function
static UnumInternalObject unum_lib_math_logb(UnumInstance* c, UnumInternalStack* f)
{
    f64 base = ULIB_INAT(1, f64, f);
    f64 num = ULIB_INAT(2, f64, f);
    f64 o = log(num) / log(base);
    ULIB_OUT1(f64, f, o);
}

// Pow function
static UnumInternalObject unum_lib_math_pow(UnumInstance* c, UnumInternalStack* f)
{
    f64 base = ULIB_INAT(1, f64, f);
    f64 power = ULIB_INAT(2, f64, f);
    f64 o = pow(base, power);
    ULIB_OUT1(f64, f, o);
}

// Sin function
static UnumInternalObject unum_lib_math_sin(UnumInstance* c, UnumInternalStack* f)
{
    f64 i = ULIB_IN1(f64, f);
    f64 o = sin(i);
    ULIB_OUT1(f64, f, o);
}

// Cos function
static UnumInternalObject unum_lib_math_cos(UnumInstance* c, UnumInternalStack* f)
{
    f64 i = ULIB_IN1(f64, f);
    f64 o = cos(i);
    ULIB_OUT1(f64, f, o);
}

// Tan function
static UnumInternalObject unum_lib_math_tan(UnumInstance* c, UnumInternalStack* f)
{
    f64 i = ULIB_IN1(f64, f);
    f64 o = tan(i);
    ULIB_OUT1(f64, f, o);
}

// Sec function
static UnumInternalObject unum_lib_math_sec(UnumInstance* c, UnumInternalStack* f)
{
    f64 i = ULIB_IN1(f64, f);
    f64 o = 1.0 / cos(i);
    ULIB_OUT1(f64, f, o);
}

// Csc function
static UnumInternalObject unum_lib_math_csc(UnumInstance* c, UnumInternalStack* f)
{
    f64 i = ULIB_IN1(f64, f);
    f64 o = 1.0 / sin(i);
    ULIB_OUT1(f64, f, o);
}

// Cot function
static UnumInternalObject unum_lib_math_cot(UnumInstance* c, UnumInternalStack* f)
{
    f64 i = ULIB_IN1(f64, f);
    f64 o = 1.0 / tan(i);
    ULIB_OUT1(f64, f, o);
}

// Inverse sin function
static UnumInternalObject unum_lib_math_asin(UnumInstance* c, UnumInternalStack* f)
{
    f64 i = ULIB_IN1(f64, f);
    f64 o = asin(i);
    ULIB_OUT1(f64, f, o);
}

// Inverse cos function
static UnumInternalObject unum_lib_math_acos(UnumInstance* c, UnumInternalStack* f)
{
    f64 i = ULIB_IN1(f64, f);
    f64 o = acos(i);
    ULIB_OUT1(f64, f, o);
}

// Inverse tan function
static UnumInternalObject unum_lib_math_atan(UnumInstance* c, UnumInternalStack* f)
{
    f64 i = ULIB_IN1(f64, f);
    f64 o = atan(i);
    ULIB_OUT1(f64, f, o);
}

// Inverse sec function
static UnumInternalObject unum_lib_math_asec(UnumInstance* c, UnumInternalStack* f)
{
    f64 i = ULIB_IN1(f64, f);
    f64 o = acos(1.0 / i);
    ULIB_OUT1(f64, f, o);
}

// Inverse csc function
static UnumInternalObject unum_lib_math_acsc(UnumInstance* c, UnumInternalStack* f)
{
    f64 i = ULIB_IN1(f64, f);
    f64 o = asin(1.0 / i);
    ULIB_OUT1(f64, f, o);
}

// Inverse cot function
static UnumInternalObject unum_lib_math_acot(UnumInstance* c, UnumInternalStack* f)
{
    f64 i = ULIB_IN1(f64, f);
    f64 o = UNUM_LIBRARY_MATH_PI_2 - atan(i);
    ULIB_OUT1(f64, f, o);
}

// Inverse tan (2) function
static UnumInternalObject unum_lib_math_atan2(UnumInstance* c, UnumInternalStack* f)
{
    f64 y = ULIB_INAT(1, f64, f);
    f64 x = ULIB_INAT(2, f64, f);
    f64 o = atan2(y, x);
    ULIB_OUT1(f64, f, o);
}

// Deg function
static UnumInternalObject unum_lib_math_deg(UnumInstance* c, UnumInternalStack* f)
{
    f64 i = ULIB_IN1(f64, f);
    f64 o = i * UNUM_LIBRARY_MATH_DEG;
    ULIB_OUT1(f64, f, o);
}

// Rad function
static UnumInternalObject unum_lib_math_rad(UnumInstance* c, UnumInternalStack* f)
{
    f64 i = ULIB_IN1(f64, f);
    f64 o = i * UNUM_LIBRARY_MATH_RAD;
    ULIB_OUT1(f64, f, o);
}

// Floor function
static UnumInternalObject unum_lib_math_floor(UnumInstance* c, UnumInternalStack* f)
{
    f64 i = ULIB_IN1(f64, f);
    f64 o = floor(i);
    ULIB_OUT1(f64, f, o);
}

// Ceil function
static UnumInternalObject unum_lib_math_ceil(UnumInstance* c, UnumInternalStack* f)
{
    f64 i = ULIB_IN1(f64, f);
    f64 o = ceil(i);
    ULIB_OUT1(f64, f, o);
}

// Round function
static UnumInternalObject unum_lib_math_round(UnumInstance* c, UnumInternalStack* f)
{
    f64 i = ULIB_IN1(f64, f);
    f64 o = round(i);
    ULIB_OUT1(f64, f, o);
}

// Abs function
static UnumInternalObject unum_lib_math_abs(UnumInstance* c, UnumInternalStack* f)
{
    f64 i = ULIB_IN1(f64, f);
    f64 o = fabs(i);
    ULIB_OUT1(f64, f, o);
}

/****************************************************************
 * Function Loading
 ****************************************************************/

// Function table
static UnumInternalNative UNUM_NATIVE_MATH[25];

// Unum code
static const str UNUM_LIBRARY_MATH =
        #include "generated/Math.h"
        ;

// Function table initialization
static bool Unum_Lib_Math_Load()
{
    size counter = 0;

    UnumInternalObject unum_lib_internal_math_f64_params[] = { [0] = UNUM_OBJECT_PRIMITIVES[UNUM_PRIMITIVE_F64], [1] = UNUM_OBJECT_PRIMITIVES[UNUM_PRIMITIVE_F64] };
    UnumInternalObject unum_lib_internal_math_f64_param[] = { [0] = UNUM_OBJECT_PRIMITIVES[UNUM_PRIMITIVE_F64] };
    UnumInternalObject unum_lib_internal_math_f64_result = UNUM_OBJECT_PRIMITIVES[UNUM_PRIMITIVE_F64];

    UNUM_NATIVE_MATH[counter++] =
            (UnumInternalNative)
    {
            .name = "unum_math_add",
            .function = unum_lib_math_add,
            .params = unum_lib_internal_math_f64_params,
            .result = unum_lib_internal_math_f64_result
    };

    UNUM_NATIVE_MATH[counter++] =
            (UnumInternalNative)
    {
            .name = "unum_math_sub",
            .function = unum_lib_math_sub,
            .params = unum_lib_internal_math_f64_params,
            .result = unum_lib_internal_math_f64_result
    };

    UNUM_NATIVE_MATH[counter++] =
            (UnumInternalNative)
    {
            .name = "unum_math_mul",
            .function = unum_lib_math_mul,
            .params = unum_lib_internal_math_f64_params,
            .result = unum_lib_internal_math_f64_result
    };

    UNUM_NATIVE_MATH[counter++] =
            (UnumInternalNative)
    {
            .name = "unum_math_div",
            .function = unum_lib_math_div,
            .params = unum_lib_internal_math_f64_params,
            .result = unum_lib_internal_math_f64_result
    };

    UNUM_NATIVE_MATH[counter++] =
            (UnumInternalNative)
    {
            .name = "unum_math_sin",
            .function = unum_lib_math_sin,
            .params = unum_lib_internal_math_f64_param,
            .result = unum_lib_internal_math_f64_result
    };

    UNUM_NATIVE_MATH[counter++] =
            (UnumInternalNative)
    {
            .name = "unum_math_cos",
            .function = unum_lib_math_cos,
            .params = unum_lib_internal_math_f64_param,
            .result = unum_lib_internal_math_f64_result
    };

    UNUM_NATIVE_MATH[counter++] =
            (UnumInternalNative)
    {
            .name = "unum_math_tan",
            .function = unum_lib_math_tan,
            .params = unum_lib_internal_math_f64_param,
            .result = unum_lib_internal_math_f64_result
    };

    UNUM_NATIVE_MATH[counter++] =
            (UnumInternalNative)
    {
            .name = "unum_math_sec",
            .function = unum_lib_math_sec,
            .params = unum_lib_internal_math_f64_param,
            .result = unum_lib_internal_math_f64_result
    };

    UNUM_NATIVE_MATH[counter++] =
            (UnumInternalNative)
    {
            .name = "unum_math_csc",
            .function = unum_lib_math_csc,
            .params = unum_lib_internal_math_f64_param,
            .result = unum_lib_internal_math_f64_result
    };

    UNUM_NATIVE_MATH[counter++] =
            (UnumInternalNative)
    {
            .name = "unum_math_cot",
            .function = unum_lib_math_cot,
            .params = unum_lib_internal_math_f64_param,
            .result = unum_lib_internal_math_f64_result
    };

    UNUM_NATIVE_MATH[counter++] =
            (UnumInternalNative)
    {
            .name = "unum_math_asin",
            .function = unum_lib_math_asin,
            .params = unum_lib_internal_math_f64_param,
            .result = unum_lib_internal_math_f64_result
    };

    UNUM_NATIVE_MATH[counter++] =
            (UnumInternalNative)
    {
            .name = "unum_math_acos",
            .function = unum_lib_math_acos,
            .params = unum_lib_internal_math_f64_param,
            .result = unum_lib_internal_math_f64_result
    };

    UNUM_NATIVE_MATH[counter++] =
            (UnumInternalNative)
    {
            .name = "unum_math_atan",
            .function = unum_lib_math_atan,
            .params = unum_lib_internal_math_f64_param,
            .result = unum_lib_internal_math_f64_result
    };

    UNUM_NATIVE_MATH[counter++] =
            (UnumInternalNative)
    {
            .name = "unum_math_asec",
            .function = unum_lib_math_asec,
            .params = unum_lib_internal_math_f64_param,
            .result = unum_lib_internal_math_f64_result
    };

    UNUM_NATIVE_MATH[counter++] =
            (UnumInternalNative)
    {
            .name = "unum_math_acsc",
            .function = unum_lib_math_acsc,
            .params = unum_lib_internal_math_f64_param,
            .result = unum_lib_internal_math_f64_result
    };

    UNUM_NATIVE_MATH[counter++] =
            (UnumInternalNative)
    {
            .name = "unum_math_acot",
            .function = unum_lib_math_acot,
            .params = unum_lib_internal_math_f64_param,
            .result = unum_lib_internal_math_f64_result
    };

    UNUM_NATIVE_MATH[counter++] =
            (UnumInternalNative)
    {
            .name = "unum_math_atan2",
            .function = unum_lib_math_atan2,
            .params = unum_lib_internal_math_f64_params,
            .result = unum_lib_internal_math_f64_result
    };

    UNUM_NATIVE_MATH[counter++] =
            (UnumInternalNative)
    {
            .name = "unum_math_deg",
            .function = unum_lib_math_deg,
            .params = unum_lib_internal_math_f64_param,
            .result = unum_lib_internal_math_f64_result
    };

    UNUM_NATIVE_MATH[counter++] =
            (UnumInternalNative)
    {
            .name = "unum_math_rad",
            .function = unum_lib_math_rad,
            .params = unum_lib_internal_math_f64_param,
            .result = unum_lib_internal_math_f64_result
    };

    UNUM_NATIVE_MATH[counter++] =
            (UnumInternalNative)
    {
            .name = "unum_math_logb",
            .function = unum_lib_math_logb,
            .params = unum_lib_internal_math_f64_params,
            .result = unum_lib_internal_math_f64_result
    };

    UNUM_NATIVE_MATH[counter++] =
            (UnumInternalNative)
    {
            .name = "unum_math_pow",
            .function = unum_lib_math_pow,
            .params = unum_lib_internal_math_f64_params,
            .result = unum_lib_internal_math_f64_result
    };

    UNUM_NATIVE_MATH[counter++] =
            (UnumInternalNative)
    {
            .name = "unum_math_floor",
            .function = unum_lib_math_floor,
            .params = unum_lib_internal_math_f64_param,
            .result = unum_lib_internal_math_f64_result
    };

    UNUM_NATIVE_MATH[counter++] =
            (UnumInternalNative)
    {
            .name = "unum_math_ceil",
            .function = unum_lib_math_ceil,
            .params = unum_lib_internal_math_f64_param,
            .result = unum_lib_internal_math_f64_result
    };

    UNUM_NATIVE_MATH[counter++] =
            (UnumInternalNative)
    {
            .name = "unum_math_round",
            .function = unum_lib_math_round,
            .params = unum_lib_internal_math_f64_param,
            .result = unum_lib_internal_math_f64_result
    };

    UNUM_NATIVE_MATH[counter++] =
            (UnumInternalNative)
    {
            .name = "unum_math_abs",
            .function = unum_lib_math_abs,
            .params = unum_lib_internal_math_f64_param,
            .result = unum_lib_internal_math_f64_result
    };

    return UNUM_TRUE;
}

#endif // UMATH_HPP
