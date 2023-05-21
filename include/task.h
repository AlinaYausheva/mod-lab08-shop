//Copyright 2021 GHA Test Team
#pragma once
#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include <vector>

struct Statistics {
	int servedClients = 0;
	int rejectedClients = 0;
	double queueAvarageLength = 0;
	double averageQueueTime = 0;
	double avarageWorkingTime = 0;
	double averageDownTime = 0;

	double lambda = 0;
	double rejectionProbability = 0;
	double reativeCapacity = 0;
	double absoluteCapacity = 0;
};


class Client
{
public:
	int itemsNum;//количестов покупок
	enum States { SERVICED, REJECTED };
	States state;
	std::chrono::system_clock::time_point arrivalTime;//время прихода клиента
	std::chrono::system_clock::time_point departureTime;//время ухода клиента

	Client(int clientIntensity, int productsAverageNum);
};

class Shop
{
public:
	int workingTime;//время работы
	int cashirNum;//количество касс
	int timeForOneItem;//время на обработку 1 товара
	int MaxQueue;//длина очереди
	int openTime;
	std::mutex mtx;
	std::queue<Client> clientQueue;
	std::vector<Client> clients;
	std::vector<std::thread> cashiers;
	std::vector<int> cashiersWorkTime;

	int totalQueueLength = 0;

	void StartWork(int cashirNum, double clientIntensity, int timeForOneItem, 
		int productsAverageNum, int MaxQueue);

	Shop(int cashirNum, int timeForOneItem, int MaxQueue);
	
	void AddToQueue(Client client);
	void ServeClient(int index);
	void GenerationClient(double clientIntensity, int productsAverageNum);
};

int CreateItemsNum(int productsAverageNum);

Statistics CalculateReal(Shop& shop);
Statistics CalculateExpected(int timeForOneItem, int productsAverageNum, int cashirNum, double clientIntensity, int MaxQueue);

