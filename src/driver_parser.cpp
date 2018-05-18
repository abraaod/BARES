#include <iostream>
#include <iomanip>
#include <vector>
#include <stack>     // stack
#include <string>    // string
#include <cassert>   // assert
#include <cmath>     // pow
#include <stdexcept> // std::runtime_error

#include "../include/parser.h"

std::vector<std::string> expressions;

void print_error_msg( const Parser::ResultType & result, std::string str )
{
    std::string error_indicator( str.size()+1, ' ');

    // Have we got a parsing error?
    error_indicator[result.at_col] = '^';
    switch ( result.type )
    {
        case Parser::ResultType::UNEXPECTED_END_OF_EXPRESSION:
            std::cout << ">>> Unexpected end of input at column (" << result.at_col << ")!\n";
            break;
        case Parser::ResultType::ILL_FORMED_INTEGER:
            std::cout << ">>> Ill formed integer at column (" << result.at_col - 1<< ")!\n";
            break;
        case Parser::ResultType::MISSING_TERM:
            std::cout << ">>> Missing <term> at column (" << result.at_col << ")!\n";
            break;
        case Parser::ResultType::EXTRANEOUS_SYMBOL:
            std::cout << ">>> Extraneous symbol after valid expression found at column (" << result.at_col << ")!\n";
            break;
        case Parser::ResultType::INTEGER_OUT_OF_RANGE:
            std::cout << ">>> Integer constant out of range beginning at column (" << result.at_col << ")!\n";
            break;
        case Parser::ResultType::MISSING_CLOSING:
            std::cout << ">>> Missing closing ”)” at column (" << result.at_col << ")!\n";
            break;
        default:
            std::cout << ">>> Unhandled error found!\n";
            break;
    }

    std::cout << "\"" << str << "\"\n";
    std::cout << " " << error_indicator << std::endl;
}

//=== Aliases
using value_type = long int; //!< Type we operate on.
using symbol = char; //!< A symbol in this implementation is just a char.
using comp = int;

bool control = true;

// Simple helper functions that identify the incoming symbol.

bool is_operator( symbol s )
{ return std::string("^*%/+-").find( s ) != std::string::npos; }

bool is_operand( symbol s )
{ return s>='0' and s<='9'; }

bool is_operator( comp s )
{ return s == 1;}

bool is_operand( comp s )
{ return s == 0; }

bool is_opening_scope( comp s )
{ return s == 2 ; }

bool is_opening_scope( symbol s )
{ return s == '(' ; }

bool is_closing_scope( comp s )
{ return s == 3 ; }

/// Converts a expression in infix notation to a corresponding profix representation.
std::string infix_to_postfix( std::string );

/// Converts a char (1-digit operand) into an integer.
value_type char2integer( std::string c )
{ return std::stoi(c); }

/// Check the operand's type of association.
bool is_right_association( symbol op )
{ return op == '^'; }


/// Change an infix expression into its corresponding postfix representation.
value_type evaluate_postfix( std::string );

/// Returns the precedence value (number) associated with an operator.
short get_precedence( symbol op )
{
    switch( op )
    {
        case '^': return 3;

        case '*':
        case '/':
        case '%': return 2;

        case '+':
        case '-': return 1;

        case '(': return 0;

        default: assert(false);
    }
    return -1;
}

/// Determines whether the first operator is >= than the second operator.
bool has_higher_or_eq_precedence( symbol op1 , symbol op2 )
{
    return ( get_precedence(op1) >= get_precedence(op2) ) ?
        is_right_association( op1 ) ? false : true  :
        false;
}

/// Execute the binary operator on two operands and return the result.
value_type execute_operator( value_type v1, value_type v2, symbol op )
{
    switch( op )
    {
        case '^':
            return pow( v1, v2 );
        case '*':
            return v1 * v2;
        case '/':
            if(v2 != 0){
                return v1/v2;
            }else{
              control = false;
              return 0;
            }
        case '%':
            //if ( v2 == 0 ) throw std::overflow_error("Division by zero");
            return v1%v2;
        case '+':
            return v1+v2;
        case '-':
            return v1-v2;
        //default:
    }
    throw std::runtime_error("Invalid operator!");
}

