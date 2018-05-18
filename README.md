# Introduction

This programming project implements a simple parser for an EBNF (_Extended Backus-Naur Form_) grammar.

The first objective is to provide a basic example of EBNF grammar and a parser, so that students may understand the concept.
The second objective is to reuse this code to do the parsing stage in the [BARES programming project](http://projetos.imd.ufrn.br/LP1_20162/bares.git).

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
* " - 3 + 4"
* "2 +"
* "  "
* "+2 + 5"
* "-02 + 4"
* "(2+3)" _(for the time being)_

Later on we might want to improve this grammar to accept other binary operations and _parenthesis_.

# TODO

- [ ] Explain the concepts behind the parsing process.
- [ ] Implement `Parser` class and its basic infrastructure for processing symbols.
- [ ] Implement the non-terminal production rules.
- [ ] Test the parsing process.
- [ ] Implement token extraction.
- [ ] Expand the grammar to handle a full [BARES](http://projetos.imd.ufrn.br/LP1_20162/bares.git) expression.

# Authorship

Program developed by Selan (<selan@dimap.ufrn.br>) and students from EDB1, 2017.2

&copy; DIMAp/UFRN 2016-2017.

