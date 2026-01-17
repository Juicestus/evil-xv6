/**
 * cc.c - simple C compiler for xv6
 * 
 * 
 */

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "user/user.h"

#include "user/cc/tok.h"
#include "user/cc/lex.h"

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        fprintf(2, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    // char* src = "int main()\n{\n\tint x = 5;\n}\n";
    // char* src = "int main()\n{\n\tint x = \"hello world\";\n}\n";
    char* src = "int main()\n"
                "{\n"
                "   int x = 5;\n"
                "   x += 1;\n"
                "   x += \"Hello, world!\";\n"
                "}\n";
    lex_t lex;
    lex_init(&lex, src);
    tok_t tk;
    do {
        tk = lex_nexttok(&lex);
        tok_print(&tk);
    } while (tk.type != TOK_EOF);
    (void)tk;

    return 0;
}
