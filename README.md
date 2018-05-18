# Introduction

This programming project implements a BARES(Basic ARithmetic Expression Evaluator based on Stacks) program. Which is capable of receive simple arithmetics expresssions, formed by:

* Integer numeric constants (-32.768 to 32.767);
* Operators (+, , /, ⇤,ˆ, %), with precedence; and
* Parentheses.

# Operations, scope, and precedence

The supported operations and scope delimiters are:

- `^`: power, right association, weight=3.
- `*`: multiplication, left association, weight=2.
- `/`: division, left association, weight=2.
- `%`: module, left association, weight=2.
- `+`: addition, left association, weight=1.
- `-`: subtraction, left association, wight=1.
- `(`: opening scope, weight=0.
- `)`: closing scope, weight=n/a.

# EBNF grammar

For support the BARES was implemented  a parser for an EBNF (_Extended Backus-Naur Form_) grammar.

Information on **EBNF grammar** may be found [here](https://en.wikipedia.org/wiki/Extended_Backus–Naur_Form).

Information on **recursive descendent parsing** may be found [here](https://en.wikipedia.org/wiki/Recursive_descent_parser).

# The Grammar

The gramar we want to parse represents arithmetic expressions with addition and subtraction of integers.

    <expr>            := <term>,{ ("+"|"-"),<term> };
    <term>            := <integer>;
    <integer>         := 0 | ["-"],<natural_number>;
    <natural_number>  := <digit_excl_zero>,{<digit>};
    <digit_excl_zero> := "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9";
    <digit>           := "0"| <digit_excl_zero>;

This grammar _accepts_ expressions like:

* "23 + 43 - 0   + -124 - 21"
* " 21"
* "-21 +     -18"
* " -54"
* "-21 - 23 + 1234"

And _rejects_ expressions like:

* "01 + 3"
* "2 +"
* "  "
* "+2 + 5"
* "-02 + 4"

Later on we might want to improve this grammar to accept other binary operations and more robust expressions.


# Authorship

Program developed by Abraão Dantas (<abraaovld@gmail.com>) at EDB1 classes, 2018.1

&copy; IMD/UFRN 2017-2018.


