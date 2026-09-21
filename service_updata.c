#include "service_updata.h"
#include "ui_menu.h"
#include "JC_String.h"
#include "JC_Scanner.h"
#include "ui_attention.h"
#include <string.h>
#include "service_retrieve.h"
#include "service_other.h"
#include "util_file.h"
#include <stdlib.h>
#include "ui_prompt.h"

void modify() {
	while (1) {
		/*
		* 该层释放
		* input
		* contacts
		*/
		updataMenu();
		string input = nextLine();

		// 字符串保护
		if (getStringStatus(input)) {
			stringStatusErrorD(input);
			deleteString(input);
			continue;
		}

		// 返回指令
		if (equalsFrom(input, "return")) {
			deleteString(input);
			break;
		}
		// 浏览指令
		if (equalsFrom(input, "retrieve")) {
			retrieve();
			deleteString(input);
			continue;
		}

		// contacts计数
		int count = 0;
		Contact* contacts = loadContactsFromFile("contacts.txt", &count);

		int matchedCount = 0;

		// 相匹配的contact计数
		for (int i = 0;i < count;i++) {
			if (contains(contacts[i].name, input) || contains(contacts[i].number, input)) {
				matchedCount++;
			}
		}

		// 如果没有相匹配的则直接释放input和contacts并重新循环
		if (!matchedCount) {
			failedToFindContact();
			deleteString(input);
			for (int i = 0;i < count;i++) {
				deleteString(contacts[i].name);
				deleteString(contacts[i].number);
			}
			free(contacts);
			continue;
		}

		int idx = 0;
		Contact* matchedContacts = (Contact*)malloc(sizeof(Contact) * matchedCount);
		// 保护matchedContacts
		if (!matchedContacts) {
			deleteString(input);
			for (int i = 0;i < count;i++) {
				deleteString(contacts[i].name);
				deleteString(contacts[i].number);
			}
			free(contacts);
			continue;
		}

		for (int i = 0;i < count;i++) {
			if (contains(contacts[i].name, input) || contains(contacts[i].number, input)) {
				char* nameContent = getStringContent(contacts[i].name);
				char* numberContent = getStringContent(contacts[i].number);
				matchedContacts[idx].name = newStringFrom(nameContent);
				matchedContacts[idx++].number = newStringFrom(numberContent);
				free(nameContent);
				free(numberContent);
			}
		}

		// 此时input已经没用，进行释放
		deleteString(input);

		// 展示已匹配的联系人供选择
		showContacts(matchedContacts, matchedCount);
		while (1) {
			/*
			* 该层释放
			* whichOne
			*/
			chooseOneContactPrompt();
			string whichOne = nextLine();
			// 字符串保护
			if (getStringStatus(whichOne)) {
				stringStatusErrorD(whichOne);
				deleteString(whichOne);
				continue;
			}
			// 返回指令
			if (equalsFrom(whichOne, "return")) {
				deleteString(whichOne);
				break;
			}
			// 记录是contacts里的哪一个匹配上了
			int flag = -1, matchedFlag = -1;
			for (int i = 0;i < count;i++) {
				if (equals(whichOne, contacts[i].name) || equals(whichOne, contacts[i].number)) {
					flag = i;
					break;
				}
			}
			for (int i = 0;i < matchedCount;i++) {
				if (equals(whichOne, matchedContacts[i].name) || equals(whichOne, matchedContacts[i].number)) {
					matchedFlag = i;
					break;
				}
			}
			// 如果没有一个匹配上则重新输入
			if (flag == -1 || matchedFlag == -1) {
				failedToFindContact();
				deleteString(whichOne);
				continue;
			}
			while (1) {
				/*
				* 该层释放
				* choice
				*/
				// 提示选择修改名字还是电话
				nameOrNumber();
				// 标记是name还是number
				int choiceFlag = 0;
				string choice = nextLine();
				// 字符串保护
				if (getStringStatus(choice)) {
					stringStatusErrorD(choice);
					deleteString(choice);
					continue;
				}

				// 返回指令
				if (equalsFrom(choice, "return")) {
					deleteString(choice);
					break;
				}
				// name指令
				if (equalsFrom(choice, "name")) {
					choiceFlag = 1;
				}
				// number指令
				else if (equalsFrom(choice, "number")) {
					choiceFlag = 2;
				}
				// 非法指令，重新输入
				else {
					char* cmd = getStringContent(choice);
					invalidCmd(cmd);
					deleteString(choice);
					free(cmd);
					continue;
				}
				while (1) {
					/*
					* 该层释放
					* info
					*/
					updataPrompt();
					string info = nextLine();
					// 字符串保护
					if (getStringStatus(info)) {
						stringStatusErrorD(info);
						deleteString(info);
						continue;
					}
					// 返回指令
					if (equalsFrom(info, "return")) {
						deleteString(info);
						break;
					}
					if (choiceFlag == 1) {
						char* content = getStringContent(info);
						string newName = NULL, newMatchedName = NULL;
						if (content) {
							newName = newStringFrom(content);
							newMatchedName = newStringFrom(content);
							free(content);
						}
						if (!newName || getStringStatus(newName) || !newMatchedName || getStringStatus(newMatchedName)) {
							deleteString(newName);
							deleteString(newMatchedName);
							failedToChangeInfo();
							deleteString(info);
							continue;
						}
						deleteString(contacts[flag].name);
						contacts[flag].name = newName;
						deleteString(matchedContacts[matchedFlag].name);
						matchedContacts[matchedFlag].name = newMatchedName;

						succeedToChangeInfo();
						deleteString(info);
						continue;
					}
					if (choiceFlag == 2) {
						char* content = getStringContent(info);
						string newNumber = NULL, newMatchedNumber = NULL;
						if (content) {
							newNumber = newStringFrom(content);
							newMatchedNumber = newStringFrom(content);
							free(content);
						}
						if (!newNumber || getStringStatus(newNumber) ||
							!newMatchedNumber || getStringStatus(newMatchedNumber)) {
							deleteString(newNumber);
							deleteString(newMatchedNumber);
							failedToChangeInfo();
							deleteString(info);
							continue;
						}
						deleteString(contacts[flag].number);
						contacts[flag].number = newNumber;
						deleteString(matchedContacts[matchedFlag].number);
						matchedContacts[matchedFlag].number = newMatchedNumber;

						succeedToChangeInfo();
						deleteString(info);
						continue;
					}
				}
			}
		}
		// 释放matchedContacts
		for (int i = 0;i < matchedCount;i++) {
			deleteString(matchedContacts[i].name);
			deleteString(matchedContacts[i].number);
		}
		free(matchedContacts);
		int status = saveContacts(contacts, "contacts.txt", count);
		if (status == -1) {
			failedToSaveFile();
		}
		else if (status == 0) {
			succeedToSaveFile("contacts.txt");
		}
		else if (status == 1) {
			failedToSaveFileAll();
		}
		for (int i = 0;i < count;i++) {
			deleteString(contacts[i].name);
			deleteString(contacts[i].number);
		}
		free(contacts);
	}
}