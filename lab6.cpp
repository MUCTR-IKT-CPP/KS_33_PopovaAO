#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <windows.h>

using namespace std;

/*
 * Абстрактный базовый класс банковского счета.
 */
class Account {
    protected:
        double balance;

    public:
        Account() {
            balance = 0;
        }

        Account(double initial_balance) {
            this->balance = initial_balance;
        }

        virtual ~Account() = default;

        virtual void deposit(double amount) = 0;
        virtual void withdraw(double amount) = 0;

        virtual double getBalance() const {
            return balance;
        }
};

/*
 * Интерфейс для поддержки переводов между счетами.
 */
class ITransferable {
    public:
        virtual ~ITransferable() = default;

        virtual void transfer(Account* to, double amount) = 0;
};

/*
 * Сберегательный счет.
 */
class SavingsAccount : public Account, public ITransferable {
    private:
        double interest_rate;

    public:
        SavingsAccount() : Account() {
            interest_rate = 0;
        }

        SavingsAccount(double initial_balance, double interest_rate) : Account(initial_balance) {
            this->interest_rate = interest_rate;
        }

        void deposit(double amount) override {
            if (amount > 0) {
                balance += amount;
            }
        }

        void withdraw(double amount) override {
            if (amount > 0 && amount <= balance) {
                balance -= amount;
            } else {
                cout << "Недостаточно средств на сберегательном счете." << endl;
            }
        }

        void transfer(Account* to, double amount) override {
            if (amount > 0 && amount <= balance) {
                balance -= amount;
                to->deposit(amount);
            } else {
                cout << "Недостаточно средств для перевода." << endl;
            }
        }

        void addInterest() {
            balance += balance * interest_rate / 100.0;
        }

        double getInterestRate() const {
            return interest_rate;
        }
};

/*
 * Расчетный счет.
 */
class CheckingAccount : public Account, public ITransferable {
    private:
        double overdraft_limit;

    public:
        CheckingAccount() : Account() {
            overdraft_limit = 0;
        }

        CheckingAccount(double initial_balance, double overdraft_limit) : Account(initial_balance) {
            this->overdraft_limit = overdraft_limit;
        }

        void deposit(double amount) override {
            if (amount > 0) {
                balance += amount;
            }
        }

        void withdraw(double amount) override {
            if (amount > 0 && (balance - amount) >= -overdraft_limit) {
                balance -= amount;
            } else {
                cout << "Превышен лимит овердрафта." << endl;
            }
        }

        void transfer(Account* to, double amount) override {
            if (amount > 0 && (balance - amount) >= -overdraft_limit) {
                balance -= amount;
                to->deposit(amount);
            } else {
                cout << "Недостаточно средств для перевода." << endl;
            }
        }

        double getOverdraftLimit() const {
            return overdraft_limit;
        }
};

/*
 * Кредитный счет.
 */
class CreditAccount : public Account, public ITransferable {
    private:
        double credit_limit;

    public:
        CreditAccount() : Account() {
            credit_limit = 0;
        }

        CreditAccount(double initial_balance, double credit_limit) : Account(initial_balance) {
            this->credit_limit = credit_limit;
        }

        void deposit(double amount) override {
            if (amount > 0) {
                balance += amount;
            }
        }

        void withdraw(double amount) override {
            if (amount > 0 && (balance - amount) >= -credit_limit) {
                balance -= amount;
            } else {
                cout << "Превышен кредитный лимит." << endl;
            }
        }

        void transfer(Account* to, double amount) override {
            if (amount > 0 && (balance - amount) >= -credit_limit) {
                balance -= amount;
                to->deposit(amount);
            } else {
                cout << "Недостаточно средств для перевода." << endl;
            }
        }

        double getCreditLimit() const {
            return credit_limit;
        }
};

/*
 * Класс банка, агрегирующего счета.
 */
class Bank {
    private:
        static const int MAX_ACCOUNTS = 100;
        Account* accounts[MAX_ACCOUNTS];
        int accounts_count;

    public:
        Bank() {
            accounts_count = 0;
        }

        void addAccount(Account* acc) {
            if (accounts_count < MAX_ACCOUNTS) {
                accounts[accounts_count] = acc;
                accounts_count++;
            } else {
                cout << "Достигнуто максимальное число аккаунтов" << endl;
            }
        }

        void applyInterest() {
            for (int i = 0; i < accounts_count; i++) {
                SavingsAccount* savings = dynamic_cast<SavingsAccount*>(accounts[i]);
                if (savings != nullptr) {
                    savings->addInterest();
                }
            }
        }

        void printAllAccounts() const {
            cout << "\n=== Информация о счетах ===" << endl;
            for (int i = 0; i < accounts_count; i++) {
                cout << "Счет " << (i + 1) << ": баланс = " << accounts[i]->getBalance() << endl;
            }
        }

