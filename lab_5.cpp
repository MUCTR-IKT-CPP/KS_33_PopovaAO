#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <windows.h>

using namespace std;

const int CONC_FOR_BUILDING = 100;
const int BRICKS_FOR_BUILDING = 500;
const int WOOD_FOR_BUILDING = 80;

/*
 * Случайный коэффициент качества.
 */
float randomQualityCoef() {
    return 0.7 + (rand() % 61) / 100.0;
}

/*
 * Базовый класс строительного материала.
 * Инкапсулирует прочность, вес, цену, качество и коэффициент качества.
 */
class BuildingMaterial {
    protected:
        float strength;
        float mass;
        float price;
        int quality;
        float quality_coef;

    public:
        /* 
        * Конструктор по умолчанию.
        */
        BuildingMaterial() {
            strength = 0;
            mass = 0;
            price = 0;
            quality = 0;
            quality_coef = 1;
        }

        /*
        * Конструктор с параметрами.
        *
        * @param strength - прочность
        * @param mass - вес
        * @param price - цена за единицу
        * @param quality - качество (1-10)
        * @param quality_coef - коэффициент качества (0.7-1.3)
        */
        BuildingMaterial(float strength, float mass, float price, int quality, float quality_coef) {
            this->strength = strength;
            this->mass = mass;
            this->price = price;
            this->quality = quality;
            this->quality_coef = quality_coef;
        }

        /*
        * Виртуальный деструктор.
        */
        virtual ~BuildingMaterial() = default;


        virtual void use() = 0;
        virtual void checkQuality() = 0;

        float getStrength() const { return strength; }
        float getMass() const { return mass; }
        float getPrice() const { return price; }
        int  getQuality() const { return quality; }
        float getQualityCoef() const { return quality_coef; }

        void setStrength(float s) { strength = s; }
};


/*
 * Кирпич.
 */
class Brick : public BuildingMaterial {
private:
    string brand;
    float  strength_coef;

public:
    /*
     * Конструктор по умолчанию.
     */
    Brick() : BuildingMaterial() {
        brand = "";
        strength_coef = 1;
    }

    /*
     * Конструктор с параметрами.
     */
    Brick(string brand, float strength, float mass, float price, int quality, float strength_coef, float quality_coef)
        : BuildingMaterial(strength, mass, price, quality, quality_coef) {
        this->brand = brand;
        this->strength_coef = strength_coef;
    }


    void use() override {
        cout << "Марка кирпича: " << brand << "\n";
    }

    void checkQuality() override {
        cout << "Качество кирпича: " << quality << "\n";
    }

    float  getStrengthCoef() const { return strength_coef; }
    string getBrand() const { return brand; }
};


/*
 * Бетон.
 */
class Concrete : public BuildingMaterial {
private:
    string brand;
    float  strength_coef;

public:
    /*
     * Конструктор по умолчанию.
     */
    Concrete() : BuildingMaterial() {
        brand = "";
        strength_coef = 1;
    }

    /*
     * Конструктор с параметрами.
     */
    Concrete(string brand, float strength, float mass, float price, int quality,
             float strength_coef, float quality_coef)
        : BuildingMaterial(strength, mass, price, quality, quality_coef)
    {
        this->brand = brand;
        this->strength_coef = strength_coef;
    }


    void use() override {
        cout << "Марка бетона " << brand << "\n";
    }

    void checkQuality() override {
        cout << "Качество бетона: " << quality << "\n";
    }

    float  getStrengthCoef() const { return strength_coef; }
    string getBrand() const { return brand; }
};


/*
 * Дерево.
 */
class Wood : public BuildingMaterial {
private:
    string species;
    float  strength_coef;

public:
    /*
     * Конструктор по умолчанию.
     */
    Wood() : BuildingMaterial() {
        species = "";
        strength_coef = 1;
    }

    /*
     * Конструктор с параметрами.
     */
    Wood(string species, float strength, float mass, float price, int quality,
         float strength_coef, float quality_coef)
        : BuildingMaterial(strength, mass, price, quality, quality_coef)
    {
        this->species = species;
        this->strength_coef = strength_coef;
    }

    void use() override {
        cout << "Порода дерева " << species << "\n";
    }
    
    void checkQuality() override {
        cout << "Качество дерева: " << quality << "\n";
    }

    float  getStrengthCoef() const { return strength_coef; }
    string getSpecies() const { return species; }
};


/*
 * Случайное колебание цены (15%).
 */
float randomPriceCoef() {
    int fluct = 15;
    return 1.0 + (rand() % (2 * fluct + 1) - fluct) / 100.0;
}

/*
 * Случайное время доставки.
 */
int randomDeliveryDays() {
    int fluct = 5;
    return 1 + rand() % fluct;
}


/*
 * Класс строительного объекта.
 * Хранит материалы, считает массу, стоимость, прочность, сроки.
 */
class BuildingObject {
    private:
        static const int MAX_BRICKS = 2000;
        static const int MAX_CONCRETES = 500;
        static const int MAX_WOODS = 500;

        Brick bricks[MAX_BRICKS];
        int bricks_count;

