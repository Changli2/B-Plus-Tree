#include "def.h"

extern int check_word2(char *word);
extern int strtolow(char *s);
extern PAGENO treesearch_page(PAGENO PageNo, char *key);
extern struct PageHdr *FetchPage(PAGENO Page);
extern int FreePage(struct PageHdr *PagePtr);
extern void sort_string_array(char **arr, size_t n);

void printHelper(int index, char *result[]) {
    printf("found %d successors:\n", index);
    for (int i = 0; i < index; i++) {
        printf("%s\n", result[i]);
    }
}

int get_successors(char *key, int k, char *result[]) {
	int CompareKeys(char *Key, char *Word);
    PAGENO FindNumPagesInTree(void);
 
	if (check_word2(key) == FALSE) {
        return 0;
    }

    // maybe unreachable but in case of unit test
    if (k <= 0) {
        printf("k should be positive not %d\n", k);
        return 0;
    }

    /* turn to lower case, for uniformity */
    strtolow(key);

    // get the leaf page# the key should reside (even key not exist)
    PAGENO page = treesearch_page(ROOT, key);

    int index = 0;
    // fetch the page, the first elem may be with in this or
    // the first one of next page
    if ((page < 1) || (page > FindNumPagesInTree())) {
        printHelper(index, result);
        return 0;
    }
    struct PageHdr *PagePtr = FetchPage(page);
    struct KeyRecord *KeyListTraverser = NULL;
    if (PagePtr != NULL) {
        KeyListTraverser = PagePtr->KeyListPtr;
    }
    char *Word;

    // find the right position
    while (KeyListTraverser != NULL) {
    	 Word = KeyListTraverser->StoredKey;
    	(*(Word + KeyListTraverser->KeyLen)) = '\0';
    	int resultValue = CompareKeys(key, Word);
    	if (resultValue == 1) {
    		break;
    	} else {
    		KeyListTraverser = KeyListTraverser->Next;
    	}
    }

    // in case the beginning key is the first key of the
    // next leaf page
    if (PagePtr != NULL && PagePtr->KeyListPtr != NULL && 
            KeyListTraverser == NULL) {
        PAGENO nextPage = PagePtr->PgNumOfNxtLfPg;
        if ((nextPage < 1) || (nextPage > FindNumPagesInTree())) {
            printHelper(index, result);
            return 0;
        }
        PagePtr = FetchPage(nextPage);//!!!
        if (PagePtr != NULL) {
            KeyListTraverser = PagePtr->KeyListPtr;
        }
    }


    // read strings into array
    while (index < k && KeyListTraverser != NULL) {
        char *elem = (char*)malloc((strlen(KeyListTraverser->StoredKey) + 1) * sizeof(char));
        strcpy(elem, KeyListTraverser->StoredKey);
    	result[index] = elem;
    	index++;
    	KeyListTraverser = KeyListTraverser->Next;

    	if (KeyListTraverser == NULL) {
    		page = PagePtr->PgNumOfNxtLfPg;
    		FreePage(PagePtr);

    		if (index < k) {
                if ((page < 1) || (page > FindNumPagesInTree())) {
                    printHelper(index, result);
                    return 0;
                }
    			PagePtr = FetchPage(page);
    			if (PagePtr != NULL) {
    				KeyListTraverser = PagePtr->KeyListPtr;
    			}
    		}
    	}
    }

    if (KeyListTraverser != NULL) {
        FreePage(PagePtr);
    }
    
    // print out the result
    printHelper(index, result);


    return 0;
}


