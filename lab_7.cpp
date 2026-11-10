#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <iostream>

/*
 * Интерфейс метрики телеметрии
 */
class IMetric {
public:
    virtual std::string name() const = 0;
    virtual double value() const = 0;
    virtual std::string unit() const = 0;
    virtual ~IMetric() = default;
};

/*
 * Класс единицы измерения "секунды".
 */
class Seconds {
public:
    static std::string unitName() { return "s"; }
};

/*
 * Класс единицы измерения "миллисекунды".
 */
class Milliseconds {
public:
    static std::string unitName() { return "ms"; }
};

/*
 * Шаблонная метрика с параметрами типа значения и единицы измерения
 * @param T - тип хранимого значения
 * @param Unit - тип единицы измерения
 */
template <typename T, typename Unit>
class Metric : public IMetric {
private:
    T _val;
    std::string _metric_name;

public:
    Metric(std::string metric_name, T val)
        : _val(val), _metric_name(metric_name)  {}

    std::string name() const override {
        return _metric_name;
    }

    double value() const override {
        return static_cast<double>(_val);
    }

    std::string unit() const override {
        return Unit::unitName();
    }
};

/*
 * Агрегатор метрик телеметрии
 */
class Telemetry {
private:
    std::vector<std::unique_ptr<IMetric>> _metrics;

public:
    /*
     * Добавление метрики в агрегатор
     * @param metric - умный указатель на метрику
     */
    void add(std::unique_ptr<IMetric> metric) {
        _metrics.push_back(std::move(metric));
    }

    /*
     * Формирование строки отчёта по всем метрикам
     * @return строка отчёта
     */
    std::string report() const {
        std::ostringstream out;
        for (size_t i = 0; i < _metrics.size(); i++) {
            if (i != 0) {
                out << " ";
            }
            out << _metrics[i]->name() << "=" << _metrics[i]->value() << _metrics[i]->unit() ;
        }
        return out.str();
    }
};

/*
 * Политика отправки без сжатия
 */
class NoCompression {
public:
    static void send(const std::string& report) {
        std::cout << report << "\n";
    }
};

/*
 * Политика простого сжатия.
 */
class SimpleCompression {
public:
    /*
     * Сообщение о длине сжатого отчёта
     * @param report - cтрока отчёта
     */
    static void send(const std::string& report) {
        std::cout << report.size() / 2 << "\n";
    }
};

/*
 * Шаблонный отправитель телеметрии с заданной политикой
 * @param Policy - политика отправки
 */
template <typename Policy>
class TelemetrySender {
public:
    static void send(const Telemetry& telemetry) {
        Policy::send(telemetry.report());
    }
};

int main() {
    Telemetry telemetry;

    while (true) {
        std::cout << "\n=== Menu ===\n";
        std::cout << "1. Add metric\n";
        std::cout << "2. Send full report\n";
        std::cout << "3. Send compressed length\n";
        std::cout << "4. Exit\n";
        std::cout << "Choose: ";

        int choice = 0;
        std::cin >> choice;

        if (choice == 1) {
            std::cout << "Type of value (1 - int, 2 - double): ";
            int val_type = 0;
            std::cin >> val_type;

            if (val_type == 1) {
                std::cout << "Name: ";
                std::string metric_name;
                std::cin >> metric_name;

                std::cout << "Value: ";
                int metric_value = 0;
                std::cin >> metric_value;

                std::cout << "Unit (1 - Seconds, 2 - Milliseconds): ";
                int unit_choice = 0;
                std::cin >> unit_choice;

                if (unit_choice == 1) {
                    telemetry.add(std::unique_ptr<IMetric>(new Metric<int, Seconds>(metric_name, metric_value)));
                } 
                else if (unit_choice == 2) {
                    telemetry.add(std::unique_ptr<IMetric>(new Metric<int, Milliseconds>(metric_name, metric_value)));
                } 
                else {
                    std::cout << "Unknown unit\n";
                }                
            }

            else if (val_type == 2) {
                std::cout << "Name: ";
                std::string metric_name;
                std::cin >> metric_name;

                std::cout << "Value: ";
                double metric_value = 0.0;
                std::cin >> metric_value;

                std::cout << "Unit (1 - Seconds, 2 - Milliseconds): ";
                int unit_choice = 0;
                std::cin >> unit_choice;

                if (unit_choice == 1) {
                    telemetry.add(std::unique_ptr<IMetric>(new Metric<double, Seconds>(metric_name, metric_value)));
                } 
                else if (unit_choice == 2) {
                    telemetry.add(std::unique_ptr<IMetric>(new Metric<double, Milliseconds>(metric_name, metric_value)));
                } 
                else {
                    std::cout << "Unknown unit\n";
                }
            }

            else {
                    std::cout << "Unknown type\n";
                }
        } 

        else if (choice == 2) {
            TelemetrySender<NoCompression>::send(telemetry);
        } 

        else if (choice == 3) {
            TelemetrySender<SimpleCompression>::send(telemetry);
        } 

        else if (choice == 4) {
            break;
        } 

        else {
            std::cout << "Unknown choice\n";
        }
    }

    return 0;
}
