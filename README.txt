# Lexical Analyzer for Simple Arithmetic Expressions

This project implements a lexical analyzer (lexer) for simple arithmetic expressions, including support for basic control structures like `if-else` and `while` loops. The lexer reads a source file containing arithmetic expressions and control structures, tokenizes the input, and provides a framework for parsing and processing the tokens.

## Table of Contents

- [Lexical Analyzer for Simple Arithmetic Expressions](#lexical-analyzer-for-simple-arithmetic-expressions)
  - [Table of Contents](#table-of-contents)
  - [Features](#features)
  - [Grammar](#grammar)
  - [Token Definitions](#token-definitions)
  - [Files](#files)
  - [How to Build](#how-to-build)
  - [How to Run](#how-to-run)
  - [Code Overview](#code-overview)
  - [Example Input File](#example-input-file)
  - [Error Handling](#error-handling)
  - [Contributing](#contributing)
  - [License](#license)

## Features

- Tokenizes arithmetic expressions with operators: `+`, `-`, `*`, `/`
- Supports integer literals and identifiers
- Includes basic control structures: `if-else` and `while` loops
- Reads input from a text file (`ex1.txt`)

## Grammar

```
<program> -> <start_op> <else_ifadesi> <finish_op> 
          | <start_op> <var> <assign_op> <expr> <finish_op>
          | <start_op> <while_ifadesi> <finish_op>
          | <start_op> <ifelse> <finish_op>

<else_ifadesi> -> <eger> <var> <karsilastirma_op> <factor> <ise> <var> <assign_op> <factor> <degilse> <var> <assign_op> <factor>

<start_op> -> <
<finish_op> -> >
<var> -> <type> <id>
<type> -> int
<id> -> <letter> <id2>
<id2> -> <letter> <id2> | <digit> <id2> | ?
<expr> -> <term> - <expr> | <term> + <expr> | <term>
<term> -> <factor> / <term> | <factor> * <term> | <factor>
<factor> -> <digit> <factor> | <digit>
<digit> -> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
<letter> -> a | b | c | ç | d | e | f | g | ? | h | ? | i | j | k | l | m | n | o | ö | p | r | s | ? | t | u | ü | v | y | z
<assign_op> -> =
<ise> -> ise
<degilse> -> degilse
<eger> -> eger
<karsilastirma_op> -> < | > | <= | >= | == | !=

<while_ifadesi> -> <while_op> <var> <karsilastirma_op> <factor> <do_op> <program>
<while_op> -> while
<do_op> -> do
```

## Token Definitions

- `LETTER` (0)
- `DIGIT` (1)
- `UNKNOWN` (99)
- `INT_LIT` (10)
- `IDENT` (11)
- `VAR` (12)
- `ASSIGN_OP` (20)
- `ADD_OP` (21)
- `SUB_OP` (22)
- `MULT_OP` (23)
- `DIV_OP` (24)
- `LEFT_PAREN` (25)
- `RIGHT_PAREN` (26)
- `START_OP` (30)
- `FINISH_OP` (31)
- `EGER` (40)
- `KARSILASTIRMA_OP` (41)
- `DEGILSE` (45)
- `ATAMA_DURUMU` (46)
- `KARSILASTIRMA_DURUMU` (48)
- `EGER_IFADESI` (50)
- `ISE` (51)
- `WHILE` (52)
- `DO` (53)

## Files

- `front.c`: The main source code file containing the lexer and parser implementation.
- `ex1.txt`: Example input file containing test expressions and control structures.

## How to Build

1. Ensure you have a C compiler installed (e.g., GCC).
2. Compile the source code:

```sh
gcc front.c -o lexer
```

## How to Run

1. Create or modify the `ex1.txt` file to include the arithmetic expressions and control structures you want to analyze.
2. Run the compiled lexer:

```sh
./lexer
```

## Code Overview

- **Global Variables**: The lexer uses several global variables for tracking the current state, including `charClass`, `lexeme`, `nextChar`, `lexLen`, `token`, `nextToken`, and `in_fp`.
- **Character Classes**: Defined for letters, digits, and unknown characters.
- **Token Codes**: Constants representing different token types.
- **Function Declarations**: Includes standard functions for lexical analysis (`addChar`, `getChar`, `getNonBlank`, `lex`, `lookup`) and custom functions for parsing and error handling (`program`, `start_op`, `finish_op`, `var`, `type`, `id`, `id2`, `expr`, `term`, `factor`, `digit`, `letter`, `assign_op`, `eger_ifadesi`, `karsilastirma_op`, `degilse`, `ise`, `while_ifadesi`, `while_op`, `do_op`, `err`).

## Example Input File

`ex1.txt`:
```txt
{
    sayi x = 10
    eger x > 5 ise
        sayi y = 20
    degilse
        sayi z = 30
}
```

## Error Handling

The `err` function prints an error message and exits the program if an unexpected token or syntax error is encountered during parsing.

## Contributing

1. Fork the repository.
2. Create your feature branch (`git checkout -b feature/AmazingFeature`).
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`).
4. Push to the branch (`git push origin feature/AmazingFeature`).
5. Open a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

By following this README, you should be able to understand, build, and run the lexical analyzer for simple arithmetic expressions. Feel free to contribute or modify the code as needed for your use case.
