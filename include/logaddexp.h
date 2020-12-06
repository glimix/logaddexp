#ifndef LOGADDEXP_H
#define LOGADDEXP_H

#define LOGADDEXP_VERSION_MAJOR 1
#define LOGADDEXP_VERSION_MINOR 1
#define LOGADDEXP_VERSION_PATCH 0

#define LOGADDEXP_VERSION "1.1.0"

/* For Windows. */
#define _USE_MATH_DEFINES

#include <float.h>
#include <math.h>

/* Computes ㏒ₑ(𝑒ˣ + 𝑒ʸ) in safe and accurate way.
 *
 * For example, `log(exp(1e3) + exp(-INFINITY))` will likely overflow,
 * while `logaddexp(1e3, -INFINITY)` will return `1e3`.
 */
inline static double logaddexp(double x, double y)
{
    double const tmp = x - y;

    if (x == y)
        return x + M_LN2;

    if (tmp > 0)
        return x + log1p(exp(-tmp));
    else if (tmp <= 0)
        return y + log1p(exp(tmp));

    return tmp;
}

inline static float logaddexpf(float x, float y)
{
    float const tmp = x - y;

    if (x == y)
        return x + M_LN2;

    if (tmp > 0)
        return x + log1pf(expf(-tmp));
    else if (tmp <= 0)
        return y + log1pf(expf(tmp));

    return tmp;
}

/* Computes ㏒ₑ(𝑠ₓ⋅𝑒ˣ + 𝑠ᵧ⋅𝑒ʸ) in safe and accurate way.
 *
 * It is a weighted version of `logaddexp`, assuming that
 * 𝑠ₓ⋅𝑒ˣ + 𝑠ᵧ⋅𝑒ʸ > 0.
 */
inline static double logaddexps(double x, double y, double sx, double sy)
{
    double const tmp = x - y;

    double const sxx = log(fabs(sx)) + x;
    double const syy = log(fabs(sy)) + y;

    if (sxx == syy) {
        if (sx * sy > 0)
            return sxx + M_LN2;
        return -DBL_MAX;
    }

    if (sx > 0 && sy > 0) {
        if (tmp > 0)
            return sxx + log1p((sy / sx) * exp(-tmp));
        else if (tmp <= 0)
            return syy + log1p((sx / sy) * exp(tmp));
    } else if (sx > 0)
        return sxx + log1p((sy / sx) * exp(-tmp));
    else
        return syy + log1p((sx / sy) * exp(tmp));
    return tmp;
}

inline static float logaddexpsf(float x, float y, float sx, float sy)
{
    float const tmp = x - y;

    float const sxx = logf(fabsf(sx)) + x;
    float const syy = logf(fabsf(sy)) + y;

    if (sxx == syy) {
        if (sx * sy > 0)
            return sxx + M_LN2;
        return -FLT_MAX;
    }

    if (sx > 0 && sy > 0) {
        if (tmp > 0)
            return sxx + log1pf((sy / sx) * expf(-tmp));
        else if (tmp <= 0)
            return syy + log1pf((sx / sy) * expf(tmp));
    } else if (sx > 0)
        return sxx + log1pf((sy / sx) * expf(-tmp));
    else
        return syy + log1pf((sx / sy) * expf(tmp));
    return tmp;
}

/* Computes ㏒ₑ(|𝑐|) and 𝑐/|𝑐|, for 𝑐 = 𝑠ₓ⋅𝑒ˣ + 𝑠ᵧ⋅𝑒ʸ.
 *
 * It is a generalisation of `logaddexps`.
 */
inline static double logaddexpg(double x, double y, double sx, double sy, double *sign)
{
    double const sxx = log(fabs(sx)) + x;
    double const syy = log(fabs(sy)) + y;

    if (sxx == syy) {
        if (sx * sy > 0) {
            if (sx > 0)
                *sign = +1.0;
            else
                *sign = -1.0;
            return sxx + M_LN2;
        } else {
            *sign = 1.0;
            return -DBL_MAX;
        }
    }

    if (sxx > syy) {
        if (sx >= 0.0)
            *sign = +1.0;
        else
            *sign = -1.0;
    } else {
        if (sy >= 0.0)
            *sign = +1.0;
        else
            *sign = -1.0;
    }

    sx *= *sign;
    sy *= *sign;
    return logaddexps(x, y, sx, sy);
}

inline static float logaddexpgf(float x, float y, float sx, float sy, float *sign)
{
    float const sxx = logf(fabsf(sx)) + x;
    float const syy = logf(fabsf(sy)) + y;

    if (sxx == syy) {
        if (sx * sy > 0) {
            if (sx > 0)
                *sign = +1.0;
            else
                *sign = -1.0;
            return sxx + M_LN2;
        } else {
            *sign = 1.0;
            return -FLT_MAX;
        }
    }

    if (sxx > syy) {
        if (sx >= 0.0)
            *sign = +1.0;
        else
            *sign = -1.0;
    } else {
        if (sy >= 0.0)
            *sign = +1.0;
        else
            *sign = -1.0;
    }

    sx *= *sign;
    sy *= *sign;
    return logaddexpsf(x, y, sx, sy);
}

/**
 * Deprecated since version 1.1.0. Please, use `logaddexpg` instead.
 */
inline static double logaddexpss(double x, double y, double sx, double sy, double *sign)
{
    return logaddexpg(x, y, sx, sy, sign);
}

#endif
