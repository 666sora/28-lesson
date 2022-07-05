#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <ctime>
#include <mutex>

enum Food {
    PIZZA,
    SOUP,
    STEAK,
    SALAD,
    SUSHI
};

Food returnFood(int foodInt) {
    switch (foodInt) {
        case 0: return PIZZA;
        case 1: return SOUP;
        case 2: return STEAK;
        case 3: return SALAD;
        case 4: return SUSHI;
    }
    return {};
}

std::string returnStringFood(Food food) {
    switch (food) {
        case 0: return "pizza";
        case 1: return "soup";
        case 2: return "steak";
        case 3: return "salad";
        case 4: return "sushi"; 
    }
    return {};
}

class OnlineDelivery {
    std::vector<Food> uncookedFood;
    std::vector<Food> cookedFood;

    int successOrder = 0;

    std::mutex mtx1;
    std::mutex mtx2;

public:
    void getOrder() {
        while (successOrder < 10) {
            std::srand(std::time(nullptr));
            std::this_thread::sleep_for(std::chrono::seconds(std::rand() % 6 + 5));

            mtx1.lock();
            Food tempFood = returnFood(std::rand() % 5);
            uncookedFood.push_back(tempFood);
            std::cout << returnStringFood(tempFood) << " was ordered" << std::endl;
            mtx1.unlock();
        }
    }

    void cookOrder() {
        while (successOrder < 10) {
            mtx1.lock();
            if (uncookedFood.empty()) {
                mtx1.unlock();
                continue;
            } 
            std::this_thread::sleep_for(std::chrono::seconds(std::rand() % 11 + 5));
            Food tempFood = uncookedFood[0];
            uncookedFood.erase(uncookedFood.begin());
            mtx1.unlock();

            mtx2.lock();
            cookedFood.push_back(tempFood);
            std::cout << returnStringFood(tempFood) << " was cooked" << std::endl;
            mtx2.unlock();
        }
    }

    void deliveryOrders() {
        while (successOrder < 10) {
            mtx2.lock();
            if (cookedFood.empty()) {
                mtx2.unlock();
                continue;
            }
            std::vector<Food> tempVec = cookedFood;
            cookedFood.clear();
            mtx2.unlock();

            std::this_thread::sleep_for(std::chrono::seconds(30));
            for (int i = 0; i < tempVec.size(); i++) {
                std::cout << returnStringFood(tempVec[i]) << " was delivered" << std::endl;
                successOrder++;
            }
        }
    }
};

int main() {
    OnlineDelivery* onlineDelivery = new OnlineDelivery;

    std::thread orderThread(&OnlineDelivery::getOrder, std::ref(onlineDelivery));

    std::thread kitchenThread(&OnlineDelivery::cookOrder, std::ref(onlineDelivery));

    std::thread deliveryThread(&OnlineDelivery::deliveryOrders, std::ref(onlineDelivery));

    orderThread.join();
    kitchenThread.detach();
    deliveryThread.join();

    delete onlineDelivery;
}

/*
Требуется реализовать упрощённую модель работы 
кухни ресторана, работающего в режиме онлайн-доставки.
Онлайн-заказ поступает каждые 5–10 секунд. Это 
случайное блюдо из пяти: пицца, суп, стейк, салат, суши. 
Официант, принявший заказ онлайн, оставляет его 
у кухни. Если кухня свободна, она принимает заказ 
и спустя 5–15 секунд возвращает уже 
готовое блюдо на выдачу. Время определяется 
случайным образом с помощью функции 
std::rand() в указанных диапазонах.
Курьер приезжает каждые 30 секунд, забирает 
готовые блюда на выдаче и развозит их по заказчикам.
Программа завершается после 10 успешных 
доставок. На каждом из этапов, будь то заказ, 
или готовка, или доставка заказа, 
выводятся соответствующие сообщения в консоль.
*/