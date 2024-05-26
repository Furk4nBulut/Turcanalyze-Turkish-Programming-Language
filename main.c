/* front.c - a lexical analyzer system for simple              arithmetic expressions */
#define _CRT_SECURE_NO_DEPRECATE
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>



/* Global declarations */
/* Variables */
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE* in_fp, * fopen();

/*
<program> -><start_op><else_ifadesi><finish_op>|<start_op><var><assign_op><expr><finish_op>
<else_ifadesi>-><eger><var><karsilastirma_op><factor><ise><var><assign_op><factor><degilse><var><assign_op><factor>
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
<ise>-> ise
<degilse> -> degilse
<eger> -> eger
<karsilastirma_op> -> < | > | <= | >= | == | !=

<while_ifadesi> -> <while_op> <var> <karsilastirma_op> <factor> <do_op> <program>
<while_op> -> while
<do_op> -> do


*
 * /
	*/



	/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();
int lookup(char ch);

/* Custom functions */
void program();
void start_op();
void finish_op();
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

/* Custom Funcs for If Else */
void eger_ifadesi();
void eger();
void karsilastirma_op();
void degilse();
void ise();
void ifelsekontrol();

/* Custom Funcs for While */
void while_ifadesi();
void while_op();
void do_op();



void err();






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

/* Custom token codes for If Else */
#define EGER 40
#define KARSILASTIRMA_OP 41
#define DEGILSE 45
#define ATAMA_DURUMU 46
#define KARSILASTIRMA_DURUMU 48
#define EGER_IFADESI 50
#define ISE 51

/* Custom token codes for While */
#define WHILE 52
#define DO 53








/* Main driver */
int main() {
    setlocale(LC_ALL, "Turkish");
	printf("ğşğşğği");
	if ((in_fp = fopen("ex4.txt", "r")) == NULL)

		printf("Dosya Okuma Hatas?. \n");
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
	default:
			addChar();
			if (strcmp(lexeme, "while") == 0)
				nextToken = WHILE;
			else if (strcmp(lexeme, "do") == 0)
				nextToken = DO;
			else
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
	}
	else
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

		else charClass = UNKNOWN;
	}
	else
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
		}
		else {
			nextToken = IDENT;
		}
		//break;
		if (strcmp(lexeme, "eger") == 0)
			nextToken = EGER;
		else if (strcmp(lexeme, "ise") == 0)
			nextToken = ISE;
		else if (strcmp(lexeme, "degilse") == 0)
			nextToken = DEGILSE;
		else if (strcmp(lexeme, "e?ittir") == 0)
			nextToken = KARSILASTIRMA_DURUMU;
		else if (strcmp(lexeme, "b?y?kt?r") == 0)
			nextToken = KARSILASTIRMA_DURUMU;
		else if (strcmp(lexeme, "k???kt?r") == 0)
			nextToken = KARSILASTIRMA_DURUMU;
		else if (strcmp(lexeme, "surece") == 0)
			nextToken = WHILE;
		else if (strcmp(lexeme, "yap") == 0)
			nextToken = DO;

		else {

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
		/* Parentheses and operators */
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
		printf("Program bitti.\n");
		break;
	} 	/* End of switch */

	printf("Sonraki sozcuk: %d, sonraki sozcukbirim %s\n",
		nextToken, lexeme);
	return nextToken;
} /* End of function lex */


/* Custom functions */

/* <program> -> <start_op> <var> <assign_op> <expr> <finish_op> | <starp_op> <while_ifadesi> <finish_op> | <star_op> <ifelse> <finish_op>  */
// Main program
void program() {
	printf("Program Baslatiliyor...\n");
	start_op();

	ifelsekontrol();

	// Function to handle repetitive VAR checks
	void handle_var_sequence() {
		while (nextToken == VAR) {
			var();
			assign_op();
			expr();
			if (nextToken == WHILE) {
				while_ifadesi();
				return;
			}
			if(nextToken == EGER) {
				eger_ifadesi();
				return;
			}
			if (nextToken == VAR) {
				handle_var_sequence();
			}
		}
	}

	if (nextToken == WHILE) {
		while_ifadesi();
	} else if (nextToken == VAR) {
		handle_var_sequence();
	}

	while (nextToken != FINISH_OP) {
		ifelsekontrol();
		handle_var_sequence();
	}

	finish_op();
}




/* <start_op> -> < */
void start_op() {
	printf("Giris --> <start_op>\n");
	if (nextToken == START_OP) {
		lex();
	}
	else {
		printf("Hata: <start_op> bekleniyor.\n");
		err();
	}
	printf("Cikis --> <start_op>\n");
}

/* <finish_op> -> > */
void finish_op() {
	printf("Giris --> <finish_op>\n");
	printf("nextToken: %d\n", nextToken);
	if (nextToken == FINISH_OP) {
		lex();

	}
	else {
		printf("Hata: <finish_op> bekleniyor.\n");
		err();
	}
	printf("Cikis --> <finish_op>\n");
}

/* <var> -> <type> <id> */
void var() {
	printf("Giris --> <var>\n");
	type();
	id();
	printf("Cikis --> <var>\n");
}

/* <type> -> int */
void type() {
	printf("Giris --> <type>\n");
	if (nextToken == VAR) {
		lex();
	}
	else {
		printf("Hata: <type> bekleniyor.\n");
		err();
	}
	printf("Cikis --> <type>\n");
}

