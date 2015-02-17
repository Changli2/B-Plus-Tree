/* builds an index on the given file "fptext"
   which should consist of documents, each starting
   with  TAG (=ctrl-V)
 */

#include <stdio.h>
#include "def.h"
#include "defn.g"

extern FILE *fpbtree, *fppost, *fptext;

extern int inithash(void);
extern int iscommon(char *word);
extern int strtolow(char *s);
extern int inhash(char *word);
extern int InsertTree(char *Key, TEXTPTR TextOffset);
extern int hfree(void);

void buildIndex(void) {
    char word[MAXWORDSIZE];
    TEXTPTR docStart; /* marks the beginning of a new document */
    int getword(FILE * fp, char *word), hgetnext(char *word);
    int c, i;

    docStart = 0;
    i = TRUE;
    inithash(); /* initialize the hash table */
    while (i != EOF) {
        fseek(fptext, (long) docStart, 0);
        /* read first character and check for TAG */
        if ((c = getc(fptext)) != TAG) {
            printf("error in buildIndex.c - %ld is not a TAG\n",
                   ftell(fptext) - 1);
            exit(-1);
        }
        /* eliminate dupl. words by hashing into a h.table */
        /* also, randomizes them somehow */
        while (((i = getword(fptext, word)) != EOF) && (i != DOCSTART)) {
            if (!iscommon(word)) {
                strtolow(word); /* turn to lower case only */
                inhash(word);
            }
        }
#ifdef DEBUG
        hprint(); /* just for testing */
#endif

        while (hgetnext(word) != EOH) {
            InsertTree(word, docStart);
        }

#ifdef DEBUG
        system("date; vmstat; echo freeing htab");
#endif

        hfree(); /* empties the hash table */

#ifdef DEBUG

        system("date; vmstat");
#endif

        if (i == DOCSTART) {
            docStart = ftell(fptext) - 1;
        }
    }
}
