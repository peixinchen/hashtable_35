#pragma once

#include <stdlib.h>

typedef int ssize_t;
typedef unsigned int size_t;

typedef int KeyType;

typedef size_t(*HashFuncType)(KeyType key, size_t capacity);
// 下面写法 C 语言不支持，但意思就是这样的
//typedef size_t(*)(KeyType key, size_t capacity) HashFuncType;

typedef enum {
	EMPTY,
	EXIST,
	DELETE
}	State;

typedef struct {
	KeyType key;
	State	state;	// 空的，已被占用的，已被删除的
}	ItemType;

typedef struct HashTable {
	ItemType *array;
	size_t  size;
	size_t  capacity;
	HashFuncType hash;
}	HashTable;

void HashTableInit(HashTable *pHT, size_t capacity, HashFuncType hash)
{
	pHT->array = (ItemType *)malloc(sizeof(ItemType)* capacity);
	pHT->capacity = capacity;
	pHT->size = 0;
	pHT->hash = hash;

	for (size_t i = 0; i < capacity; i++) {
		pHT->array[i].state = EMPTY;
	}
}

void HashTableDestroy(HashTable *pHT)
{
	free(pHT->array);
	pHT->capacity = 0;
	pHT->size = 0;
	pHT->hash = NULL;
}

// 查找/删除/插入
ssize_t HashSearch(const HashTable *pHT, KeyType key)
{
	size_t origin_index = pHT->hash(key, pHT->capacity);
	size_t index = origin_index;
	int i = 1;
	while (1) {
		if (pHT->array[index].state == EMPTY) {
			return -1;
		}

		if (pHT->array[index].state == EXIST 
			&& pHT->array[index].key == key) {
			return index;
		}

		// 二次探测
		index = (origin_index + i * i) % pHT->capacity;
		i++;

		// 如果哈希表满了，可能会死循环
		// 我们的哈希表是不会满的
	}

	return -1;
}

// 1. 先查找
// 2.找到了把状态置为已删除
ssize_t HashRemove(HashTable *pHT, KeyType key)
{
	// 1. 先根据 哈希函数 计算下标
	size_t origin_index = pHT->hash(key, pHT->capacity);
	size_t index = origin_index;
	int i = 1;
	while (pHT->array[index].state != EMPTY) {
		if (pHT->array[index].state == EXIST
			&& pHT->array[index].key == key) {
		
			pHT->array[index].state = DELETE;
			pHT->size--;
			return index;
		}

		// 二次探测
		index = (origin_index + i * i) % pHT->capacity;
		i++;
	}
	return -1;
}

int HashInsert(HashTable *pHT, KeyType key);

/*
1. 负载因子
2. 利用临时变量做扩容
3. 看起来会循环调用，但实际不会
*/
void ExpandIfRequired(HashTable *pHT)
{
	if (pHT->size * 10 / pHT->capacity < 8) {
		// 不扩容
		return;
	}

	HashTable temp;
	HashTableInit(&temp, pHT->capacity * 2, pHT->hash);

	for (size_t i = 0; i < pHT->capacity; i++) {
		if (pHT->array[i].state == EXIST) {
			HashInsert(&temp, pHT->array[i].key);
		}
	}

	free(pHT->array);
	pHT->array = temp.array;
	pHT->capacity = temp.capacity;
}

// 插入
// 成功返回 0，失败返回 -1
// 重复 key，插入失败
int HashInsert(HashTable *pHT, KeyType key)
{
	if (HashSearch(pHT, key) != -1) {
		return -1;
	}

	ExpandIfRequired(pHT);

	size_t origin_index = pHT->hash(key, pHT->capacity);
	size_t index = origin_index;
	int i = 1;
	while (1) {
		if (pHT->array[index].state != EXIST) {
			break;
		}

		// 二次探测
		index = (origin_index + i * i) % pHT->capacity;
		i++;
	}

	pHT->array[index].key = key;
	pHT->array[index].state = EXIST;
	pHT->size++;

	return 0;
}

size_t mod(KeyType key, size_t capacity)
{
	return key % capacity;
}

#include <stdio.h>

void Test()
{
	HashTable ht;
	HashTableInit(&ht, 7, mod);

	HashInsert(&ht, 0);
	HashInsert(&ht, 7);
	HashInsert(&ht, 14);
	HashInsert(&ht, 21);
	HashInsert(&ht, 28);
	HashInsert(&ht, 8);
	HashInsert(&ht, 9);
	HashInsert(&ht, 10);

	HashRemove(&ht, 7);
	printf("%d\n", HashSearch(&ht, 7));
	printf("%d\n", HashSearch(&ht, 14));

	HashTableDestroy(&ht);
}