/* <id> -> <letter> <id2> */
void id() {
	printf("Giris --> <id>\n");
	letter();
	id2();
	printf("Cikis --> <id>\n");
}

/* <id2> -> <letter> <id2> | <digit> <id2> | ? */
void id2() {
	printf("Giris --> <id2>\n");
	if (nextToken == IDENT) {
		lex();
	}
	else if (nextToken == INT_LIT) {
		lex();
	}
	else {
		return;
	}
	printf("Cikis --> <id2>\n");
}

/* <expr> -> <term> - <expr> | <term> + <expr> | <term> */
void expr() {
	printf("Giris --> <expr>\n");
	term();
	if (nextToken == SUB_OP) {
		lex();
		expr();
	}
	else if (nextToken == ADD_OP) {
		lex();
		expr();
	}
	printf("Cikis --> <expr>\n");
}

/* <term> -> <factor> / <term> | <factor> * <term> | <factor> */
void term() {
	printf("Giros --> <term>\n");
	factor();
	if (nextToken == DIV_OP) {
		lex();
		term();
	}
	else if (nextToken == MULT_OP) {
		lex();
		term();
	}
	printf("Cikis --> <term>\n");
}

/* <factor> -> <digit> <factor> | <digit> */
void factor() {
	printf("Giris --> <factor>\n");
	digit();
	if (nextToken == INT_LIT) {
		lex();
	}
	printf("Cikis --> <factor>\n");
}
/* <digit> -> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 */
void digit() {
	printf("Giris --> <digit>\n");
	if (nextToken == INT_LIT) {
		lex();
	}
	printf("Cikis --> <digit>\n");
}

/* <letter> -> a | b | c | ? | d | e | f | g | ? | h | ? | i | j | k | l | m | n | o | ? | p | r | s | ? | t | u | ? | v | y | z */
void letter() {
	printf("Giris --> <letter>\n");
	if (nextToken == IDENT) {
		lex();
	}
	printf("Cikis --> <letter>\n");
}

/* <assign_op> -> = */
void assign_op() {
	printf("Giris --> <assign_op>\n");
	if (nextToken == ASSIGN_OP) {
		lex();
	}
	else {
		printf("Hata	: <assign_op> bekleniyor.\n");
		err();
	}
	printf("Cikis --> <assign_op>\n");
}

/* Error function */
	void err() {
	printf("Hata fonksiyonu calisti. \n");
	exit(0);
}


/* End of custom functions */

/* custome funcs for if else */

/* <eger_ifadesi> -> <eger> <var><karsilastirma_op> <factor> <dogruluk_kontrol> <degilse> <dogruluk_kontrol> <finish_op> */
void eger_ifadesi() {
	printf("Giris --> <eger_ifadesi>\n");
	lex();
	var();
	karsilastirma_op();
	karsilastirma_op();
	factor();
	ise();

	program();

	degilse();
	program();

	printf("Cikis --> <eger_ifadesi>\n");
}

/* <eger> -> eger */

/* <karsilastirma_op> -> < | > | <= | >= | == | != */
void karsilastirma_op() {
	printf("Giris --> <karsilastirma_op>\n");
	if (nextToken == ASSIGN_OP) {
		lex();
	}
	else {
		printf("Hata: <karsilastirma_op> bekleniyor.\n");
		err();
	}
	printf("Cikis --> <karsilastirma_op>\n");
}


/* <degilse> -> degilse */
void degilse() {
	printf("Giris --> <degilse>\n");
	if (nextToken == DEGILSE) {
		lex();
	}
	else {
		printf("Hata: <degilse> bekleniyor.\n");
		err();
	}
	printf("Cikis --> <degilse>\n");
}

void ise() {
	printf("Giris --> <ise>\n");
	if (nextToken == ISE) {
		lex();
	}
	else {
		printf("Hata: <ise> bekleniyor.\n");
		err();
	}
	printf("Cikis --> <degilse>\n");
}
void ifelsekontrol() {
	if (nextToken == EGER) {
		eger_ifadesi();
	} else if (nextToken == WHILE) {
		while_ifadesi();
	}
}

/* correct data type func for if else and program */
int isCorrectDataType(char* lexeme) {
	if (strcmp(lexeme, "sayi") == 0) {
		return 1;
	}
	else {
		return 0;
	}
}

/* <while_ifadesi> -> <while_op> <var> <karsilastirma_op> <factor> <do_op> <program> */
void while_ifadesi() {
	printf("Giris --> <while_ifadesi>\n");
	while_op();
	var();
	karsilastirma_op();
	karsilastirma_op();
	factor();
	do_op();
	program();
	printf("Cikis --> <while_ifadesi>\n");
}

/* <while_op> -> while */
void while_op() {
	printf("Giris --> <while_op>\n");
	if (nextToken == WHILE) {
		lex();
	} else {
		printf("Hata: <while_op> bekleniyor.\n");
		err();
	}
	printf("Cikis --> <while_op>\n");
}

/* <do_op> -> do */
void do_op() {
	printf("Giris --> <do_op>\n");
	if (nextToken == DO) {
		lex();
	} else {
		printf("Hata: <do_op> bekleniyor.\n");
		err();
	}
	printf("Cikis --> <do_op>\n");
}




/* End of custom functions */