/**
 * @file OperationItem.cpp
 * @brief Operation Item (minimum common operador/operand data) implementation file.
 * @author Guillermo M. Paris
 * @date 2019-10-27
 */

#include <cstddef>
#include "OperationItem.h"

OperationItem  OperationItem::operandTable[static_cast<size_t>(OperationId::total)] =
{
    {OperationId::number,           0, '0', 0},
    {OperationId::openParenthesis,  4, '(', 0},
    {OperationId::closeParenthesis, 4, ')', 0},
    {OperationId::power,            1, '^', 0},
    {OperationId::multiply,         2, '*', 0},
    {OperationId::divide,           2, '/', 0},
    {OperationId::plus,             3, '+', 0},
    {OperationId::minus,            3, '-', 0}
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
