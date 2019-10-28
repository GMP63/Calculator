/**
 * @file test.cpp
 * @brief Unit tests file.
 * @author Guillermo M. Paris
 * @date 2019-10-27
 */

#include <cassert>
#include <cstddef>
#include <iostream>
#include "test-macros.h"
#include "NodeFactory.h"
#include "ExpressionParser.h"
#include "ArithmeticEvaluator.h"
#include "OperationItem.h"

void nodeTests(TEST_REF)
{
    NodeFactory<OperationItem>* factory = NodeFactory<OperationItem>::getOrCreateInstance();
    Node<OperationItem>* pA = factory->createNode(OperationItem(OperationId::openParenthesis));
    Node<OperationItem>* pB = factory->createNode(OperationItem(OperationId::number, 255));
    pB->setParent(nullptr); // B will be the root node
    Node<OperationItem>* pC = factory->createNode(OperationItem(OperationId::closeParenthesis));
    Tree<OperationItem> tree(pB); // B is now the root node

    tree.insertUnderCurrentOnLeft(pA); // A is now left child of B
    tree.insertUnderCurrentOnRight(pC); // C is now right child of B
    EXPECT_EQ(pB->getLeft(), pA);
    EXPECT_EQ(pB->getRight(), pC);
    EXPECT_EQ(pA->getParent(), pB);
    EXPECT_EQ(pC->getParent(), pB);
    EXPECT_TRUE(pB->isParentOf(pA));
    EXPECT_TRUE(pB->isParentOf(pC));
    EXPECT_TRUE(pA->isLeftChildOf(pB));
    EXPECT_TRUE(pC->isRightChildOf(pB));
    factory->destroyNode(pA);
    factory->destroyNode(pB);
    factory->destroyNode(pC);
}

void badParsingTests(TEST_REF)
{
    ExpressionParser::Error error0 =  ExpressionParser::Error::success;
    ExpressionParser::Error error1 =  ExpressionParser::Error::voidExpression;
    ExpressionParser::Error error2 =  ExpressionParser::Error::unaryOp;
    ExpressionParser::Error error3 =  ExpressionParser::Error::contiguousOp;
    ExpressionParser::Error error4 =  ExpressionParser::Error::multidigit;
    ExpressionParser::Error error5 =  ExpressionParser::Error::decimal;
    ExpressionParser::Error error6 =  ExpressionParser::Error::unknownChar;
    ExpressionParser::Error error7 =  ExpressionParser::Error::noMatchingParenthesis;

    const ExpressionParser::Error errors[8] =
    {
        error0, error1, error2, error3,
        error4, error5, error6, error7
    };

    static const char* expression0 = "1 + "; // mising digit is interpreted as "void" meaning 0
    static const char* expression1 = "\n  	  \n";
    static const char* expression2 = "-1 + 1";
    static const char* expression3 = "2 ** 3";
    static const char* expression4 = "1 + 10";
    static const char* expression5 = "1.1 + 1";
    static const char* expression6 = "1 @ 1";
    static const char* expression7 = "1 + (4 * 2";

    static const char* expressions[8] =
    {
        expression0, expression1, expression2, expression3,
        expression4, expression5, expression6, expression7
    };

    for(int i = 0; i < 8; i++)
    {
        ExpressionParser parser(expressions[i], ExpressionParser::Verbosity::none);
        if (i == 0)
            EXPECT_TRUE(parser.finishedOK());
        else
            EXPECT_FALSE(parser.finishedOK());

        EXPECT_EQ(int(parser.getError()), int(errors[i]));
    }
}

void parseAndEvaluatorTests(TEST_REF)
{
    const int result1 =  2;
    const int result2 = 14;
    const int result3 =  3;
    const int result4 = 10;
    const int result5 = 22;
    const int results[5] = {result1, result2, result3, result4, result5};

    static const char* expression1 = "1 + 1";
    static const char* expression2 = "5-6/2+3*4";
    static const char* expression3 = "3+4*(2+1*1*(4-(1+1)))-(9 + 7)";
    static const char* expression4 = "1*2*3*(2-1/3)";
    static const char* expression5 = "(4 + 5 * (7 - 3)) - 2";
    static const char* expressions[5] = {expression1, expression2, expression3,expression4, expression5};

    for(int i = 0; i < 5; i++)
    {
        ExpressionParser parser(expressions[i], ExpressionParser::Verbosity::none);
        EXPECT_TRUE(parser.finishedOK());
        EXPECT_EQ(ArithmeticEvaluator(parser.getTree()).getResult(), results[i]);
    }

    ExpressionParser parser("4+5+7/2", ExpressionParser::Verbosity::none);
    EXPECT_TRUE(parser.finishedOK());
    EXPECT_EQ(ArithmeticEvaluator(parser.getTree()).getResult(), 12.5);
}

template <>
NodeFactory<OperationItem>* NodeFactory<OperationItem>::pInstance(nullptr);

int main(int argc, char* argv[])
{
    START_TESTS;
    NodeFactory<OperationItem>* factory = NodeFactory<OperationItem>::getOrCreateInstance();

    nodeTests(TEST);
    badParsingTests(TEST);
    parseAndEvaluatorTests(TEST);

    NodeFactory<OperationItem>::destroyInstance();
    PRINT_RESULTS(std::cout);
}

