#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENTRIES 100
#define MAX_KEY_LENGTH 20
#define MAX_VALUE_LENGTH 20


typedef struct { // 定义键值对结构
    char key[MAX_KEY_LENGTH];
    char value[MAX_VALUE_LENGTH];
} KeyValuePair;

int main() {
    KeyValuePair pairs[MAX_ENTRIES]; //用于存储键值对
    int count = 0; //count为实际存储的键值对数量
    
    //打开并读取文件
    FILE* file = fopen("data.txt", "r"); //确保项目文件夹下有data.txt文件
    if (file == NULL) { //如果无法打开则退出
        printf("Could not open data.txt\n");
        return 1;
    }
    
    char line[50];
    //逐行读取文件内容
    while (fgets(line, sizeof(line), file) && count < MAX_ENTRIES) {
        char* colon = strchr(line, ':'); //查找冒号位置来分割键和值。//strchr函数用于在给定的C字符串Str中查找字符Val的第一次出现。它返回指向该字符所在位置的指针。如果未找到，则返回null。
        if (colon == NULL) {
            continue; //若没有冒号，则跳过这行，继续读取下一行
        }
        
        //分割键和值
        *colon = '\0'; // 将冒号替换为字符串结束符
        char* key = line;
        char* value = colon + 1;
        
        //去除值末尾的换行符
        char* newline = strchr(value, '\n'); //strchr函数用于在给定的C字符串Str中查找字符Val的第一次出现。它返回指向该字符所在位置的指针。如果未找到，则返回null。
        if (newline != NULL) {
            *newline = '\0';
        }
        
        //存储键值对
        strcpy(pairs[count].key, key);
        strcpy(pairs[count].value, value);
        count++;
    }
    
    fclose(file);
    
    
    //处理输入，即可实现输入键输出对应值
    char input[50];
    while (1) {
        printf("Please enter a key:");
        scanf("%s", input);
        
        if (strcmp(input, "Quit") == 0) {
            break;
        }
        
        int found = 0;
        for (int i = 0; i < count; i++) { //查找匹配的键
            if (strcmp(input, pairs[i].key) == 0) { //找到匹配键
                printf("%s\n", pairs[i].value); //输出对应值
                found = 1; //标记为找到
                break;
            }
        }
        
        if (!found) { //若未找到匹配键,则输出Error
            printf("Error\n");
        }
    }
    
    return 0;
}