#include <iostream>
#include <string>
#include <map>
#include <thread>
#include <functional>
#include <mutex>
#include <chrono>



class Swimmer {
    std::string name = "";

    float speedMS = 0;

    float distance = 0;

    std::thread driveway;

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
            if (distance > 100) {
                distance = 100;
                return;
            }
        }
    }

    float getDistance() {
        return distance;
    }

    void startSwim() {
        driveway = std::thread(std::bind(&setDistance, this));
    }

    ~Swimmer() {
        if (driveway.joinable()) {
            driveway.join();
        } 
    }
};

bool is_finish(Swimmer swimmer[6]) {
    for (int i = 0; i < 6; i++) {
        if (swimmer[i].getDistance() < 100) return false;
    }
    return true;
}

void showLeaderboard(Swimmer swimmer[6]) {
    for (int i = 0; i < 6; i++) {
        std::cout << swimmer[i].getName() << " swam " << swimmer[i].getDistance() << " metres" << std::endl;
    }
}

void totalLeaderboard(Swimmer swimmer[6]) {
    std::map<float, std::string> leaderboard;
    for (int i = 0; i < 6; i++) {
        leaderboard.insert(std::pair<float, std::string>(swimmer[i].getDistance() / swimmer[i].getSpeedMS(), swimmer[i].getName()));
    }
    int i = 0;
    for (auto it: leaderboard) {
        std::cout << i + 1 << ": " << it.second << " with time " << it.first << " sec" << std::endl;
        i++;
    }
}

int main() {
    Swimmer* swimmer = new Swimmer[6];
    
    for (int i = 0; i < 6; i++) {
        swimmer[i].setName();
        swimmer[i].setSpeedMS();
    }

    for (int i = 0; i < 6; i++) {
        swimmer[i].startSwim();
    }

    while(!is_finish(swimmer)) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        showLeaderboard(swimmer);
        std::cout << std::endl;
    }
    
    totalLeaderboard(swimmer);

    delete[] swimmer;
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