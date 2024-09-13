/*
 * hash_table.c
 *
 *  Created on: Aug 10, 2024
 *      Author: Nejc
 */

#include "hash_table.h"
#include "freertos_os2.h"

#define REHASH_THRESHOLD 0.75

static inline void HT_Resize(HT_HashTable_t *ht);
static void HT_ResizeAndRehash(HT_HashTable_t *ht);
static int HT_Hash(HT_HashTable_t *ht, int k);
// static int HT_Rehash(HT_HashTable_t *ht, int k, int i);

static inline void HT_Resize(HT_HashTable_t *ht) {

	ht->m = 2 * ht->m + 1;

}

static int HT_Hash(HT_HashTable_t *ht, int k) {

	return (k * ht->p) % ht->m;

}

/*
static int HT_Rehash(HT_HashTable_t *ht, int k, int i) {

	return (k + ht->c1 * i + ht->c2 * (int) pow(i, 2)) % ht->m;

}
*/

static void HT_ResizeAndRehash(HT_HashTable_t *ht) {

	HT_Element_t **oldTable = ht->table;
	HT_Element_t *oldPair, *toFree;
	int oldSize = ht->m;
	HT_Resize(ht);
	if (!(ht->table = (HT_Element_t **) pvPortMalloc(sizeof(HT_Element_t *) * ht->m))) {
		// raise error
	}
	ht->numOfElements = 0;
	for (int i = 0; i<ht->m; i++) {
		ht->table[i] = NULL;
	}
	for (int i = 0; i<oldSize; i++) {
		oldPair = oldTable[i];
		while (oldPair != NULL) {
			if (!oldPair->isDeleted) {
				HT_Insert(ht, oldPair->key, oldPair->value);
			}
			toFree = oldPair;
			oldPair = oldPair->nextElement;
			vPortFree(toFree);
		}
	}
	vPortFree(oldTable);

}

HT_HashTable_t *HT_Create(int p, int m) {

	HT_HashTable_t *ht;
	if (!(ht = (HT_HashTable_t *) pvPortMalloc(sizeof(HT_HashTable_t)))) {
		// raise error
	}
	if (!(ht->table = (HT_Element_t **) pvPortMalloc(sizeof(HT_Element_t *) * m))) {
		// raise error
	}
	// initialize to NULL
	for (int i = 0; i<m; i++) {
		ht->table[i] = NULL;
	}
	// ht->c1 = c1;
	// ht->c2 = c2;
	ht->m = m;
	ht->p = p;
	ht->numOfElements = 0;

	return ht;

}

void *HT_Search(HT_HashTable_t *ht, int key) {

	HT_Element_t *pair = ht->table[HT_Hash(ht, key)];
	while (pair != NULL) {
		if (pair->key == key && !pair->isDeleted) {
			return pair->value;
		}
		pair = pair->nextElement;
	}

	return NULL;

}

void HT_Insert(HT_HashTable_t *ht, int key, void *value) {

	if ((double) ht->numOfElements / ht->m >= REHASH_THRESHOLD) {
		HT_ResizeAndRehash(ht);
	}
	HT_Element_t *toInsert, *current;
	int index = HT_Hash(ht, key);
	current = ht->table[index];
	// check for duplicate keys. If one is found,
	// check if it might be deleted. If yes, then insert at that
	// location, otherwise reject insertion
	while (current != NULL) {
		if (current->key == key) {
			if (current->isDeleted) {
				current->value = value;
				current->isDeleted = 0;
			}
			return;
		}
		current = current->nextElement;
	}
	if (!(toInsert = (HT_Element_t *) pvPortMalloc(sizeof(HT_Element_t)))) {
		// raise error
	}
	toInsert->key = key;
	toInsert->value = value;
	toInsert->isDeleted = 0;
	toInsert->nextElement = ht->table[index];
	ht->table[index] = toInsert;
	ht->numOfElements++;

}

void HT_Delete(HT_HashTable_t *ht, int key) {

	HT_Element_t *toDelete = ht->table[HT_Hash(ht, key)];
	while (toDelete != NULL) {
		if (toDelete->key == key) {
			if (toDelete->isDeleted) {
				return;
			} else {
				toDelete->isDeleted = 1;
			}
		}
		toDelete = toDelete->nextElement;
	}

}

void HT_Print(HT_HashTable_t *ht) {

	HT_Element_t *toPrint;
	for (int i = 0; i<ht->m; i++) {
		printf("%d -> ", i);
		toPrint = ht->table[i];
		while (toPrint != NULL) {
			printf("(%d, %p)[%d] ", toPrint->key, toPrint->value, toPrint->isDeleted);
			toPrint = toPrint->nextElement;
		}
		printf("\n");
	}

}

void HT_Clear(HT_HashTable_t *ht) {

	HT_Element_t *toClear, *toClearCon;
	for (int i = 0; i<ht->m; i++) {
		toClear = ht->table[i];
		while (toClear != NULL) {
			toClearCon = toClear;
			toClear = toClear->nextElement;
			vPortFree(toClearCon);
		}
	}
	vPortFree(ht->table);
	vPortFree(ht);

}
