#include "ui_attention.h"
#include <stdio.h>

// -------------------------------
// is not debug
void stringStatusError(void) {
	printf("This string is not available,please turn to the manager\n");
}

// -------------------------------
// is debug
void stringStatusErrorD(string str) {
	printf("The status of the string is %s\n", statusToString(getStringStatus(str)));
}

// -------------------------------
void invalidCmd(const char* cmd) {
	if (!cmd) {
		printf("Invalid cmd!\n");
		return;
	}

	printf("Invalid cmd -- %s!\n", cmd);
}

void failedToSaveFile(void) {
	printf(
		"Failed to save the contact into the file\n\n"
	);
}

void contactIsDuplicate(void) {
	printf("This contact has been recorded!\n");
}

void failedToFindContact(void) {
	printf("Failed to find the contact!\n");
}

void failedToChangeInfo(void) {
	printf("Failed to change the info you just input!\n");
}

void failedToSaveFileAll(void) {
	printf(
		"Failed to save some contacts into the file\n\n"
	);
}