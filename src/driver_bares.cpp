/**
 * @file       driver_bares.cpp
 * @brief      Implementação de um driver básico para o bares.
 * @author     Prof. Selan Rodrigues dos Santos and Israel Medeiros Fontes
 * @since      10/11/2017
 * @date       12/11/2017
 */

#include <iostream>  // cout, endl
#include <iomanip>
#include <vector>
#include "parser.hpp"
#include "token.hpp"
#include "bares.hpp"
// To help change type if we want to.

void print_msg( const Parser::ParserResult & result, std::string str )
{
    switch ( result.type )
    {
        case Parser::ParserResult::UNEXPECTED_END_OF_EXPRESSION:
            std::cout << ">>> Unexpected end of input at column (" << result.at_col << ")!\n";
            break;
        case Parser::ParserResult::ILL_FORMED_INTEGER:
            std::cout << ">>> Ill formed integer at column (" << result.at_col << ")!\n";
            break;
        case Parser::ParserResult::MISSING_TERM:
            std::cout << ">>> Missing <term> at column (" << result.at_col << ")!\n";
            break;
        case Parser::ParserResult::EXTRANEOUS_SYMBOL:
            std::cout << ">>> Extraneous symbol after valid expression found at column (" << result.at_col << ")!\n";
            break;
        case Parser::ParserResult::INTEGER_OUT_OF_RANGE:
            std::cout << ">>> Integer constant out of range beginning at column (" << result.at_col << ")!\n";
            break;
        case Parser::ParserResult::MISSING_CLOSING_PARENTHESIS:
            std::cout << ">>> Missing closing ')' "<< "at column (" << result.at_col << ")!" << "\n";
            break;
        default:
            std::cout << ">>> Unhandled error found!\n";
            break;
    }

    std::cout << "\"" << str << "\"\n";
}


int main()
{
 
    std::vector<std::string> expressions;
    std::string aux;
    Parser my_parser;

    while (std::getline(std::cin, aux))
    {
        expressions.push_back( aux );
    }

    for( const auto & expr : expressions )
    {
        auto result = my_parser.parse( expr );

        if ( result.type != Parser::ParserResult::PARSER_OK )
        {
            print_msg( result, expr );
        }
        else
        {
            auto lista = my_parser.get_tokens();
            auto postfix = infix_to_postfix( lista );
            auto resultado = evaluate_postfix( postfix );
            
            if(resultado.type == RESULT_OK)
                std::cout << resultado.value << std::endl;
            else
                std::cout << ">>> Division by zero! \n" << expr << std::endl;
            
        }
    }

    std::cout << "\n>>> Normal exiting...\n";

    return EXIT_SUCCESS;
}