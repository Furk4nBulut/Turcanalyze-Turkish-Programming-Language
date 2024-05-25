
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;


/* Token codes */
public class Token {
    /* Character classes */
    public static final int LETTER = 0;
    public static final int DIGIT = 1;
    public static final int UNKNOWN = 99;

    /* Token codes */
    public static final int INT_LIT = 10;
    public static final int IDENT = 11;
    public static final int ASSIGN_OP = 20;
    public static final int ADD_OP = 21;
    public static final int SUB_OP = 22;
    public static final int MULT_OP = 23;
    public static final int DIV_OP = 24;
    public static final int LEFT_PAREN = 25;
    public static final int RIGHT_PAREN = 26;
    public static final int VAR = 12;
    public static final int START_OP = 30;
    public static final int FINISH_OP = 31;

    /* Custom token codes for If Else */
    public static final int EGER = 40;
    public static final int KARSILASTIRMA_OP = 41;
    public static final int DEGILSE = 45;
    public static final int ATAMA_DURUMU = 46;
    public static final int KARSILASTIRMA_DURUMU = 48;
    public static final int EGER_IFADESI = 50;
    public static final int ISE = 51;

    public static String getTokenName(int token) {
        switch (token) {
            case INT_LIT: return "INT_LIT";
            case IDENT: return "IDENT";
            case ASSIGN_OP: return "ASSIGN_OP";
            case ADD_OP: return "ADD_OP";
            case SUB_OP: return "SUB_OP";
            case MULT_OP: return "MULT_OP";
            case DIV_OP: return "DIV_OP";
            case LEFT_PAREN: return "LEFT_PAREN";
            case RIGHT_PAREN: return "RIGHT_PAREN";
            case VAR: return "VAR";
            case START_OP: return "START_OP";
            case FINISH_OP: return "FINISH_OP";
            case EGER: return "EGER";
            case KARSILASTIRMA_OP: return "KARSILASTIRMA_OP";
            case DEGILSE: return "DEGILSE";
            case ATAMA_DURUMU: return "ATAMA_DURUMU";
            case KARSILASTIRMA_DURUMU: return "KARSILASTIRMA_DURUMU";
            case EGER_IFADESI: return "EGER_IFADESI";
            case ISE: return "ISE";
            default: return "UNKNOWN";
        }
    }
}