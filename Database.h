#pragma once

bool InitDataBase();

bool SaveRecordToSQLite(const char* timeStr, int duration);

void PrintAllRecord();

bool DeleterRecord(int id);