        Concrete concretes[MAX_CONCRETES];
        int concretes_count;

        Wood woods[MAX_WOODS];
        int woods_count;

        int delivery_days;

    public:
    /*
     * Конструктор по умолчанию.
     */
    BuildingObject() {
        bricks_count = 0;
        concretes_count = 0;
        woods_count = 0;
        delivery_days = 0;
    }

    /*
     * Добавить кирпичи.
     */
    void addBricks(int count, float strength, float mass, float price, int quality, float strength_coef, string brand) {
        float batch_quality_coef = randomQualityCoef();
        for (int i = 0; i < count && bricks_count < MAX_BRICKS; i++) {
            bricks[bricks_count] = Brick{brand, strength, mass, price, quality, strength_coef, batch_quality_coef};
            bricks_count++;
        }
        int days = randomDeliveryDays();
        if (days > delivery_days) delivery_days = days;
    }

    /*
     * Добавить бетон.
     */
    void addConcretes(int count, float strength, float mass, float price, int quality, float strength_coef, string brand) {
        float batch_quality_coef = randomQualityCoef();
        for (int i = 0; i < count && concretes_count < MAX_CONCRETES; i++) {
            concretes[concretes_count] = Concrete{brand, strength, mass, price, quality, strength_coef, batch_quality_coef};
            concretes_count++;
        }
        int days = randomDeliveryDays();
        if (days > delivery_days) delivery_days = days;
    }

    /*
     * Добавить дерево.
     */
    void addWoods(int count, float strength, float mass, float price, int quality, float strength_coef, string species) {
        float batch_quality_coef = randomQualityCoef();
        for (int i = 0; i < count && woods_count < MAX_WOODS; i++) {
            woods[woods_count] = Wood{species, strength, mass, price, quality, strength_coef, batch_quality_coef};
            woods_count++;
        }
        int days = randomDeliveryDays();
        if (days > delivery_days) delivery_days = days;
    }

    /*
     * Общая масса.
     */
    float totalMass() const {
        float sum = 0;
        for (int i = 0; i < bricks_count; i++) sum += bricks[i].getMass();
        for (int i = 0; i < concretes_count; i++) sum += concretes[i].getMass();
        for (int i = 0; i < woods_count; i++) sum += woods[i].getMass();
        return sum;
    }

    /*
     * Общая стоимость с учётом колебаний цен.
     */
    float totalCost() const {
        float sum = 0;
        for (int i = 0; i < bricks_count; i++)
            sum += bricks[i].getPrice() * randomPriceCoef();
        for (int i = 0; i < concretes_count; i++)
            sum += concretes[i].getPrice() * randomPriceCoef();
        for (int i = 0; i < woods_count; i++)
            sum += woods[i].getPrice() * randomPriceCoef();
        return sum;
    }

    /*
     * Стоимость кирпичей.
     */
    float totalBricksCost() const {
        float sum = 0;
        for (int i = 0; i < bricks_count; i++)
            sum += bricks[i].getPrice() * randomPriceCoef();
        return sum;
    }

    /*
     * Стоимость бетона.
     */
    float totalConcretesCost() const {
        float sum = 0;
        for (int i = 0; i < concretes_count; i++)
            sum += concretes[i].getPrice() * randomPriceCoef();
        return sum;
    }

    /*
     * Стоимость дерева.
     */
    float totalWoodsCost() const {
        float sum = 0;
        for (int i = 0; i < woods_count; i++)
            sum += woods[i].getPrice() * randomPriceCoef();
        return sum;
    }

    /*
     * Среднее качество кирпичей.
     */
    float avgBrickQuality() const {
        if (bricks_count == 0) return 0;
        float sum = 0;
        for (int i = 0; i < bricks_count; i++) sum += bricks[i].getQuality();
        return sum / bricks_count;
    }

    /*
     * Среднее качество бетона.
     */
    float avgConcreteQuality() const {
        if (concretes_count == 0) return 0;
        float sum = 0;
        for (int i = 0; i < concretes_count; i++) sum += concretes[i].getQuality();
        return sum / concretes_count;
    }

    /*
     * Среднее качество дерева.
     */
    float avgWoodQuality() const {
        if (woods_count == 0) return 0;
        float sum = 0;
        for (int i = 0; i < woods_count; i++) sum += woods[i].getQuality();
        return sum / woods_count;
    }

    /*
     * Минимальная прочность.
     */
    float minStrength() const {
        float min_s = 1e9;
        for (int i = 0; i < bricks_count; i++) {
            float s = bricks[i].getStrength() * bricks[i].getStrengthCoef()
                    * bricks[i].getQualityCoef();
            if (s < min_s) min_s = s;
        }
        for (int i = 0; i < concretes_count; i++) {
            float s = concretes[i].getStrength() * concretes[i].getStrengthCoef()
                    * concretes[i].getQualityCoef();
            if (s < min_s) min_s = s;
        }
        for (int i = 0; i < woods_count; i++) {
            float s = woods[i].getStrength() * woods[i].getStrengthCoef()
                    * woods[i].getQualityCoef();
            if (s < min_s) min_s = s;
        }
        return min_s;
    }

