/**
 * @file OperationItem.h
 * @brief Operation Item (minimum common operador/operand data) interface file.
 * @author Guillermo M. Paris
 * @date 2019-10-27
 */

#ifndef _OPERATIONITEM_H
#define _OPERATIONITEM_H

#include <cstdint>

enum class OperationId : char
{
    first = 0, number = first, openParenthesis, closeParenthesis, power, multiply, divide, plus, minus, total
};

struct OperationItem
{
    OperationItem() = delete;
    OperationItem(OperationId oid);
    OperationItem(OperationId oid, int value);
    OperationItem(OperationId oid, char pri, char sym, char val) : id(oid), priority(pri), symbol(sym), value(char(val)) {}

    static void adjustPriorityAndSymbolAccordingToId(OperationItem& objToSet);
    
    static OperationItem  operandTable[static_cast<size_t>(OperationId::total)];

    OperationId id;
    char        priority;
    char        symbol;
    int8_t      value; // For only one digit in this example. Change to double for future general calculator use.
};


//  static OperantionItem operandTable[static_cast<size_t>(OperationId::total)];

#endif // _OPERATIONITEM_H
