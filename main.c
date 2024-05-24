#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/* Global declarations */
/* Variables */
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp, *fopen();

/*
* BNF Grammar
* <program> -> <statement_list>
* <statement_list> -> <statement> | <statement> <statement_list>
* <statement> -> <assignment> | <if_statement> | <while_loop>
* <assignment> -> <var> <assign_op> <expr> <finish_op>
* <if_statement> -> if <condition> <start_op> <statement_list> <finish_op> <else_part> <finish_op>
* <else_part> -> else <start_op> <statement_list> <finish_op> | ε
* <while_loop> -> while <condition> <start_op> <statement_list> <finish_op>
* <condition> -> <expr> <comp_op> <expr>
* <comp_op> -> == | != | < | > | <= | >=
*/

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
/* Custom token codes */
#define VAR 12
#define START_OP 30
#define FINISH_OP 31
#define IF_OP 32
#define ELSE_OP 33
#define WHILE_OP 34
#define COMP_OP 35

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();
int lookup(char ch);

/* Custom functions */
void program();
void statement_list();
void statement();
void assignment();
void if_statement();
void else_part();
void while_loop();
void condition();
void comp_op();
void var();
void type();
void id();
void id2();
void expr();
void term();
void factor();
void digit();
void letter();
void assign_op();
void err();

/* Main driver */
int main() {
    if ((in_fp = fopen("ex.txt", "r")) == NULL)
        printf("Dosya Okuma Hatası. \n");
    else {
        getChar();
        do {
            lex();
            program();
        } while (nextToken != EOF);
    }
}

/* Open the input data file and process its contents
lookup - a function to lookup operators and parentheses
and return the token */
int lookup(char ch) {
    switch (ch) {

    case '(':
        addChar();
        nextToken = LEFT_PAREN;
        break;

    case ')':
        addChar();
        nextToken = RIGHT_PAREN;
        break;

    case '+':
        addChar();
        nextToken = ADD_OP;
        break;

    case '-':
        addChar();
        nextToken = SUB_OP;
        break;

    case '*':
        addChar();
        nextToken = MULT_OP;
        break;

    case '/':
        addChar();
        nextToken = DIV_OP;
        break;

    /* Assigment operator assigned to "=" */
    case '=':
        addChar();
        nextToken = ASSIGN_OP;
        break;

    /* Start operator assigned to "<" */
    case '{':
        addChar();
        nextToken = START_OP;
        break;

    /* Finish operator assigned to ">" */
    case '}':
        addChar();
        nextToken = FINISH_OP;
        break;

    /* Custom token codes */
    case 'i':
        addChar();
        nextToken = IF_OP;
        break;

    case 'e':
        addChar();
        nextToken = ELSE_OP;
        break;

    case 'w':
        addChar();
        nextToken = WHILE_OP;
        break;

    default:
        addChar();
        nextToken = EOF;
        break;
    }
    return nextToken;
}

/* addChar - a function to add nextChar to lexeme */
void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    } else
        printf("Hata. \n");
}

/* getChar - a function to get the next character of
input and determine its character class */
void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else if (nextChar == '<' || nextChar == '>')
            charClass = UNKNOWN; /* Custom operators */
        else
            charClass = UNKNOWN;
    } else
        charClass = EOF;
}

/* getNonBlank - a function to call getChar until it
returns a non-whitespace character */
void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}

/* lex - a simple lexical analyzer for arithmetic
expressions */
int lex() {
    lexLen = 0;
    getNonBlank();
    switch (charClass) {
    /* Parse identifiers */
    case LETTER:
        addChar();
        getChar();
        while (charClass == LETTER || charClass == DIGIT) {
            addChar();
            getChar();
        }

        /* If the read lexeme is a variable, it assigns the value VAR to the nextToken
        If the read lexeme is a identifier, it assigns the value IDENT to the nextToken
        (The isEqual function is defined below the code block) */
        if (isCorrectDataType(lexeme)) {
            nextToken = VAR;
        } else {
            nextToken = IDENT;
        }
        break;

    /* Parse integer literals */
    case DIGIT:
        addChar();
        getChar();
        while (charClass == DIGIT) {
            addChar();
            getChar();
        }
        nextToken = INT_LIT;
        break;

    /* Parse custom operators */
    case UNKNOWN:
        lookup(nextChar);
        getChar();
        break;

    /* EOF */
    case EOF:
        nextToken = EOF;
        lexeme[0] = 'E';
        lexeme[1] = 'O';
        lexeme[2] = 'F';
        lexeme[3] = 0;
        break;
    } /* End of switch */

    printf("Sonraki sozcuk: %d, sonraki sozcukbirim %s\n", nextToken, lexeme);
    return nextToken;
} /* End of function lex */

/* Custom functions */

void start_op() {
    if (nextToken == START_OP) {
        lex(); // Consume '{'
    } else {
        printf("Hata: '{' bekleniyor.\n");
        err();
    }
}
void finish_op() {
    if (nextToken == FINISH_OP) {
        lex(); // Consume '}'
    } else {
        printf("Hata: '}' bekleniyor.\n");
        err();
    }
}


/* <program> -> <statement_list> */
void program() {
    printf("Program Baslatiliyor...\n");
    statement_list();
}

/* <statement_list> -> <statement> | <statement> <statement_list> */
void statement_list() {
    statement();
    while (nextToken != FINISH_OP && nextToken != EOF) {
        statement();
    }
}

/* <statement> -> <assignment> | <if_statement> | <while_loop */
void statement() {
    if (nextToken == VAR) {
        assignment();
    } else if (nextToken == IF_OP) {
        if_statement();
    } else if (nextToken == WHILE_OP) {
        while_loop();
    } else {
        printf("Hata: Geçersiz ifade.\n");
        err();
    }
}

