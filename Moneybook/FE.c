#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

Expense *list = NULL;

int main() {
    
    float choice;
    while (1) {

        printf("\n==== 支出紀錄 ====\n");
        printf("1 新增紀錄\n");
        printf("2 顯示紀錄\n");
        printf("0 離開\n");

        if (readFloat(&choice) == INPUT_BACK) continue;
        switch ((int)choice) {
            case 0:
                return 0;
                break;
            case 1:
                addExpense();
                break;
            case 2:
                showExpense();
                break;
            default:
                printf("Invalid option\n");
        }
    }

    return 0;
}