#define _CRT_SECURE_NO_WARNINGS
#define InsertSize 30
#include "Database.h"
#include <ctime>
#include <iostream>
#include <chrono>
#include <stdio.h>
#include <string>
#include <thread>

//tm_year + 1900才是现在的时间 tm_mon + 1
//		timeInfo.tm_year + 1900, //今年
//      timeInfo.tm_mon + 1, //本月
//      timeInfo.tm_mday, //当天
//      timeInfo.tm_hour, //小时
//      timeInfo.tm_min, //分钟
//      timeInfo.tm_sec //秒

//void tomatoClock_Menu(){
//	std::cout << "----------------------------\n";
//	std::cout << "            番茄钟\n";
//	std::cout << "\t1、\n";
//}

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

int main() {
	if (!InitDataBase()) {
		std::cerr << "数据库初始化失败，程序退出。\n";
		return -1;
	}
	std::cout << "你需要多长时间的番茄钟？\n";
	int min = 0;
reinput:
	std::cin >> min;
	if (min < 3) {
		std::cout << "重新输入！\n";
		goto reinput;
	}
	std::string nowTime = format_TimeString();
	SaveRecordToSQLite(nowTime.c_str(), min);
	PrintAllRecord();
	return 0;
}