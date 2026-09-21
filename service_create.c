#include "service_create.h"
#include "ui_menu.h"
#include "JC_String.h"
#include "JC_Scanner.h"
#include "ui_attention.h"
#include <string.h>
#include "util_file.h"
#include "ui_prompt.h"

void create() {
	while (1) {
		createMenu();

		string name = nextLine();

		if (getStringStatus(name)) {
			stringStatusErrorD(name);
			deleteString(name);
			continue;
		}

		if (equalsFrom(name, "return")) {
			deleteString(name);
			break;
		}

		while (1) {
			string number = nextLine();

			if (getStringStatus(number)) {
				stringStatusErrorD(number);
				deleteString(number);
				break;
			}

			if (isContactDuplicate(name, number, "contacts.txt")) {
				contactIsDuplicate();
				break;
			}

			if (equalsFrom(number, "return")) {
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