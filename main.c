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
* <programs> -> <durum_list>
* <durum_list> -> <karsilastirma_durumu> | <atama_durumu>
* <karsilastirma_durumu> -> <start_op> <eger_ifadesi>
* <eger_ifadesi> -> <eger> <var><karsilastirma_op> <factor> <dogruluk_kontrol> <degilse> <dogruluk_kontrol> <finish_op>
* <eger> -> eger
* <karsilastirma_op> -> < | > | <= | >= | == | !=
* <dogruluk_kontrol> -> <dogruluk>
* <dogruluk> -> dogru | yanlis
* <degilse> -> degilse
* <atama_durumu> -> <program>
*
* <program> -> <start_op> <var> <assign_op> <expr> <finish_op>
* <start_op> -> <
* <finish_op> -> >
* <var> -> <type> <id>
* <type> -> int
* <id> -> <letter> <id2>
* <id2> -> <letter> <id2> | <digit> <id2> | ε
* <expr> -> <term> - <expr> | <term> + <expr> | <term>
* <term> -> <factor> / <term> | <factor> * <term> | <factor>
* <factor> -> <digit> <factor> | <digit>
* <digit> -> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
* <letter> -> a | b | c | ç | d | e | f | g | ğ | h | ı | i | j | k | l | m | n | o | ö | p | r | s | ş | t | u | ü | v | y | z
* <assign_op> -> =
*

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
void dogruluk_kontrol();
void dogruluk();
void degilse();
void atama_durumu();
void durum_list();
void karsilastirma_durumu();
void programs();



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
#define DOGRULUK_KONTROL 42
#define DOGRU 43
#define YANLIS 44
#define DEGILSE 45
#define ATAMA_DURUMU 46
#define DURUM_LIST 47
#define KARSILASTIRMA_DURUMU 48
#define PROGRAMS 49
#define EGER_IFADESI 50





