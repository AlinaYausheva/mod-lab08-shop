//Copyright 2021 GHA Test Team
#include <iostream>
#include "task.h"

int main()
{
    int cashirNum;
    double clientIntensity;
    int timeForOneItem;
    int productsAverageNum;
    int MaxQueue;

    std::cin >> cashirNum;
    std::cin >> clientIntensity;
    std::cin >> timeForOneItem;
    std::cin >> productsAverageNum;
    std::cin >> MaxQueue;

    Shop shop(cashirNum, timeForOneItem, MaxQueue);
    shop.StartWork(cashirNum, clientIntensity, timeForOneItem,
        productsAverageNum, MaxQueue);
    Statistics real = CalculateReal(shop);
    Statistics expected = CalculateExpected(timeForOneItem, productsAverageNum, cashirNum, clientIntensity, MaxQueue);
    
    std::cout << "Satistics:\n";
    std::cout << "Served clients: " << real.servedClients << '\n';
    std::cout << "Rejected clients: " << real.rejectedClients << '\n';
    std::cout << "Queue avarage length: " << real.queueAvarageLength << '\n';
    std::cout << "Average queue time: " << real.averageQueueTime << '\n';
    std::cout << "Avarage working time: " << real.avarageWorkingTime << '\n';
    std::cout << "Average down time: " << real.averageDownTime << '\n';
    std::cout << "Real data:\n";
    std::cout << "Rejection probability: " << real.rejectionProbability << '\n';
    std::cout << "Reative capacity: " << real.reativeCapacity << '\n';
    std::cout << "Absolute capacity: " << real.absoluteCapacity << '\n';
    std::cout << "Expected data:\n";
    std::cout << "Rejection probability: " << expected.rejectionProbability << '\n';
    std::cout << "Reative capacity: " << expected.reativeCapacity << '\n';
    std::cout << "Absolute capacity: " << expected.absoluteCapacity << '\n';

    return 0;
}