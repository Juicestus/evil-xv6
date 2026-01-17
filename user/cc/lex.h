#ifndef _CC_LEX_H_
#define _CC_LEX_H_

#include "user/cc/util.h"
#include "user/cc/tok.h"
#include "user/user.h"

// TODO: 
//  lex char literal
//  lex hex int literal 
//  lex octal int literal
//  lex scientific notation float literal
//  lex string escapes

#define MAX_IDENT_LEN   256
#define MAX_NUM_LEN     256
#define STR_DELIM       '"'


struct lex {
    char*   src;
    char    curchr;
    char    nextchr;
    size_t  index;      // size_t?
    size_t  end;
    // tok_t*  token;
};
typedef struct lex lex_t;

// #define LEX_TOKEN(L)    (L->token)

int lex_init(lex_t* l, const char* src);
int lex_free(lex_t* l);
char lex_nextchr(lex_t* l);
pos_t lex_location(lex_t* l) ;
bool lex_tok_oneline_comment(lex_t* l);
bool lex_tok_multiline_comment(lex_t* l);
tok_t lex_nexttok(lex_t* l);
bool lex_tok_kwd(lex_t* l, tok_t* tok);
bool lex_tok_numeric(lex_t* l, tok_t* tok);
bool lex_tok_str(lex_t* l, tok_t* tok);
bool lex_tok_elem(lex_t* l, tok_t* tok);

// impl:

int lex_init(lex_t* l, const char* src)
{
    l->end = strlen(src);
#ifdef LEX_USE_RAW_SRC
#else
    l->src = (char*)malloc(l->end+1);  // todo: catch malloc fail
    l->src[l->end] = 0;
    strcpy(l->src, src);
#endif

    l->index = 0;
    lex_nextchr(l);
    lex_nextchr(l);

    // nextchr
    return 0;
}

int lex_free(lex_t* l)
{
#ifdef LEX_USE_RAW_SRC
#else
    free((void*)l->src);
#endif
    return 0;
}

char lex_nextchr(lex_t* l)
{
    char last = l->curchr;
    l->curchr = l->nextchr;
    l->nextchr = (l->index < l->end) ? l->src[l->index++] : 0;
    return last;
}

pos_t lex_location(lex_t* l) 
{
    pos_t pos = {0};
    for (int i = 0; i < l->index - 1; i++) 
    {
        pos.col++;
        if (i < l->end && l->src[i] == '\n') 
        {
            pos.line++;
            pos.col = 0;
        }
    }
    return pos;
}

// auto shutdown
// test everything
// plex data
// performance 

bool lex_tok_oneline_comment(lex_t* l) 
{
    if (l->curchr != '/' || l->nextchr != '/') 
        return false;
    while (l->curchr && l->curchr != '\n')
        lex_nextchr(l);
    lex_nextchr(l);    
    return true;
}

bool lex_tok_multiline_comment(lex_t* l)
{
    if (l->curchr != '/' || l->nextchr != '*') 
        return false;
    while (l->curchr && (l->curchr != '*' || l->nextchr != '/'))
        lex_nextchr(l);    
    lex_nextchr(l);    
    lex_nextchr(l);    
    return true;
}

tok_t lex_nexttok(lex_t* l)
{
    tok_t tok = {0};
    tok.type = TOK_EOF;

    while (l->curchr && iswhitespace(l->curchr))
    {
        lex_nextchr(l);
    }
    if (lex_tok_oneline_comment(l) || lex_tok_multiline_comment(l))
    {
        return lex_nexttok(l);
    }

    tok.pos = lex_location(l);
    tok.data = l->src + l->index - 2;

    int _ = lex_tok_kwd(l, &tok)
    || lex_tok_numeric(l, &tok)
    || lex_tok_str(l, &tok)
    || lex_tok_elem(l, &tok);
    (void)_;

    return tok;
}

bool lex_tok_kwd(lex_t* l, tok_t* tok)
{
    if (!isalpha(l->curchr))    
        return false;
    // tok->data = l->src + l->index - 2;  // could make it a data index? or reference as position?
    while (isalpha(l->curchr) || isnumeric(l->curchr))
    {
        lex_nextchr(l);
        tok->len++;
    } 
    // printn(tok->data, tok->len);    putchar('\n');
    if ((tok->type = lookup_kwd(tok->data, tok->len)) != TOK_ID)
    {
        // free(tok->data);      // optional
        // tok->data = "";       // optional
        // OK!
    }

    return true;
}

bool lex_tok_numeric(lex_t* l, tok_t* tok)
{
    if (!isnumeric(l->curchr)) return false;

    tok->type = TOK_INT;
    while (isnumeric(l->curchr))
    {
        tok->len++;
        lex_nextchr(l);
    }
    if (l->curchr == '.')
    {
        tok->type = TOK_FLOAT;
        tok->len++;
        lex_nextchr(l);
    }
    return true;
}

bool lex_tok_str(lex_t* l, tok_t* tok)
{
    if (l->curchr != STR_DELIM) return false;
    tok->data++;
    lex_nextchr(l);
    // tok->data = l->src + l->index - 2;
    while (l->curchr && l->curchr != STR_DELIM)
    {
        tok->len++;
        lex_nextchr(l);
    }
    lex_nextchr(l);
    tok->type = TOK_STR;
    return true;
}

bool lex_tok_elem(lex_t* l, tok_t* tok)
{
    char lastchr = l->curchr;
    if (l->curchr) lex_nextchr(l);
    // printf("checking for elem %c%c%c\n", lastchr, l->curchr, l->nextchr);
    // tok->data = l->src + l->index - 3;
    int op; // replace w/ tok->type
    if ((op = lookup_op3(lastchr, l->curchr, l->nextchr)) != -1)
    {
        tok->type = op;
        tok->len = 3;
        lex_nextchr(l);
        lex_nextchr(l);
        return true;
    }
    if ((op = lookup_op2(lastchr, l->curchr)) != -1)
    {
        tok->type = op;
        tok->len = 2;
        lex_nextchr(l);
        return true;
    }
    tok->len = 1;
    tok->type = lastchr;
    return false;
}

#endif