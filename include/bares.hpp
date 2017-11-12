
/**
 * @file bares.hpp
 * @brief      Declaração das funções do bares.
 * @author     Prof. Selan Rodrigues dos Santos and Israel Medeiros Fontes
 * @since      10/11/2017
 * @date       12/11/2017
 */

#include <vector>
#include "token.hpp"

using value_type = long int;

enum code{
	RESULT_OK = 0,
	DIVISON_BY_ZERO
};

struct Result{
	code type;
	value_type value;
};

/// Simple helper functions.
bool is_operator( Token _tok );
bool is_operand( Token _tok );
bool is_opening_scope( Token _tok );
bool is_closing_scope( Token _tok );

/// Determines whether the first operator is > than the second operator.
bool has_higher_precedence( Token _tok1, Token _tok2 );

/// Converts a expression in infix notation to a corresponding profix representation.
std::vector<Token> infix_to_postfix( std::vector<Token> _infix );

/// Converts a char (1-digit operand) into an integer.

/// Execute the binary operator on two operands and return the result.
Result execute_operator( value_type, value_type, Token _tok );

/// Change an infix expression into its corresponding postfix representation.
Result evaluate_postfix( std::vector<Token> postfix );


/// Returns the precedence value (number) associated with an operator.
short get_precedence( Token _tok );
