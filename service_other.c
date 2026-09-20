#include "service_other.h"
#include "util_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui_attention.h"

static int compareByName(const void* a, const void* b) {
	const Contact* ca = (const Contact*)a;
	const Contact* cb = (const Contact*)b;

	// 1. 转小写比较字母顺序（忽略大小写）
	string lowerA = toLowerCase(ca->name);
	string lowerB = toLowerCase(cb->name);

	const char* lowA = getStringContent(lowerA);
	const char* lowB = getStringContent(lowerB);

	int result = strcmp(lowA, lowB);

	deleteString(lowerA);
	deleteString(lowerB);

	// 2. 如果字母顺序不同，直接返回（例如 "apple" vs "banana"）
	if (result != 0) {
		return result;
	}

	// 3. 如果字母完全相同（例如 "BeNana" vs "banana" -> 忽略大小写后都是 "benana"）
	//    则按照 ASCII 码排（大写 B(66) 优先于 小写 b(98)）
	const char* origA = getStringContent(ca->name);
	const char* origB = getStringContent(cb->name);

	return strcmp(origA, origB);
}
static int compareByNumber(const void* a, const void* b) {
	const Contact* ca = (const Contact*)a;
	const Contact* cb = (const Contact*)b;
	const char* numberA = getStringContent(ca->number);
	const char* numberB = getStringContent(cb->number);
	// 防御性处理：如果号码是 NULL，把它视为空字符串来比，避免崩溃
	if (!numberA && !numberB) return 0;
	if (!numberA) return -1;
	if (!numberB) return 1;
	return strcmp(numberA, numberB);
}

static void displayContacts(Contact* contacts, int count) {
	for (int i = 0; i < count; i++) {
		const char* name = getStringContent(contacts[i].name);
		const char* number = getStringContent(contacts[i].number);
		// 只输出数据，不加边框和提示文字
		printf("%s|%s\n", name ? name : "", number ? number : "");
	}
}

bool showThroughName(const char* filename) {
	int count = 0;
	Contact* contacts = loadContactsFromFile(filename, &count);

	if (!contacts || count == 0) {
		return true;  // 空文件也算成功
	}

	qsort(contacts, count, sizeof(Contact), compareByName);
	displayContacts(contacts, count);

	for (int i = 0; i < count; i++) {
		deleteString(contacts[i].name);
		deleteString(contacts[i].number);
	}
	free(contacts);

	return true;
}

bool showThroughNumber(const char* filename) {
	int count = 0;
	Contact* contacts = loadContactsFromFile(filename, &count);

	if (!contacts || count == 0) {
		return true;
	}

	qsort(contacts, count, sizeof(Contact), compareByNumber);
	displayContacts(contacts, count);

	for (int i = 0; i < count; i++) {
		deleteString(contacts[i].name);
		deleteString(contacts[i].number);
	}
	free(contacts);

	return true;
}

void showContacts(Contact* contacts, int count) {
	if (!contacts || count <= 0) {
		failedToFindContact();
		return;
	}
	displayContacts(contacts, count);
}

int saveContacts(Contact* contacts, const char* filename, const int count) {
	if (!contacts || !filename || count < 0) return -1;

	if (!count) {
		if (!cleanFile(filename))return -1;
	}

	bool flag = true;
	for (int i = 0;i < count;i++) {
		if (i == 0) {
			bool firstSave = saveContactW(contacts[i].name, contacts[i].number, filename);
			if (!firstSave)flag = false;
		}
		else {
			if (!saveContactA(contacts[i].name, contacts[i].number, filename)) {
				flag = false;
			}
		}
	}
	return flag ? 0 : 1;
}