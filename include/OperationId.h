/**
 * @file OperationId.h
 * @brief Operation ID Enumeration for all the possible operations.
 * @author Guillermo M. Paris
 * @date 2019-11-01
 */

#ifndef _OPERATIONID_H
#define _OPERATIONID_H

enum class OperationId : char
{
    first = 0, number = first, e, pi, phi, openParenthesis, closeParenthesis,
    firstFunction, sin = firstFunction, cos, tan, sinh, cosh, tanh, exp,
    asin, acos, atan, asinh, acosh, atanh, ln, log10, log2, sqrroot, cubroot,
    lastFunction, gamma = lastFunction,
    factorial, power, multiply, divide, reminder, positive, negative , plus, minus, total
};

#endif // _OPERATIONID_H
