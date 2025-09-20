#include <stdio.h>

#include <string.h> //为了比较两个字符串是否相等，需要使用字符串比较函数strcmp，它位于标准库<string.h>中
                    //C语言中，不能直接用==来比较两个字符串是否相等，因为==比较的是指针的地址，而不是字符串的内容

int main() {
    char input[20]; //定义一个字符数组，用于存储输入的字符串

    while (1) { //使用无限循环,直到遇到break退出
        printf("Please enter a string:");
        scanf("%s", input); 
        
        if (strcmp(input, "Dian") == 0) { //使用strcmp函数比较输入的字符串和"Dian"是否相等
            printf("2002\n");
        } else if (strcmp(input, "Quit") == 0) { //如果输入的是"Quit"，则退出循环
            break; 
        } else {
            printf("Error\n"); //如果输入的既不是"Dian"也不是"Quit"而是其他字符串，则输出"Error"
        }

    }


    return 0;
}