        Account* getAccount(int index) {
            if (index >= 0 && index < accounts_count) {
                return accounts[index];
            }
            return nullptr;
        }

        int getCount() const {
            return accounts_count;
        }
};

int main() {
    SetConsoleOutputCP(65001);
    srand(time(0));

    Bank bank;

    while (true) {
        cout << "\n=== Меню ===" << endl;
        cout << "1. Создать счет" << endl;
        cout << "2. Пополнить счет" << endl;
        cout << "3. Снять средства" << endl;
        cout << "4. Перевести средства" << endl;
        cout << "5. Вывести баланс" << endl;
        cout << "6. Вывести информацию по всем счетам" << endl;
        cout << "0. Выход" << endl;
        cout << "Ваш выбор: ";

        int choice;
        cin >> choice;

        if (choice == 0) {
            cout << "Выход из программы." << endl;
            break;
        }
        else if (choice == 1) {
            cout << "\nКакой счет создать?" << endl;
            cout << "1. Сберегательный (SavingsAccount)" << endl;
            cout << "2. Текущий (CheckingAccount)" << endl;
            cout << "3. Кредитный (CreditAccount)" << endl;
            cout << "Ваш выбор: ";
            
            int type;
            cin >> type;

            double initial_amount;
            cout << "\nВведите начальную сумму: ";
            cin >> initial_amount;

            if (type == 1) {
                double rate;
                cout << "Введите процентную ставку: ";
                cin >> rate;
                SavingsAccount* acc = new SavingsAccount(initial_amount, rate);
                bank.addAccount(acc);
            }
            else if (type == 2) {
                double overdraft;
                cout << "Введите лимит овердрафта: ";
                cin >> overdraft;
                CheckingAccount* acc = new CheckingAccount(initial_amount, overdraft);
                bank.addAccount(acc);
            }
            else if (type == 3) {
                double credit_limit;
                cout << "Введите кредитный лимит: ";
                cin >> credit_limit;
                CreditAccount* acc = new CreditAccount(initial_amount, credit_limit);
                bank.addAccount(acc);
            }
            else {
                cout << "Неверный тип счета." << endl;
                continue;
            }
            cout << "Счет успешно создан." << endl;
        }
        else if (choice == 2) {
            int acc_num;
            double amount;
            cout << "\nВведите номер счета (1-" << bank.getCount() << "): ";
            cin >> acc_num;
            
            if (acc_num < 1 || acc_num > bank.getCount()) {
                cout << "Неверный номер счета." << endl;
                continue;
            }
            
            cout << "\nВведите сумму для пополнения: ";
            cin >> amount;

            bank.getAccount(acc_num - 1)->deposit(amount);
            cout << "Счет пополнен." << endl;
        }
        else if (choice == 3) {
            int acc_num;
            double amount;
            cout << "Введите номер счета (1-" << bank.getCount() << "): ";
            cin >> acc_num;
            
            if (acc_num < 1 || acc_num > bank.getCount()) {
                cout << "Неверный номер счета." << endl;
                continue;
            }
            
            cout << "\nВведите сумму для снятия: ";
            cin >> amount;

            bank.getAccount(acc_num - 1)->withdraw(amount);
        }
        else if (choice == 4) {
            int from_num, to_num;
            double amount;
            cout << "\nВведите номер счета-отправителя: ";
            cin >> from_num;
            cout << "Введите номер счета-получателя: ";
            cin >> to_num;
            
            if (from_num < 1 || from_num > bank.getCount() || to_num < 1 || to_num > bank.getCount()) {
                cout << "Неверный номер счета." << endl;
                continue;
            }

            cout << "Введите сумму перевода: ";
            cin >> amount;

            ITransferable* transfer_acc = dynamic_cast<ITransferable*>(bank.getAccount(from_num - 1));
            if (transfer_acc != nullptr) {
                transfer_acc->transfer(bank.getAccount(to_num - 1), amount);
            } else {
                cout << "Этот счет не поддерживает переводы." << endl;
            }
        }
        else if (choice == 5) {
            int acc_num;
            cout << "\nВведите номер счета (1-" << bank.getCount() << "): ";
            cin >> acc_num;
            
            if (acc_num < 1 || acc_num > bank.getCount()) {
                cout << "Неверный номер счета." << endl;
                continue;
            }

            cout << "Баланс счета " << acc_num << ": " << bank.getAccount(acc_num - 1)->getBalance() << endl;
        }
        else if (choice == 6) {
            bank.printAllAccounts();
        }
        else {
            cout << "Неверный пункт меню." << endl;
        }
    }

    for (int i = 0; i < bank.getCount(); i++) {
        delete bank.getAccount(i);
    }

    return 0;
}