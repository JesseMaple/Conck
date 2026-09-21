#include "ui_menu.h"
#include <stdio.h>
#include "ui_prompt.h"

// 主菜单
// #mainMenu
void mainMenu(void) {
	printf(
		"\n===================================\n\n\n"
		"Welcome to use \"ContactBook\"\n\n"
		"input \"look up\" to find the contact\n\n"
		"input \"add\" to add a contact\n\n"
		"input \"modify\" to modify a contact\n\n"
		"input \"delete\" to delete a contact\n\n"
		"input \"exit\" to exit the program\n\n\n"
		"===================================\n\n"
	);
}

// #createMenu
void createMenu(void) {
	printf(
		"\n===================================\n\n\n"
		"input \"return\" to return to last page\n\n"
		"input the information of the contact as below\n\n"
		"name\n"
		"number\n\n"
		"===================================\n\n"
	);
}

// #retrieveMenu
void retrieveMenu(void) {
	retrievePrompt();
	printf("Please input the information of the contact you want to find:\n");
}

// #updateMenu
void updataMenu(void) {
	printf(
		"\n===================================\n\n\n"
		"input \"return\" to return to last page\n\n"
		"input \"retrieve\" to look through the contacts\n\n"
		"input the information of the contact you want to modify\n\n"
		"===================================\n\n"
	);
}

// #deleteMenu
void deleteMenu(void) {
	printf(
		"\n===================================\n\n\n"
		"input \"return\" to return to last page\n\n"
		"input \"retrieve\" to look through the contacts\n\n"
		"input the information of the contact you want to delete\n\n"
		"===================================\n\n"
	);
}