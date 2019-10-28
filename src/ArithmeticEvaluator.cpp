/**
 * @file ArithmeticEvaluator.h
 * @brief Evaluator of a pure operand/operator/operand binary tree. Implementation file.
 * @author Guillermo M. Paris
 * @date 2019-10-27
 */

#include <cmath>
#include <iostream>
#include "ArithmeticEvaluator.h"
#include "OperationItem.h"

double ArithmeticEvaluator::evaluateNode(const Node<OperationItem>* pNode)
{
    if (pNode == nullptr) return 0.0;

    OperationItem nodeData(pNode->getData());

    double resultLeft = evaluateNode(pNode->getLeft());
    double resultRight = evaluateNode(pNode->getRight());

    switch(nodeData.id)
    {
    case OperationId::number:
        return static_cast<double>(nodeData.value);
 
    case OperationId::power:
        return pow(resultLeft, resultRight);

    case OperationId::multiply:
        return resultLeft * resultRight;

    case OperationId::divide:
        return resultLeft / resultRight;

    case OperationId::plus:
        return resultLeft + resultRight;

    case OperationId::minus:
        return resultLeft - resultRight;

    default:
        assert(false);
        return 0.0;
    }
}



