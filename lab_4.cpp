#include <iostream>
#include <string>

using namespace std;


struct Payment {
    string sender;
    string receiver;
    double amount;
};


struct Transfer {
    string from_account;
    string to_account;
    double amount;
};


struct Exchange {
    string currency_from;
    string currency_to;
    double amount_from;
    double amount_to;
};


enum class TransactionType {
    PAYMENT,
    TRANSFER,
    EXCHANGE
};


class Transaction {
public:
    void *transaction_data;
    TransactionType type;
    bool is_completed;

    /*
     * Конструктор по умолчанию.
     */
    Transaction() {
        transaction_data = nullptr;
        type = TransactionType::PAYMENT;
        is_completed = false;
    }

    /*
     * Конструктор с параметрами.
     *
     * @param data - указатель на структуру данных транзакции
     * @param type - тип транзакции
     * @param is_completed - завершена ли транзакция
     */
    Transaction(void *data, TransactionType type, bool is_completed) {
        this->transaction_data = data;
        this->type = type;
        this->is_completed = is_completed;
    }

    /*
     * Конструктор копирования.
     *
     * @param other - копируемая транзакция
     */
    Transaction(const Transaction &other) {
        this->type = other.type;
        this->is_completed = other.is_completed;

        if (other.transaction_data == nullptr) {
            this->transaction_data = nullptr;
            return;
        }

        switch (this->type) {
        case TransactionType::PAYMENT:
            this->transaction_data = new Payment(*(Payment *)other.transaction_data);
            break;
        case TransactionType::TRANSFER:
            this->transaction_data = new Transfer(*(Transfer *)other.transaction_data);
            break;
        case TransactionType::EXCHANGE:
            this->transaction_data = new Exchange(*(Exchange *)other.transaction_data);
            break;
        }
    }

    /*
     * Конструктор перемещения.
     *
     * @param other - перемещаемая транзакция
     */
    Transaction(Transaction &&other) {
        transaction_data = other.transaction_data;
        type = other.type;
        is_completed = other.is_completed;
        other.transaction_data = nullptr;
    }

    /*
     * Оператор копирующего присваивания.
     *
     * @param other - присваиваемая транзакция
     * @return ссылка на текущий объект
     */
    Transaction &operator=(const Transaction &other) {
        if (this == &other) {
            return *this;
        }

        if (transaction_data != nullptr) {
            if (type == TransactionType::PAYMENT) {
                delete (Payment *)transaction_data;
            }
            else if (type == TransactionType::TRANSFER) {
                delete (Transfer *)transaction_data;
            }
            else {
                delete (Exchange *)transaction_data;
            }
        }

        type = other.type;
        is_completed = other.is_completed;

        if (other.transaction_data == nullptr) {
            transaction_data = nullptr;
        }
        else if (type == TransactionType::PAYMENT) {
            transaction_data = new Payment(*(Payment *)other.transaction_data);
        }
        else if (type == TransactionType::TRANSFER) {
            transaction_data = new Transfer(*(Transfer *)other.transaction_data);
        }
        else {
            transaction_data = new Exchange(*(Exchange *)other.transaction_data);
        }

        return *this;
    }

    /*
     * Оператор перемещающего присваивания.
     *
     * @param other - перемещаемая транзакция
     * @return ссылка на текущий объект
     */
    Transaction &operator=(Transaction &&other) {
        if (this == &other) {
            return *this;
        }

        if (transaction_data != nullptr) {
            if (type == TransactionType::PAYMENT) {
                delete (Payment *)transaction_data;
            }
            else if (type == TransactionType::TRANSFER) {
                delete (Transfer *)transaction_data;
            }
            else {
                delete (Exchange *)transaction_data;
            }
        }

        transaction_data = other.transaction_data;
        type = other.type;
        is_completed = other.is_completed;
        other.transaction_data = nullptr;

        return *this;
    }

    /*
     * Деструктор.
     */
    ~Transaction() {
        if (transaction_data != nullptr) {
            switch (type) {
            case TransactionType::PAYMENT:
                delete (Payment *)transaction_data;
                break;

            case TransactionType::EXCHANGE:
                delete (Exchange *)transaction_data;
                break;

            case TransactionType::TRANSFER:
                delete (Transfer *)transaction_data;
                break;
            }
        }
    }
};



class TransactionProcessor {
    public:
        /*
        * Конструктор по умолчанию.
        */
        TransactionProcessor() {
            transactions_ = nullptr;
            size_ = 0;
        }

