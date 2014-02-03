/*
 * Copyright (C) 2006, 2007 Apple Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef WTF_MathExtras_h
#define WTF_MathExtras_h

#include <math.h>

#if PLATFORM(WIN_OS)

#include "kjs/JSImmediate.h"
#include <limits>

#if HAVE(FLOAT_H)
#include <float.h>
#endif

#endif

#ifndef M_PI
const double piDouble = 3.14159265358979323846;
const float piFloat = 3.14159265358979323846f;
#else
const double piDouble = M_PI;
const float piFloat = static_cast<float>(M_PI);
#endif

#ifndef M_PI_4
const double piOverFourDouble = 0.785398163397448309616;
const float piOverFourFloat = 0.785398163397448309616f;
#else
const double piOverFourDouble = M_PI_4;
const float piOverFourFloat = static_cast<float>(M_PI_4);
#endif     // !BUILDING_KDE__

#if COMPILER(MSVC)

#ifndef BUILDING_KDE__
inline bool isinf(double num)
{
    return !_finite(num) && !_isnan(num);
}
inline bool isnan(double num)
{
    return _isnan(num);
}
inline long lround(double num)
{
    return num > 0 ? num + 0.5 : ceil(num - 0.5);
}
inline long lroundf(float num)
{
    return num > 0 ? num + 0.5f : ceilf(num - 0.5f);
}
inline double round(double num)
{
    return num > 0 ? floor(num + 0.5) : ceil(num - 0.5);
}
inline float roundf(float num)
{
    return num > 0 ? floorf(num + 0.5f) : ceilf(num - 0.5f);
}
inline bool signbit(double num)
{
    return _copysign(1.0, num) < 0;
}
#endif

#ifndef BUILDING_KDE__
// FIXME: where to get std::numeric_limits from?
// Work around a bug in Win, where atan2(+-infinity, +-infinity) yields NaN instead of specific values.
inline double wtf_atan2(double x, double y)
{
    static double posInf = std::numeric_limits<double>::infinity();
    static double negInf = -std::numeric_limits<double>::infinity();

    double result = KJS::NaN;

    if (x == posInf && y == posInf) {
        result = piOverFourDouble;
    } else if (x == posInf && y == negInf) {
        result = 3 * piOverFourDouble;
    } else if (x == negInf && y == posInf) {
        result = -piOverFourDouble;
    } else if (x == negInf && y == negInf) {
        result = -3 * piOverFourDouble;
    } else {
        result = ::atan2(x, y);
    }

    return result;
}
#else      // !BUILDING_KDE__

#define wtf_atan2(x, y) atan2(x, y)

#endif     // !BUILDING_KDE__

#if COMPILER(MSVC)

// Work around a bug in the Microsoft CRT, where fmod(x, +-infinity) yields NaN instead of x.
inline double wtf_fmod(double x, double y)
{
    return (!isinf(x) && isinf(y)) ? x : fmod(x, y);
}

#define fmod(x, y) wtf_fmod(x, y)

#endif // #if COMPILER(MSVC)

#define atan2(x, y) wtf_atan2(x, y)

#endif // #if COMPILER(MSVC)

inline double deg2rad(double d)
{
    return d * piDouble / 180.0;
}
inline double rad2deg(double r)
{
    return r * 180.0 / piDouble;
}
inline double deg2grad(double d)
{
    return d * 400.0 / 360.0;
}
inline double grad2deg(double g)
{
    return g * 360.0 / 400.0;
}
inline double rad2grad(double r)
{
    return r * 200.0 / piDouble;
}
inline double grad2rad(double g)
{
    return g * piDouble / 200.0;
}

inline float deg2rad(float d)
{
    return d * piFloat / 180.0f;
}
inline float rad2deg(float r)
{
    return r * 180.0f / piFloat;
}
inline float deg2grad(float d)
{
    return d * 400.0f / 360.0f;
}
inline float grad2deg(float g)
{
    return g * 360.0f / 400.0f;
}
inline float rad2grad(float r)
{
    return r * 200.0f / piFloat;
}
inline float grad2rad(float g)
{
    return g * piFloat / 200.0f;
}

#endif // #ifndef WTF_MathExtras_h
