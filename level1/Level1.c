#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define FLOORS 5 //5层
#define ROWS 4  //4行
#define COLS 4  //4列
#define DAYS 7  //一个星期7天
#define MAX_USERS 27 //26个字母用户（A~Z）+ 1个管理员
#define FILENAME "library_data.dat"

//结构体，枚举
typedef enum { //日期结构体，枚举一个星期七天
    SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY
} Day;

typedef struct { //座位状态
    char status; //按要求，'0':空闲,'1':已预约,'A'-'Z':被对应用户预约（只有管理员可以看到）
} Seat;

typedef struct { //用户预约记录
    char reservations[50][50]; //存储预约信息的字符串
    int count;
} UserReservations;

typedef struct { //系统全局状态
    Seat seats[DAYS][FLOORS][ROWS][COLS]; //四维数组：天-层-行-列
    UserReservations users[MAX_USERS];    //用户预约记录
    char currentUser;                     //建立“当前登录用户”的数组，'\0'结束符表示未登录，'A'-'Z'表示用户，'Admin'表示管理员
} LibrarySystem;

LibrarySystem lib; //建立lib全局变量，保存系统状态

//初始化整个系统
//遍历多维数组使用四层嵌套循环
//使用四层嵌套的for循环来初始化四维数组，因为座位状态是一个四维数组（d天、f层、r行、c列）,需要分别用四个循环变量来遍历每一个座位，并将其状态设置为'0'（空闲),每一维都需要遍历，所以需要四层循环。
void initializeSystem() {
    for (int d = 0; d < DAYS; d++) {
        for (int f = 0; f < FLOORS; f++) {
            for (int r = 0; r < ROWS; r++) {
                for (int c = 0; c < COLS; c++) {
                    lib.seats[d][f][r][c].status = '0'; //初始化所有座位为空闲，均为'0',即status='0'
                }
            }
        }
    }
    
    //初始化用户预约记录
    for (int i = 0; i < MAX_USERS; i++) {
        lib.users[i].count = 0;
    }
    
    lib.currentUser = '\0'; //'\0'表明初始状态未登录
}


//Level1-1: 登录程序
int main() {
    char input[10];
    char username[10];
    printf("=== Library Seat Reservation System ===\n");
    printf("Input:");
    scanf("%s", input);
    if (strcasecmp(input, "Login") == 0) {
        printf("Please enter a username:");
        scanf("%s", username);
    
        if (strlen(username) == 1 && username[0] >= 'A' && username[0] <= 'Z') {
            lib.currentUser = username[0];
            printf("Login succeeded\n");
        } else if (strcasecmp(username, "Admin") == 0) {
            lib.currentUser = '&'; //为管理员设置特殊标记
            printf("Administrator login successful\n");
        } else {
            printf("Invalid username\n");
        }
    } else {
        printf("Please input 'Login' first\n");
    }

    
    return 0;
}




