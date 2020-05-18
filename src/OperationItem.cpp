/**
 * @file OperationItem.cpp
 * @brief Operation Item (minimum common operador/operand data) implementation file.
 * @author Guillermo M. Paris
 * @date 2019-10-27
 */

#include <cstddef>
#include "OperationItem.h"

const OperationItem  OperationItem::operandTable[static_cast<size_t>(OperationId::total)] =
{
    {OperationId::number,           0, "num",  0},
    {OperationId::e,                1, "e",    0},
    {OperationId::pi,               1, "pi",   0},
    {OperationId::phi,              1, "phi",  0},
    {OperationId::openParenthesis,  7, "(",    0},
    {OperationId::closeParenthesis, 7, ")",    0},
    {OperationId::sin,              1, "sin",  0},
    {OperationId::cos,              1, "cos",  0},
    {OperationId::tan,              1, "tan",  0},
    {OperationId::sinh,             1, "sinh", 0},
    {OperationId::cosh,             1, "cosh", 0},
    {OperationId::tanh,             1, "tanh", 0},
    {OperationId::exp,              1, "exp",  0},
    {OperationId::asin,             1, "asin", 0},
    {OperationId::acos,             1, "acos", 0},
    {OperationId::atan,             1, "atan", 0},
    {OperationId::asinh,            1, "asih", 0},
    {OperationId::acosh,            1, "acoh", 0},
    {OperationId::atanh,            1, "atah", 0},
    {OperationId::ln,               1, "ln",   0},
    {OperationId::log10,            1, "log",  0},
    {OperationId::log2,             1, "ltwo", 0},
    {OperationId::sqrroot,          1, "sqrt", 0},
    {OperationId::cubroot,          1, "curt", 0},
    {OperationId::gamma,            1, "gama", 0},
    {OperationId::factorial,        2, "!", 0},
    {OperationId::power,            3, "^", 0},
    {OperationId::multiply,         4, "*", 0},
    {OperationId::divide,           4, "/", 0},
    {OperationId::reminder,         4, "%", 0},
    {OperationId::positive,         5, "+", 0},
    {OperationId::negative,         5, "-", 0},
    {OperationId::plus,             6, "+", 0},
    {OperationId::minus,            6, "-", 0}
};

void OperationItem::adjustPriorityAndSymbolAccordingToId(OperationItem& objToSet)
{
    size_t idx = static_cast<size_t>(objToSet.id);
    size_t min = static_cast<size_t>(OperationId::first); // = 0
    size_t max = static_cast<size_t>(OperationId::total);
    if (min <= idx && idx < max)
    {
        objToSet.priority = operandTable[idx].priority;
        objToSet.symbol = operandTable[idx].symbol;
    }
}

OperationItem::OperationItem(OperationId oid)
{
    id = oid;
    priority = operandTable[static_cast<size_t>(id)].priority;
    symbol = operandTable[static_cast<size_t>(id)].symbol;
    value = operandTable[static_cast<size_t>(id)].value;
}

OperationItem::OperationItem(OperationId oid, int val)
{
    id = oid;
    priority = operandTable[static_cast<size_t>(id)].priority;
    symbol = operandTable[static_cast<size_t>(id)].symbol;
    value = val;
}