std::vector<std::string> infix_to_postfix( Parser & obj )
{
    std::stack< std::string > s; // auxiliary data structure.
    std::string postfix = "1234"; // stores the postfix expression
    std::vector < std::string > aux;
    // Process each incoming symbol
    auto lista = obj.get_tokens();
    Token t;
    for( auto si = lista.begin(); si != lista.end(); si++)
    {
        t = *si;
        if ( is_operand( (int) t.type ) )
             aux.push_back( t.value); // send it straight to the output symbol queue.
        else if ( is_opening_scope( (int) t.type )){
            s.push( t.value ); // always goes into the "waiting room"
        }
        else if ( is_closing_scope( (int) t.type ))
        {
            // Pop out all pending operations.
            while( not is_opening_scope( s.top()[0] ) )
            {
                // remove operator and send it to the postfix expression.
                aux.push_back( s.top()+"");
                s.pop();
            }
            // Don't forget to get rid of the opening scope.
            s.pop();
        }
        else if ( is_operator( (int) t.type ))
        {
            // Send out the "waiting" operator with higher or equal precedence...
            // unless they have equal precedence AND are right associated.
            while ( not s.empty() and has_higher_or_eq_precedence( s.top()[0], t.value[0] ) )
            {
                aux.push_back (s.top()+""); // send it to the output
                s.pop(); // get rid of the operator.
            }
            // The incoming symbol always goes into the "waiting room".
            s.push( t.value ) ;
        }
        else // white space or whatever
        {
            // Do nothing. Just ignore this...
        }
    }

    // Clear out any pending operators stored in the stack.
    while ( not s.empty() )
    {
        aux.push_back( s.top()+"");
        s.pop();
    }


    return aux;
}

value_type evaluate_postfix( std::vector<std::string> postfix )
{
    std::stack< value_type > s;
    int c_operand = 0, c_operator = 0;

    for( auto si = postfix.begin(); si != postfix.end(); si++ )
    {
      if ( is_operand( (*si)[0] ) )
          c_operand++;
      else if ( is_operator( (*si)[0] ) )
      {
        c_operator++;
      }
    }

    // For each operator/operando in the input postfix expression do this...
    for( auto si = postfix.begin(); si != postfix.end(); si++ )
    {
        if ( is_operand( (*si)[0] ) ){

            s.push( char2integer(*si) ); // Do not forget to convert it into integer.

        }
        else if ( is_operator( (*si)[0] ) )
        {
            // IMPORTANT: Pop out operandos in reverse order!
            auto op2 = s.top(); s.pop();
            auto op1 = s.top(); s.pop();
            // The result of the operation is pushed back into the stack.
            s.push( execute_operator( op1, op2, (*si)[0] ) );

        }
        //else assert( false ); // unexpected symbol...
    }
    return s.top();
}


int main(int argc,char *argv[])
{
    Parser my_parser; // Instancia um parser.

    expressions = reader_file(argv[1]);

    // Tentar analisar cada expressão da lista.
    for( const auto & expr : expressions )
    {
        // Fazer o parsing desta expressão.
        auto result = my_parser.parse( expr );
        // Preparar cabeçalho da saida.
        std::cout << std::setfill('=') << std::setw(80) << "\n";
        std::cout << std::setfill(' ') << ">>> Parsing \"" << expr << "\"\n";
        // Se deu pau, imprimir a mensagem adequada.
        if ( result.type != Parser::ResultType::OK ){
            print_error_msg( result, expr );
        }
        else{
          std::cout << ">>> Expression SUCCESSFULLY parsed!\n";

          auto postfix = infix_to_postfix(my_parser);

          auto result = evaluate_postfix( postfix );
          if(control){
            std::cout << ">>> Result is: " << result << std::endl;
          }else{
            std::cout << "Division by zero!" << std::endl;
            control = true;
          }

        }

    }

    std::cout << "\n>>> Normal exiting...\n";

    return EXIT_SUCCESS;
}
