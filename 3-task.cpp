#include <iostream>
#include <vector>
#include <map>
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

std::map<int,std::pair<Food, std::string>> orderFood{
        {0, {PIZZA,"pizza"} },
        {1, {SOUP, "soup"} },
        {2, {STEAK, "steak"} },
        {3, {SALAD, "salad"} },
        {4, {SUSHI, "sushi"} }
};

class OnlineDelivery {
    std::vector<std::pair<Food, std::string>> uncookedFood;
    std::vector<std::pair<Food, std::string>> cookedFood;

    int successOrder = 0;

    std::mutex mtx1;
    std::mutex mtx2;
    std::mutex mtx3;

public:
    void getOrder() {
        
        while (true) {
            mtx3.lock();
            if (successOrder > 10) {
                mtx3.unlock();
                break;
            }
            mtx3.unlock();

            std::srand(std::time(nullptr));
            std::this_thread::sleep_for(std::chrono::seconds(std::rand() % 6 + 5));

            mtx1.lock();
            std::pair<Food, std::string> tempFood = orderFood[std::rand() % 5];
            uncookedFood.push_back(tempFood);
            std::cout << tempFood.second << " was ordered" << std::endl;
            mtx1.unlock();
        }
    }

    void cookOrder() {
        while (true) {
            mtx3.lock();
            if (successOrder > 10) {
                mtx3.unlock();
                break;
            }
            mtx3.unlock();

            mtx1.lock();
            if (uncookedFood.empty()) {
                mtx1.unlock();
                continue;
            } 
            std::this_thread::sleep_for(std::chrono::seconds(std::rand() % 11 + 5));
            std::pair<Food, std::string> tempFood = uncookedFood[0];
            uncookedFood.erase(uncookedFood.begin());
            mtx1.unlock();

            mtx2.lock();
            cookedFood.push_back(tempFood);
            std::cout << tempFood.second << " was cooked" << std::endl;
            mtx2.unlock();
        }
    }

    void deliveryOrders() {
        while (true) {
            mtx3.lock();
            if (successOrder > 10) {
                mtx3.unlock();
                break;
            }
            mtx3.unlock();

            mtx2.lock();
            if (cookedFood.empty()) {
                mtx2.unlock();
                continue;
            }
            std::vector<std::pair<Food, std::string>> tempVec = cookedFood;
            cookedFood.clear();
            mtx2.unlock();

            std::this_thread::sleep_for(std::chrono::seconds(30));
            for (int i = 0; i < tempVec.size(); i++) {
                std::cout << tempVec[i].second << " was delivered" << std::endl;
                
                mtx3.lock();
                successOrder++;
                mtx3.unlock();
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