    /*
     * Максимальная прочность.
     */
    float maxStrength() const {
        float max_s = 0;
        for (int i = 0; i < bricks_count; i++) {
            float s = bricks[i].getStrength() * bricks[i].getStrengthCoef() * bricks[i].getQualityCoef();
            if (s > max_s) max_s = s;
        }
        for (int i = 0; i < concretes_count; i++) {
            float s = concretes[i].getStrength() * concretes[i].getStrengthCoef() * concretes[i].getQualityCoef();
            if (s > max_s) max_s = s;
        }
        for (int i = 0; i < woods_count; i++) {
            float s = woods[i].getStrength() * woods[i].getStrengthCoef() * woods[i].getQualityCoef();
            if (s > max_s) max_s = s;
        }
        return max_s;
    }

    /*
     * Общая прочность.
     */
    float totalStrength() const {
        float sum = 0;
        for (int i = 0; i < bricks_count; i++)
            sum += bricks[i].getStrength() * bricks[i].getStrengthCoef() * bricks[i].getQualityCoef();
        for (int i = 0; i < concretes_count; i++)
            sum += concretes[i].getStrength() * concretes[i].getStrengthCoef() * concretes[i].getQualityCoef();
        for (int i = 0; i < woods_count; i++)
            sum += woods[i].getStrength() * woods[i].getStrengthCoef() * woods[i].getQualityCoef();
        return sum;
    }

    /*
     * Достаточно ли материалов.
     */
    bool hasEnoughBuildingMaterials() const {
        return concretes_count >= CONC_FOR_BUILDING
        && bricks_count >= BRICKS_FOR_BUILDING
        && woods_count >= WOOD_FOR_BUILDING;
    }

    /*
     * Можно ли строить при данном бюджете.
     */
    bool canBuild(float money) const {
        return hasEnoughBuildingMaterials() && totalCost() <= money;
    }

    /*
     * Детальный отчёт по каждому материалу и итоговые показатели.
     */
    void printReport() const {
        cout << "\n=== Отчёт ===\n";

        cout << "\n--- Кирпичи ---\n";
        cout << "Количество: " << bricks_count << "\n";
        cout << "Среднее качество: " << avgBrickQuality() << "\n";
        cout << "Стоимость: " << totalBricksCost() << "\n";

        cout << "\n--- Бетон ---\n";
        cout << "Количество: " << concretes_count << "\n";
        cout << "Среднее качество: " << avgConcreteQuality() << "\n";
        cout << "Стоимость: " << totalConcretesCost() << "\n";

        cout << "\n--- Дерево ---\n";
        cout << "Количество: " << woods_count << "\n";
        cout << "Среднее качество: " << avgWoodQuality() << "\n";
        cout << "Стоимость: " << totalWoodsCost() << "\n";

        cout << "\n--- Итоговые показатели ---\n";
        cout << "Общая масса: " << totalMass() << " кг\n";
        cout << "Общая стоимость: " << totalCost() << "\n";
        cout << "Мин. прочность: " << minStrength() << "\n";
        cout << "Макс. прочность: " << maxStrength() << "\n";
        cout << "Сроки строительства: " << delivery_days << " дней\n";
        cout << "Достаточно материалов: " << (hasEnoughBuildingMaterials() ? "да" : "нет") << "\n";
    }

    /*
    * Симуляция 10 лет эксплуатации.
    */
    void simulate10Years() const {
        cout << "\n=== Симуляция 10 лет ===\n";

        float avg_quality = (avgBrickQuality() + avgConcreteQuality() + avgWoodQuality()) / 3.0;
        float base_wear = 0.05 - (avg_quality - 1.0) * (0.04 / 9.0);

        float strengths[10];
        float strength = totalStrength();
        float max_s = strength;

        for (int year = 0; year < 10; year++) {
            float wear = base_wear;
            wear *= 1.5;
            strength *= (1 - wear);

            if ((year + 1) % 3 == 0) {
                strength *= 1.3;
                if (strength > max_s) strength = max_s;
            }

            strengths[year] = strength;
        }

        cout << "\nНачальная прочность: " << totalStrength() << "\n";
        cout << "Прочность через 10 лет: " << strengths[9] << "\n";
        cout << "Потеря прочности: " << (totalStrength() - strengths[9]) << "\n";
    }
};


int main() {
    SetConsoleOutputCP(65001);
    srand(time(0));

    BuildingObject object;

    object.addBricks(500, 10, 3, 15, 8, 1.2f, "M150");
    object.addConcretes(100, 20, 20, 25, 9, 1.3f, "M300");
    object.addWoods(80, 15, 5, 30, 7, 1.1f, "Сосна");

    while (true) {
        float budget;
        cout << "Введите бюджет: ";
        cin >> budget;

        object.printReport();

        if (object.canBuild(budget)) {
            cout << "\nСтроительство возможно.\n";
        } else {
            cout << "\nНедостаточно средств или материалов.\n";
        }

        object.simulate10Years();
    }

    return 0;
}
