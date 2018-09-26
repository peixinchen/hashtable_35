#pragma once

#include <stdint.h>

// 将一个实体经过有限步的运算  -> 下标
size_t hash(char ch)
{
	return ch - 'a';
}

char FindFirstOnce(char *str)
{
	int count[26] = { 0 };

	// 记录每个字符的出现次数
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