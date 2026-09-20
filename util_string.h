#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef struct String String;

typedef String* string;

typedef enum Status {
	NULLWHOLESTRING = -1,
	AVAILUABLE = 0,
	NULLPOINTER = 1,
	FAILEDTOREALLOC = 2,
	FAILEDTOAPPEND = 3,
}Status;

string newString();

string newStringFrom(const char* content);

void deleteString(String* str);

int getStringStatus(const String* str);

const char* getStringContent(const string str);

bool setStringContent(string str, const char* content);

size_t getStringLength(const string str);

string appendString(const string src1, const string src2);

char* toArray(const string str);

string toLowerCase(const string str);

string toUpperCase(const string str);

string trim(const string str);

bool equals(const string str1, const string str2);

bool contains(const string str1, const string str2);

bool containsFrom(const string str, const char* ch);

string reverseString(const string str);

string* splitString(const string str, char delimiter, int* count);

const char* statusToString(int status);

string nextLine();
