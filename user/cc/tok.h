#ifndef _CC_TOK_H_
#define _CC_TOK_H_

#include "user/user.h"
#include "user/cc/util.h"

enum _TOK {
    TOK_EOF = 0,
    TOK_ID = 256,
    TOK_INT,
    TOK_FLOAT,
    TOK_STR,

    TOK_EQUAL,          // ==
    TOK_NEQUAL,         // !=
    TOK_LEQUAL,         // <=
    TOK_GEQUAL,         // >=
    TOK_LSHIFT,         // <<
    TOK_LSHIFTEQUAL,    // <<=
    TOK_RSHIFT,         // >>
    TOK_RSHIFTEQUAL,    // >>=
    TOK_PLUSEQUAL,      // +=
    TOK_MINUSEQUAL,     // -=
    TOK_TIMESEQUAL,     // *=
    TOK_DIVIDEEQUAL,    // /=
    TOK_MODEQUAL,       // %=
    TOK_PLUSPLUS,       // ++
    TOK_MINUSMINUS,     // --
    TOK_ANDEQUAL,       // &=
    TOK_ANDAND,         // &&
    TOK_OREQUAL,        // |=
    TOK_OROR,           // ||
    TOK_XOREQUAL,       // ^=
    TOK_RSINGLEARROW,   // ->

#define TOK_R_LIST_START TOK_R_AUTO
    TOK_R_AUTO,         // auto
    TOK_R_BREAK,        // break
    TOK_R_CASE,         // case
    TOK_R_CHAR,         // char
    TOK_R_CONST,        // const
    TOK_R_CONTINUE,     // continue
    TOK_R_DEFAULT,      // default
    TOK_R_DO,           // do
    TOK_R_DOUBLE,       // double
    TOK_R_ELSE,         // else
    TOK_R_ENUM,         // enum
    TOK_R_EXTERN,       // extern
    TOK_R_FLOAT,        // float
    TOK_R_FOR,          // for
    TOK_R_GOTO,         // goto
    TOK_R_IF,           // if
    TOK_R_INLINE,       // inline
    TOK_R_INT,          // int
    TOK_R_LONG,         // long
    TOK_R_REGISTER,     // register
    TOK_R_RETURN,       // return
    TOK_R_SHORT,        // short
    TOK_R_STATIC,       // static
    TOK_R_STRUCT,       // struct
    TOK_R_SWITCH,       // switch
    TOK_R_TYPEDEF,      // typedef
    TOK_R_UNION,        // union
    TOK_R_UNSIGNED,     // unsigned
    TOK_R_VOID,         // void
    TOK_R_VOLATILE,     // volatile
    TOK_R_WHILE,        // while

    TOK_R_LIST_END /* always the last entry */
};
typedef int toktype_t;

struct pos {
    int line;
    int col;
};
typedef struct pos pos_t;

struct tok {
    pos_t       pos;
    toktype_t   type;
    char*       data;
    size_t      len;
};
typedef struct tok tok_t;

