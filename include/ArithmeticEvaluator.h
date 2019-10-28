/**
 * @file ArithmeticEvaluator.h
 * @brief Evaluator of a pure operand/operator/operand binary tree. Interface file.
 * @author Guillermo M. Paris
 * @date 2019-10-27
 */

#ifndef _ARITHMETICEVALUATOR_H
#define _ARITHMETICEVALUATOR_H

#include "Tree.h"

struct OperationItem;

class ArithmeticEvaluator
{
public:
    ArithmeticEvaluator(Tree<OperationItem>* ptree)
    : lastError(0), pTree(ptree) {result = evaluateNode(ptree->getRoot());}

    double getResult() {return result;}
    int    getError()  {return lastError;}
    operator bool()    {return lastError == 0;}

private:
    double evaluateNode(const Node<OperationItem>* node);

    int    lastError;
    double result;
    Tree<OperationItem>* pTree;
};

#endif // _ARITHMETICEVALUATOR_H
