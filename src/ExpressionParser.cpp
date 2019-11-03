/**
 * @file ExpressionParser.cpp
 * @brief Character expression parser and operand/operator/operand binary tree builder.
 *        Implementation file.
 * @author Guillermo M. Paris
 * @date 2019-10-27
 */

#include <iomanip>
#include <iostream>
#include <string>
#include "ExpressionParser.h"
#include "NodeFactory.h"
#include "OperationItem.h"

static void debugItem(const char* msg, const OperationItem& item, bool noCR = false)
{
    bool isNumber = (item.id == OperationId::number ? true : false);
    std::cout << (msg ? msg : "") << " (" ;
    if (isNumber)
        std::cout << static_cast<int>(item.value);
    else
        std::cout << item.symbol;
    
    std::cout << ")  " << static_cast<int>(item.priority) << '!'
              << (noCR ? ' ': '\n');
}

static void debugNode(const char* msg, const Node<OperationItem>* pNode)
{
    if (pNode == nullptr)
    {
        std::cout << " NULL POINTER TO NODE !!\n";
        return;
    }

    debugItem(msg, pNode->getData(), true);
    
    int len = 0;
    if (msg)
    {
        len = -1;
        while(msg[++len] != 0);
    }

    std::string sPadding(1 + len, ' ');
    std::cout << "  #" << pNode->getSequenceNo() << "\n" << sPadding
              << "node " << std::hex << pNode << " , parent " << pNode->getParent()
              << ", left " << pNode->getLeft() << " , right " << pNode->getRight()
              << std::dec << "\n";
}

const char*  ExpressionParser::errors[static_cast<size_t>(Error::total)] =
{
    "Ok. No error.",
    "No arithmetic expression to parse.",
    "Unary operators like + or - are not supported in this version.",
    "Two consecutive operators.",
    "Two consecutive digits. Multi-digit numbers are not supported in this version.",
    "Decimal numbers are not supported in this version.",
    "Unknown character.",
    "Unbalanced number of parenthesis."
};

const char* ExpressionParser::getErrorMessage(int index)
{
    const int min = static_cast<int>(Error::first);
    const int max = static_cast<int>(Error::total);
    if (min <= index < max)
       return errors[index];
    else
    {
        return "";
    }
}


ExpressionParser::ExpressionParser(const char* pcExpressionr, Verbosity v)
    : verbosity(v)
    , lastError(Error::success)
    , cLastParsed(0)
    , lastIndex(0)
    , szExpression(pcExpressionr)
    , pTree(nullptr)
{
    parseExpression(pcExpressionr);
}

std::ostream& ExpressionParser::operator << (std::ostream& os)
{
    printTree(os);
    return os;
}

const char*  ExpressionParser::expressionSanityCheck(const char* pcExpression)
{
    if (pcExpression == nullptr || *pcExpression == '\0')
    {
        lastError = Error::voidExpression; // no expression to parse.
        return nullptr;
    }

    bool whiteChar = true;
    char c = 0;
    int l = 0;
    int trimIndex = 0;
    int parenthesisBalance = 0;
    while ((c = pcExpression[l++]) != 0)
    {
        if (c == ' ' ||  c == '\t' || c == '\r' || c == '\n')
            continue;

        if (whiteChar)
        {
            trimIndex = l - 1;
            whiteChar = false;
        }

        if (c == '(')
            parenthesisBalance++;
        else if (c == ')')
            parenthesisBalance--;
    }

    if (whiteChar)
    {
        lastError = Error::voidExpression; // void input expression
        cLastParsed = ' ';
        lastIndex = l - 1;
        return nullptr;
    }
    else if (parenthesisBalance != 0)
    {
        lastError = Error::noMatchingParenthesis; // no matching parenthesis found
        cLastParsed = ' ';
        lastIndex = l - 1;
        return nullptr;
    }
    else if (trimIndex > 0)
    {
        pcExpression += trimIndex; // skip all the "white" leading chars
    }

    return pcExpression;
}

