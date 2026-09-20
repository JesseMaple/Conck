#include "util_file.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool saveContactA(string name, string number, const char* filename) {
	if (!filename || getStringStatus(name) || getStringStatus(number)) {
		return false;
	}

	FILE* file = fopen(filename, "a");
	if (!file)return false;

	fprintf(file, "%s|%s\n", getStringContent(name), getStringContent(number));

	fclose(file);
	return true;
}
bool saveContactW(string name, string number, const char* filename) {
	if (!filename || getStringStatus(name) || getStringStatus(number)) {
		return false;
	}
	FILE* file = fopen(filename, "w");
	if (!file)return false;
	fprintf(file, "%s|%s\n", getStringContent(name), getStringContent(number));
	fclose(file);
	return true;
}

int getCount(const char* filename) {
	FILE* file = fopen(filename,"r");
	if (!file)return -1;

	int count = 0;
	char buffer[500];
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		count++;
	}

	fclose(file);
	return count;
}

bool isContactDuplicate(const String* name, const String* number, const char* filename) {
	if (!filename || getStringStatus(name) || getStringStatus(number)) {
		return false;
	}

    // 对输入的 name 和 number 做 trim，用于比较
    string trimmedInputName = trim(name);
    string trimmedInputNumber = trim(number);

    const char* inputName = getStringContent(trimmedInputName);
    const char* inputNumber = getStringContent(trimmedInputNumber);

    if (!inputName || !inputNumber) {
        deleteString(trimmedInputName);
        deleteString(trimmedInputNumber);
        return false;
    }

    FILE* file = fopen(filename, "r");
    if (!file) {
        deleteString(trimmedInputName);
        deleteString(trimmedInputNumber);
        return false;
    }

    char buffer[500];
    bool found = false;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        string line = newStringFrom(buffer);
        int partsCount = 0;
        string* parts = splitString(line, '|', &partsCount);

        if (partsCount == 2) {
            string trimmedName = trim(parts[0]);
            string trimmedNumber = trim(parts[1]);

            const char* existingName = getStringContent(trimmedName);
            const char* existingNumber = getStringContent(trimmedNumber);

            if (existingName && existingNumber) {
                if (strcmp(existingName, inputName) == 0 &&
                    strcmp(existingNumber, inputNumber) == 0) {
                    found = true;
                }
            }

            deleteString(trimmedName);
            deleteString(trimmedNumber);
        }

        for (int i = 0; i < partsCount; i++) {
            deleteString(parts[i]);
        }
        free(parts);
        deleteString(line);

        if (found) break;
    }

    fclose(file);
    deleteString(trimmedInputName);
    deleteString(trimmedInputNumber);

    return found;
}

Contact* loadContactsFromFile(const char* filename, int* outCount) {
    if (!filename || !outCount) {
        *outCount = 0;
        return NULL;
    }

    int count = getCount(filename);
    if (count <= 0) {
        *outCount = 0;
        return NULL;
    }

    FILE* file = fopen(filename, "r");
    if (!file) {
        *outCount = 0;
        return NULL;
    }

    Contact* contacts = (Contact*)malloc(sizeof(Contact) * count);
    if (!contacts) {
        fclose(file);
        *outCount = 0;
        return NULL;
    }

    char buffer[500];
    int index = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL && index < count) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        string line = newStringFrom(buffer);
        int partsCount = 0;
        string* parts = splitString(line, '|', &partsCount);

        if (partsCount == 2) {
            const char* namePart = getStringContent(parts[0]);
            const char* numberPart = getStringContent(parts[1]);

            if (namePart && numberPart) {
                contacts[index].name = newStringFrom(namePart);
                contacts[index].number = newStringFrom(numberPart);
                index++;
            }
        }

        for (int i = 0; i < partsCount; i++) {
            deleteString(parts[i]);
        }
        free(parts);
        deleteString(line);
    }

    fclose(file);

    *outCount = index;
    if (index == 0) {
        free(contacts);
        return NULL;
    }

    return contacts;
}

Contact* findContact(Contact* contacts, const String* info, int* outCount, int count) {
	if (!contacts || !info || !outCount || count <= 0) {
        if (outCount)*outCount = 0;
		return NULL;
	}
	string trimmedInfo = trim(info);
	int matchCount = 0;
	for (int i = 0; i < count; i++) {
		if (contains(contacts[i].name, trimmedInfo) || contains(contacts[i].number, trimmedInfo)) {
			matchCount++;
		}
	}
	if (matchCount == 0) {
		*outCount = 0;
		deleteString(trimmedInfo);
		return NULL;
	}
	Contact* matchedContacts = (Contact*)malloc(sizeof(Contact) * matchCount);
	if (!matchedContacts) {
		*outCount = 0;
		deleteString(trimmedInfo);
		return NULL;
	}
	int idx = 0;
	for (int i = 0; i < count; i++) {
		if (contains(contacts[i].name, trimmedInfo) || contains(contacts[i].number, trimmedInfo)) {
			matchedContacts[idx].name = newStringFrom(getStringContent(contacts[i].name));
			matchedContacts[idx].number = newStringFrom(getStringContent(contacts[i].number));
			idx++;
		}
	}
	deleteString(trimmedInfo);
	*outCount = matchCount;
	return matchedContacts;
}

bool cleanFile(const char* filename) {
	if (!filename) {
		return false;
	}
	FILE* file = fopen(filename, "w");
	if (!file) {
		return false;
	}
	fclose(file);
	return true;
}