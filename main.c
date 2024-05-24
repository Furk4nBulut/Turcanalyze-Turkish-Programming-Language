#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* Token kodları */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

/* Özel token kodları */
#define START_OP 30
#define VAR 31
#define FINISH_OP 32
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

/* Karakter sınıfları */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Global değişkenler */
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp;

/* Fonksiyon bildirimleri */
void addChar();
void getChar();
void getNonBlank();
int lex();
int lookup(char ch);
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
void err();
void eger_ifadesi();
void eger();
void karsilastirma_op();
void dogruluk_kontrol();
void degilse();
void atama_durumu();
void durum_list();
void karsilastirma_durumu();
void programs();
int isCorrectDataType(char *lexeme);

int main() {
    if ((in_fp = fopen("ex.txt", "r")) == NULL) {
        printf("Dosya Okuma Hatası.\n");
        return 1;
    }
    getChar();
    do {
        if (lex() == EOF) // Değişiklik burada
            break;       // Değişiklik burada
        programs();
    } while (1);         // Değişiklik burada
    fclose(in_fp);        // Değişiklik burada
    return 0;
}

/* Diğer fonksiyonlar ve kodlar buraya gelecek */

/* getChar - Giriş verisinden bir sonraki karakteri alır ve karakter sınıfını belirler */
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

/* getNonBlank - getChar() fonksiyonunu çağırarak, bir sonraki boşluk olmayan karakteri alır */
void getNonBlank() {
    while (isspace(nextChar)) {
        getChar();
    }
}

/* lookup - Operatörleri ve parantezleri araştıran ve ilgili token kodunu döndüren bir fonksiyon */
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
        case '{':
            addChar();
            nextToken = START_OP;
            break;
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

/* addChar - Sonraki karakteri lexeme'e ekler */
void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = '\0';  // String sonunu belirtmek için '\0' karakterini ekle
    } else {
        printf("Hata: Lexeme boyut sınırını aştı.\n");
    }
}

/* lex - Basit bir leksikal analiz fonksiyonu */
int lex() {
    lexLen = 0;
    getNonBlank();
    switch (charClass) {
        /* Harf dizisi */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = IDENT;
            break;
        /* Sayısal literal */
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
            nextToken = lookup(nextChar); // Değişiklik burada
            getChar();
            break;
        /* Dosya sonu */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = '\0';
            break;
    }
    return nextToken; // Değişiklik burada
}

/* Ana fonksiyonlar ve özel durumlar için gerekli fonksiyonlar */

/* Ana fonksiyonlar */
void programs() {
    printf("Giris --> <programs>\n");
    program();
    printf("Cikis --> <programs>\n");
}
void type() {
    printf("Giris --> <type>\n");
    if (nextToken == VAR) {
        lex();
    } else {
        printf("Hata: <type> bekleniyor.\n");
        exit(1);
    }
    printf("Cikis --> <type>\n");
}
void id() {
    printf("Giris --> <id>\n");
    if (nextToken == IDENT) {
        lex();
    } else {
        printf("Hata: <id> bekleniyor.\n");
        printf("Hata: <id2> bekleniyor.\n");
        exit(1);
    }
    printf("Cikis --> <id>\n");
}

void id2() {
    printf("Giris --> <id2>\n");
    if (nextToken == IDENT) {
        lex();
    } else {
        printf("Hata: <id2> bekleniyor.\n");
        exit(1);
    }
    printf("Cikis --> <id2>\n");
}
void expr() {
    printf("Giris --> <expr>\n");
    if (nextToken == IDENT) {
        id2();
    } else if (nextToken == INT_LIT) {
        lex();
    } else {
        printf("Hata: <expr> bekleniyor.\n");
        exit(1);
    }
    printf("Cikis --> <expr>\n");
}
void term() {
    printf("Giris --> <term>\n");
    if (nextToken == MULT_OP || nextToken == DIV_OP) {
        lex();
    } else {
        printf("Hata: <term> bekleniyor.\n");
        exit(1);
    }
    printf("Cikis --> <term>\n");
}
void factor() {
    printf("Giris --> <factor>\n");
    if (nextToken == LEFT_PAREN) {
        lex();
        expr();
        if (nextToken == RIGHT_PAREN) {
            lex();
        } else {
            printf("Hata: ')' bekleniyor.\n");
            exit(1);
        }
    } else {
        printf("Hata: '(' bekleniyor.\n");
        exit(1);
    }
    printf("Cikis --> <factor>\n");
}
void digit() {
    printf("Giris --> <digit>\n");
    if (nextToken == INT_LIT) {
        lex();
    } else {
        printf("Hata: <digit> bekleniyor.\n");
        exit(1);
    }
    printf("Cikis --> <digit>\n");
}
void letter() {
    printf("Giris --> <letter>\n");
    if (nextToken == IDENT) {
        lex();
    } else {
        printf("Hata: <letter> bekleniyor.\n");
        exit(1);
    }
    printf("Cikis --> <letter>\n");
}
void assign_op() {
    printf("Giris --> <assign_op>\n");
    if (nextToken == ASSIGN_OP) {
        lex();
    } else {
        printf("Hata: <assign_op> bekleniyor.\n");
        exit(1);
    }
    printf("Cikis --> <assign_op>\n");
}
void err() {
    printf("Hata: Geçersiz karakter.\n");
    exit(1);
}

