#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

/*
 * Профиль пользователя социальной сети
 */
struct SocialMediaProfile {
    string username;
    int age;
    int number_of_friends;
    int registration_year;
    bool is_premium;
    int last_login;
};

/*
 * Вывод всех профилей на экран
 *
 * @param profiles - массив профилей
 * @param n - количество элементов
 */
void printProfiles(SocialMediaProfile* profiles, int n) {
    for (int i = 0; i < n; i++) {
        cout << profiles[i].username << "\n"
             << "age: " << profiles[i].age << "\n"
             << "friends: " << profiles[i].number_of_friends << "\n"
             << "year: " << profiles[i].registration_year << "\n"
             << "premium: " << (profiles[i].is_premium ? "yes" : "no") << "\n"
             << "last_login: " << profiles[i].last_login << " days\n\n";
    }
    cout << "\n";
}

/*
 * Генерация случайного имени пользователя
 *
 * @param index - индекс для уникальности имени
 * @return строка-имя пользователя
 */
string randomUsername(int index) {
    string names[] = {
        "Alice", "Bob", "Charlie", "Diana", "Eve",
        "Frank", "Grace", "Henry", "Ivy", "Jack",
        "Kate", "Liam", "Mia", "Noah", "Olivia",
        "Peter", "Quinn", "Rose", "Sam", "Tina"
    };
    return names[rand() % 20] + "_" + to_string(index);
}

/*
 * Выделение памяти под n профилей
 *
 * @param n - количество профилей
 * @return указатель на массив
 */
SocialMediaProfile* allocateProfiles(int n) {
    return new SocialMediaProfile[n];
}

/*
 * Освобождение памяти
 *
 * @param profiles - массив профилей
 */
void freeProfiles(SocialMediaProfile* profiles) {
    delete[] profiles;
}

/*
 * Заполнение массива случайными данными
 *
 * @param profiles - массив
 * @param n - количество
 */
void fillRandom(SocialMediaProfile* profiles, int n) {
    for (int i = 0; i < n; i++) {
        profiles[i].username = randomUsername(i);
        profiles[i].age = 14 + rand() % 70;
        profiles[i].number_of_friends = rand() % 1000;
        profiles[i].registration_year = 2010 + rand() % 16;
        profiles[i].is_premium = rand() % 2;
        profiles[i].last_login = rand() % 365;
    }
}

/*
 * Поиск пользователей, не заходивших более X дней
 *
 * @param profiles - массив профилей
 * @param n - размер массива
 */
void findNotActive(SocialMediaProfile* profiles, int n) {
    int x;
    cout << "Enter days X: ";
    cin >> x;

    cout << "\nUsers inactive more than " << x << " days:\n";
    bool found = false;
    for (int i = 0; i < n; i++) {
        if (profiles[i].last_login > x) {
            cout << profiles[i].username
                 << " (" << profiles[i].last_login << " days)\n";
            found = true;
        }
    }
    if (!found) {
        cout << "None\n";
    }
}

/*
 * Анализ аудитории: средний возраст, среднее число друзей, количество премиум-пользователей
 *
 * @param profiles - массив профилей
 * @param n - размер массива
 */
void analyzeAudience(SocialMediaProfile* profiles, int n) {
    int sum_age = 0;
    int sum_friends = 0;
    int premium_count = 0;

    for (int i = 0; i < n; i++) {
        sum_age += profiles[i].age;
        sum_friends += profiles[i].number_of_friends;
        if (profiles[i].is_premium) {
            premium_count++;
        }
    }

    double avg_age = (double)sum_age / n;
    int avg_friends = sum_friends / n;

    cout << "\nAudience analysis\n";
    cout << "Average age: " << avg_age << "\n";
    cout << "Average friends: " << avg_friends << "\n";
    cout << "Premium users: " << premium_count << "\n";
}

/*
 * Поиск пользователей, зарегистрированных не позднее указанного года, и их сортировка по количеству друзей
 *
 * @param profiles - массив профилей
 * @param n - размер массива
 */
