//Copyright 2021 GHA Test Team
#include "task.h"

Client::Client(int clientIntensity, int productsAverageNum) {
	this->itemsNum = CreateItemsNum(productsAverageNum);
	this->arrivalTime = std::chrono::system_clock::now();
	this->departureTime = std::chrono::system_clock::now();
	this->state = REJECTED;
}

int CreateItemsNum(int p) {
	srand(time(NULL));
	return rand() % (2 * p) + 1;
}

void Shop::GenerationClient(double clientIntensity, int productsAverageNum) {

	do {
		std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
		Client client(clientIntensity, productsAverageNum);
		AddToQueue(client);
		totalQueueLength += clientQueue.size();
		std::this_thread::sleep_for(std::chrono::milliseconds((long long)(1000 / clientIntensity)));
		std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
		openTime += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	} while (openTime < workingTime);;
}

void Shop::StartWork(int cashirNum, double clientIntensity, int timeForOneItem,
	int productsAverageNum, int MaxQueue) {

	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	std::thread clientThread(&Shop::GenerationClient, this, clientIntensity, productsAverageNum);
	for (int i = 0; i < cashirNum; i++) {
		cashiersWorkTime.push_back(0);
		cashiers.push_back(std::thread(&Shop::ServeClient, this, i));
	}	
	for (int i = 0; i < cashiers.size();i++) {
		cashiers[i].join();
	}
	clientThread.join();
	std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
	openTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

Shop::Shop(int cashirNum, int timeForOneItem, int MaxQueue) {
	this->workingTime = 60000;
	this->cashirNum = cashirNum;
	this->timeForOneItem = timeForOneItem;
	this->MaxQueue = MaxQueue;
	this->openTime = 0;
}


void Shop::AddToQueue(Client client) {
	mtx.lock();
	if (clientQueue.size() < MaxQueue) {
		clientQueue.push(client);
	}
	else {
		clients.push_back(client);
	}
	mtx.unlock();
}

void Shop::ServeClient(int index) {
	while (true) {
		mtx.lock();
		if (!clientQueue.empty()) {
			std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
			Client c = clientQueue.front();
			clientQueue.pop();
			mtx.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(c.itemsNum* timeForOneItem));
			c.departureTime = std::chrono::system_clock::now();
			c.state = Client::States::SERVICED;
			clients.push_back(c);
			std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
			cashiersWorkTime[index] += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		}
		else {
			mtx.unlock();
			if (openTime < workingTime) {
				std::this_thread::yield();
			}
			else {
				break;
			}
		}
	}
}

Statistics CalculateReal(Shop& shop) {
	Statistics stat;
	for (int i = 0; i < shop.clients.size(); i++) {
		if (shop.clients[i].state == Client::States::SERVICED) {
			stat.servedClients++;
		}
		else {
			stat.rejectedClients++;
		}
	}
	stat.queueAvarageLength = (double)shop.totalQueueLength / ((double)stat.servedClients + (double)stat.rejectedClients);
	int t = 0;
	for (int i = 0; i < shop.clients.size(); i++) {
		t += std::chrono::duration_cast<std::chrono::milliseconds>(shop.clients[i].departureTime - shop.clients[i].arrivalTime).count();
	}
	stat.averageQueueTime = (double)t / (double)shop.clients.size();
	t = 0;
	for (int i = 0; i < shop.cashirNum; i++) {
		t += shop.cashiersWorkTime[i];
	}
	stat.avarageWorkingTime = (double)t / (double)shop.cashirNum;
	t = 0;
	for (int i = 0; i < shop.cashirNum; i++) {
		t += shop.openTime - shop.cashiersWorkTime[i];
	}
	stat.averageDownTime = (double)t / (double)shop.cashirNum;

	stat.lambda = ((double)stat.servedClients + (double)stat.rejectedClients) / (double)shop.openTime;
	stat.rejectionProbability = (double)stat.rejectedClients / (double)shop.openTime;
	stat.reativeCapacity = 1 - (double)stat.rejectionProbability;
	stat.absoluteCapacity = stat.lambda * (1 - stat.rejectionProbability);
	return stat;
}
Statistics CalculateExpected(int timeForOneItem, int productsAverageNum, int cashirNum, double clientIntensity, int MaxQueue) {
	Statistics stat;
	stat.lambda = clientIntensity;
	double mu = 1000 / ((double)productsAverageNum * (double)timeForOneItem);
	double ro = stat.lambda / mu;
	double freeProbability = 0;
	double a = 1;
	double b = 1;
	for (int i = 0; i < cashirNum; i++) {
		freeProbability += a / b;
		a *= ro;
		b *= i + 1;
	}
	freeProbability += a / b;
	for (int i = cashirNum + 1; i <= cashirNum + MaxQueue; i++) {
		a *= ro;
		b *= cashirNum;
		freeProbability += a / b;
	}
	freeProbability = 1 / freeProbability;
	stat.rejectionProbability = (a / b) * freeProbability;

	stat.reativeCapacity = 1 - stat.rejectionProbability;
	stat.absoluteCapacity = stat.lambda * stat.reativeCapacity;
	return stat;
}