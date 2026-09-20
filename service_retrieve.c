#include "service_retrieve.h"
#include "ui_menu.h"
#include "util_string.h"
#include "ui_attention.h"
#include <string.h>
#include "service_other.h"
#include "ui_prompt.h"
#include "util_file.h"
#include <stdlib.h>

void retrieve(bool isDebug) {
	/*
	进入 look up
	1. 按文件顺序显示所有联系人（不排序）
	2. 进入循环：
     - 输入 "name" → 按姓名排序重新显示 → 继续循环
     - 输入 "number" → 按号码排序重新显示 → 继续循环
     - 输入 "return" → 退出，返回主菜单
     - 输入其他内容 → 作为关键字，做模糊匹配，显示匹配结果 → 继续循环
	*/

	/*
	* 创建联系人数组并从文件中读取联系人数据
	* 调用 showContacts 函数显示联系人
	* 创建循环，处理用户输入
	* 创建string实例并检查其状态
	* 如果是return则释放内存并返回上一级
	* 如果是name则按姓名排序并显示联系人
	* 如果是number则按号码排序并显示联系人
	* 如果是其他内容，则作为关键字进行模糊匹配并显示匹配结果
	*/

	
	int count = 0;
	// 现已取得联系人于内存
	Contact* contacts = loadContactsFromFile("contacts.txt", &count);
	showContacts(contacts, count);
	retrieveMenu();
	while(1) {
		string input = nextLine();
		if(getStringStatus(input)) {
			if(!isDebug) {
				stringStatusError();
				deleteString(input);
				continue;
			}
			else {
				stringStatusErrorD(input);
				deleteString(input);
				continue;
			}
		}
 
		if(!strcmp(getStringContent(input), "return")) {
			for(int i=0; i < count; i++) {
				deleteString(contacts[i].name);
				deleteString(contacts[i].number);
			}
			free(contacts);
			deleteString(input);
			break;
		}
	 
		if(!strcmp(getStringContent(input), "name")) {
			rerangeSucceed();
			showThroughName("contacts.txt");
			deleteString(input);
			continue;
		}
	 
		if(!strcmp(getStringContent(input), "number")) {
			rerangeSucceed();
			showThroughNumber("contacts.txt");
			deleteString(input);
			continue;
		}
	 
		if(!strcmp(getStringContent(input), "prompt")) {
			retrievePrompt();
			deleteString(input);
		continue;
		}
	 
		int matchedCount = count;
		Contact* matchedContacts = findContact(contacts, input, &matchedCount, count);
		showContacts(matchedContacts, matchedCount);
	
		for(int i=0; i < matchedCount; i++) {
			deleteString(matchedContacts[i].name);
			deleteString(matchedContacts[i].number);
		}
		free(matchedContacts);
		deleteString(input);
	}

	/*
	* findContactBy***
	*/
}