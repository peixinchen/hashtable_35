#pragma once

#include <stdint.h>

// ��һ��ʵ�徭�����޲�������  -> �±�
size_t hash(char ch)
{
	return ch - 'a';
}

char FindFirstOnce(char *str)
{
	int count[26] = { 0 };

	// ��¼ÿ���ַ��ĳ��ִ���
	for (int i = 0; i < strlen(str); i++) {
		count[hash(str[i])] += 1;
	}

	for (int i = 0; i < strlen(str); i++) {
		if (count[hash(str[i])] == 1) {
			return str[i];
		}
	}
}



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