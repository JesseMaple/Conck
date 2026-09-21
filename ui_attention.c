#include "ui_attention.h"
#include <stdio.h>

// -------------------------------
// #stringStatusErrorD
void stringStatusErrorD(string str) {
	printf("The status of the string is %s\n", stringStatusToCharArray(getStringStatus(str)));
}

// -------------------------------
// #invalidCmd
void invalidCmd(const char* cmd) {
	if (!cmd) {
		printf("Invalid cmd!\n");
		return;
	}

	printf("Invalid cmd -- %s!\n", cmd);
}

// #failedToSaveFile
void failedToSaveFile(void) {
	printf(
		"Failed to save the contact into the file\n\n"
	);
}

// #contactIsDuplicate
void contactIsDuplicate(void) {
	printf("This contact has been recorded!\n");
}

// #failedToFindContact
void failedToFindContact(void) {
	printf("Failed to find the contact!\n");
}

// #failedToChangeInfo
void failedToChangeInfo(void) {
	printf("Failed to change the info you just input!\n");
}

// #failedToSaveFileAll
void failedToSaveFileAll(void) {
	printf(
		"Failed to save some contacts into the file\n\n"
	);
}