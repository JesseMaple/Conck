#include "util_file.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// #saveContactA
bool saveContactA(string name, string number, const char* filename) {
	if (!filename || getStringStatus(name) || getStringStatus(number)) {
		return false;
	}

	FILE* file = fopen(filename, "a");
	if (!file)return false;

	char* nameContent = getStringContent(name);
	char* numberContent = getStringContent(number);
	fprintf(file, "%s|%s\n", nameContent, numberContent);

	free(nameContent);
	free(numberContent);
	fclose(file);
	return true;
}

// #saveContactW
bool saveContactW(string name, string number, const char* filename) {
	if (!filename || getStringStatus(name) || getStringStatus(number)) {
		return false;
	}
	FILE* file = fopen(filename, "w");
	if (!file)return false;
	char* nameContent = getStringContent(name);
	char* numberContent = getStringContent(number);
	fprintf(file, "%s|%s\n", nameContent, numberContent);
	fclose(file);
    free(nameContent);
	free(numberContent);
	return true;
}

// #getCount
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

// #isContactDuplicate
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
		free((void*)inputName);
		free((void*)inputNumber);
        return false;
    }

    FILE* file = fopen(filename, "r");
    if (!file) {
        deleteString(trimmedInputName);
        deleteString(trimmedInputNumber);
        free((void*)inputName);
        free((void*)inputNumber);
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
        string* parts = NULL;
        int splitStatus = splitString(line, '|', &parts, &partsCount);
        if (splitStatus != SUCCESSFULOP) {
            deleteString(line);
            continue;
        }

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
    free((void*)inputName);
    free((void*)inputNumber);

    return found;
}

// #loadContactsFromFile
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
        string* stringArray = NULL;
        int splitStatus = splitString(line, '|', &stringArray, &partsCount);
        if (splitStatus != SUCCESSFULOP) {
			for (int i = 0; i < partsCount; i++) {
				deleteString(stringArray[i]);
			}
			free(stringArray);
			deleteString(line);
			continue;
        }

        if (partsCount == 2) {
            const char* namePart = getStringContent(stringArray[0]);
            const char* numberPart = getStringContent(stringArray[1]);

            if (namePart && numberPart) {
                contacts[index].name = newStringFrom(namePart);
                contacts[index].number = newStringFrom(numberPart);
                index++;
            }

            free((void*)namePart);
            free((void*)numberPart);
        }

        for (int i = 0; i < partsCount; i++) {
            deleteString(stringArray[i]);
        }
        free(stringArray);
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

// #findContact
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
			char* nameContent = getStringContent(contacts[i].name);
			char* numberContent = getStringContent(contacts[i].number);
			matchedContacts[idx].name = newStringFrom(nameContent);
			matchedContacts[idx].number = newStringFrom(numberContent);
			free(nameContent);
			free(numberContent);
			idx++;
		}
	}
	deleteString(trimmedInfo);
	*outCount = matchCount;
	return matchedContacts;
}

// #cleanFile
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