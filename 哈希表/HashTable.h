#pragma once

#include <stdlib.h>

typedef int ssize_t;
typedef unsigned int size_t;

typedef int KeyType;

typedef size_t(*HashFuncType)(KeyType key, size_t capacity);
// ����д�� C ���Բ�֧�֣�����˼����������
//typedef size_t(*)(KeyType key, size_t capacity) HashFuncType;

typedef enum {
	EMPTY,
	EXIST,
	DELETE
}	State;

typedef struct {
	KeyType key;
	State	state;	// �յģ��ѱ�ռ�õģ��ѱ�ɾ����
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

// ����/ɾ��/����
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

		// ����̽��
		index = (origin_index + i * i) % pHT->capacity;
		i++;

		// �����ϣ�����ˣ����ܻ���ѭ��
		// ���ǵĹ�ϣ���ǲ�������
	}

	return -1;
}

// 1. �Ȳ���
// 2.�ҵ��˰�״̬��Ϊ��ɾ��
ssize_t HashRemove(HashTable *pHT, KeyType key)
{
	// 1. �ȸ��� ��ϣ���� �����±�
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

		// ����̽��
		index = (origin_index + i * i) % pHT->capacity;
		i++;
	}
	return -1;
}

int HashInsert(HashTable *pHT, KeyType key);

/*
1. ��������
2. ������ʱ����������
3. ��������ѭ�����ã���ʵ�ʲ���
*/
void ExpandIfRequired(HashTable *pHT)
{
	if (pHT->size * 10 / pHT->capacity < 8) {
		// ������
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

// ����
// �ɹ����� 0��ʧ�ܷ��� -1
// �ظ� key������ʧ��
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

		// ����̽��
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