bool  ExpressionParser::generateNewItem(char c, SearchStrategy& newStrategy,
                                        OperationItem* newItemToComplete)
{
    OperationId prevId = newItemToComplete->id; // retrieve the former Id for previous iteration
    newItemToComplete->id = OperationId::number; // default initial values is for number operands
    newItemToComplete->value = 0;

    if(c == '(')
    {
        newItemToComplete->id = OperationId::openParenthesis;
        newStrategy = SearchStrategy::noIterate;
    }
    else if(c == ')')
    {
        newItemToComplete->id = OperationId::closeParenthesis;
        newStrategy = SearchStrategy::rightToLeft;
    }
    else if (c == '+' || c == '-')
    {
        if (prevId != OperationId::number && prevId != OperationId::closeParenthesis)
        {
            lastError = Error::unaryOp; // unary + or - not supported in this version :(.
            cLastParsed = c;
            return false;
        }

        if (c == '+')
            newItemToComplete->id = OperationId::plus;
        else
            newItemToComplete->id = OperationId::minus;
    }
    else if (c == '*' || c == '/' || c == '^')
    {
        if (prevId != OperationId::number && prevId != OperationId::closeParenthesis)
        {
            lastError = Error::contiguousOp; // two consecutive operators.
            cLastParsed = c;
            return false;
        }

        if (c == '*')
            newItemToComplete->id = OperationId::multiply;
        else if (c == '/')
            newItemToComplete->id = OperationId::divide;
        else
        {
            newItemToComplete->id = OperationId::power;
            newStrategy = SearchStrategy::rightToLeft;
        }
    }
    else if('0' <= c &&  c <= '9')  // numeric digit, only one.
    {
        if (prevId == OperationId::number)
        {
            lastError = Error::multidigit; // too many digits
            cLastParsed = c;
            return false;
        }

        newItemToComplete->id = OperationId::number;
        newItemToComplete->value = c - '0';
    }
    else if(c == '.')
    {
        lastError = Error::decimal; // no decimal point in this version :(
        cLastParsed = c;
        return false;
    }
    else // if a parsing error
    {
        lastError = Error::unknownChar; // Unrecognized character
        cLastParsed = c;
        return false;
    }

    return true;
}

void  ExpressionParser::removeFakeOpenParenthesisRoot()
{
    NodeFactory<OperationItem>* pFactory = NodeFactory<OperationItem>::getOrCreateInstance();

    Node<OperationItem>* pCurrentRoot = pTree->getRoot();
    Node<OperationItem>* pNewRoot = pCurrentRoot->getRight();
    if (pNewRoot != nullptr)
    {
        if (verbosity >= Verbosity::full)
        {
            debugNode("Deleting False Root node:", pCurrentRoot);
            debugNode("New Root node:", pNewRoot);
        }
        pNewRoot->setParent(nullptr);
        pTree->setRootAndCurrent(pNewRoot);
        pFactory->destroyNode(pCurrentRoot);
    }
}

