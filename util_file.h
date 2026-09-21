#pragma once

#include <stdbool.h>
#include "JC_String.h"
#include "service_other.h"

bool saveContactA(string name, string number, const char* filename);
bool saveContactW(string name, string number, const char* filename);

int getCount(const char* filename);

bool isContactDuplicate(const String* name, const String* number, const char* filename);

Contact* loadContactsFromFile(const char* filename, int* outCount);

Contact* findContact(Contact* contacts, const String* info, int* outCount, int count);

bool cleanFile(const char* filename);