//
// Created by Valentyn Valentiev on 2/18/2023.
//

#include "hybrid_select_1_exchange.h"
#include "common_array.h"

/**
 * Гібридний алгоритм "вибір№1 – обмін"
 * @return Час сортування clock_t.
 */
clock_t SortingHybrid_1_exchange() {
    // Підготовка сортування, виділення пам'яті
    clock_t start_measure, end_measure;
    int ***array_3d = GetPointer_3DArray();
    int p = GetDimension('P');
    int m = GetDimension('M');
    int n = GetDimension('N');

    //
    start_measure = clock();

    end_measure = clock();

    return end_measure - start_measure;
}

/**
 * Гібридний алгоритм "вибір№1 – обмін"
 * @return Час сортування clock_t.
 */
clock_t SortingHybrid_1_exchange_vector() {
    // Підготовка сортування, виділення пам'яті
    clock_t start_measure, end_measure;
    int ***array_3d = GetPointer_3DArray();
    int p = GetDimension('P');
    int m = GetDimension('M');
    int n = GetDimension('N');

    //
    start_measure = clock();

    end_measure = clock();

    return end_measure - start_measure;
}