/* Main driver */
int main() {
	if ((in_fp = fopen("ex.txt", "r")) == NULL)
 		printf("Dosya Okuma Hatası. \n");
 	else {
 		getChar();
 	do {
 		lex();
 		programs();
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


/* Custom functions */

/* <program> -> <start_op> <var> <assign_op> <expr> <finish_op> */
void program(){
	printf("Program Baslatiliyor...\n");
	start_op();
	var();
	assign_op();
	expr();
	finish_op();
}

/* <start_op> -> < */
void start_op(){
	printf("Giris --> <start_op>\n");
	if(nextToken == START_OP){
		lex();
	}
	else{
		printf("Hata: <start_op> bekleniyor.\n");
		err();
	}
	printf("Cikis --> <start_op>\n");
}

/* <finish_op> -> > */
void finish_op(){
	printf("Giris --> <finish_op>\n");
	printf("nextToken: %d\n", nextToken);
	if(nextToken == FINISH_OP){
		lex();
		printf("Program Sonlandiriliyor...\n");
		//exit(0);
	} else {
		printf("Hata: <finish_op> bekleniyor.\n");
		err();
	}
	printf("Cikis --> <finish_op>\n");
}

/* <var> -> <type> <id> */
void var(){
	printf("Giris --> <var>\n");
	type();
	id();
	printf("Cikis --> <var>\n");
}

/* <type> -> int */
void type(){
	printf("Giris --> <type>\n");
	if(nextToken == VAR){
		lex();
	}
	else{
		printf("Hata: <type> bekleniyor.\n");
		err();
	}
	printf("Cikis --> <type>\n");
}

/* <id> -> <letter> <id2> */
void id(){
	printf("Giris --> <id>\n");
	letter();
	id2();
	printf("Cikis --> <id>\n");
}

/* <id2> -> <letter> <id2> | <digit> <id2> | ε */
void id2(){
	printf("Giris --> <id2>\n");
	if(nextToken == IDENT){
		lex();
	}
	else if(nextToken == INT_LIT){
		lex();
	}
	else{
		return;
	}
	printf("Cikis --> <id2>\n");
}

/* <expr> -> <term> - <expr> | <term> + <expr> | <term> */
void expr(){
	printf("Giris --> <expr>\n");
	term();
	if(nextToken == SUB_OP){
		lex();
		expr();
	}
	else if(nextToken == ADD_OP){
		lex();
		expr();
	}
	printf("Cikis --> <expr>\n");
}

/* <term> -> <factor> / <term> | <factor> * <term> | <factor> */
void term(){
	printf("Giros --> <term>\n");
	factor();
	if(nextToken == DIV_OP){
		lex();
		term();
	}
	else if(nextToken == MULT_OP){
		lex();
		term();
	}
	printf("Cikis --> <term>\n");
}

/* <factor> -> <digit> <factor> | <digit> */
void factor(){
	printf("Giris --> <factor>\n");
	digit();
	if(nextToken == INT_LIT){
		lex();
	}
	printf("Cikis --> <factor>\n");
}
/* <digit> -> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 */
void digit(){
	printf("Giris --> <digit>\n");
	if(nextToken == INT_LIT){
		lex();
	}
	printf("Cikis --> <digit>\n");
}

/* <letter> -> a | b | c | ç | d | e | f | g | ğ | h | ı | i | j | k | l | m | n | o | ö | p | r | s | ş | t | u | ü | v | y | z */
void letter(){
	printf("Giris --> <letter>\n");
	if(nextToken == IDENT){
		lex();
	}
	printf("Cikis --> <letter>\n");
}

/* <assign_op> -> = */
void assign_op(){
	printf("Giris --> <assign_op>\n");
	if(nextToken == ASSIGN_OP){
		lex();
	}
	else{
		printf("Hata	: <assign_op> bekleniyor.\n");
		err();
	}
	printf("Cikis --> <assign_op>\n");
}

/* Error function */
void err(){
	printf("Hata fonksiyonu calisti. \n");
	exit(0);
}


/* End of custom functions */

/* custome funcs for if else */

/* <eger_ifadesi> -> <eger> <var><karsilastirma_op> <factor> <dogruluk_kontrol> <degilse> <dogruluk_kontrol> <finish_op> */
void eger_ifadesi(){
	printf("Giris --> <eger_ifadesi>\n");
	eger();
	var();
	karsilastirma_op();
	factor();
	dogruluk_kontrol();
	degilse();
	dogruluk_kontrol();
	finish_op();
	printf("Cikis --> <eger_ifadesi>\n");
}

/* <eger> -> eger */
void eger(){
	printf("Giris --> <eger>\n");
	if(nextToken == EGER){
		lex();
	}
	else{
		printf("Hata: <eger> bekleniyor.\n");
		err();
	}
	printf("Cikis --> <eger>\n");
}

/* <karsilastirma_op> -> < | > | <= | >= | == | != */
void karsilastirma_op(){
	printf("Giris --> <karsilastirma_op>\n");
	if(nextToken == KARSILASTIRMA_OP){
		lex();
	}
	else{
		printf("Hata: <karsilastirma_op> bekleniyor.\n");
		err();
	}
	printf("Cikis --> <karsilastirma_op>\n");
}

/* <dogruluk_kontrol> -> <dogruluk> */
void dogruluk_kontrol(){
	printf("Giris --> <dogruluk_kontrol>\n");
	if(nextToken == DOGRULUK_KONTROL){
		lex();
	}
	else{
		printf("Hata: <dogruluk_kontrol> bekleniyor.\n");
		err();
	}
	printf("Cikis --> <dogruluk_kontrol>\n");
}

/* <dogruluk> -> dogru | yanlis */
void dogruluk(){
	printf("Giris --> <dogruluk>\n");
	if(nextToken == DOGRU){
		lex();
	}
	else if(nextToken == YANLIS){
		lex();
	}
	else{
		printf("Hata: <dogruluk> bekleniyor.\n");
		err();
	}
	printf("Cikis --> <dogruluk>\n");
}

/* <degilse> -> degilse */
void degilse(){
	printf("Giris --> <degilse>\n");
	if(nextToken == DEGILSE){
		lex();
	}
	else{
		printf("Hata: <degilse> bekleniyor.\n");
		err();
	}
	printf("Cikis --> <degilse>\n");
}

/* <atama_durumu> -> <program> */
void atama_durumu(){
	printf("Giris --> <atama_durumu>\n");
	program();
	printf("Cikis --> <atama_durumu>\n");
}

/* <durum_list> -> <karsilastirma_durumu> | <atama_durumu> */
void durum_list(){
	printf("Giris --> <durum_list>\n");
	if(nextToken == KARSILASTIRMA_DURUMU){
		lex();

		karsilastirma_durumu();
	}
	else if(nextToken == ATAMA_DURUMU){
		lex();

		atama_durumu();
	}
	else{
		printf("Hata: <karsilastirma_durumu> veya <atama_durumu> bekleniyor.\n");
		err();
	}
	printf("Cikis --> <durum_list>\n");
}

/* <karsilastirma_durumu> -> <start_op> <eger_ifadesi> */

void karsilastirma_durumu(){
	printf("Giris --> <karsilastirma_durumu>\n");
	start_op();
	eger_ifadesi();
	printf("Cikis --> <karsilastirma_durumu>\n");
}

/* <programs> -> <durum_list> */
void programs(){
	printf("Giris --> <programs>\n");
	lex();

	durum_list();
	printf("Cikis --> <programs>\n");
}

/* correct data type func for if else and program */
int isCorrectDataType(char *lexeme){
	if(strcmp(lexeme, "int") == 0){
		return 1;
	}
	else{
		return 0;
	}
}

/* End of custom functions */







