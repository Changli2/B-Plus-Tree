#include <stdio.h>
#include <string.h>
#include "def.h"

extern int check_word2(char *word);
extern int strtolow(char *s);
extern struct PageHdr *FetchPage(PAGENO Page);
extern int FreePage(struct PageHdr *PagePtr);

extern int CompareKeys(char *Key, char *Word);
extern PAGENO FindNumPagesInTree(void);
extern void sort_string_array(char **arr, size_t n);

void printHelperPredecessors(int index, char *result[]) {
    printf("found %d predecessors:\n", index);
    for (int i = 0; i < index; i++) {
        printf("%s\n", result[i]);
    }
}

/*
	recursive call to go right then go down
	until the k elems or as most as possible filled
*/

void find(char *result[], int k, char *key, int *index, int numOfKeyLeft, 
		struct KeyRecord *KeyListTraverser, struct PageHdr *PagePtr) {
	//numOfKeyLeft--;
	if (*index == k) {
		// if there are enough elems in the result array
		return;
	}

	if (IsNonLeaf(PagePtr) && numOfKeyLeft < 0) {
		return;
	} 

	if (IsLeaf(PagePtr) && numOfKeyLeft == 0) {
		return;
	}


	if (numOfKeyLeft > 0) {
		char *Word;
		Word = KeyListTraverser->StoredKey;
    	(*(Word + KeyListTraverser->KeyLen)) = '\0';
		int compKeyResult = CompareKeys(key, Word);

		if (compKeyResult == 2) {
			// Only when the key is greater than the current key we go right
			find(result, k, key, index, numOfKeyLeft - 1, 
					KeyListTraverser->Next, PagePtr); 
			if (*index == k) {
				return;
			}
			// if (numOfKeyLeft == 0 && IsNonLeaf(PagePtr)) {
			// 	struct PageHdr *PagePtr2 = FetchPage(PagePtr->PtrToFinalRtgPg);
			// 	find(result, k, key, index, PagePtr2->NumKeys, 
			// 		PagePtr2->KeyListTraverser, PagePtr2);
			// 	// FreePage(PagePtr2);//!!!!!!
			// } else {
			// 	find(result, k, key, index, numOfKeyLeft - 1, 
			// 		KeyListTraverser->Next, PagePtr); 
			// }
		}
	}
	
	

	if (IsNonLeaf(PagePtr)) {
		// If this is nonleaf node, search down
		PAGENO child;
		if (numOfKeyLeft == 0) {
			child = PagePtr->PtrToFinalRtgPg;
		} else {
			child = KeyListTraverser->PgNum;
		}
		struct PageHdr *PagePtrDown = FetchPage(child);
		find(result, k, key, index, PagePtrDown->NumKeys, 
			PagePtrDown->KeyListPtr, PagePtrDown); 
		// FreePage(PagePtr2);//!!!!!!
		if (*index == k) {
			return;
		}

	} else {
		// if leaf node, fill the result array
		char *Word;
		Word = KeyListTraverser->StoredKey;
    	(*(Word + KeyListTraverser->KeyLen)) = '\0';
		int compKeyResult = CompareKeys(key, Word);
		if (compKeyResult == 2) {
			assert(*index < k);
			result[*index] = Word;
			*index = *index + 1;
		}
		
	}

	return;
}
int get_predecessors(char *key, int k, char *result[]) {
	
	
    //key = key;
    //k = k;
    //result = result;
    //printf("IMPLEMENT  ME\n");
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

    int arrIndex = 0;
    int *index = &arrIndex;
	//int *count = &arrIndex;
	struct PageHdr *PagePtr = FetchPage(ROOT);
	
	if (PagePtr != NULL) {
		struct KeyRecord *KeyListTraverser = PagePtr->KeyListPtr;
		find(result, k, key, index, PagePtr->NumKeys, KeyListTraverser, PagePtr);
	}
    
    //FreePage(PagePtr)
    sort_string_array(result, *index);
	printHelperPredecessors(*index, result);
    return 0;
}
