#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

/*
 * Вывод массива значений яркости пикселей на экран.
 *
 * @param img - двумерный массив
 * @param n - размерность массива
 */
void printArr(int** img, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << img[i][j] << "\t";
        }
        cout << "\n";
    }
}

/*
 * Освобождение памяти, выделенной под массив
 *
 * @param img - двумерный массив
 * @param n - размерность массива
 */
void freeArr(int** img, int n) {
    for (int i = 0; i < n; i++) {
        delete[] img[i];
    }
    delete[] img;
}

/*
 * Инверсия изображения
 *
 * @param img - двумерный массив 
 * @param n - размерность массива
 */
void invertImage(int** img, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            img[i][j] = 255 - img[i][j];
        }
    }

    cout << "\nInverted image:\n";
    printArr(img, n);
}

/*
 * Бинаризация по порогу
 *
 * @param img - двумерный массив 
 * @param n - размерность массива
 */
void binarizeImage(int** img, int n) {
    int threshold;
    cout << "Enter threshold (0-255): ";
    cin >> threshold;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            img[i][j] = (img[i][j] > threshold) ? 1 : 0;
        }
    }

    cout << "\nBinarized image:\n";
    printArr(img, n);
}

/*
 * Поиск строки с максимальной суммой значений
 *
 * @param img - двумерный массив 
 * @param n - размерность массива
 */
void findBrightestRow(int** img, int n) {
    int best_row = 0;
    int best_sum = -1;

    for (int i = 0; i < n; i++) {
        int row_sum = 0;
        for (int j = 0; j < n; j++) {
            row_sum += img[i][j];
        }
        if (row_sum > best_sum) {
            best_sum = row_sum;
            best_row = i;
        }
    }
    cout << "Brightest row: " << best_row << "\n";
}

/*
 * Передача массива через void*, преобразование к int* и вывод
 * 
 * @param ptr - указатель на тип void
 * @param n - размерность массива
 */
void printViaVoid(void* ptr, int n) {
    int* flat = (int*)ptr;

    cout << "\nImage received via void*:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << flat[i * n + j] << "\t";
        }
        cout << "\n";
    }
}


/*
 * Вывод меню и чтение выбора пользователя
 *
 * @return номер выбора пользователя
 */
int askChoice() {
    cout << "\nChoose an action:\n";
    cout << "1 - Invert image\n";
    cout << "2 - Binarize by threshold\n";
    cout << "3 - Find brightest row\n";
    cout << "4 - Print array via void*\n";
    cout << "Your choice: ";

    int choice;
    cin >> choice;
    return choice;
}

int main() {
    int n;
    cout << "Enter N: ";
    cin >> n;
    if (n < 1) {
        cout << "N must be positive.\n";
        return 1;
    }

    int** img = new int*[n];
    for (int i = 0; i < n; i++) {
        img[i] = new int[n];
    }

    srand(time(0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            img[i][j] = rand() % 256;
        }
    }

    cout << "\nGenerated image:\n";
    printArr(img, n);

    int choice = askChoice();

    if (choice == 1) {
        invertImage(img, n);
    }
    else if (choice == 2) {
        binarizeImage(img, n);
    }
    else if (choice == 3) {
        findBrightestRow(img, n);
    }
    else if (choice == 4) {
        int* flat = new int[n * n];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                flat[i * n + j] = img[i][j];
            }
        }

        printViaVoid((void*)flat, n);

        delete[] flat;
    }
    else {
        cout << "Unknown choice.\n";
        freeArr(img, n);
        return 1;
    }

    freeArr(img, n);
    return 0;
}