void ExpressionParser::parseExpression(const char* pcExpression)
{
    pcExpression = expressionSanityCheck(pcExpression);
    if (pcExpression == nullptr)
        return;

    // default values
    OperationId prevId = OperationId::openParenthesis;  // initial operation at root node is '(',
    OperationItem opRoot(OperationId::openParenthesis); // as an upward iteration stopper to be deleted at the end.

    NodeFactory<OperationItem>* pFactory = NodeFactory<OperationItem>::getOrCreateInstance();

    // Initialize the tree with the '(' node, as a mark to be deleted at the end.
    pTree = new Tree<OperationItem>(pFactory->createNode(opRoot));
    char c = 0;
    while ((c = *pcExpression++) != 0) // Loop to o the entire expression parsing.
    {
        if(c == '\0')
            break;  // protective, not rationally needed.
        else if (c == ' ' ||  c == '\t' || c == '\r' || c == '\n')
            continue;

        OperationItem opNewItemToRank(prevId, 0);  // Default is former Id
        SearchStrategy search = SearchStrategy::leftToRigth; // Default is left-associative.

        if (! generateNewItem(c, search, &opNewItemToRank)) // check the new char and generate a new item
        {
            lastIndex = pcExpression - szExpression;
            return;
        }

        OperationItem::adjustPriorityAndSymbolAccordingToId(opNewItemToRank);

        if (verbosity >= Verbosity::full)
        {
            debugItem("Incoming New Item:", opNewItemToRank);
        }

        if (search != SearchStrategy::noIterate)
        {
            if (search == SearchStrategy::rightToLeft)
            {
                pTree->searchUp( [opNewItemToRank] (const Node<OperationItem>* pnode) -> bool {
                    // continue searching up while supplied item is less priority than current.
                    return pnode->getData().priority < opNewItemToRank.priority;
                });
            }
            else // left to right --> try to iterate the tree upwards.
            {
                pTree->searchUp( [opNewItemToRank] ( const Node<OperationItem>* pnode) -> bool {
                    // continue searching up while supplied item is equal or less priority than current.
                    return pnode->getData().priority <= opNewItemToRank.priority;
                });
            }
        }

        if(opNewItemToRank.id == OperationId::closeParenthesis)
        {
            // New generated ')' node must not be inserted. Matching '(' is to be removed instead.

            Node<OperationItem>* pCurrent = pTree->getCurrent();
            if (verbosity >= Verbosity::full)
            {
                debugNode("Deleting Current Item:", pCurrent);
            }
            // Remove former '(' node matching the new generated ')'. Also, force the new current = parent
            assert( pTree->deleteCurrentPreserveRight(true) );
            pFactory->destroyNode(pCurrent);
        }
        else // new node creation and inserton into the tree
        {
            Node<OperationItem>* pNewNode = pFactory->createNode(opNewItemToRank);

            if (verbosity >= Verbosity::full)
            {
                debugNode("Inserting New Item:", pNewNode);
            }

            bool zigzag = true; // right current subtree is going to be hang on the left of new node.
            assert( pTree->insertUnderCurrentOnRight(pNewNode, zigzag) );
            pTree->skipDownRight(); // make the recently inserted node the current one.
        }

        if (verbosity == Verbosity::extra)
        {
            printTree(std::cout);
        }

        prevId = opNewItemToRank.id; // prepare for next iteration
    }

    // Finally remove the '(' node as a root node.
    removeFakeOpenParenthesisRoot();
}

void  ExpressionParser::printNode( const Node<OperationItem>* pNode, int indent,
                                   std::ostream& os, bool norecursive /* = false */) const
{
    if (pNode == nullptr) return;

    if (pNode == pTree->getRoot())
    {
        if (verbosity >= Verbosity::full)
            os << '\n';

        if (verbosity == Verbosity::full) // not for extra, that'd be too much info !
            os << "----------------- AST TREE -----------------\n";
    }

    if (!norecursive)
        printNode(pNode->getRight(), indent + indentMargin, os, norecursive);

    OperationItem nodeData(pNode->getData());
    os << std::string(indent, ' ') << "(";
    switch (nodeData.id)
    {
    case OperationId::number:
        os << int(nodeData.value);
        break;

    case OperationId::openParenthesis:
    case OperationId::closeParenthesis:
    case OperationId::power:
    case OperationId::multiply:
    case OperationId::divide:
    case OperationId::plus:
    case OperationId::minus:
        os << nodeData.symbol;
        break;

    default:
        os << "error";
        break;
    }

    os << ") " ;
    if (verbosity >= Verbosity::full)
        os << int(nodeData.priority) << "!   #" << pNode->getSequenceNo();

    os << std::endl;

    if (!norecursive)
      printNode(pNode->getLeft(), indent + indentMargin, os, norecursive);
}

void  ExpressionParser::destroyNode( Node<OperationItem>* const pNode,
                                     bool norecursive /* = false */)
{
    if (pNode == nullptr) return;

    if (!norecursive)
    {
        destroyNode(pNode->getLeft());
        destroyNode(pNode->getRight());
    }

    NodeFactory<OperationItem>::getOrCreateInstance()->destroyNode(pNode);
}

void  ExpressionParser::destroyTree()
{
    destroyNode(pTree->getRoot());
    delete pTree;
    pTree = nullptr;
}
