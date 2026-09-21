#define _CRT_SECURE_NO_WARNINGS
#define InsertSize 30
#include "Database.h"
#include <ctime>
#include <iostream>
#include <chrono>
#include <stdio.h>
#include <string>
#include <cstdlib>

//tm_year + 1900才是现在的时间 tm_mon + 1
//		timeInfo.tm_year + 1900, //今年
//      timeInfo.tm_mon + 1, //本月
//      timeInfo.tm_mday, //当天
//      timeInfo.tm_hour, //小时
//      timeInfo.tm_min, //分钟
//      timeInfo.tm_sec //秒

enum{five = 5, ten = 10, fifteen = 15, thirty = 30, sixty = 60};

auto GetNowTimes_return(){
	struct tm timeInfo;
	auto now_Time = std::chrono::system_clock::now();
	auto now_Time_Pointer = std::chrono::system_clock::to_time_t(now_Time);
	localtime_s(&timeInfo, &now_Time_Pointer);
	return timeInfo; 
}

std::string format_TimeString(){
	struct tm timeInfo_Insert = GetNowTimes_return();
	char InsertTime[InsertSize];
	sprintf_s(InsertTime,InsertSize, "%04d-%02d-%02d %02d:%02d:%02d", 
		timeInfo_Insert.tm_year + 1900,
		timeInfo_Insert.tm_mon + 1,
		timeInfo_Insert.tm_mday,
		timeInfo_Insert.tm_hour,
		timeInfo_Insert.tm_min,
		timeInfo_Insert.tm_sec
	);

	return std::string(InsertTime);
}

void fixedTimeTomato() {
	system("cls");
	int choice = 0;
	std::cout << "\t你需要多长时间的番茄钟? \n";
	std::cout << "\t1、5分钟\t2、十分钟\t3、十五分钟\t4、三十分钟\t5、六十分钟\n";
	std::cin >> choice;
	switch (choice) {
		case 1:{
			SaveRecordToSQLite(format_TimeString().c_str(), five);
			break;
		}
		case 2:{
			SaveRecordToSQLite(format_TimeString().c_str(), ten);
			break;
		}
		case 3:{
			SaveRecordToSQLite(format_TimeString().c_str(), fifteen);
			break;
		}
		case 4:{
			SaveRecordToSQLite(format_TimeString().c_str(), thirty);
			break;
		}
		case 5:{
			SaveRecordToSQLite(format_TimeString().c_str(), sixty);
			break;
		}
	}
}

void autoTimeTomato() {
	system("cls");
	int choice = 0;
	std::cout << "\t你需要多长时间的番茄钟? (直接输入时间即可)\n";
	std::cin >> choice;
	SaveRecordToSQLite(format_TimeString().c_str(), choice);
}

void exitprogram(){
	std::cout << "欢迎下次使用！\n";
	std::exit(0);
}

void tomatoClock_Menu(){
	std::cout << "--------------------------------------\n";
	std::cout << "\t 欢迎使用番茄钟\n";
	std::cout << "1、创建一个固定时间的番茄钟\n";
	std::cout << "2、创建一个自定义时间的番茄钟\n";
	std::cout << "3、查询所有已存储的番茄钟\n";
	std::cout << "4、删除指定已存储的番茄钟\n";
	std::cout << "5、退出\n";
	std::cout << "--------------------------------------\n";
}

void tomatoClock_Choice(int i){
	switch (i) {
		case 1: {
			fixedTimeTomato();
			break;
		}
		case 2: {
			autoTimeTomato();
			break;
		}
		case 3: {
			PrintAllRecord();
			system("pause");
			break;
		}
		case 4: {
			int delid = 0;
			PrintAllRecord();
			std::cout << "你想删除哪个时间？\n";
			std::cin >> delid;
			DeleterRecord(delid);
			system("pause");
			break;
		}
		case 5:{
			exitprogram();
			break;
		}
		default: {
			std::cout << "重新选择！\n";
			break;
		}
	}
}

int main() {
	if (!InitDataBase()) {
		std::cerr << "数据库初始化失败，程序退出。\n";
		return -1;
	}
	int user_Choice = 0;
	while (1) {
		system("cls");
		tomatoClock_Menu();
		std::cin >> user_Choice;
		tomatoClock_Choice(user_Choice);
	}
	return 0;
}