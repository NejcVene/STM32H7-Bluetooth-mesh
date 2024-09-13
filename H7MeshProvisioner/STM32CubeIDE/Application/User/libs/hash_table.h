/*
 * hash_table.h
 *
 *  Created on: Aug 9, 2024
 *      Author: Nejc
 */

#ifndef INC_HASH_TABLE_H_
#define INC_HASH_TABLE_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct HT_Element_t {
	int key, isDeleted;
	void *value;
	struct HT_Element_t *nextElement;
} HT_Element_t;

// m is size
// p, c1, c2 are constants
typedef struct {
	HT_Element_t **table;
	int p, m, numOfElements; // c1, c2;
} HT_HashTable_t;


HT_HashTable_t *HT_Create(int p, int m);
void *HT_Search(HT_HashTable_t *ht, int key);
void HT_Insert(HT_HashTable_t *ht, int key, void *value);
void HT_Delete(HT_HashTable_t *ht, int key);
void HT_Print(HT_HashTable_t *ht);
void HT_Clear(HT_HashTable_t *ht);

#endif /* INC_HASH_TABLE_H_ */
