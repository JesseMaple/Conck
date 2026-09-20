#include "util_string.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <wctype.h>

typedef struct String {
    char* buffer;
    int status;
}String;

typedef enum {
    INIT_STRING_SIZE = 200,
    GROWTH = 2,
}Sizes;

// 构造函数
/* change the string */
// finished
string newString() {
    string str = (string)malloc(sizeof(String)); // 给结构体本身分配堆内存
    if (!str) {// 分配失败，返回空指针
        return NULL;
    }

    str->buffer = (char*)calloc(INIT_STRING_SIZE, sizeof(char));
    if (!str->buffer) {// 内容内存分配失败，返回str并修改状态为空指针
        str->status = NULLPOINTER;
    }
    else {
        str->status = AVAILUABLE;
    }

    return str;
}

/* change the string */
// finished
string newStringFrom(const char* content) {
    // 检查char非空，如果非空则更改状态
    bool isNullPointer = false;
    if (!content) isNullPointer = true;

    // 为string分配内存
    string str = (string)malloc(sizeof(String));
    // 分配内存失败
    if (!str) return NULL;
    if (isNullPointer) {
        str->status = NULLPOINTER;
        str->buffer = NULL;
        return str;
    }

    size_t len = strlen(content);
    size_t buffer_size = len + 1;

    str->buffer = (char*)malloc(buffer_size);
    if (!str->buffer) {
        str->status = NULLPOINTER;   // 标记状态为错误
        str->buffer = NULL;          // 显式置空
    }
    else {
        memcpy(str->buffer, content, buffer_size);
        str->status = AVAILUABLE;
    }
    return str;
}

// 析构函数
/* change the string */
// finished
// Attention:remember to free and ponit to NULL
void deleteString(string str) {
    if (!str) return;
    if (str->buffer) {
        free(str->buffer);
        str->buffer = NULL;
    }
    free(str);
}

// 获取状态
// finished
int getStringStatus(const String* str) {
    if (!str) return NULLWHOLESTRING;
    return str->status;
}

// 获取内容
// finished
const char* getStringContent(const string str) {
    // 如果string为空或没有内容
    if (!str || !str->buffer)return NULL;
    if (*str->buffer == '\0')return '\0';
    return str->buffer;
}

// 设置内容（支持任意长度，动态分配）
/* change the string */
// finished
bool setStringContent(string str, const char* content) {
    if (!str) return false;  // 无效对象，直接返回，调用方应该检查str是否为空

    // 如果 content 为 NULL，视为清空字符串
    if (!content) {
        if (str->buffer)free(str->buffer);
        str->buffer = (char*)malloc(1);
        if (str->buffer) {
            str->buffer[0] = '\0';
            str->status = AVAILUABLE;
        }
        else {
            str->buffer = NULL;
            str->status = NULLPOINTER;
        }
        return true;
    }

    size_t len = strlen(content);
    char* new_buf = (char*)malloc(len + 1);  // 分配精确大小的新缓冲区
    if (!new_buf) {
        // 分配失败，原string内容不变，状态不变
        return false;
    }

    // 复制内容（包含结尾 '\0'）
    memcpy(new_buf, content, len + 1);
    // 释放旧缓冲区
    if (str->buffer) {
        free(str->buffer);
    }
    str->buffer = new_buf;
    str->status = AVAILUABLE;

    return true;
}

// finished
size_t getStringLength(const string str) {
    if (!str || !str->buffer) {
        return 0;
    }
    return strlen(str->buffer);
}

/* return new string */
// finished
string appendString(const string src1, const string src2) {
    // 若两个str均为空
    if (!src1 || !src2) return NULL;
    // 如果内容均空
    if ((!src1->buffer || *src1->buffer == '\0') && (!src2->buffer || *src2->buffer == '\0')) {
        return newString();
    }
    if (!src1->buffer || *src1->buffer == '\0') {
        return newStringFrom(getStringContent(src2)); // 拷贝
    }
    if (!src2->buffer || *src2->buffer == '\0') {
        return newStringFrom(getStringContent(src1));
    }

    // 均为非空内容
    size_t len1 = strlen(src1->buffer);
    size_t len2 = strlen(src2->buffer);
    char* combined = (char*)malloc(len1 + len2 + 1);
    if (!combined) {
        // ★ 只在 combined 分配失败时使用 FAILEDTOAPPEND 状态 ★
        string err = (string)malloc(sizeof(String));
        if (!err) return NULL;
        err->buffer = NULL;
        err->status = FAILEDTOAPPEND;
        return err;
    }

    memcpy(combined, src1->buffer, len1);
    memcpy(combined + len1, src2->buffer, len2 + 1);

    string result = (string)malloc(sizeof(String));
    if (!result) {
        free(combined);
        return NULL;
    }
    result->buffer = combined;
    result->status = AVAILUABLE;
    return result;
}

