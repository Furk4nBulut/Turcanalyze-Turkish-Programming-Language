/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

/**
 *
 * @author binnursoztutar
 */
public class LexicalAnalyzer {
    private int charClass;
    private char[] lexeme = new char[100];
    private char nextChar;
    private int lexLen;
    private int nextToken;
    private BufferedReader in_fp;

    public LexicalAnalyzer(String filename) throws IOException {
        in_fp = new BufferedReader(new FileReader(filename));
        getChar();
    }

    private void addChar() {
        if (lexLen <= 98) {
            lexeme[lexLen++] = nextChar;
            lexeme[lexLen] = 0;
        } else {
            System.out.println("Error: lexeme is too long.");
        }
    }

    private void getChar() throws IOException {
        int nextInt = in_fp.read();
        if (nextInt != -1) {
            nextChar = (char) nextInt;
            if (Character.isLetter(nextChar))
                charClass = Token.LETTER;
            else if (Character.isDigit(nextChar))
                charClass = Token.DIGIT;
            else charClass = Token.UNKNOWN;
        } else {
            charClass = -1;
        }
    }

    private void getNonBlank() throws IOException {
        while (Character.isWhitespace(nextChar))
            getChar();
    }

    public int lex() throws IOException {
        lexLen = 0;
        getNonBlank();
        switch (charClass) {
            case Token.LETTER:
                addChar();
                getChar();
                while (charClass == Token.LETTER || charClass == Token.DIGIT) {
                    addChar();
                    getChar();
                }
                if (isCorrectDataType(String.valueOf(lexeme).trim())) {
                    nextToken = Token.VAR;
                } else {
                    nextToken = Token.IDENT;
                }
                String lexStr = String.valueOf(lexeme).trim();
                if (lexStr.equals("eger"))
                    nextToken = Token.EGER;
                else if (lexStr.equals("ise"))
                    nextToken = Token.ISE;
                else if (lexStr.equals("degilse"))
                    nextToken = Token.DEGILSE;
                else if (lexStr.equals("eşittir") || lexStr.equals("büyüktür") || lexStr.equals("küçüktür"))
                    nextToken = Token.KARSILASTIRMA_DURUMU;
                break;

            case Token.DIGIT:
                addChar();
                getChar();
                while (charClass == Token.DIGIT) {
                    addChar();
                    getChar();
                }
                nextToken = Token.INT_LIT;
                break;

            case Token.UNKNOWN:
                lookup(nextChar);
                getChar();
                break;

            case -1:
                nextToken = -1;
                lexeme[0] = 'E';
                lexeme[1] = 'O';
                lexeme[2] = 'F';
                lexeme[3] = 0;
                break;
        }
        System.out.println("Next token is: " + nextToken + ", Next lexeme is " + String.valueOf(lexeme).trim());
        return nextToken;
    }

    private int lookup(char ch) {
        switch (ch) {
            case '(':
                addChar();
                nextToken = Token.LEFT_PAREN;
                break;
            case ')':
                addChar();
                nextToken = Token.RIGHT_PAREN;
                break;
            case '+':
                addChar();
                nextToken = Token.ADD_OP;
                break;
            case '-':
                addChar();
                nextToken = Token.SUB_OP;
                break;
            case '*':
                addChar();
                nextToken = Token.MULT_OP;
                break;
            case '/':
                addChar();
                nextToken = Token.DIV_OP;
                break;
            case '=':
                addChar();
                nextToken = Token.ASSIGN_OP;
                break;
            case '{':
                addChar();
                nextToken = Token.START_OP;
                break;
            case '}':
                addChar();
                nextToken = Token.FINISH_OP;
                break;
            default:
                addChar();
                nextToken = -1;
                break;
        }
        return nextToken;
    }

    private boolean isCorrectDataType(String lexeme) {
        return lexeme.equals("int");
    }

    public int getNextToken() {
        return nextToken;
    }
}