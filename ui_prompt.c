#include "ui_prompt.h"
#include <stdio.h>

// #succeedToSaveFile
void succeedToSaveFile(const char* filename) {
	printf("Successfully saved the contact to the file: %s\n\n", filename);
}

// #retrievePrompt
void retrievePrompt(void) {
	printf(
		"\nInput \"return\" to return to last page\n"
		"Input \"name\" to rerange the contacts by name\n"
		"Input \"number\" to rerange the contacts by number\n"
		"Input \"prompt\" to show the prompt again\n\n"
	);
}

// #nameOrNumber
void nameOrNumber(void) {
	printf("\nYou can input \"return\" to return to last page\n");
	printf("Or input name or number to change\n");
}

// #updataPrompt
void updataPrompt(void) {
	printf("\nYou can input \"return\" to return to last page\n");
	printf("Please input the information of the contact you choice to modify:\n");
}

// #succeedToChangeInfo
void succeedToChangeInfo(void) {
	printf("Successfully changed the contact!PS:the information may not been saved,please pay attention to the following information!\n");
}

// #rerangeSucceed
void rerangeSucceed(void) {
	printf("Successfully reranged the contacts!\n");
}

// #chooseOneContactPrompt
void chooseOneContactPrompt(void) {
	printf("\nChoose a contact or input \"return\" to return to last page\n");
}

// #deleteConfirm
void deleteConfirm(void) {
	printf("\nAre you sure to delete this contact? Input \"yes\" to delete or \"no\" to cancel\n");
}

// #deleteCancel
void deleteCancel(void) {
	printf("\nYou have canceled the deletion of the contact.\n");
}

// #deleteSuccess
void deleteSuccess(void) {
	printf("\nThe contact has been deleted successfully!\n");
}