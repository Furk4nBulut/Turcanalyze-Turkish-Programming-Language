
import java.io.IOException;

public class Main {
    public static void main(String[] args) {
        try {
            LexicalAnalyzer lexicalAnalyzer = new LexicalAnalyzer("ex.txt");
            int token;
            do {
                token = lexicalAnalyzer.lex();
                program(lexicalAnalyzer);
            } while (token != -1);
        } catch (IOException e) {
            System.out.println("File Reading Error.");
        }
    }

    public static void program(LexicalAnalyzer lexer) throws IOException {
        System.out.println("Starting program...");
        start_op(lexer);
        ifelsekontrol(lexer);
        var(lexer);
        assign_op(lexer);
        expr(lexer);
        finish_op(lexer);
    }

    public static void start_op(LexicalAnalyzer lexer) throws IOException {
        System.out.println("Enter <start_op>");
        if (lexer.getNextToken() == Token.START_OP) {
            lexer.lex();
        } else {
            System.out.println("Error: <start_op> expected.");
            err();
        }
        System.out.println("Exit <start_op>");
    }

    public static void finish_op(LexicalAnalyzer lexer) throws IOException {
        System.out.println("Enter <finish_op>");
        if (lexer.getNextToken() == Token.FINISH_OP) {
            lexer.lex();
            System.out.println("Ending program...");
            System.exit(0);
        } else {
            System.out.println("Error: <finish_op> expected.");
            err();
        }
        System.out.println("Exit <finish_op>");
    }

    public static void var(LexicalAnalyzer lexer) throws IOException {
        System.out.println("Enter <var>");
        type(lexer);
        id(lexer);
        System.out.println("Exit <var>");
    }

    public static void type(LexicalAnalyzer lexer) throws IOException {
        System.out.println("Enter <type>");
        if (lexer.getNextToken() == Token.VAR) {
            lexer.lex();
        } else {
            System.out.println("Error: <type> expected.");
            err();
        }
        System.out.println("Exit <type>");
    }

    public static void id(LexicalAnalyzer lexer) throws IOException {
        System.out.println("Enter <id>");
        letter(lexer);
        id2(lexer);
        System.out.println("Exit <id>");
    }

    public static void id2(LexicalAnalyzer lexer) throws IOException {
        System.out.println("Enter <id2>");
        if (lexer.getNextToken() == Token.IDENT) {
            lexer.lex();
        } else if (lexer.getNextToken() == Token.INT_LIT) {
            lexer.lex();
        } else {
            return;
        }
        System.out.println("Exit <id2>");
    }

    public static void expr(LexicalAnalyzer lexer) throws IOException {
        System.out.println("Enter <expr>");
        term(lexer);
        if (lexer.getNextToken() == Token.SUB_OP) {
            lexer.lex();
            expr(lexer);
        } else if (lexer.getNextToken() == Token.ADD_OP) {
            lexer.lex();
            expr(lexer);
        }
        System.out.println("Exit <expr>");
    }

    public static void term(LexicalAnalyzer lexer) throws IOException {
        System.out.println("Enter <term>");
        factor(lexer);
        if (lexer.getNextToken() == Token.DIV_OP) {
            lexer.lex();
            term(lexer);
        } else if (lexer.getNextToken() == Token.MULT_OP) {
            lexer.lex();
            term(lexer);
        }
        System.out.println("Exit <term>");
    }

    public static void factor(LexicalAnalyzer lexer) throws IOException {
        System.out.println("Enter <factor>");
        digit(lexer);
        if (lexer.getNextToken() == Token.INT_LIT) {
            lexer.lex();
        }
        System.out.println("Exit <factor>");
    }

    public static void digit(LexicalAnalyzer lexer) throws IOException {
        System.out.println("Enter <digit>");
        if (lexer.getNextToken() == Token.INT_LIT) {
            lexer.lex();
        }
        System.out.println("Exit <digit>");
    }

    public static void letter(LexicalAnalyzer lexer) throws IOException {
        System.out.println("Enter <letter>");
        if (lexer.getNextToken() == Token.IDENT) {
            lexer.lex();
        }
        System.out.println("Exit <letter>");
    }

    public static void assign_op(LexicalAnalyzer lexer) throws IOException {
        System.out.println("Enter <assign_op>");
        if (lexer.getNextToken() == Token.ASSIGN_OP) {
            lexer.lex();
        } else {
            System.out.println("Error: <assign_op> expected.");
            err();
        }
        System.out.println("Exit <assign_op>");
    }

    public static void err() {
        System.out.println("Error function called.");
        System.exit(0);
    }

    /* Custom methods for if-else control */
    public static void eger_ifadesi(LexicalAnalyzer lexer) throws IOException {
        System.out.println("Enter <eger_ifadesi>");
        lexer.lex();
        var(lexer);
        karsilastirma_op(lexer);
        factor(lexer);
        ise(lexer);
        var(lexer);
        assign_op(lexer);
        factor(lexer);
        degilse(lexer);
        var(lexer);
        assign_op(lexer);
        factor(lexer);
        finish_op(lexer);
        System.out.println("Exit <eger_ifadesi>");
    }

    public static void karsilastirma_op(LexicalAnalyzer lexer) throws IOException {
        System.out.println("Enter <karsilastirma_op>");
        if (lexer.getNextToken() == Token.ASSIGN_OP) {
            lexer.lex();
        } else {
            System.out.println("Error: <karsilastirma_op> expected.");
            err();
        }
        System.out.println("Exit <karsilastirma_op>");
    }

    public static void degilse(LexicalAnalyzer lexer) throws IOException {
        System.out.println("Enter <degilse>");
        if (lexer.getNextToken() == Token.DEGILSE) {
            lexer.lex();
        } else {
            System.out.println("Error: <degilse> expected.");
            err();
        }
        System.out.println("Exit <degilse>");
    }

    public static void ise(LexicalAnalyzer lexer) throws IOException {
        System.out.println("Enter <ise>");
        if (lexer.getNextToken() == Token.ISE) {
            lexer.lex();
        } else {
            System.out.println("Error: <ise> expected.");
            err();
        }
        System.out.println("Exit <ise>");
    }

    public static void ifelsekontrol(LexicalAnalyzer lexer) throws IOException {
        if (lexer.getNextToken() == Token.EGER) {
            eger_ifadesi(lexer);
        }
    }
}