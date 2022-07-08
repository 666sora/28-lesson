#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <thread>
#include <functional>
#include <mutex>
#include <chrono>


class Swimmer {
    std::string name = "";

    int timeSec = 0;

    float speedMS = 0;

    float distance = 0;

    std::thread driveway;

    static std::mutex mtx;

public:
    void setName() {
        std::cout << "Input name of swimmer: ";
        std::cin >> name;
    }
    std::string getName() {
        return name;
    }

    void setSpeedMS() {
        std::cout << "Input speed in meter per second: ";
        std::cin >> speedMS;
    }
    float getSpeedMS() {
        return speedMS;
    }

    void setDistance() {
        while (distance < 100) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            distance += speedMS;
            timeSec++;
            if (distance >= 100) {
                distance = 100;
                mtx.lock();
                std::cout << name << " just finished with time: " << timeSec << std::endl;
                mtx.unlock();
                return;
            }
            else {
                mtx.lock();
                std::cout << name << " swam " << distance << " metres" << std::endl;
                mtx.unlock();
            }
        }
    }

    int getTime() {
        return timeSec;
    }

    float getDistance() {
        return distance;
    }

    void startSwim() {
        driveway = std::thread(std::bind(&Swimmer::setDistance, this));
    }

    void joinThread() {
        driveway.join();
    }
};

std::mutex Swimmer::mtx;

int main() {
    std::vector<Swimmer*> swimmer;
    for (int i = 0; i < 6; i++) {
        Swimmer* tempObj = new Swimmer();
        swimmer.push_back(tempObj);
    }
    
    for (auto &s: swimmer) {
        s->setName();
        s->setSpeedMS();
    }

    for (auto &s: swimmer) {
        s->startSwim();
    }

    for (auto &s: swimmer) {
        s->joinThread();
    }

    std::sort(swimmer.begin(), swimmer.end(), [](Swimmer* firstPtr, Swimmer* secondPtr) {
        return firstPtr->getTime() < secondPtr->getTime();
    });

    int i = 0;
    for (auto &s: swimmer) {
        std::cout << i + 1 << ": " << s->getName() << " with time " << s->getTime() << " sec" << std::endl;
        i++;
    }

    for (auto &s: swimmer) {
        delete s;
    }
}

/*
Реализуйте симуляцию состязаний по заплыву на 100 метров.
Всего шесть дорожек и шесть пловцов. Имена пловцов указываются 
вначале из стандартного ввода. Каждый из них плывёт с разной 
скоростью, которая также задаётся пользователем при старте, 
через стандартный ввод в метрах в секунду.
Каждую секунду в стандартный вывод выдаётся информация о том, 
сколько проплыл тот или иной пловец.
Как только все пловцы коснулись 100-метровой отметки, 
заплыв заканчивается и выводится таблица с итоговыми 
результатами, отсортированная по возрастанию итогового времени заплыва.
*/