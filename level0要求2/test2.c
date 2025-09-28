#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 50
#define MAX_KEY_LENGTH 10
#define MAX_VALUE_LENGTH 10
#define MAX_ENTRIES 100

// 定义结构体来存储键值对
typedef struct {
    char key[MAX_KEY_LENGTH]; //创建key和value数组，用于存储键和值
    char value[MAX_VALUE_LENGTH];
} KeyValuePair; //KeyValuePair是一个结构体，包含两个字符数组成员：key和value，分别用于储存键和值，其长度由 MAX_KEY_LENGTH 和 MAX_VALUE_LENGTH 常量定义。



// 解析文件,存储键值对
int parseFile(const char* filename, KeyValuePair* pairs, int* count) { // parseFile函数用于解析指定文件中的键值对，并将其存储在提供的数组中。它接受文件名、存储键值对的数组和一个指向整数的指针（用于存储键值对的数量）作为参数。
    FILE* file = fopen(filename, "r");
    if (file == NULL) { //检查文件是否成功打开,如果打开失败，打印错误信息
        printf("Error: Could not open  the file");
        return 0;
    }
    

    char line[MAX_LINE_LENGTH];
    *count = 0;
    
    while (fgets(line, sizeof(line), file)) { // 创建循环，fgets函数从文件中逐行读取文本，存储在line数组中，直到遇到换行符或文件结束符为止。
        char* colonPos = strchr(line, ':'); // 查找冒号位置,strchr函数用于在字符串中查找第一个出现的冒号':'，并返回指向该字符的指针colonPos
        if (colonPos == NULL) { //如果未找到该字符，则返回NULL
            continue; // 如果没有冒号，跳过该行
        }
        
        //分割键和值
        *colonPos = '\0';
        char* key = line; //键key指向行的开始位置
        char* value = colonPos + 1; //值value指向冒号后的第一个字符
        
        //去除值中的换行符
        //当使用 fgets() 读取文件行时，每行的末尾会包含一个换行符\n,这段代码查找值中的换行符并将其替换为字符串结束符\0
        char* newlinePos = strchr(value, '\n');
        if (newlinePos != NULL) {
            *newlinePos = '\0';
        }
        
        //验证键和值的长度和字符
        if (strlen(key) > 0 && strlen(key) <= MAX_KEY_LENGTH && 
            strlen(value) > 0 && strlen(value) <= MAX_VALUE_LENGTH) {
            
            //检查键的有效性，是否只包含字母、数字和下划线
            //（根据题目要求，键只能包含这些字符，其他字符（如空格、标点等）都是无效的）
            //isalnum 函数用于判断一个字符是否属于字母（A-Z，a-z）或数字（0-9），返回值为非零值表示是字母或数字，返回0表示不是字母也不是数字
            //如果字符key[i]不是字母或数字(!isalnum(key[i]))，并且也不是下划线(key[i] != '_')，则说明该字符是无效的，则跳出循环
            int valid = 1;
            for (int i = 0; key[i]; i++) {
                if (!isalnum(key[i]) && key[i] != '_') {
                    valid = 0;
                    break;
                }
            }
            
            //存储有效的键值对
            if (valid) { //检查valid变量的值。如果valid为 1（真)，表示当前处理的键值对通过了所有验证（长度合适、字符合法），可以保存
                strcpy(pairs[*count].key, key); //strcpy函数用于将key字符串复制到pairs数组中当前索引位置的key成员中
                strcpy(pairs[*count].value, value); //value也是一样
                (*count)++; //将count指针指向的整数值加1，表示已经成功存储了一个新的键值对
            }
        }
        
        //检查是否达到最大条目数,若超出范围则停止读取
        if (*count >= MAX_ENTRIES) { //如果count的值大于等于MAX_ENTRIES，表示已经存储了足够多的键值对，达到了预设的最大限制,跳出循环，停止进一步读取文件
            break;
        }
    }
    
    fclose(file); //要记得关闭文件
    return 1;
}

int main() {
    KeyValuePair pairs[MAX_ENTRIES];
    int count; //count用于存储键值对的数量
    
    if (!parseFile("data.txt", pairs, &count)) { //调用parseFile函数解析data.txt文件中的键值对，并将其存储在pairs数组中。如果解析失败，程序将输出错误信息并返回1
        return 1;
    }

    printf("Loaded %d key-value pairs from data.txt\n", count); //输出读取的键值对数量
    
    char input[20]; //数组用于存储用户输入的键
    
    while (1) {
        printf("Enter a key: ");
        scanf("%s", input);
        
        
        if (strcmp(input, "Quit") == 0) { // 如果输入"Quit"，则退出循环
            break;
        }
        
        int found = 0; // found用于标记是否找到匹配的键
        for (int i = 0; i < count; i++) { // 遍历存储的键值对，查找与输入键匹配的值
            if (strcmp(input, pairs[i].key) == 0) { // strcmp函数用于比较两个字符串是否相等
                printf("%s\n", pairs[i].value); // 如果找到匹配的键，输出对应的值
                found = 1; // 标记为找到
                break;
            }
        }
        
        if (!found) { // 如果未找到匹配的键，输出Error
            printf("Error\n");
        }
    }
    
    return 0;
}