/**
 * @file calc.cpp
 * @brief Entry point implementation of the one digit calculator machine.
 * @author Guillermo M. Paris
 * @date 2019-10-27
 */

#include <cstring>
#include <iostream>
#include "NodeFactory.h"
#include "ArithmeticEvaluator.h"
#include "ExpressionParser.h"
#include "OperationItem.h"

template <> NodeFactory<OperationItem>* NodeFactory<OperationItem>::pInstance(nullptr);

const char* szTitle1 = "==============================";
const char* szTitle2 = " Expression #";
const char* szTitle3 = "==================";

int main (int argc, char* argv[])
{
    if (argc < 2 ||  (argc < 3 && argv[1][0] == '-' && argv[1][1] == 'v'))
    {
        std::cout << "Usage: calc [-v[0-3]]  <expression 1> <expression 2> ... <expression n>\n"
        << "Example: calc -v1 1+1 5-6/2+3*4 '3+4*(2+1*1*(4-(1+1)))-16' 1*2*3*(2-1/3) '(4 + 5 * (7 - 3)) - 2'\n"
        << std::endl;
       return EXIT_FAILURE;
    }

    int index = 1;
    ExpressionParser::Verbosity verbosity = ExpressionParser::Verbosity::none;

    if (argv[1] && argv[1][0] == '-')
    {
        int arg1Len = strlen(argv[1]);
        if (arg1Len > 1 && argv[1][1] == 'v')
        {
            if (arg1Len > 2)
            {
                char c = argv[1][2];
                if ('3' <= c && c <= '9')
                    verbosity = ExpressionParser::Verbosity::extra;
                else if (c == '2')
                    verbosity = ExpressionParser::Verbosity::full;
                else if (c == '1')
                    verbosity = ExpressionParser::Verbosity::partial;
                else
                    verbosity = ExpressionParser::Verbosity::none;
            }
            else // argv[1] == "-v"
            {
                verbosity = ExpressionParser::Verbosity::partial;
            }
        }
        index++;
    }

    bool success = true;
    int base = (index - 1);
    NodeFactory<OperationItem>::getOrCreateInstance();

    for(; index < argc; index++)
    {
        if (verbosity == ExpressionParser::Verbosity::none)
            std::cout << "\nExpression #" << (index - base) <<  " : ";
        else
            std::cout << '\n' << szTitle1 << szTitle2 << (index - base) << ' ' << szTitle1 << '\n';

        ExpressionParser parser(argv[index], verbosity);
        if (!parser.finishedOK())
        {
            char cBad = parser.getFaultyChar();
            int  pos = parser.getExpressionIndex();
            int errorCode = parser.getIntError();
            std::cout << "ERROR " << errorCode << " parsing the expresion:\n"
                      << argv[index] << '\n';

            if (cBad != '\0')
            {
                std::cout << (pos >= 2 ? std::string(pos, ' ') : "") << "^-----\n"
                        << "At position " << pos << " got character \""
                        << cBad << "\" .\n";
            }

            std::cout << parser.getLastErrorMessage()
                      << " Ignoring it!\n";
            success = false;
            continue;
        }
        else if (verbosity != ExpressionParser::Verbosity::none)
            parser << std::cout << std::endl;

        ArithmeticEvaluator evaluator(parser.getTree());
        if (!evaluator)
        {
            std::cout << "ERROR " << evaluator.getError() << " parsing the expresion: " << argv[index] << " . Ignoring it!\n";
            success = false;
            continue;
        }

        std::cout.precision(15);
        std::cout << "Result = " << evaluator.getResult();
        if (verbosity == ExpressionParser::Verbosity::none)
            std::cout << '\n';
        else
        std::cout << '\n' << szTitle1 << szTitle3 << szTitle1 << '\n';
    }

    NodeFactory<OperationItem>::destroyInstance();
    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