void findOlds(SocialMediaProfile* profiles, int n) {
    int year;
    cout << "Enter year: ";
    cin >> year;

    int count = 0;
    for (int i = 0; i < n; i++) {
        if (profiles[i].registration_year <= year) {
            count++;
        }
    }

    if (count == 0) {
        cout << "\nNo users registered before " << year << "\n";
        return;
    }

    SocialMediaProfile* olds = new SocialMediaProfile[count];

    int idx = 0;
    for (int i = 0; i < n; i++) {
        if (profiles[i].registration_year <= year) {
            olds[idx] = profiles[i];
            idx++;
        }
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            if (olds[j].number_of_friends > olds[j + 1].number_of_friends) {
                SocialMediaProfile temp = olds[j];
                olds[j] = olds[j + 1];
                olds[j + 1] = temp;
            }
        }
    }

    cout << "\nOlds registered before " << year << "\n";
    for (int i = 0; i < count; i++) {
        cout << olds[i].username 
             << "   year: " << olds[i].registration_year
             << "   friends: " << olds[i].number_of_friends << "\n";
    }

    delete[] olds;
}

/*
 * Сортировка массива по имени пользователя без учёта регистра
 *
 * @param profiles - массив профилей
 * @param n - размер массива
 */
void sortByName(SocialMediaProfile* profiles, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {

            string a = profiles[j].username;
            string b = profiles[j + 1].username;

            for (int k = 0; k < (int)a.size(); k++) {
                a[k] = tolower(a[k]);
            }
            for (int k = 0; k < (int)b.size(); k++) {
                b[k] = tolower(b[k]);
            }

            if (a > b) {
                SocialMediaProfile temp = profiles[j];
                profiles[j] = profiles[j + 1];
                profiles[j + 1] = temp;
            }
        }
    }

    cout << "\nSorted by username \n";
    printProfiles(profiles, n);
}

/*
 * Имитация отправки сообщений пользователям, не заходившим более 30 дней
 *
 * @param profiles - массив профилей
 * @param n - размер массива
 */
void sendNotifications(SocialMediaProfile* profiles, int n) {
    cout << "\n Sending notifications \n";
    int sent = 0;
    for (int i = 0; i < n; i++) {
        if (profiles[i].last_login > 30) {
            cout << "Notification sent to " << profiles[i].username
                 << " (" << profiles[i].last_login << " days inactive)\n";
            sent++;
        }
    }
    cout << "Total notifications: " << sent << "\n";
}

/*
 * Вывод меню и чтение выбора пользователя.
 *
 * @return номер выбора
 */
int askChoice() {
    cout << "\nChoose an action:\n";
    cout << "1 - Find inactive users (more than X days)\n";
    cout << "2 - Audience analysis\n";
    cout << "3 - Find olds (registered before year)\n";
    cout << "4 - Sort by username\n";
    cout << "5 - Send notifications\n";
    cout << "0 - Exit\n";
    cout << "Your choice: ";

    int choice;
    cin >> choice;
    return choice;
}

int main() {
    srand(time(0));

    int n;
    cout << "Enter N: ";
    cin >> n;

    if (n < 1) {
        cout << "N must be positive.\n";
        return 1;
    }

    SocialMediaProfile* profiles = allocateProfiles(n);
    fillRandom(profiles, n);

    cout << "\nGenerated profiles:\n";
    printProfiles(profiles, n);

    while (true) {
        int choice = askChoice();

        if (choice == 0) {
            break;
        }
        else if (choice == 1) {
            findNotActive(profiles, n);
        }
        else if (choice == 2) {
            analyzeAudience(profiles, n);
        }
        else if (choice == 3) {
            findOlds(profiles, n);
        }
        else if (choice == 4) {
            sortByName(profiles, n);
        }
        else if (choice == 5) {
            sendNotifications(profiles, n);
        }
        else {
            cout << "Unknown choice.\n";
        }
    }

    freeProfiles(profiles);
    return 0;
}