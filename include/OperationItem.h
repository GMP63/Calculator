/**
 * @file OperationItem.h
 * @brief Operation Item (minimum common operador/operand data) interface file.
 * @author Guillermo M. Paris
 * @date 2019-10-27
 */

#ifndef _OPERATIONITEM_H
#define _OPERATIONITEM_H

#include <cstdint>
#include "OperationId.h"

struct OperationItem
{
    OperationItem() = delete;
    OperationItem(OperationId oid);
    OperationItem(OperationId oid, int value);
    OperationItem(OperationId oid, char pri, const char* sym, char val)
                 : id(oid), priority(pri), symbol(sym), value(val) {}

    static void adjustPriorityAndSymbolAccordingToId(OperationItem& objToSet);
    
    static const unsigned int   symbolLength = 4;
    static const OperationItem  operandTable[static_cast<size_t>(OperationId::total)];

    OperationId id;
    char        priority;
    const char* symbol;
    double      value;
};


//  static OperantionItem operandTable[static_cast<size_t>(OperationId::total)];

#endif // _OPERATIONITEM_H
