#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <mutex>

std::mutex mtx;

void trainDriving(char name, int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
    std::cout << name << " train is waiting for docking" << std::endl;

    mtx.lock();
    std::string answer;
    do {
        std::cout << "Input depart for departure train " << name << std::endl;
        std::cin >> answer;
    } while (answer != "depart");
    std::cout << name << " train has departed" << std::endl;
    mtx.unlock();
}

int main() {
    int trainSeconds[3];
    for (int i = 0; i < 3; i++) {
        int temp;
        do {
            std::cout << "Input seconds for train " << i + 1 << ": ";
            std::cin >> temp;
        } while (temp <= 0);
        trainSeconds[i] = temp;
    }

    std::thread train_1(trainDriving, 'A', trainSeconds[0]);

    std::thread train_2(trainDriving, 'B', trainSeconds[1]);

    std::thread train_3(trainDriving, 'C', trainSeconds[2]);

    if (train_1.joinable()) train_1.join();
    if (train_2.joinable()) train_2.join();
    if (train_3.joinable()) train_3.join();
}

/*
С помощью многопоточности реализуйте простую модель железнодорожного вокзала.
С трёх точек отправления в разное время отбывают три разных поезда с условными 
обозначениями A, B, C. Все они идут на один и тот же вокзал и могут дойти до него в разное время.
На этом целевом вокзале единовременно может находиться только один поезд. 
Остальные поезда должны ждать, пока уже занятое место освободится.
В начале программы пользователь последовательно вводит для каждого 
поезда время в пути до вокзала (в секундах). После этого поезда 
начинают своё движение за заданное количество времени. Как только 
поезд прибыл на вокзал, он ожидает от пользователя команды depart, 
которая сигнализирует о его отбытии с вокзала.
По всем событиям (прибытие на вокзал, ожидание свободного места, 
отбытие с вокзала) вместе с условным обозначением поезда выводятся сообщения в консоль.
Программа завершается, когда на вокзале побывают все три поезда.
*/