//Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include "task.h"

TEST(TestCaseName1, TestName1) {
	int average = 12;
	int num = CreateItemsNum(average);
	EXPECT_TRUE(num > 0 && num <= average * 2);
}

TEST(TestCaseName2, TestName2) {
	int cashirNum = 4;
	double clientIntensity = 0.5;
	int timeForOneItem = 600;
	int productsAverageNum = 8;
	int MaxQueue = 3;

	Shop shop(cashirNum, timeForOneItem, MaxQueue);
	shop.StartWork(cashirNum, clientIntensity, timeForOneItem,
		productsAverageNum, MaxQueue);
	EXPECT_TRUE(shop.openTime <= shop.workingTime + 
		productsAverageNum * 2 * timeForOneItem);
}

TEST(TestCaseName3, TestName3) {
	int cashirNum = 3;
	double clientIntensity = 1;
	int timeForOneItem = 600;
	int productsAverageNum = 5;
	int MaxQueue = 3;

	Shop shop(cashirNum, timeForOneItem, MaxQueue);
	shop.StartWork(cashirNum, clientIntensity, timeForOneItem,
		productsAverageNum, MaxQueue);
	EXPECT_EQ(shop.cashiers.size(), cashirNum);
}

TEST(TestCaseName4, TestName4) {
	int cashirNum = 4;
	double clientIntensity = 1;
	int timeForOneItem = 800;
	int productsAverageNum = 7;
	int MaxQueue = 4;

	Shop shop(cashirNum, timeForOneItem, MaxQueue);
	shop.StartWork(cashirNum, clientIntensity, timeForOneItem,
		productsAverageNum, MaxQueue);
	bool flag = true;
	for (int i = 0; i < cashirNum; i++) {
		if (shop.cashiersWorkTime[i] > shop.openTime)
			flag = false;
	}
	EXPECT_TRUE(flag);
}

TEST(TestCaseName5, TestName5) {
	int cashirNum = 3;
	double clientIntensity = 1;
	int timeForOneItem = 600;
	int productsAverageNum = 10;
	int MaxQueue = 3;

	Shop shop(cashirNum, timeForOneItem, MaxQueue);
	shop.StartWork(cashirNum, clientIntensity, timeForOneItem,
		productsAverageNum, MaxQueue);
	double n = 0;
	for (int i = 0; i < shop.clients.size(); i++) {
		n += (double)shop.clients[i].itemsNum;
	}
	n /= (double)shop.clients.size();
	EXPECT_TRUE(n >= productsAverageNum - 1 &&
		n <= productsAverageNum + 1);
}

TEST(TestCaseName6, TestName6) {
	int cashirNum = 4;
	double clientIntensity = 0.5;
	int timeForOneItem = 300;
	int productsAverageNum = 15;
	int MaxQueue = 4;

	Shop shop(cashirNum, timeForOneItem, MaxQueue);
	shop.StartWork(cashirNum, clientIntensity, timeForOneItem,
		productsAverageNum, MaxQueue);
	EXPECT_EQ(shop.workingTime * clientIntensity / 1000, shop.clients.size());
}

TEST(TestCaseName7, TestName7) {
	int cashirNum = 4;
	double clientIntensity = 0.5;
	int timeForOneItem = 300;
	int productsAverageNum = 15;
	int MaxQueue = 4;

	Shop shop(cashirNum, timeForOneItem, MaxQueue);
	shop.StartWork(cashirNum, clientIntensity, timeForOneItem,
		productsAverageNum, MaxQueue);
	Statistics real = CalculateReal(shop);
	EXPECT_TRUE(real.servedClients == shop.clients.size() && 
		real.rejectedClients == 0);
}

TEST(TestCaseName8, TestName8) {
	int cashirNum = 0;
	double clientIntensity = 1;
	int timeForOneItem = 600;
	int productsAverageNum = 5;
	int MaxQueue = 3;

	Shop shop(cashirNum, timeForOneItem, MaxQueue);
	shop.StartWork(cashirNum, clientIntensity, timeForOneItem,
		productsAverageNum, MaxQueue);
	Statistics real = CalculateReal(shop);
	EXPECT_TRUE(real.servedClients == 0 &&
		real.rejectedClients == shop.clients.size());
}

TEST(TestCaseName9, TestName9) {
	int cashirNum = 3;
	double clientIntensity = 2;
	int timeForOneItem = 500;
	int productsAverageNum = 12;
	int MaxQueue = 5;

	Shop shop(cashirNum, timeForOneItem, MaxQueue);
	shop.StartWork(cashirNum, clientIntensity, timeForOneItem,
		productsAverageNum, MaxQueue);
	Statistics real = CalculateReal(shop);
	EXPECT_TRUE(real.rejectedClients > 0);
}
