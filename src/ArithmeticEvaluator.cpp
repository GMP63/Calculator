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
        return nodeData.value;

    case OperationId::sin:
        return sin(resultRight);
 
    case OperationId::cos:
        return cos(resultRight);
 
    case OperationId::tan:
        return tan(resultRight);

    case OperationId::sinh:
        return sinh(resultRight);
 
    case OperationId::cosh:
        return cosh(resultRight);
 
    case OperationId::tanh:
        return tanh(resultRight);
 
    case OperationId::exp:
        return exp(resultRight);
 
    case OperationId::asin:
        return asin(resultRight);
 
    case OperationId::acos:
        return acos(resultRight);
 
    case OperationId::atan:
        return atan(resultRight);
 
    case OperationId::asinh:
        return asinh(resultRight);
 
    case OperationId::acosh:
        return acosh(resultRight);
 
    case OperationId::atanh:
        return atanh(resultRight);

    case OperationId::ln:
        return log(resultRight);

    case OperationId::log10:
        return log10(resultRight);

    case OperationId::log2:
        return log2(resultRight);

    case OperationId::sqrroot:
        assert(resultRight >= 0);
        return sqrt(resultRight);

    case OperationId::cubroot:
        if (resultRight == 0)
            return 0;
        else
        {
            bool positive = (resultRight > 0 ? true : false);
            double result = exp(log(positive ? resultRight : -resultRight)/3);
            return (positive ? result : -result); 
        }

    case OperationId::gamma:
        return tgamma(resultRight);

    case OperationId::factorial:
        return factorial(resultLeft);

    case OperationId::power:
        return pow(resultLeft, resultRight);

    case OperationId::multiply:
        return resultLeft * resultRight;

    case OperationId::divide:
        return resultLeft / resultRight;

    case OperationId::reminder:
    {
        return fmod(resultLeft, resultRight);
    }

    case OperationId::positive: // + absolute value
        return (resultRight > 0 ? resultRight : -resultRight);

    case OperationId::negative:
        return -resultRight;

    case OperationId::plus:
        return resultLeft + resultRight;

    case OperationId::minus:
        return resultLeft - resultRight;

    default:
        assert(false);
        return 0.0;
    }
}

double ArithmeticEvaluator::factorial(double n)
{
	long long l, retVal = 1, m = static_cast<long>(n);
	for(l = 2; l <= m; l++)
        retVal *= l;

	return static_cast<double>(retVal);
}



