#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>

#define INPUT_BACK -1
#define EXPENSE_DATA_FILE "data/expenseData.csv"

// 結構資料
typedef struct {
    char date[20];
    char shop[50];
    float amount;
} Expense;

int readFloat(float *value);
int readString(char *buffer, int size);
void addExpense();
void showExpense();

#endif