// finish
char* toArray(const string str) {
    if (!str || !str->buffer)return NULL;
    if (*str->buffer == '\0')return '\0';
    size_t len = getStringLength(str) + 1;
    char* content = (char*)malloc(len);
    return content ? (char*)memcpy(content, str->buffer, len) : NULL;
}

/* return new string */
// finished
string toLowerCase(const string str) {
    if (!str)return NULL;
    if (!str->buffer)return newStringFrom(NULL);
    if (*str->buffer == '\0')return newStringFrom("");

    size_t len = getStringLength(str);

    string newOne = (string)malloc(sizeof(String));
    if (!newOne)return NULL;

    newOne->buffer = (char*)malloc(len + 1);
    if (!newOne->buffer) {
        newOne->buffer = NULL;
        newOne->status = NULLPOINTER;
        return newOne;
    }

    memcpy(newOne->buffer, str->buffer, len + 1);

    for (size_t i = 0;i < len;i++) {
        newOne->buffer[i] = tolower((unsigned char)newOne->buffer[i]);
    }

    newOne->status = AVAILUABLE;
    return newOne;
}

/* return new string */
// finished
string toUpperCase(const string str) {
    if (!str || !str->buffer)return NULL;
    if (!str->buffer)return newStringFrom(NULL);
    if (*str->buffer == '\0')return newStringFrom("");

    size_t len = getStringLength(str);

    string newOne = (string)malloc(sizeof(String));
    if (!newOne)return NULL;

    newOne->buffer = (char*)malloc(len + 1);
    if (!newOne->buffer) {
        newOne->buffer = NULL;
        newOne->status = NULLPOINTER;
        return newOne;
    }

    memcpy(newOne->buffer, str->buffer, len + 1);

    for (size_t i = 0;i < len;i++) {
        newOne->buffer[i] = toupper((unsigned char)newOne->buffer[i]);
    }

    newOne->status = AVAILUABLE;
    return newOne;
}

/* return new string */
// finished
string trim(const string str) {
    if (!str)return NULL;
    if (!str->buffer)return newStringFrom(NULL);
    if (*str->buffer == '\0')return newStringFrom("");

    // 获取内容长度（不含 '\0'）
    size_t len = getStringLength(str);

    // 找第一个非空白字符
    size_t start = 0;
    while (start < len && isspace((unsigned char)str->buffer[start])) {
        start++;
    }

    // 如果全是空白，返回空串
    if (start == len) {
        return newString();
    }

    // 找最后一个非空白字符
    size_t end = len - 1;
    while (end > start && isspace((unsigned char)str->buffer[end])) {
        end--;
    }

    // 新字符串长度 = end - start + 1（不含 '\0'）
    size_t new_len = end - start + 1;

    // 分配新结构体和缓冲区
    string temp = (string)malloc(sizeof(String));
    if (!temp) return NULL;

    temp->buffer = (char*)malloc(new_len + 1);
    if (!temp->buffer) {
        temp->buffer = NULL;
        temp->status = NULLPOINTER;
        return temp;
    }

    // 复制子串（从 start 开始，共 new_len 个字符）
    memcpy(temp->buffer, str->buffer + start, new_len);
    temp->buffer[new_len] = '\0';   // 手动添加结束符

    temp->status = AVAILUABLE;
    return temp;
}

// finished
bool equals(const string str1, const string str2) {
    // 如果指针相同（同一对象），直接相等
    if (str1 == str2) return true;

    // 如果任一为 NULL，返回 false（除非两者都是 NULL，但前面已经处理了）
    if (!str1 || !str2) return false;

    // 处理 buffer 为空的情况
    if ((!str1->buffer || *str1->buffer == '\0') && (!str2->buffer || *str2->buffer == '\0')) return true;
    if ((!str1->buffer || *str1->buffer == '\0') || (!str2->buffer || *str2->buffer == '\0')) return false;

    // 直接用 strcmp 比较 C 字符串
    return strcmp(str1->buffer, str2->buffer) == 0;
}

// finished
bool contains(const string str1, const string str2) {
    if (!str2)return true;
    if (!str2->buffer)return str1 != NULL;
    if (*str2->buffer == '\0') {
        if (!str1)return false;
        if (!str1->buffer)return false;
        return true;
    }

    if (!str1 || !str1->buffer || *str1->buffer == '\0')return false;

    return strstr(str1->buffer, str2->buffer) != NULL;
}

// finished
bool containsFrom(const string str, const char* ch) {
    if (!ch)return str != NULL;
    if (*ch == '\0') {
        if (!str)return false;
        if (!str->buffer)return false;
        return true;
    }

    if (!str || !str->buffer || *str->buffer == '\0')return false;

    return strstr(str->buffer, ch) != NULL;
}

