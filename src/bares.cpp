/**
 * @file       bares.cpp
 * @brief      Implementação das funções do bares.
 * @author     Prof. Selan Rodrigues dos Santos and Israel Medeiros Fontes
 * @since      10/11/2017
 * @date       12/11/2017
 */

#include <string>    // string
#include <cassert>   // assert
#include <cmath>     // pow
#include <vector>    // vector

#include "token.hpp" // Token
#include "bares.hpp" // Bares
#include "stack.hpp" // Stack

#define FIRST 0

using value_type = long int;
using sc::stack;

short get_precedence( Token _tok )
{
    // Only '+', '-', '*', '%', '/', and '^' (for exponentiation) operators are expected;
    switch( _tok.value[FIRST] )
    {
        case '^': return 3;
        //=============================================================

        case '*':
        case '/':
        case '%': return 2;

        //=============================================================
        case '+':
        case '-': return 1;

        //=============================================================
        default: return 0;
    }
}

bool is_operand( Token _tok ) { return _tok.type == Token::token_t::OPERAND; }

bool is_operator( Token _tok ) { return _tok.type == Token::token_t::OPERATOR; }

bool is_opening_scope( Token _tok ) { return _tok.value[FIRST] == '('; }

bool is_closing_scope( Token _tok ) { return _tok.value[FIRST] == ')'; }

bool is_right_association( Token _tok ) { return _tok.value[FIRST] == '^'; }

// The comparison usually is: top() >= incoming_symbol
bool has_higher_precedence( Token _tok1, Token _tok2 )
{
    // op1 >= op2.
    // Just converts operator into number.
    auto w1 = get_precedence( _tok1 );
    auto w2 = get_precedence( _tok2 );

    // Special case: '^' >= '^'??? NO!!!
    if ( _tok1.value[FIRST] == _tok2.value[FIRST] and is_right_association( _tok1 ) )
        return false;

    return w1 >= w2;
}

std::vector<Token> infix_to_postfix( std::vector<Token> _infix )
{
    // Stores the expression in postfix representation.
    std::vector<Token> postfix;

    // Stack, to help us convert from infix to postfix.
    stack<Token> st;

    // Run through each symbol in the incoming expression.
    for( Token s : _infix )
    {
        if ( is_operand( s ) )
        {
            postfix.push_back(s); // Operand goes straight to the output string.
        }
        else if ( is_opening_scope( s ) )
        {
            st.push( s ); // goes into the stack, regardless of who is on top.
        }
        else if ( is_closing_scope( s ) )
        {
            // Pops out all symbols until we get to his 'brother'
            while( not st.empty() and not is_opening_scope( st.top() ) )
            {
                postfix.push_back(st.top()); // the operand on top.
                st.pop();
                 // All operands on the stack goes to the output string.
            }
            // Don't forget to remove the opening scope.
            st.pop();
        }
        else if ( is_operator( s ) )
        {
            // top() >= s ??? yes-> goes to output; no-> stay on the stack.
            while(  not st.empty() and has_higher_precedence( st.top(), s ) )
            {
                postfix.push_back(st.top());   // goes to output.
                st.pop(); // remove the operator
            }

            // The incoming operator always goes into the stack.
            st.push(s);
        }
        else
        {
            // Do nothing...  spaces, tab, etc.
        }
    } // For: processing input.

    // Dump all remaining elements from the stack into the outpu.
    while( not st.empty() )
    {
        postfix.push_back(st.top());
        st.pop();
    }

    return postfix;
}


value_type Token2integer(Token _tok)
{
    return std::stoi(_tok.value);
}

Result execute_operator( value_type term1, value_type term2, Token _tok)
{
    Result result;
    switch( _tok.value[FIRST] )
    {
        case '+':  result.value = term1 + term2; break;
        case '-':  result.value = term1 - term2; break;
        case '*':  result.value = term1 * term2; break;
        case '^':  result.value = pow( term1, term2 ); break;
        case '/':  if ( term2 == 0 ){ result.type = DIVISON_BY_ZERO; return result; } 
                   else result.value = term1 / term2;
                   break;
        case '%':  if ( term2 == 0 ) { result.type = DIVISON_BY_ZERO; return result; }
                   else result.value = term1 % term2;
                   break;
    }
    result.type = RESULT_OK;
    return result;
}

Result evaluate_postfix( std::vector<Token> postfix )
{
    // Stack of numbers.
    stack< value_type > st;
    Result result;
    // Process each symbol in the postfix experssion.
    for( u_int i = 0; i < postfix.size(); ++i /*symbol s : postfix*/ )
    {
        if(is_operand(postfix[i])){
            st.push(Token2integer(postfix[i]));
        }
        else if ( is_operator( postfix[i] ) )
        {
            // Pops out the two terms of the binary operation...
            auto term2 = st.top(); st.pop(); // Invert the order first!
            auto term1 = st.top(); st.pop();
            // ... executes the operation, and.../
            result = execute_operator( term1, term2, postfix[i] );
            // ... pushes the result back onto the stack.
            st.push( result.value );
        }
    }

    // If everything goes smoothly, the result should be on the top of the stack.
    result.value = st.top();
    return result;
}