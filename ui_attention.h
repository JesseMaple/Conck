#pragma once
#include "JC_String.h"

// -------------------------------
// is not debug
void stringStatusError(void);

// -------------------------------
// is debug
void stringStatusErrorD(string str);

// -------------------------------
void invalidCmd(const char* cmd);

void failedToSaveFile(void);

void contactIsDuplicate(void);

void failedToFindContact(void);

void failedToChangeInfo(void);

void failedToSaveFileAll(void);