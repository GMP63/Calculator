/**
 * @file ExpressionParser.h
 * @brief Character expression parser and operand/operator/operand binary tree builder.
 *        Interface file.
 * @author Guillermo M. Paris
 * @date 2019-10-27
 */

#ifndef _EXPRESSIONPARSER_H
#define _EXPRESSIONPARSER_H

#include <iostream>
#include <unordered_map>
#include "OperationId.h"
#include "Tree.h"

struct OperationItem;

class ExpressionParser
{
    static const int rootMargin = 2;
    static const int indentMargin = 8;
    static const int maxNumberOfDigits = 20; // for max of uint_64_t

public:

    enum class Error
    {
        first = 0, success = 0, voidExpression, contiguousOp,
        missingOp, incorrectDecimalPoint, tooManyDigits,
        unknownFunction, unknownChar, noMatchingParenthesis, total
    };

    enum class Verbosity // for debug purpose, only
    {
        none = 0, partial, full, extra
    };

    ExpressionParser() = delete;
    ExpressionParser(const char* pcExpr, Verbosity v);
    ~ExpressionParser()                     {if(pTree != nullptr) destroyTree();}

    Tree<OperationItem>* getTree()    const {return pTree;}
    Error getError()                  const {return lastError;}
    int   getIntError()               const {return static_cast<int>(lastError);}
    bool  finishedOK()                const {return lastError == Error::success;}
    const char* getLastErrorMessage() const {return errors[int(lastError)];}
    char  getFaultyChar()             const {return cLastParsed;}
    int   getExpressionIndex()        const {return lastIndex;}
    void  setVerbosity(Verbosity v)         {verbosity = v;}
    void  printTree(std::ostream& os) const {printNode(pTree->getRoot(), rootMargin, os);}

    std::ostream& operator << (std::ostream& os);

    static const char* getErrorMessage(int index);

private:
    enum class SearchStrategy : char
    {
        leftToRigth = 0,
        noIterate,
        rightToLeft
    };

    void  populateFunctionNamesTable();
    const char* expressionSanityCheck(const char* pcExpression);
    bool  parseNumberForward(double& returnValue, const char* & currentLine);
    bool  parseAlphabeticForward(OperationId& returnOp, const char* & currentLine);
    bool  parseNewItem(const char* & currentLine, SearchStrategy& newStrategy, OperationItem* newItemToComplete);
    void  parseExpression(const char* pcExpression);
    void  removeFakeOpenParenthesisRoot();
    void  printNode(const Node<OperationItem>* node, int indent, std::ostream& os, bool norecursive = false) const;
    void  destroyNode(Node<OperationItem>* const node, bool norecursive = false);
    void  destroyTree();

    static const char*  errors[static_cast<size_t>(Error::total)];

    Verbosity      verbosity;
    Error          lastError;
    char           cLastParsed;
    int            lastIndex;
    const char*    szExpression;
    Tree<OperationItem>* pTree;
    std::unordered_map<uint32_t, OperationId> functionNames;
};

#endif // _EXPRESSIONPARSER_H