toktype_t lookup_kwd(const char *s, size_t len)
{
    switch (len)
    {
        case 2:
            if (memcmp(s, "do", 2) == 0) return TOK_R_DO;
            if (memcmp(s, "if", 2) == 0) return TOK_R_IF;
            break;
        case 3:
            if (memcmp(s, "for", 3) == 0) return TOK_R_FOR;
            if (memcmp(s, "int", 3) == 0) return TOK_R_INT;
            break;
        case 4:
            if (memcmp(s, "auto", 4) == 0) return TOK_R_AUTO;
            if (memcmp(s, "char", 4) == 0) return TOK_R_CHAR;
            if (memcmp(s, "enum", 4) == 0) return TOK_R_ENUM;
            if (memcmp(s, "goto", 4) == 0) return TOK_R_GOTO;
            if (memcmp(s, "long", 4) == 0) return TOK_R_LONG;
            if (memcmp(s, "void", 4) == 0) return TOK_R_VOID;
            break;
        case 5:
            if (memcmp(s, "break", 5) == 0) return TOK_R_BREAK;
            if (memcmp(s, "const", 5) == 0) return TOK_R_CONST;
            if (memcmp(s, "float", 5) == 0) return TOK_R_FLOAT;
            if (memcmp(s, "short", 5) == 0) return TOK_R_SHORT;
            if (memcmp(s, "union", 5) == 0) return TOK_R_UNION;
            // while (0);
            break;
        case 6:
            if (memcmp(s, "double", 6) == 0) return TOK_R_DOUBLE;
            if (memcmp(s, "extern", 6) == 0) return TOK_R_EXTERN;
            if (memcmp(s, "inline", 6) == 0) return TOK_R_INLINE;
            if (memcmp(s, "return", 6) == 0) return TOK_R_RETURN;
            if (memcmp(s, "static", 6) == 0) return TOK_R_STATIC;
            if (memcmp(s, "struct", 6) == 0) return TOK_R_STRUCT;
            if (memcmp(s, "switch", 6) == 0) return TOK_R_SWITCH;
            if (memcmp(s, "while", 6) == 0) return TOK_R_WHILE;
            break;
        case 8:
            if (memcmp(s, "continue", 8) == 0) return TOK_R_CONTINUE;
            if (memcmp(s, "unsigned", 8) == 0) return TOK_R_UNSIGNED;
            if (memcmp(s, "volatile", 8) == 0) return TOK_R_VOLATILE;
            break;
    }

    return TOK_ID;
}

inline int lookup_escape_code(char c)
{
    switch (c)
    {
        case 'a':   return '\a';
        case 'b':   return '\b';
        case 'f':   return '\f';
        case 'n':   return '\n';
        case 'r':   return '\r';
        case 't':   return '\t';
        case 'v':   return '\v';
        case '\\':  return '\\';
        case '"':   return '"';
        case '\'':  return '\'';
        default:
            return -1; 
    }
}


toktype_t lookup_op3(char a, char b, char c)
{
    if (a == '<' && b == '<' && c == '=')
        return TOK_LSHIFTEQUAL; 
    if (a == '>' && b == '>' && c == '=')
        return TOK_RSHIFTEQUAL;
    return -1;
}


toktype_t lookup_op2(char a, char b)
{
    switch (a)
    {
        case '=':
            if (b == '=') return TOK_EQUAL;
            break;
        case '!':
            if (b == '=') return TOK_NEQUAL;
            break;
        case '<':
            if (b == '=') return TOK_LEQUAL;
            if (b == '<') return TOK_LSHIFT;
            break;
        case '>':
            if (b == '=') return TOK_GEQUAL;
            if (b == '>') return TOK_RSHIFT;
            break;
        case '+':
            if (b == '=') return TOK_PLUSEQUAL;
            if (b == '+') return TOK_PLUSPLUS;
            break;
        case '-':
            if (b == '=') return TOK_MINUSEQUAL;
            if (b == '-') return TOK_MINUSMINUS;
            if (b == '>') return TOK_RSINGLEARROW;
            break;
        case '*':
            if (b == '=') return TOK_TIMESEQUAL;
            break;
        case '/':
            if (b == '=') return TOK_DIVIDEEQUAL;
            break;
        case '%':
            if (b == '=') return TOK_MODEQUAL;
            break;
        case '&':
            if (b == '=') return TOK_ANDEQUAL;
            if (b == '&') return TOK_ANDAND;
            break;
        case '|':
            if (b == '=') return TOK_OREQUAL;
            if (b == '|') return TOK_OROR;
            break;
        case '^':
            if (b == '=') return TOK_XOREQUAL;
            break;
    }
    return -1;
}

void tok_print(tok_t* tok)
{
    printf("(%d, %d)\t%d\t", tok->pos.line, tok->pos.col, tok->type);
    if (tok->type != 0) printn(tok->data, tok->len); 
    putchar('\n');
}

#endif