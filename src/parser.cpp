/**
 * @file       parser.cpp
 * @brief      Implementação das funções do parser.
 * @author     Prof. Selan Rodrigues dos Santos and Israel Medeiros Fontes
 * @since      10/11/2017
 * @date       12/11/2017
 */

#include "parser.hpp"
#include <iterator>
#include <algorithm>

/// Converts a valid character to the corresponding terminal symbol.
Parser::terminal_symbol_t  Parser::lexer( char c_ ) const
{
    switch( c_ )
    {
        case '+':  return terminal_symbol_t::TS_PLUS;
        case '-':  return terminal_symbol_t::TS_MINUS;
        case '/':  return terminal_symbol_t::TS_SLASH;
        case '%':  return terminal_symbol_t::TS_MOD;
        case '*':  return terminal_symbol_t::TS_ASTERISK;
        case '^':  return terminal_symbol_t::TS_CARRET;
        case '(':  return terminal_symbol_t::TS_SCOPE_OPEN;
        case ')':  return terminal_symbol_t::TS_SCOPE_CLOSE;
        case ' ':  return terminal_symbol_t::TS_WS;
        case '0':  return terminal_symbol_t::TS_ZERO;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':  return terminal_symbol_t::TS_NON_ZERO_DIGIT;
        case   9:  return terminal_symbol_t::TS_TAB;
        case '\0': return terminal_symbol_t::TS_EOS; // end of string: the $ terminal symbol
    }
    return terminal_symbol_t::TS_INVALID;
}


/// Consumes a valid character from the expression being parsed.
void Parser::next_symbol( void )
{
    // Advances iterator to the next valid symbol for processing
    std::advance( it_curr_symb, 1 );
}

/// Checks whether we reached the end of the expression string.
bool Parser::end_input( void ) const
{
    // "Fim de entrada" ocorre quando o iterador chega ao
    // fim da string que guarda a expressão.
    return it_curr_symb == expr.end();
}

/// Verifies whether the current symbol is equal to the terminal symbol requested.
bool Parser::peek( terminal_symbol_t c_ ) const
{
    // Verificar se o código fornecido no argumento corresponde
    // ao caractere na "ponta da agulha" (apontado pelo iterador).
    return ( not end_input() and
             lexer( *it_curr_symb ) == c_ );
}

/// Tries to match the current character to a symbol passed as argument.
bool Parser::accept( terminal_symbol_t c_ )
{
    // Se o caractere atual for o requisitado, o método consome o
    // caractere da entrada.
    if ( peek( c_ ) )
    {
        next_symbol();
        return true;
    }

    return false;
}

/// Verify whether the next valid symbol is the one expected; if it is so, the method accepts it.
bool Parser::expect( terminal_symbol_t c_ )
{
    // Salte todos os caracteres em branco e tente validar
    // o primeiro caractere que surgir.
    skip_ws();
    return accept( c_ );
}


/// Ignores any white space or tabs in the expression until reach a valid symbol or end of input.
void Parser::skip_ws( void )
{
    // Simplemente salta todos os caracteres em branco.
    // Lembrar de verificar se a entrada não acabou, para evitar de
    // acessar um iterador inválido.
    while ( not end_input() and
            ( lexer( *it_curr_symb ) == Parser::terminal_symbol_t::TS_WS  or
              lexer( *it_curr_symb ) == Parser::terminal_symbol_t::TS_TAB ) )
    {
        next_symbol();
    }
}


/// Converts from string to integer.
Parser::input_int_type Parser::str_to_int( std::string input_str_ )
{
    // Creating input stream.
    std::istringstream iss( input_str_ );

    // Resulting value.
    Parser::input_int_type value;
    iss >> value; // Ignore trailling white space.

    //std::cout << ">> input_str: \"" << input_str_ << "\" \n";
    //std::cout << ">> value: " << value << " \n";

    // Check for error during convertion.
    if ( iss.fail() )
        throw std::runtime_error( "str_to_int(): Erro, illegal integer format." );

    return value;
}

//=== Non Terminal Symbols (NTS) methods.

/*! Processando uma expressão.
 *
 * Produção:
 * <expr> := <term>,{ ("+"|"-"),<term> }
 *
 * De acordo com a gramática (acima), uma expressão pode ser apenas um
 * termo isolado ou seguido de um ou mais termos com um + ou - entre eles.
 *
 */
Parser::ParserResult Parser::expression()
{
    auto result = term();

    while( result.type == ParserResult::PARSER_OK )
    {
         if ( expect( Parser::terminal_symbol_t::TS_MINUS ) )
         {
             // Guarda o token "-" na lista.
             token_list.emplace_back( Token( "-", Token::token_t::OPERATOR ) );
         }
         else if ( expect( Parser::terminal_symbol_t::TS_PLUS ) )
         {
             // Guarda o token "+" na lista.
             token_list.emplace_back( Token( "+", Token::token_t::OPERATOR ) );
         }

         else if ( expect( Parser::terminal_symbol_t::TS_ASTERISK ) )
         {
            //Guarda o token "*" na lista.
            token_list.emplace_back( Token( "*", Token::token_t::OPERATOR ) );
         }
         else if ( expect( Parser::terminal_symbol_t::TS_SLASH ) )
         {
            //Guarda o token "/" na lista.
            token_list.emplace_back( Token( "/", Token::token_t::OPERATOR ) );
         }
         else if ( expect( Parser::terminal_symbol_t::TS_CARRET ) )
         {
            //Guarda o token "^" na lista.
            token_list.emplace_back( Token( "^", Token::token_t::OPERATOR ) );
         }
         else if ( expect( Parser::terminal_symbol_t::TS_MOD ) )
         {
            //Guarda o token "%" na lista.
            token_list.emplace_back( Token( "%", Token::token_t::OPERATOR ) );
         }
         else break;
        
        // Processa outro termo.
        result = term();
    }

    return result;
}

 // Produção:   <term> := <integer>;
