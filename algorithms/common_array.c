//
// Created by Valentyn Valentiev on 2/18/2023.
//

#include "common_array.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>
#include "../settings.h"

static int ***array3d_pointer = NULL;
static int *vector_pointer = NULL;
static int P = 0;
static int M = 0;
static int N = 0;

/**
 * Отримання розмірів трьохвимірного масиву.
 * @param C Р - Кількість перерізів\n
 *          М - Кількість рядків\n
 *          N - Кількість стовпчиків\n
 * @return Повертає обраний вимір трьохвимірного масиву.
 */
int GetDimension(char C) {
    switch (C) {
        case 'P':
            return P;
        case 'M':
            return M;
        case 'N':
            return N;
        default:
            return -1;
    }
}

/**
 * Виділення пам'яті на вектор
 * @param n - довжина вектора.
 */
void AllocateVector(int n) {
    srand(time(NULL));
#ifndef SUP_DEBUG
    printf("Starting common vector allocation...\n");
#endif
    N = n;

    vector_pointer = (int *) malloc(N * sizeof(int));
    if (vector_pointer == NULL) {
        printf("\n\nFATAL: common vector allocation failure (P)!\n\n");
        exit(errno);
    }

#ifndef SUP_DEBUG
    printf("\t-Resource common vector allocated successfully.\n");
#endif
}

/**
 * Виділення пам'яті на трьохвимірний масив заданого розміру.
 * У випадку помилки виділення пам'яті - аварійне завершення програми з кодом 8.
 * @param p - Кількість перерізів
 * @param m - Кількість рядків
 * @param n - Кількість стовпчиків
 */
void Allocate3DArray(int p, int m, int n) {

#ifndef SUP_DEBUG
    printf("Starting common 3DArray allocation...\n");
#endif

    P = p;
    M = m;
    N = n;

    array3d_pointer = (int ***) malloc(P * sizeof(int **));
    if (array3d_pointer == NULL) {
        printf("\n\nFATAL: common 3d array allocation failure (P)!\n\n");
        exit(errno);
    }

    for (int k = 0; k < P; k++) {

        array3d_pointer[k] = (int **) malloc(M * sizeof(int *));

        if (array3d_pointer[k] == NULL) {
            printf("\n\nFATAL: common 3d array allocation failure (%d : M)!\n\n", k);
            exit(errno);
        }

        for (int i = 0; i < M; i++) {
            array3d_pointer[k][i] = (int *) malloc(N * sizeof(int));

            if (array3d_pointer[k][i] == NULL) {
                printf("\n\nFATAL: common 3d array allocation failure (%d : %d : N)!\n\n", k, i);
                exit(errno);
            }
        }
    }

#ifndef SUP_DEBUG
    printf("\t-Resource common 3DArray allocation successful.\n");
#endif
}

/**
 * Отримання вказівника на трьохвимірний масив.
 * @return Вказівник на трьохвимірний масив.
 */
int ***GetPointer_3DArray() {
    return array3d_pointer;
}

int *GetPointer_Vector() {
    return vector_pointer;
}

/**
 * Звільнення пам'яті зайнятої трьохвимірним масивом.
 * У випадку помилки звільнення пам'яті - аварійне завершення програми.
 */
void Deallocate3DArray() {
    if (array3d_pointer != NULL) {
        for (int k = 0; k < P; k++) {
            for (int i = 0; i < M; i++) {
                free(array3d_pointer[k][i]);
            }
            free(array3d_pointer[k]);
        }
        free(array3d_pointer);

        array3d_pointer = NULL;
        P = 0;
        M = 0;
        N = 0;

#ifndef SUP_DEBUG
        printf("Resource common 3DArray deallocated.\n");
#endif

    } else {
        printf("FATAL: tried to deallocate NULL pointer, possible memory leak!");
        exit(EADDRNOTAVAIL);
    }
}

/**
 * Звільнення пам'яті зайнятої вектором.
 */
void DeallocateVector() {
    if (vector_pointer != NULL) {
        // Free the memory for the array
        free(vector_pointer);
        vector_pointer = NULL;
#ifndef SUP_DEBUG
        printf("Array deallocated.\n");
#endif
    } else {
        fprintf(stderr, "FATAL: tried to deallocate null pointer, possible memory leak!\n");
        exit(EADDRNOTAVAIL);
    }
}

/**
 * Заповнення трьохвимірного масиву.
 * @param sort випадок відсортованності (SORTED_CASE, RANDOM_CASE або BACK_SORTED_CASE).
 */
void Fill3DArray(int sort) {
    if (sort == SORTED_CASE) {
        int number = 0;
        for (int k = 0; k < P; k++)
            for (int j = 0; j < N; j++)
                for (int i = 0; i < M; i++)
                    array3d_pointer[k][i][j] = number++;
    } else if (sort == RANDOM_CASE) {
        for (int k = 0; k < P; k++)
            for (int j = 0; j < N; j++)
                for (int i = 0; i < M; i++)
                    array3d_pointer[k][i][j] = rand() % (2 * P * M * N);
    } else if (sort == BACK_SORTED_CASE) {
        int number = P * M * N;
        for (int k = 0; k < P; k++)
            for (int j = 0; j < N; j++)
                for (int i = 0; i < M; i++)
                    array3d_pointer[k][i][j] = number--;
    }
}

/**
 * Заповнення вектора.
 * @param sort - випадок відсортованості.
 */
void FillVector(int sort) {
    if (sort == SORTED_CASE) {
        int number = 0;
        for (int i = 0; i < N; i++)
            vector_pointer[i] = number++;
    } else if (sort == RANDOM_CASE) {
        for (int i = 0; i < N; i++)
            vector_pointer[i] = rand() % (2 * N);
    } else if (sort == BACK_SORTED_CASE) {
        int number = N;
        for (int i = 0; i < N; i++)
            vector_pointer[i] = number--;
    }
}

/**
 * Перевірка на відсортованість першого
 * рядка кожного перерізу за неспаданням.
 * У випадку помилки перевірки - аварійне завершення програми.
 * Так як особливого сенсу у цій функції немає вимкнути її виконання можна у settings.h
 */
void VerifySorting() {
#ifdef VERIFY_SORT

#ifndef SUP_DEBUG
    printf("Verify started: \n");
#endif //SUP_DEBUG

    bool isSorted = true;
    for (int p = 0; p < P; ++p) {
        for (int n = 0; n < N - 1; ++n) {
            if (array3d_pointer[p][0][n] > array3d_pointer[p][0][n + 1]) { //якщо попередній елемент більший за наступний - помилка сортування.
                isSorted = false;
                break;
            }
        }
    }

    if (isSorted) {
#ifndef SUP_DEBUG
        printf("\t-Verified successful.\n");
#endif //SUP_DEBUG
    } else {
        printf("\n\nFATAL: sorting check failure!");
        exit(ENOMSG);
    }

#endif //VERIFY_SORT
}

void VerifySortingVector() {
#ifdef VERIFY_SORT

#ifndef SUP_DEBUG
    printf("Verify started: \n");
#endif //SUP_DEBUG

    bool isSorted = true;
    for (int i = 0; i < N - 1; ++i) {
        if (vector_pointer[i] > vector_pointer[i + 1]) { //if previous element is greater than the next - sorting failure.
            isSorted = false;
            break;
        }
    }

    if (isSorted) {
#ifndef SUP_DEBUG
        printf("\t-Verified successful.\n");
#endif //SUP_DEBUG
    } else {
        printf("\n\nFATAL: sorting check failure!");
        exit(ENOMSG);
    }

#endif //VERIFY_SORT
}