#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define IF 30
#define ELSE 31
#define INT 32  // "int" anahtar kelimesi için yeni bir belirteç

FILE* in_fp;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
int charClass;
char symbolTable[100][100];  // Değişkenleri depolamak için sembol tablosu
int symbolCount = 0;

void addChar();
void getChar();
void getNonBlank();
int lex();
int lookup(char ch);
void expr();
void factor();
void term();
void assignment();
void ifStatement();
void elseStatement();
void variableDeclaration();  // Değişken bildirimini işlemek için fonksiyon

int main() {
    /* Giriş veri dosyasını aç ve içeriğini işle */
    if ((in_fp = fopen("sample1.txt", "r")) == NULL) {
        printf("HATA - front.in açılamadı\n");
        return 1;
    } else {
        getChar();
        do {
            lex();
            if (nextToken == INT) {
                variableDeclaration();
            } else if (nextToken == IDENT && nextChar == '=') {
                assignment();
            } else if (nextToken == IF) {
                ifStatement();
            } else {
                expr();
            }
        } while (nextToken != EOF);
    }

    return 0;
}

void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = '\0';  // Lexeme'yi null karakterle sonlandır
    } else {
        printf("Hata - lexeme çok uzun\n");
    }
}

void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = UNKNOWN;
    } else {
        charClass = EOF;
    }
}

void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}

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
        case '=':
            addChar();
            nextToken = ASSIGN_OP;
            break;
        default:
            addChar();
            nextToken = EOF;
            break;
    }

    return nextToken;
}

int lex() {
    lexLen = 0;
    getNonBlank();

    switch (charClass) {
        /* Tanımlayıcıları ve anahtar kelimeleri ayrıştır */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }

            if (strcmp(lexeme, "ise") == 0)
                nextToken = IF;
            else if (strcmp(lexeme, "degilse") == 0)
                nextToken = ELSE;
            else if (strcmp(lexeme, "sayi") == 0)  // "int" anahtar kelimesini kontrol et
                nextToken = INT;  // INT belirteci ataması
            else
                nextToken = IDENT;
            break;
        /* Tamsayıları ayrıştır */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;
        /* Parantezler ve operatörler */
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
            lexeme[3] = '\0';
            break;
    }

    printf("Sonraki belirteç: %d, Sonraki lexeme: %s\n", nextToken, lexeme);
    return nextToken;
}

void assignment() {

    printf("<atama> gir\n");
    strcpy(symbolTable[symbolCount], lexeme);  // Değişken adını sembol tablosuna kopyala
    symbolCount++;

    lex();  // '=' belirteci tüket
    expr();

    printf("Değişkene atama yap: %s\n", symbolTable[symbolCount - 1]);
    printf("<atama> çık\n");
}

void ifStatement() {
    printf("<ifDeyimi> gir\n");
    lex();  // 'if' belirteci tüket
    expr();

    // If bloğunu işle
    printf("<ifBlok> gir\n");
    expr();
    printf("<ifBlok> çık\n");

    if (nextToken == ELSE) {
        // Else bloğunu işle
        printf("<elseBlok> gir\n");
        lex();  // 'else' belirteci tüket
        expr();
        printf("<elseBlok> çık\n");
    }

    printf("<ifDeyimi> çık\n");
}

void expr() {
    printf("<ifade> gir\n");
    term();
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        int currentToken = nextToken;  // Geçerli belirteci sakla
        lex();
        term();

        if (currentToken == ADD_OP) {
            printf("Toplama işlemi yap\n");
        } else if (currentToken == SUB_OP) {
            printf("Çıkarma işlemi yap\n");
        }
    }
    printf("<ifade> çık\n");
}

void term() {
    printf("<terim> gir\n");
    factor();
    while (nextToken == MULT_OP || nextToken == DIV_OP) {
        int currentToken = nextToken;  // Geçerli belirteci sakla
        lex();
        factor();

        if (currentToken == MULT_OP) {
            printf("Çarpma işlemi yap\n");
        } else if (currentToken == DIV_OP) {
            printf("Bölme işlemi yap\n");
        }
    }
    printf("<terim> çık\n");
}

void factor() {
    printf("<faktör> gir\n");
    if (nextToken == IDENT || nextToken == INT_LIT) {
        lex();
    } else {
        if (nextToken == LEFT_PAREN) {
            lex();
            expr();
            if (nextToken == RIGHT_PAREN) {
                lex();
            } else {
                printf("Hata - ')' bekleniyor\n");
            }
        } else {
            printf("Hata - Tanımlayıcı, tamsayı veya '(' bekleniyor\n");
        }
    }
    printf("<faktör> çık\n");
}

void variableDeclaration() {
    printf("<değişkenBildirimi> gir\n");
    lex();  // 'int' belirteci tüket

    if (nextToken == IDENT) {
        strcpy(symbolTable[symbolCount], lexeme);  // Değişken adını sembol tablosuna kopyala
        symbolCount++;
        lex();  // Tanımlayıcıyı tüket
        if (nextToken == ASSIGN_OP) {
            lex();  // '=' belirteci tüket
            expr();
            printf("Değişkene atama yap: %s\n", symbolTable[symbolCount - 1]);
        }
    } else {
        printf("Hata - 'int' kelimesinden sonra tanımlayıcı bekleniyor\n");
    }

    printf("<değişkenBildirimi> çık\n");
}
