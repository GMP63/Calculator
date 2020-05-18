/**
 * @file ExpressionParser.cpp
 * @brief Character expression parser and operand/operator/operand binary tree builder.
 *        Implementation file.
 * @author Guillermo M. Paris
 * @date 2019-10-27
 */

#include <iomanip>
#include <cmath>
#include <iostream>
#include <string>
#include "ExpressionParser.h"
#include "NodeFactory.h"
#include "OperationItem.h"

union uintchar4
{
    uintchar4(const char* sym);

    uint32_t number;
    char symbol[4];
};

uintchar4::uintchar4(const char* sym)
{
    unsigned n = 0;
    if (sym)
    {
        for (; n < 4; n++)
            if ((symbol[n] = sym[n]) == 0)
                break;
        n++;
    }

    for (; n < 4; n++)
        symbol[n] = 0;
}

static void debugItem(const char* msg, const OperationItem& item, bool noCR = false)
{
    bool isNumber = (item.id == OperationId::number ? true : false);
    std::cout << (msg ? msg : "") << " (" ;
    if (isNumber)
        std::cout << item.value;
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
    "Two consecutive operators.",
    "Missing operator.",
    "Incorrect placement of the decimal point.",
    "Too many decimal digits.",
    "Unknown function name.",
    "Unknown character.",
    "Unbalanced number of parenthesis."
};

const char* ExpressionParser::getErrorMessage(int index)
{
    const int min = static_cast<int>(Error::first);
    const int max = static_cast<int>(Error::total);
    if (min <= index && index < max)
       return errors[index];
    else
    {
        return "";
    }
}


ExpressionParser::ExpressionParser(const char* pcExpression, Verbosity v)
    : verbosity(v)
    , lastError(Error::success)
    , cLastParsed(0)
    , lastIndex(0)
    , szExpression(pcExpression)
    , pTree(nullptr)
{
    parseExpression(pcExpression);
}

std::ostream& ExpressionParser::operator << (std::ostream& os)
{
    printTree(os);
    return os;
}

