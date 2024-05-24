/* front.c - a lexical analyzer system for simple              arithmetic expressions */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


/* Global declarations */
/* Variables */
int charClass;
char lexeme [100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp, *fopen();

/*
 * <program> -> <bas_op> + <deg> + <atama_op> + <cumle>
 * <bas_op> -> _
 * <deg> -> <harf> <degisken>
 * <degisken> -> <harf> <degisken> | <sayi> <degisken> | <empty>
 * <atama_op> -> =
 * <cumle> -> <islem> | <faktör>
 * <islem> -> <terim> + <terim> | <terim> - <terim>
 * <terim> -> <faktör> * <faktör> | <faktör> / <faktör>
 * <faktör> -> <sayi> <faktör> | <sayi>
 * <sayi> -> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
 * <harf> -> a | b |c | ç | d | e | f | g | ğ | h | ı | i | j | k | l | m | n | o | ö | p | r | s | ş | t | u | ü | v | y | z
 *
 * write a program that will parse the given grammar and print the result of the parsing.
 * The program should read the input from a file called "front.in" and write the output to a file called "front.out"
 * /
	*/



/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();
/* Custom Function declarations */
int lookup(char ch);
void program();
void bas_op();
void deg();
void degisken();
void atama_op();
void cumle();
void islem();
void terim();
void faktor();
void sayi();
void harf();
void bitis_op();

int isEqual(char *str1, char *str2);
int isCorrectDataType(char *str);
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
/* Custom Token codes */
#define VAR 12
#define START_OP 30
#define FINISH_OP 31




/* Main driver */
int main() {
	if ((in_fp = fopen("ex.txt", "r")) == NULL)
 		printf("Hata. \n");
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
		case '<':
 			addChar();
 			nextToken = START_OP;
 			break;

		/* Finish operator assigned to ">" */
		case '>':
 			addChar();
 			nextToken = FINISH_OP;
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
		if(isCorrectDataType(lexeme)){
			nextToken = VAR;
		}
		else{
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
		break;
	} 	/* End of switch */

	printf("Sonraki sozcuk: %d, sonraki sozcukbirim %s\n",
	nextToken, lexeme);
	return nextToken;
} /* End of function lex */













/* Function program
 * <program> -> <bas_op> + <deg> + <atama_op> + <cumle>
 * This function will parse the given grammar and print the result of the parsing.
 */
void program(){
	bas_op();
	deg();
	atama_op();
	cumle();
	bitis_op();
}

/* Function bas_op
 * <bas_op> -> _
 * This function will parse the given grammar and print the result of the parsing.
 */
void bas_op(){
	printf("bas_op: %d\n", nextToken);
	if(nextToken ==  START_OP ){
		lex();

	}
	else{
		err();
	}
}

/* Function deg
 * <deg> -> <harf> <degisken>
 * This function will parse the given grammar and print the result of the parsing.
 */
void deg(){
	printf("Deg: %d\n", nextToken);
	harf();
	degisken();
}

/* Function degisken
 * <degisken> -> <harf> <degisken> | <sayi> <degisken> | <empty>
 * This function will parse the given grammar and print the result of the parsing.
 */
void degisken(){
	printf("degisken: %d\n", nextToken);
	if(nextToken == VAR){
		lex();

		degisken();
	}
	else if(nextToken == INT_LIT){
		lex();

		degisken();
	}
	else{
		return;
	}
}

/* Function atama_op
 * <atama_op> -> =
 * This function will parse the given grammar and print the result of the parsing.
 */
void atama_op(){
	if(nextToken == ASSIGN_OP){
		lex();

	}
	else{
		err();
	}
}

/* Function cumle
 * <cumle> -> <islem> | <faktör>
 * This function will parse the given grammar and print the result of the parsing.
 */
void cumle(){
	if(nextToken == INT_LIT){
		printf("\n Faktor");
		faktor();

	}
	else{
		printf("işlem");
		islem();
	}
}


/* Function islem
 * <islem> -> <terim> + <terim> | <terim> - <terim>
 * This function will parse the given grammar and print the result of the parsing.
 */
void islem(){
	terim();
	if(nextToken == ADD_OP){
		lex();

		terim();
	}
	else if(nextToken == SUB_OP){
		lex();

		terim();
	}
	else{
		err();
	}
}

/* Function terim
 * <terim> -> <faktör> * <faktör> | <faktör> / <faktör>
 * This function will parse the given grammar and print the result of the parsing.
 */
void terim(){
	faktor();
	if(nextToken == MULT_OP){
		lex();

		faktor();
	}
	else if(nextToken == DIV_OP){
		lex();

		faktor();
	}
	else{
		err();
	}
}

/* Function faktor
 * <faktör> -> <sayi> <faktör> | <sayi>
 * This function will parse the given grammar and print the result of the parsing.
 */
void faktor(){
	sayi();
	if(nextToken == INT_LIT){
		lex();

		faktor();
	}
	else{
		if (nextToken == LEFT_PAREN) {
			lex();
			cumle();
			if (nextToken == RIGHT_PAREN)
				lex();
			else
				printf("Hata.");
		} /* End of if (nextToken == ... */
	}
}

/* Function sayi
 * <sayi> -> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
 * This function will parse the given grammar and print the result of the parsing.
 */
void sayi(){
	printf("Sayi: %d\n", nextToken);
	if(nextToken == INT_LIT){
		lex();
	}
	else{
		err();
	}
}


/* Function harf
 * <harf> -> a | b |c | ç | d | e | f | g | ğ | h | ı | i | j | k | l | m | n | o | ö | p | r | s | ş | t | u | ü | v | y | z
 * This function will parse the given grammar and print the result of the parsing.
 */
void harf(){
	printf("Harf: %d\n", nextToken);
	if(nextToken == IDENT ){
		printf("burada hata var");
		lex();

	}
	else{
		err();
	}
}

/* Function bitis_op
 * <bitis_op> -> _
 * This function will parse the given grammar and print the result of the parsing.
 */
void bitis_op(){
	printf("\n bitis_op: %d\n", nextToken);
	if(nextToken == FINISH_OP){
		lex();

	}
	else{
		err();
	}
}

/* Function isEqual
 * This function will compare two strings and return 1 if they are equal, otherwise it will return 0.
 */
int isEqual(char *str1, char *str2){
	if(strcmp(str1, str2) == 0){
		return 1;
	}
	else{
		return 0;
	}
}

/* Function isCorrectDataType
 * This function will check if the given string is a correct data type.
 * If the string is a correct data type, it will return 1, otherwise it will return 0.
 */
/* isCorrectDataType - In the method with type restriction,
only digit and string values are defined. */


int isCorrectDataType(char* string) {
	if (!strcmp(string, "sayi") || !strcmp(string, "soz"))
		return 1;
	else{
		return 0;
	}
}

/* Function err
 * This function will print an error message and exit the program.
 */
void err() {
	printf("Error Function");
	exit(1);
}



/* generete test txt file codes.
 *
 * _a1 = 1 + 2
 *
 * /
	*/





