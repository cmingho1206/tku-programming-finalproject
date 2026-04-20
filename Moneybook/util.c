#include "util.h"
#include <string.h>

// 檔案處理
static void writeExpenseToCSV(Expense e) {
    FILE *fp = fopen(EXPENSE_DATA_FILE, "a");
    if (fp == NULL) {
        printf("Cannot open file\n");
        return;
    }
    fprintf(fp, "%s,%s,%.2f\n", e.date, e.shop, e.amount);
    fclose(fp);
}

// 指標應用
static int readExpense(FILE *fp, Expense *e) {
    return fscanf(fp, "%19[^,],%49[^,],%f\n",
                  e->date,
                  e->shop,
                  &e->amount) == 3;
}

int readFloat(float *value) {
    char buf[100];
    if (readString(buf, sizeof(buf)) == INPUT_BACK) return INPUT_BACK;
    *value = atof(buf);
    return 0;
}

int readString(char *buffer, int size) {
    if (fgets(buffer, size, stdin) == NULL) return INPUT_BACK;
    buffer[strcspn(buffer, "\n")] = 0;
    if (strcasecmp(buffer, "back") == 0) return INPUT_BACK;
    return 0;
}

void addExpense() {
    Expense e;
    printf("輸入日期(YYYY-MM-DD) : ");
    if (readString(e.date, sizeof(e.date)) == INPUT_BACK) return;
    printf("輸入商店名稱 : ");
    if (readString(e.shop, sizeof(e.shop)) == INPUT_BACK) return;
    printf("輸入金額 : ");
    if (readFloat(&e.amount) == INPUT_BACK) return;
    writeExpenseToCSV(e);
    printf("紀錄已新增\n");
}

// 記憶體動態配置
void showExpense() {
    FILE *fp = fopen(EXPENSE_DATA_FILE, "r");
    if (fp == NULL) {
        printf("No data file.\n");
        return;
    }
    int capacity = 10;
    int count = 0;
    Expense *list = malloc(sizeof(Expense) * capacity);
    Expense e;
    while (readExpense(fp, &e)) {
        if (count >= capacity) {
            capacity *= 2;
            list = realloc(list, sizeof(Expense) * capacity);
        }
        list[count++] = e;
    }
    fclose(fp);
    printf("\n%-15s %-20s %10s\n", "Date", "Shop", "Amount");
    printf("------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-15s %-20s %10.2f\n",
               list[i].date,
               list[i].shop,
               list[i].amount);
    }
    free(list);
}