void program();
void durum_list();
void atama_durumu();
void karsilastirma_durumu();

/* Özel durumlar */
void eger_ifadesi();
void eger();
void karsilastirma_op();
void dogruluk_kontrol();
void degilse();
void var(); // Eksik olan fonksiyon
void factor(); // Eksik olan fonksiyon

/* Ana fonksiyonlar */

/* <program> -> <start_op> <durum_list> <finish_op> */
void program() {
    printf("Program basladi...\n");
    start_op();
    durum_list();
    finish_op();
}

/* <start_op> -> { */
void start_op() {
    printf("Giris --> <start_op>\n");
    if (nextToken == START_OP) {
        lex();
    } else {
        printf("Hata: <start_op> bekleniyor.\n");
        exit(1);
    }
    printf("Cikis --> <start_op>\n");
}

/* <finish_op> -> } */
void finish_op() {
    printf("Giris --> <finish_op>\n");
    if (nextToken == FINISH_OP) {
        lex();
        printf("Program sonlandırılıyor...\n");
        exit(0);
    } else {
        printf("Hata: <finish_op> bekleniyor.\n");
        exit(1);
    }
    printf("Çıkış --> <finish_op>\n");
}

/* <durum_list> -> <atama_durumu> | <karsilastirma_durumu> */
void durum_list() {
    printf("Giris --> <durum_list>\n");
    if (nextToken == ATAMA_DURUMU) {
        atama_durumu();
    } else if (nextToken == KARSILASTIRMA_DURUMU) {
        karsilastirma_durumu();
    } else {
        printf("Hata: <atama_durumu> veya <karsilastirma_durumu> bekleniyor.\n");
        exit(1);
    }
    printf("Cikis --> <durum_list>\n");
}

/* <atama_durumu> -> <program> */
void atama_durumu() {
    printf("Giris --> <atama_durumu>\n");
    program();
    printf("Cikis --> <atama_durumu>\n");
}

/* <karsilastirma_durumu> -> <start_op> <eger_ifadesi> */
void karsilastirma_durumu() {
    printf("Giris --> <karsilastirma_durumu>\n");
    start_op();
    eger_ifadesi();
    printf("Cikis --> <karsilastirma_durumu>\n");
}

/* Özel durumlar */

/* <eger_ifadesi> -> <eger> <var> <karsilastirma_op> <factor> <dogruluk_kontrol> <degilse> <dogruluk_kontrol> <finish_op> */
void eger_ifadesi() {
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
void eger() {
    printf("Giris --> <eger>\n");
    if (nextToken == EGER) {
        lex();
    } else {
        printf("Hata: <eger> bekleniyor.\n");
        exit(1);
    }
    printf("Cikis --> <eger>\n");
}

/* <karsilastirma_op> -> < | > | <= | >= | == | != */
void karsilastirma_op() {
    printf("Giris --> <karsilastirma_op>\n");
    if (nextToken == KARSILASTIRMA_OP) {
        lex();
    } else {
        printf("Hata: <karsilastirma_op> bekleniyor.\n");
        exit(1);
    }
    printf("Cikis --> <karsilastirma_op>\n");
}

/* <dogruluk_kontrol> -> <dogruluk> */
void dogruluk_kontrol() {
    printf("Giris --> <dogruluk_kontrol>\n");
    if  (nextToken == DOGRULUK_KONTROL) {
        lex();
    } else {
        printf("Hata: <dogruluk_kontrol> bekleniyor.\n");
        exit(1);
    }
    printf("Cikis --> <dogruluk_kontrol>\n");
}

/* <degilse> -> degilse */
void degilse() {
    printf("Giris --> <degilse>\n");
    if (nextToken == DEGILSE) {
        lex();
    } else {
        printf("Hata: <degilse> bekleniyor.\n");
        exit(1);
    }
    printf("Cikis --> <degilse>\n");
}

/* <var> -> <type> <id> */
void var() {
    printf("Giris --> <var>\n");
    type();
    id();
    printf("Cikis --> <var>\n");
}



