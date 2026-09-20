#include "service_updata.h"
#include "ui_menu.h"
#include "util_string.h"
#include "ui_attention.h"
#include <string.h>
#include "service_retrieve.h"
#include "service_other.h"
#include "util_file.h"
#include <stdlib.h>
#include "ui_prompt.h"

void modify(bool isDebug) {
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

		// 返回指令
		if (!strcmp(getStringContent(input), "return")) {
			deleteString(input);
			break;
		}
		// 浏览指令
		if (!strcmp(getStringContent(input), "retrieve")) {
			retrieve(isDebug);
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
				matchedContacts[idx].name = newStringFrom(getStringContent(contacts[i].name));
				matchedContacts[idx++].number = newStringFrom(getStringContent(contacts[i].number));
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
				// 用户模式
				if (!isDebug) {
					stringStatusError();
					deleteString(whichOne);
					continue;
				}
				// 开发者模式
				else {
					stringStatusErrorD(whichOne);
					deleteString(whichOne);
					continue;
				}
			}
			// 返回指令
			if (!strcmp(getStringContent(whichOne), "return")) {
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
					// 用户模式
					if (!isDebug) {
						stringStatusError();
						deleteString(choice);
						continue;
					}
					// 开发者模式
					else {
						stringStatusErrorD(choice);
						deleteString(choice);
						continue;
					}
				}

				// 返回指令
				if (!strcmp(getStringContent(choice), "return")) {
					deleteString(choice);
					break;
				}
				// name指令
				if (!strcmp(getStringContent(choice), "name")) {
					choiceFlag = 1;
				}
				// number指令
				else if (!strcmp(getStringContent(choice), "number")) {
					choiceFlag = 2;
				}
				// 非法指令，重新输入
				else {
					invalidCmd(getStringContent(choice));
					deleteString(choice);
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
						// 用户模式
						if (!isDebug) {
							stringStatusError();
							deleteString(info);
							continue;
						}
						// 开发者模式
						else {
							stringStatusErrorD(info);
							deleteString(info);
							continue;
						}
					}
					// 返回指令
					if (!strcmp(getStringContent(info), "return")) {
						deleteString(info);
						break;
					}
					if (choiceFlag == 1) {
						// 修改信息如果失败
						if (!setStringContent(contacts[flag].name, getStringContent(info))||!setStringContent(matchedContacts[matchedFlag].name, getStringContent(info))) {
							failedToChangeInfo();
							deleteString(info);
							continue;
						}
						else {
							succeedToChangeInfo();
							deleteString(info);
							continue;
						}
					}
					if (choiceFlag == 2) {
						if (!setStringContent(contacts[flag].number, getStringContent(info))||!setStringContent(matchedContacts[matchedFlag].number, getStringContent(info))) {
							failedToChangeInfo();
							deleteString(info);
							continue;
						}
						else {
							succeedToChangeInfo();
							deleteString(info);
							continue;
						}
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