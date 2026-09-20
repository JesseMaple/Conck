#pragma once

#include <stdbool.h>
#include "util_string.h"

typedef struct Contact {
	string name;
	string number;
}Contact;

static int compareByName(const void* a, const void* b);
static int compareByNumber(const void* a, const void* b);

static void displayContacts(Contact* contacts, int count);

bool showThroughName(const char* filename);

bool showThroughNumber(const char* filename);

void showContacts(Contact* contacts, int count);

int saveContacts(Contact* contacts, const char* filename, const int count);