#include "service_create.h"
#include "ui_menu.h"
#include "util_string.h"
#include "ui_attention.h"
#include <string.h>
#include "util_file.h"
#include "ui_prompt.h"

void create(bool isDebug) {
	while (1) {
		createMenu();

		string name = nextLine();

		if (getStringStatus(name)) {
			// 用户模式
			if (!isDebug) {
				stringStatusError();
				deleteString(name);
				continue;
			}
			// 开发者模式
			else {
				stringStatusErrorD(name);
				deleteString(name);
				continue;
			}
		}

		if (!strcmp(getStringContent(name), "return")) {
			deleteString(name);
			break;
		}

		while (1) {
			string number = nextLine();

			if (getStringStatus(number)) {
				// 用户模式
				if (!isDebug) {
					stringStatusError();
					deleteString(number);
					break;
				}
				// 开发者模式
				else {
					stringStatusErrorD(number);
					deleteString(number);
					break;
				}
			}

			if (isContactDuplicate(name, number, "contacts.txt")) {
				contactIsDuplicate();
				break;
			}

			if (!strcmp(getStringContent(number), "return")) {
				deleteString(number);
				break;
			}

			if (!saveContactA(name, number, "contacts.txt")) {
				failedToSaveFile();
			} else {
				succeedToSaveFile("contacts.txt");
				break;
			}
		}

		deleteString(name);
	}
}