        /*
        * Конструктор с параметрами.
        *
        * @param capacity - начальное количество транзакций
        */
        TransactionProcessor(int capacity) {
            transactions_ = nullptr;
            size_ = 0;

            if (capacity > 0) {
                transactions_ = new Transaction[capacity];
                size_ = capacity;
            }
        }

        /*
        * Конструктор копирования.
        *
        * @param other - копируемый объект
        */
        TransactionProcessor(const TransactionProcessor &other) {
            transactions_ = nullptr;
            size_ = 0;

            if (other.size_ > 0) {
                transactions_ = new Transaction[other.size_];
                for (int i = 0; i < other.size_; i++) {
                    transactions_[i] = other.transactions_[i];
                }
                size_ = other.size_;
            }
        }

        /*
        * Оператор копирующего присваивания.
        *
        * @param other - присваиваемый объект
        * @return ссылка на текущий объект
        */
        TransactionProcessor &operator=(const TransactionProcessor &other) {
            if (this == &other) {
                return *this;
            }

            delete[] transactions_;
            transactions_ = nullptr;
            size_ = 0;

            if (other.size_ > 0) {
                transactions_ = new Transaction[other.size_];
                for (int i = 0; i < other.size_; i++) {
                    transactions_[i] = other.transactions_[i];
                }
                size_ = other.size_;
            }

            return *this;
        }

        /*
        * Конструктор перемещения. Забирает массив у источника.
        *
        * @param other - перемещаемый процессор
        */
        TransactionProcessor(TransactionProcessor &&other) {
            transactions_ = other.transactions_;
            size_ = other.size_;

            other.transactions_ = nullptr;
            other.size_ = 0;
        }

        /*
        * Оператор перемещающего присваивания.
        *
        * @param other - перемещаемый объект
        * @return ссылка на текущий объект
        */
        TransactionProcessor &operator=(TransactionProcessor &&other) {
            if (this == &other) {
                return *this;
            }

            delete[] transactions_;

            transactions_ = other.transactions_;
            size_ = other.size_;

            other.transactions_ = nullptr;
            other.size_ = 0;

            return *this;
        }

        /*
        * Деструктор.
        */
        ~TransactionProcessor() {
            delete[] transactions_;
        }

        /*
        * Добавление транзакции в массив.
        *
        * @param transaction - добавляемая транзакция
        */
        void addTransaction(const Transaction &transaction) {
            Transaction *new_arr = new Transaction[size_ + 1];

            for (int i = 0; i < size_; i++) {
                new_arr[i] = transactions_[i];
            }

            delete[] transactions_;
            transactions_ = new_arr;

            transactions_[size_] = transaction;
            size_++;
        }

        /*
        * Обработка одной транзакции.
        *
        * @param index - индекс транзакции
        */
        void processTransaction(int index) {
            if (index < 0 || index >= size_) {
                cout << "Invalid index\n";
                return;
            }
            transactions_[index].is_completed = true;
        }

        /*
        * Вывод отчёта по всем транзакциям.
        */
        void printReport() const {
            cout << "\nTransaction Report\n";
            if (size_ == 0) {
                cout << "(no transactions)\n";
                return;
            }

            for (int i = 0; i < size_; i++) {
                cout << i << ": ";
                printOne(i);
            }
        }

        /*
        * Поиск транзакций по типу.
        *
        * @param type - искомый тип транзакции
        */
        void findByType(TransactionType type) const {
            cout << "\nSearch by type\n";
            bool found = false;
            for (int i = 0; i < size_; i++) {
                if (transactions_[i].type == type) {
                    cout << i << ": ";
                    printOne(i);
                    found = true;
                }
            }
            if (!found) {
                cout << "(none)\n";
            }
        }

        /*
        * Статистика: сколько транзакций каждого типа завершено.
        */
        void printStatistics() const {
            int completed[3] = {0, 0, 0};
            int total[3] = {0, 0, 0};

            for (int i = 0; i < size_; i++) {
                int idx = (int)transactions_[i].type;
                total[idx]++;
                if (transactions_[i].is_completed) completed[idx]++;
            }

            cout << "\nStatistics\n";
            cout << "PAYMENT:  " << completed[0] << " / " << total[0] << "\n";
            cout << "TRANSFER: " << completed[1] << " / " << total[1] << "\n";
            cout << "EXCHANGE: " << completed[2] << " / " << total[2] << "\n";
        }

    private:
        Transaction *transactions_;
        int size_;

