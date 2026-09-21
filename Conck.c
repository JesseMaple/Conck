#include "ui_menu.h"
#include "JC_String.h"
#include "JC_Scanner.h"
#include "ui_attention.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "service_create.h"
#include "service_retrieve.h"
#include "service_updata.h"
#include "service_delete.h"

int main(void) {
	MAIN:
	while (1) {
		mainMenu();

		// 用户输入
		string input = nextLine();

		// 检查字符串状态
		if (getStringStatus(input)) {
			stringStatusErrorD(input);
			deleteString(input);
			continue;
			
		}

		string cmd = trim(input);
		deleteString(input);
		input = NULL;

		if (equalsFrom(cmd,"look up")) {
			retrieve();
			deleteString(cmd);
		}
		else if (equalsFrom(cmd,"add")) {
			create();
			deleteString(cmd);
		}
		else if (equalsFrom(cmd,"modify")) {
			modify();
			deleteString(cmd);
		}
		else if (equalsFrom(cmd,"delete")) {
			delete();
			deleteString(cmd);
		}
		else if (equalsFrom(cmd,"exit")) {
			deleteString(cmd);
			exit(0);
		}
		else {
			char* cmdContent = getStringContent(cmd);
			invalidCmd(cmdContent);
			deleteString(cmd);
			free(cmdContent);
			goto MAIN;
		}
	}
}