#include "ui_menu.h"
#include "util_string.h"
#include "ui_attention.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "service_create.h"
#include "service_retrieve.h"
#include "service_updata.h"
#include "service_delete.h"

bool isDebug = true;

int main(void) {
	MAIN:
	while (1) {
		mainMenu();

		// 用户输入
		string input = nextLine();

		// 检查字符串状态
		if (getStringStatus(input)) {
			// 用户模式
			if (!isDebug) {
				stringStatusError();
				deleteString(input);
				continue;
			}
			// 开发者模式
			else {
				stringStatusErrorD(input);
				deleteString(input);
				continue;
			}
		}

		string cmd = trim(input);
		deleteString(input);
		input = NULL;

		if (!strcmp(getStringContent(cmd), "look up")) {
			retrieve(isDebug);
			deleteString(cmd);
		}
		else if (!strcmp(getStringContent(cmd), "add")) {
			create(isDebug);
			deleteString(cmd);
		}
		else if (!strcmp(getStringContent(cmd), "modify")) {
			modify(isDebug);
			deleteString(cmd);
		}
		else if (!strcmp(getStringContent(cmd), "delete")) {
			delete(isDebug);
			deleteString(cmd);
		}
		else if (!strcmp(getStringContent(cmd), "exit")) {
			deleteString(cmd);
			exit(0);
		}
		else {
			invalidCmd(getStringContent(cmd));
			deleteString(cmd);
			goto MAIN;
		}
	}
}