void  ExpressionParser::populateFunctionNamesTable()
{
    unsigned int min = static_cast<unsigned int>(OperationId::firstFunction);
    unsigned int max = static_cast<unsigned int>(OperationId::lastFunction);
    for (unsigned int u = min; u <= max; u++)
    {
        const OperationItem & opItem = OperationItem::operandTable[u];
        uintchar4 uKey(opItem.symbol);
        functionNames.insert(std::make_pair(uKey.number , opItem.id));
    }
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

bool  ExpressionParser::parseNumberForward(double& returnValue, const char* & currentParsingLine)
{
    bool decimalPoint = false;
    bool engNotation = false;
    char c = *currentParsingLine;
    int digitCount = 0;
    std::string sNumber;

    do
    {
        if (c == '.')
        {
            if (decimalPoint)
            {
                lastError = Error::incorrectDecimalPoint; // error, two decimal points.
                cLastParsed = c;
                return false; // error, two decimal points.
            }

            decimalPoint = true;
        }
        else if (c == 'e' || c == 'E')
        {
            if (engNotation)
            {                        // Two exponent marks! Then second 'E' was not an engineering notation mark.
               --currentParsingLine; // give back 'E' to the curent line. Not processed here.
                break;
            }

            engNotation = true;

            char cNext = *(1 + currentParsingLine);
            if (('0' <= cNext && cNext <= '9') || cNext == '+' || cNext == '-')
            {
                sNumber.append(1, c);
                sNumber.append(1, cNext);
                currentParsingLine += 2;
                digitCount += 2;
                c = *currentParsingLine;
                continue;
            }
            else // 'E' was not an engineering notation mark.
            {
                --currentParsingLine; // give back 'E' to the curent line. Not processed here.
                break;
            }
        }

        sNumber.append(1, c);
        if ('0' <= c && c <= '9')
            digitCount++;

        c = *(++currentParsingLine);
    }
    while ((('0' <= c && c <= '9') || c == '.' || c == 'e' || c == 'E') && digitCount < maxNumberOfDigits);

    if (digitCount >= maxNumberOfDigits)
    {
        lastError = Error::tooManyDigits; // error, too many decimal digits.
        cLastParsed = c;
        return false;
    }

    if (sNumber.back() == '.') // if last numeric character was '.'
    {
        sNumber.append(1, '0'); // avoid ending in '.' ,  "nnn.0" is better
    }

    returnValue = std::stold(sNumber);
    return true;
}

bool  ExpressionParser::parseAlphabeticForward(OperationId & returnOp, const char* & currentLine)
{
    using functionNamesIter = std::unordered_map<uint32_t, OperationId>::iterator;

    char name[6] = {0}; // word to be search must be wholy clear.
    name[0] = *currentLine; // caller function assures this is a valid letter char
    name[1] = *(1 + currentLine);

    if ((name[1] < 'A' || name[1] > 'Z') && (name[1] < 'a' || name[1] > 'z'))
    {
        if (name[0] == 'e') // just one letter => Euler number
        {
            currentLine++; // skip the complete constant name
            returnOp = OperationId::e;
            return true;
        }

        currentLine++;
        cLastParsed = name[1];
        lastError = Error::unknownChar; // Unrecognized character.
        return false; // not a letter char
    }

    name[2] = *(2 + currentLine);
    if ((name[2] < 'A' || name[2] > 'Z') && (name[2] < 'a' || name[2] > 'z') && name[2] != '(')
    {
        if (name[0] == 'p' && name[1] == 'i' ) // just two letters => Pitágoras number
        {
            currentLine += 2; // skip the complete constant name
            returnOp = OperationId::pi;
            return true;
        }

        currentLine += 2;
        cLastParsed = name[2];
        lastError = Error::unknownChar; // Unrecognized character.
        return false; // neither a letter char nor a parenthesis
    }

    int nameLength = 2; // assume function names are 2 char long.

    if (name[2] != '(')
    {
        nameLength++; // function name is at least 3 char long.
        name[3] = *(3 + currentLine);
        if ((name[3] < 'A' || name[3] > 'Z') && (name[3] < 'a' || name[3] > 'z') && name[3] != '(')
        {
            if (name[0] == 'p' && name[1] == 'h' && name[2] == 'i' ) // just three letters => Armonic number
            {
                currentLine += 3; // skip the complete constant name
                returnOp = OperationId::phi;
                return true;
            }

            currentLine += 3;
            cLastParsed = name[3];
            lastError = Error::unknownChar; // Unrecognized character.
            return false; // neither a letter char nor a parenthesis
        }

        if (name[3] != '(')
        {
            nameLength++; // function name is 4 char long.
            if ((name[4] = *(4 + currentLine)) != '(') // bad finished function name
            {
                cLastParsed = name[0];
                lastError = Error::unknownFunction; // Unrecognized function name.
                return false; // Unknown operator or function name.
            }
        }
        else
            name[3] = 0; // removing parenthesis from the name.
    }
    else
        name[2] = 0; // removing parenthesis from the name.

    if (functionNames.empty())     // function name table not built yet, this is the very first use.
        populateFunctionNamesTable(); // Then, populate the table just one time.

    uintchar4 uKey(name);
    functionNamesIter it = functionNames.find(uKey.number);

    if (it == functionNames.end())
    {
        cLastParsed = name[0];
        lastError = Error::unknownFunction; // Unrecognized function name.
        return false; // Unknown operator or function name.
    }

    currentLine += nameLength; // skip the complete function name
    returnOp = it->second;
    return true;
}

bool  ExpressionParser::parseNewItem(const char* & currentParsingLine,
                                     SearchStrategy& newStrategy, OperationItem* newItemToComplete)
{
    char c = *currentParsingLine;
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
        if (prevId == OperationId::number || prevId == OperationId::factorial
            || prevId == OperationId::closeParenthesis)
        {
            if (c == '+')
                newItemToComplete->id = OperationId::plus;
            else
                newItemToComplete->id = OperationId::minus;
        }
        else // unary operators + or -
        {
            if (c == '+')
                newItemToComplete->id = OperationId::positive;
            else
                newItemToComplete->id = OperationId::negative;

            newStrategy = SearchStrategy::noIterate;
        }
    }
    else if (c == '*' || c == '/' || c == '%' || c == '^' || c == '!')
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
        else if (c == '%')
            newItemToComplete->id = OperationId::reminder;
        else if (c == '^')
        {
            newItemToComplete->id = OperationId::power;
            newStrategy = SearchStrategy::rightToLeft;
        }
        else // '!'
            newItemToComplete->id = OperationId::factorial;
    }
    else if(('0' <= c && c <= '9') || c == '.')  // numeric digits, up to ExpressionParser::maxNumberOfDigits.
    {
        double value = 0.0;
        if (parseNumberForward(value, currentParsingLine))
        {
            newItemToComplete->id = OperationId::number;
            newItemToComplete->value = value;
            return true;
        }
        else
        {
            return false;
        }
    }
    else if(('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))  // posible alphabetic function names.
    {
        if (prevId == OperationId::number)
        {
            lastError = Error::missingOp; // Missing operator between number and function name.
            cLastParsed = c;
            return false;
        }

        OperationId opId = OperationId::openParenthesis; // initial neutral invalid value
        if (parseAlphabeticForward(opId, currentParsingLine))
        {
            newItemToComplete->id = OperationId::number;

            if (opId == OperationId::pi)
                newItemToComplete->value = 4 * atan(1);
            else if (opId == OperationId::phi)
                newItemToComplete->value = (1 + sqrt(5))/2;
            else if (opId == OperationId::e)
                newItemToComplete->value = exp(1);

            else // real true operation (function) code
            {
                newItemToComplete->id = opId;
                newItemToComplete->value = 0;
            }
            return true;
        }
        else
        {
            return false;
        }
    }
    else // if a parsing error
    {
        lastError = Error::unknownChar; // Unrecognized character.
        cLastParsed = c;
        return false;
    }

    currentParsingLine++; // fetch next character
    return true;
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
    while ((c = *pcExpression) != 0) // Loop to o the entire expression parsing.
    {
        if(c == '\0')
            break;  // protective, not rationally needed.
        else if (c == ' ' ||  c == '\t' || c == '\r' || c == '\n')
        {
            pcExpression++;
            continue;
        }

        OperationItem opNewItemToRank(prevId, 0);  // Default is former Id
        SearchStrategy search = SearchStrategy::leftToRigth; // Default is left-associative.

        if (! parseNewItem(pcExpression, search, &opNewItemToRank)) // check the new char and generate a new item
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
    if (OperationId::first <= nodeData.id && nodeData.id < OperationId::total)
    {
        if (nodeData.id == OperationId::number)
            os << nodeData.value;
        else
            os << nodeData.symbol;
    }
    else
        os << "error";

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