/* return new string */
// finished
string reverseString(const string str) {
    if (!str)return NULL;
    if (!str->buffer)return newStringFrom(NULL);
    if (*str->buffer == '\0')return newStringFrom("");

    size_t len = strlen(str->buffer);

    string temp = (string)malloc(sizeof(String));
    if (!temp) return NULL;

    temp->buffer = (char*)malloc(len + 1);
    if (!temp->buffer) {
        temp->buffer = NULL;
        temp->status = NULLPOINTER;
        return temp;
    }

    // 复制原字符串
    memcpy(temp->buffer, str->buffer, len + 1);

    // 反转：交换首尾
    for (size_t i = 0; i < len / 2; i++) {
        char c = temp->buffer[i];
        temp->buffer[i] = temp->buffer[len - 1 - i];
        temp->buffer[len - 1 - i] = c;
    }

    temp->status = AVAILUABLE;
    return temp;
}

/* return new string */
// finish
string* splitString(const string str, char delimiter, int* count) {
    if (!str || !str->buffer || !count) return NULL;

    char* buf = str->buffer;
    size_t len = strlen(buf);

    // 特殊情况：空字符串 -> 返回包含一个空串的数组
    if (len == 0) {
        string* arr = (string*)malloc(sizeof(string));
        if (!arr) {
            *count = 0;
            return NULL;
        }

        arr[0] = newString();  // 空串对象
        if (!arr[0]) {
            free(arr);
            return NULL;
        }
        *count = 1;
        return arr;
    }

    // 第一遍遍历：统计分隔符个数，确定子串数量
    int delim_count = 0;
    for (size_t i = 0; i < len; i++) {
        if (buf[i] == delimiter) delim_count++;
    }
    int token_count = delim_count + 1;
    *count = token_count;

    // 分配结果数组（指针数组）
    string* arr = (string*)malloc(token_count * sizeof(string));
    if (!arr) {
        *count = 0;
        return NULL;
    }

    // 第二遍遍历：切分并填充数组
    int idx = 0;
    size_t start = 0;
    for (size_t i = 0; i <= len; i++) {
        if (buf[i] == delimiter || buf[i] == '\0') {
            size_t token_len = i - start;

            // 提取子串到临时C字符串
            char* token_buf = (char*)malloc(token_len + 1);
            if (!token_buf) {
                // 内存失败，清理已分配的所有对象
                for (int j = 0; j < idx; j++) {
                    deleteString(arr[j]);
                }
                free(arr);
                *count = 0;
                return NULL;
            }
            memcpy(token_buf, buf + start, token_len);
            token_buf[token_len] = '\0';

            // 创建新的String对象
            arr[idx] = newStringFrom(token_buf);
            free(token_buf);

            if (!arr[idx] || arr[idx]->status != AVAILUABLE) {
                // 创建失败，清理
                for (int j = 0; j < idx; j++) {
                    if (arr[j]) deleteString(arr[j]);
                }
                free(arr);
                *count = 0;
                return NULL;
            }

            idx++;
            start = i + 1;
        }
    }

    return arr;
}

// finish
const char* statusToString(int status) {
    switch (status) {
    case -1: {
        return "nullwholestring";
    }
    case 0: {
        return "available";
    }
    case 1: {
        return "null pointer";
    }
    case 2: {
        return "failed to realloc";
    }
    case 3: {
        return "failed to append";
    }
    }
}

/* return new string */
string nextLine() {
    // 1. 分配 String 结构体
    string str = (string)malloc(sizeof(String));
    if (!str) {
        // 如果连结构体都分配不了，只能返回 NULL（这也符合你的“返回NULL”风格）
        return NULL;
    }

    // 2. 初始分配 buffer（使用 calloc，自动清零）
    str->buffer = (char*)calloc(INIT_STRING_SIZE, sizeof(char));
    if (!str->buffer) {
        // 初始化失败：置状态为 NULLPOINTER，buffer 为 NULL，返回 str
        str->status = NULLPOINTER;
        str->buffer = NULL;
        return str;
    }

    size_t capacity = INIT_STRING_SIZE;
    size_t len = 0;
    int ch;

    // 3. 逐字符读取
    while ((ch = getchar()) != EOF && ch != '\n') {
        // 检查是否需要扩容（len+1 是留给 '\0' 的位置）
        if (len + 1 >= capacity) {
            size_t new_capacity = capacity * GROWTH;
            char* new_buffer = (char*)realloc(str->buffer, new_capacity);
            if (!new_buffer) {
                // 扩容失败：释放旧 buffer，置 NULL，状态设为 FAILEDTOREALLOC，返回 str
                free(str->buffer);
                str->buffer = NULL;
                str->status = FAILEDTOREALLOC;
                return str;
            }
            str->buffer = new_buffer;
            capacity = new_capacity;
        }
        str->buffer[len++] = (char)ch;
    }

    // 4. 添加结尾 '\0'
    if (len + 1 <= capacity) {
        str->buffer[len] = '\0';
    }
    else {
        // 理论上不会发生（因为每次扩容都保证了 len+1 <= capacity）
        // 但为了保险，强行截断
        str->buffer[capacity - 1] = '\0';
    }
    str->status = AVAILUABLE;

    return str;
}