/* <assignment> -> <var> <assign_op> <expr> <finish_op> */
void assignment() {
    printf("Giris --> <assignment>\n");
    var();
    assign_op();
    expr();
    finish_op();
    printf("Cikis --> <assignment>\n");
}

/* <if_statement> -> if <condition> <start_op> <statement_list> <finish_op> <else_part> <finish_op> */
void if_statement() {
    printf("Giris --> <if_statement>\n");
    if (nextToken == IF_OP) {
        lex(); // Consume 'if'
        condition();
        start_op();
        statement_list();
        finish_op();
        else_part();
        finish_op();
    } else {
        printf("Hata: 'if' bekleniyor.\n");
        err();
    }
    printf("Cikis --> <if_statement>\n");
}

/* <else_part> -> else <start_op> <statement_list> <finish_op> | ε */
void else_part() {
    printf("Giris --> <else_part>\n");
    if (nextToken == ELSE_OP) {
        lex(); // Consume 'else'
        start_op();
        statement_list();
        finish_op();
    }
    printf("Cikis --> <else_part>\n");
}

/* <while_loop> -> while <condition> <start_op> <statement_list> <finish_op> */
void while_loop() {
    printf("Giris --> <while_loop>\n");
    if (nextToken == WHILE_OP) {
        lex(); // Consume 'while'
        condition();
        start_op();
        statement_list();
        finish_op();
    } else {
        printf("Hata: 'while' bekleniyor.\n");
        err();
    }
    printf("Cikis --> <while_loop>\n");
}

/* <condition> -> <expr> <comp_op> <expr> */
void condition() {
    printf("Giris --> <condition>\n");
    expr();
    comp_op();
    expr();
    printf("Cikis --> <condition>\n");
}

/* <comp_op> -> == | != | < | > | <= | >= */
void comp_op() {
    printf("Giris --> <comp_op>\n");
    // Assuming the comparison operator is a single character
    if (nextToken == ASSIGN_OP || nextToken == ADD_OP || nextToken == SUB_OP || nextToken == MULT_OP || nextToken == DIV_OP) {
        lex(); // Consume comparison operator
    } else {
        printf("Hata: Karşılaştırma operatörü bekleniyor.\n");
        err();
    }
    printf("Cikis --> <comp_op>\n");
}

void var() {
    printf("Giris --> <var>\n");
    if (nextToken == VAR) {
        lex(); // Consume variable
    } else {
        printf("Hata: Değişken bekleniyor.\n");
        err();
    }
    printf("Cikis --> <var>\n");
}

void type() {
    printf("Giris --> <type>\n");
    if (nextToken == INT_LIT) {
        lex(); // Consume data type
    } else {
        printf("Hata: Veri tipi bekleniyor.\n");
        err();
    }
    printf("Cikis --> <type>\n");
}

void id() {
    printf("Giris --> <id>\n");
    if (nextToken == IDENT) {
        lex(); // Consume identifier
    } else {
        printf("Hata: Tanımlayıcı bekleniyor.\n");
        err();
    }
    printf("Cikis --> <id>\n");
}

void id2() {
    printf("Giris --> <id2>\n");
    if (nextToken == IDENT) {
        lex(); // Consume identifier
    } else {
        printf("Hata: Tanımlayıcı bekleniyor.\n");
        err();
    }
    printf("Cikis --> <id2>\n");
}

void expr() {
    printf("Giris --> <expr>\n");
    term();
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        lex(); // Consume operator
        term();
    }
    printf("Cikis --> <expr>\n");
}

void term() {
    printf("Giris --> <term>\n");
    factor();
    while (nextToken == MULT_OP || nextToken == DIV_OP) {
        lex(); // Consume operator
        factor();
    }
    printf("Cikis --> <term>\n");
}

void factor() {
    printf("Giris --> <factor>\n");
    if (nextToken == INT_LIT || nextToken == IDENT) {
        lex(); // Consume integer literal or identifier
    } else if (nextToken == LEFT_PAREN) {
        lex(); // Consume '('
        expr();
        if (nextToken == RIGHT_PAREN) {
            lex(); // Consume ')'
        } else {
            printf("Hata: ')' bekleniyor.\n");
            err();
        }
    } else {
        printf("Hata: Geçersiz ifade.\n");
        err();
    }
    printf("Cikis --> <factor>\n");
}

void digit() {
    printf("Giris --> <digit>\n");
    if (nextToken == INT_LIT) {
        lex(); // Consume digit
    } else {
        printf("Hata: Sayı bekleniyor.\n");
        err();
    }
    printf("Cikis --> <digit>\n");
}

void letter() {
    printf("Giris --> <letter>\n");
    if (nextToken == IDENT) {
        lex(); // Consume letter
    } else {
        printf("Hata: Harf bekleniyor.\n");
        err();
    }
    printf("Cikis --> <letter>\n");
}

 void assign_op() {
    printf("Giris --> <assign_op>\n");
    if (nextToken == ASSIGN_OP) {
        lex(); // Consume assignment operator
    } else {
        printf("Hata: Atama operatörü bekleniyor.\n");
        err();
    }
    printf("Cikis --> <assign_op>\n");

}

void err() {
    printf("Hata: Program sonlandırılıyor.\n");
    exit(0);
}


/* isCorrectDataType function */
int isCorrectDataType(char *lexeme){
    if(strcmp(lexeme, "int") == 0 || strcmp(lexeme, "baris") == 0 || strcmp(lexeme, "furkan") == 0 || strcmp(lexeme, "deniz") == 0){
        return 1;
    }
    else{
        return 0;
    }
}

/* End of custom functions */