Parser::ParserResult Parser::term()
{
    skip_ws();
    ParserResult result = ParserResult( ParserResult::code_t::MISSING_TERM, std::distance( expr.begin(), it_curr_symb) );

    if( expect( terminal_symbol_t::TS_SCOPE_OPEN) ){
        token_list.emplace_back( Token( "(", Token::token_t::SCOPE_OPEN) );
        result = expression();
        
        if(result.type == ParserResult::code_t::PARSER_OK)
        {
            if( not expect( terminal_symbol_t::TS_SCOPE_CLOSE) )
            {
                return ParserResult( ParserResult::code_t::MISSING_CLOSING_PARENTHESIS , std::distance( expr.begin(), it_curr_symb) );
            }
            token_list.emplace_back( Token( ")", Token::token_t::SCOPE_CLOSE) );
        }
    }
    else{ result = integer(); }

    return result;
}

/* Processando um inteiro.
 *
 * Produção:
 * <integer> := 0 | ["-"],<natural_number>;
 */
Parser::ParserResult Parser::integer()
{
    // Se aceitarmos um zero, então o inteiro acabou aqui.
    if ( accept( terminal_symbol_t::TS_ZERO ) )
    {
        // Guarda o token "zero" na lista.
        token_list.emplace_back( Token( "0", Token::token_t::OPERAND ) );
        return ParserResult( ParserResult::code_t::PARSER_OK );
    }


    // Guardar o início do número natural (que vai ser um token).
    auto begin_token = it_curr_symb;

    accept( terminal_symbol_t::TS_MINUS );
    auto result =  natural_number();
    // Se veio um número natural correto, vamos tokeniza e salvar.
    if ( result.type == ParserResult::PARSER_OK )
    {
        // Número natural finalizado, vamos copiar o token.
        std::string token_str;
        std::copy( begin_token, it_curr_symb, std::back_inserter( token_str ) );

        // Tentar converter a string em um inteiro grande
        input_int_type token_int;
        try {  token_int = str_to_int( token_str ); }
        catch( std::runtime_error e )
        {
            // Vai car aqui apenas se for um inteiro com muitos dígitos,
            // acima da quantidade máxima aceitada por um long long.
            return ParserResult( ParserResult::code_t::INTEGER_OUT_OF_RANGE,
                    std::distance( expr.begin(), begin_token ) );
        }

        // Teste de range válido.
        if ( token_int < -32767 or token_int > 32768 )
        {
            return ParserResult( ParserResult::code_t::INTEGER_OUT_OF_RANGE,
                    std::distance( expr.begin(), begin_token ) );
        }

        // Coloca o novo token na nossa lista de tokens.
        token_list.emplace_back( Token( token_str, Token::token_t::OPERAND ) );
    }

    return result; // O resultado de natural_number().
}

/*
 * Produção:
 * <natural_number> := <digit_excl_zero>,{<digit>};
 */
Parser::ParserResult Parser::natural_number()
{
    // Tem que vir um número que não seja zero! (de acordo com a definição).
    if ( not digit_excl_zero() )
        return ParserResult( ParserResult::code_t::ILL_FORMED_INTEGER, std::distance( expr.begin(), it_curr_symb ) ) ;

    // Cosumir os demais dígitos, se existirem...
    while( digit() ) /* empty */ ;

    return ParserResult( ParserResult::code_t::PARSER_OK );
}

bool Parser::digit_excl_zero()
{
    return accept( terminal_symbol_t::TS_NON_ZERO_DIGIT );
}

/*
 * Produção:
 * <digit> := "0"| <digit_excl_zero>;
 */
bool Parser::digit()
{
    return ( accept( terminal_symbol_t::TS_ZERO ) or digit_excl_zero() ) ? true : false;

/*
    if ( not accept( terminal_symbol_t::TS_ZERO ) )
    {
        return digit_excl_zero();
    }
*/
    return true;
}

/*!
 * This is the parser's entry point.
 * This method tries to (recursivelly) validate k expression.
 * During this process, we also stored the tokens into a container.
 *
 * \param e_ The string with the expression to parse.
 * \return The parsing result.
 */
Parser::ParserResult Parser::parse( std::string e_ )
{
    expr = e_; //  Guarda a expressão no membro correspondente.
    it_curr_symb = expr.begin(); // Define o simbolo inicial a ser processado.

    token_list.clear();
    auto result = expression();

    // Verificar se ainda sobrou algo na expressão.
    if ( result.type == ParserResult::PARSER_OK )
    {
        // Neste momento não deveria ter nada sobrando na string, a não ser
        // espaços em branco.
        skip_ws(); // Vamos "consumir" os espaços em branco, se existirem....
        if ( not end_input() ) // Se estiver tudo ok, deveríamos estar no final da string.
        {
            return ParserResult( ParserResult::EXTRANEOUS_SYMBOL, std::distance( expr.begin(), it_curr_symb ) );
        }
    }

    return result;

}


/// Return the list of tokens, which is the by-product created during the syntax analysis.
std::vector< Token >
Parser::get_tokens( void ) const
{
    return token_list;
}



//==========================[ End of parse.cpp ]==========================//
