#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> //包含ctype.h头文件，以便使用isalnum函数
                   //isalnum函数是一个字符分类函数，用于检查一个字符是否为字母或数字

#define FLOORS 5
#define ROWS 4
#define COLS 4
#define DAYS 7
#define FILENAME "library_data.dat"

// 座位状态
typedef struct {
    char status; // '0':空闲, '1':已预约, '2':被当前用户预约
} Seat;

// 系统全局状态
typedef struct {
    Seat seats[DAYS][FLOORS][ROWS][COLS]; // 四维数组：天-层-行-列
    char currentUser; // 当前登录用户 '\0'表示未登录，'A'-'Z'表示用户
} LibrarySystem;

LibrarySystem lib;

void initializeSystem() {
    for (int d = 0; d < DAYS; d++) {
        for (int f = 0; f < FLOORS; f++) {
            for (int r = 0; r < ROWS; r++) {
                for (int c = 0; c < COLS; c++) {
                    lib.seats[d][f][r][c].status = '0';
                }
            }
        }
    }
    lib.currentUser = '\0';
}

void saveData() {
    FILE* file = fopen(FILENAME, "wb");
    if (file) {
        fwrite(&lib, sizeof(LibrarySystem), 1, file);
        fclose(file);
    }
}

void loadData() {
    FILE* file = fopen(FILENAME, "rb");
    if (file) {
        fread(&lib, sizeof(LibrarySystem), 1, file);
        fclose(file);
    } else {
        initializeSystem();
    }
}

void handleLogin() {
    char username[10];
    printf("Please enter a username: ");
    scanf("%s", username);
    
    if (strlen(username) == 1 && username[0] >= 'A' && username[0] <= 'Z') {
        lib.currentUser = username[0];
        printf("Login successful\n");
    } else {
        printf("Invalid username(Please enter a letter between A-Z)\n");
    }
}

int getDayIndex(const char* day) {
    const char* days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    for (int i = 0; i < DAYS; i++) {
        if (strcasecmp(day, days[i]) == 0) {
            return i;
        }
    }
    return -1;
}

void showSeats(int day, int floor) {
    const char* days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    printf("%s Floor %d seating situation:\n", days[day], floor + 1);
    
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            printf("%c", lib.seats[day][floor][r][c].status);
        }
        printf("\n");
    }
}

int reserveSeat(int day, int floor, int row, int col) {
    if (lib.seats[day][floor][row][col].status == '0') {
        lib.seats[day][floor][row][col].status = '2'; // 标记为被当前用户预约
        return 1;
    }
    return 0;
}

void showReservations() {
    if (lib.currentUser == '\0') {
        printf("Please log in first.\n");
        return;
    }
    
    const char* days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    printf("Your appointment record:\n");
    
    int found = 0;
    for (int d = 0; d < DAYS; d++) {
        for (int f = 0; f < FLOORS; f++) {
            for (int r = 0; r < ROWS; r++) {
                for (int c = 0; c < COLS; c++) {
                    if (lib.seats[d][f][r][c].status == '2') {
                        printf("%s Floor %d Seat %d %d\n", days[d], f + 1, r + 1, c + 1);
                        found = 1;
                    }
                }
            }
        }
    }
    
    if (!found) {
        printf("No appoitment record.\n");
    }
}

void clearData() {
    if (lib.currentUser != 'A') {
        printf("Only administrators can clear the data.\n");
        return;
    }
    
    initializeSystem();
    printf("All data has been cleared.\n");
}

void processCommand() {
    char command[20];
    printf("Please enter a command: ");
    scanf("%s", command);
    
    if (strcasecmp(command, "Login") == 0) {
        handleLogin();
    } else if (strcasecmp(command, "Exit") == 0) {
        lib.currentUser = '\0';
        printf("Logged out successfully\n");
    } else if (strcasecmp(command, "Quit") == 0) {
        saveData();
        printf("The program has exited.\n");
        exit(0);
    } else if (lib.currentUser == '\0') {
        printf("Please log in first\n");
    } else if (strcasecmp(command, "Reservation") == 0) {
        showReservations();
    } else if (strcasecmp(command, "Clear") == 0) {
        clearData();
    } else {
        int day = getDayIndex(command);
        if (day != -1) {
            char subcmd[10];
            scanf("%s", subcmd);
            
            if (strcasecmp(subcmd, "Floor") == 0) {
                int floor;
                scanf("%d", &floor);
                if (floor >= 1 && floor <= FLOORS) {
                    showSeats(day, floor - 1);
                } else {
                    printf("Invalid floor number (1-%d)\n", FLOORS);
                }
            } else {
                printf("Invalid command\n");
            }
        } else if (strcasecmp(command, "Reserve") == 0) {
            char dayStr[10], floorStr[10], seatStr[10];
            int floor, row, col;
            
            scanf("%s %s %d %s %d %d", dayStr, floorStr, &floor, seatStr, &row, &col);
            
            int day = getDayIndex(dayStr);
            if (day == -1) {
                printf("Invalid date\n");
                return;
            }
            
            if (floor < 1 || floor > FLOORS) {
                printf("Invalid floor number (1-%d)\n", FLOORS);
                return;
            }
            
            if (row < 1 || row > ROWS || col < 1 || col > COLS) {
                printf("Invalid seat number (行: 1-%d, 列: 1-%d)\n", ROWS, COLS);
                return;
            }
            
            if (reserveSeat(day, floor - 1, row - 1, col - 1)) {
                printf("Appointment successful\n");
            } else {
                printf("The seat has been reserved.\n");
            }
        } else {
            printf("Invalid command\n");
        }
    }
}

int main() {
    loadData();
    
    printf("=== Library Seat Reservation System ===\n");
    printf("Available commands:\n");
    printf("Login\n");
    printf("Exit - Log out\n");
    printf("Quit - Exit the program\n");
    printf("[Date] Floor [n] - Check the seat situation on a certain day on a certain floor.\n");
    printf("Reserve [Date] Floor [n] Seat [r] [c] - Reserve a seat\n");
    printf("Reservation - Check my appointment\n");
    printf("Clear - Clear all data (admin only)\n");
    printf("Date: Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday\n");
    
    while (1) {
        processCommand();
    }
    
    return 0;
}