        /*
        * Вывод информации по одной транзакции.
        *
        * @param i - индекс транзакции
        */
        void printOne(int i) const {
            if (transactions_[i].transaction_data == nullptr) {
                cout << "(no data)\n";
            }
            else if (transactions_[i].type == TransactionType::PAYMENT) {
                Payment *p = (Payment *)transactions_[i].transaction_data;
                cout << "[PAYMENT] "
                    << (transactions_[i].is_completed ? "completed" : "pending")
                    << " | " << p->sender << " -> " << p->receiver
                    << " : " << p->amount << "\n";
            }
            else if (transactions_[i].type == TransactionType::TRANSFER) {
                Transfer *t = (Transfer *)transactions_[i].transaction_data;
                cout << "[TRANSFER] "
                    << (transactions_[i].is_completed ? "completed" : "pending")
                    << " | " << t->from_account << " -> " << t->to_account
                    << " : " << t->amount << "\n";
            }
            else {
                Exchange *e = (Exchange *)transactions_[i].transaction_data;
                cout << "[EXCHANGE] "
                    << (transactions_[i].is_completed ? "completed" : "pending")
                    << " | " << e->amount_from << " " << e->currency_from
                    << " -> " << e->amount_to << " " << e->currency_to << "\n";
            }
        }
};


/*
 * Добавление транзакции.
 *
 * @param processor - ссылка на обработчик транзакций
 */
void addTransaction(TransactionProcessor &processor) {
    cout << "Choose type: 0 - PAYMENT, 1 - TRANSFER, 2 - EXCHANGE\n";
    cout << "Your choice: ";
    int t;
    cin >> t;

    if (t == 0) {
        Payment *p = new Payment;
        cout << "Sender: ";
        cin >> p->sender;
        cout << "Receiver: ";
        cin >> p->receiver;
        cout << "Amount: ";
        cin >> p->amount;
        processor.addTransaction(Transaction(p, TransactionType::PAYMENT, false));
    }
    else if (t == 1) {
        Transfer *tr = new Transfer;
        cout << "From account: ";
        cin >> tr->from_account;
        cout << "To account: ";
        cin >> tr->to_account;
        cout << "Amount: ";
        cin >> tr->amount;
        processor.addTransaction(Transaction(tr, TransactionType::TRANSFER, false));
    }
    else {
        Exchange *e = new Exchange;
        cout << "Currency from (RUB or USD): ";
        cin >> e->currency_from;
        cout << "Amount from: ";
        cin >> e->amount_from;

        if (e->currency_from == "RUB") {
            e->currency_to = "USD";
            e->amount_to = e->amount_from / 90.0;
        }
        else {
            e->currency_to = "RUB";
            e->amount_to = e->amount_from * 90.0;
        }

        processor.addTransaction(Transaction(e, TransactionType::EXCHANGE, false));
    }
}


/*
 * Запрос типа транзакции у пользователя для поиска по типу.
 *
 * @return выбранный тип
 */
TransactionType askType() {
    cout << "Choose type: 0 - PAYMENT, 1 - TRANSFER, 2 - EXCHANGE\n";
    cout << "Your choice: ";
    int t;
    cin >> t;

    if (t == 0) return TransactionType::PAYMENT;
    if (t == 1) return TransactionType::TRANSFER;
    return TransactionType::EXCHANGE;
}


/*
 * Вывод меню и чтение выбора пользователя.
 *
 * @return номер выбора
 */
int askChoice() {
    cout << "\nMenu\n";
    cout << "1 - Add transaction\n";
    cout << "2 - Process transaction\n";
    cout << "3 - Find by type\n";
    cout << "4 - Print report\n";
    cout << "5 - Print statistics\n";
    cout << "0 - Exit\n";
    cout << "Your choice: ";

    int choice;
    cin >> choice;
    return choice;
}


/*
 * Точка входа. Меню для работы с транзакциями.
 *
 * @return 0 при успешном завершении
 */
int main() {
    TransactionProcessor processor;

    while (true) {
        int choice = askChoice();

        if (choice == 0) {
            break;
        }
        else if (choice == 1) {
            addTransaction(processor);
        }
        else if (choice == 2) {
            int index;
            cout << "Enter index: ";
            cin >> index;
            processor.processTransaction(index);
        }
        else if (choice == 3) {
            TransactionType type = askType();
            processor.findByType(type);
        }
        else if (choice == 4) {
            processor.printReport();
        }
        else if (choice == 5) {
            processor.printStatistics();
        }
        else {
            cout << "Unknown choice\n";
        }
    }

    return 0;
}