#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <ctime>
#include <mutex>

std::mutex mtx_1;
std::mutex mtx_2;
std::mutex mtx_3;

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

int successOrder = 0;

void getOrder(std::vector<Food>& uncookedFood) {
    while (successOrder < 10) {
        std::srand(std::time(nullptr));
        std::this_thread::sleep_for(std::chrono::seconds(std::rand() % 6 + 5));

        mtx_1.lock();
        Food tempFood = returnFood(std::rand() % 5);
        uncookedFood.push_back(tempFood);
        std::cout << returnStringFood(tempFood) << " was ordered" << std::endl;
        mtx_1.unlock();
    }
}

void cookOrder(std::vector<Food>& uncookedFood, std::vector<Food>& cookedFood) {
    while (successOrder < 10) {
        mtx_2.lock();
        std::this_thread::sleep_for(std::chrono::seconds(std::rand() % 11 + 5));
        if (uncookedFood.empty()) return;
        Food tempFood = uncookedFood[0];
        uncookedFood.erase(uncookedFood.begin());
        cookedFood.push_back(tempFood);

        std::cout << returnStringFood(tempFood) << " was cooked" << std::endl;
        mtx_2.unlock();
    }
}

int main() {
    std::vector<Food> uncookedFood;
    std::vector<Food> cookedFood;
    
    std::thread get(getOrder, std::ref(uncookedFood));
    std::thread cook(cookOrder, std::ref(uncookedFood), std::ref(cookedFood));

    while (successOrder < 10) {
        std::this_thread::sleep_for(std::chrono::seconds(30));
        mtx_3.lock();
        for (int i = 0; i < cookedFood.size(); i++) {
            std::cout << returnStringFood(cookedFood[i]) << " was delivered" << std::endl;
            successOrder++;
        }
        cookedFood.clear();
        mtx_3.unlock();
    }
    get.join();
    cook.detach();
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