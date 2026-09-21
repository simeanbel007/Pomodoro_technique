#include"sqlite3.h"
#include"Database.h"
#include<iostream>

enum{five = 5, ten = 10, fifteen = 15, thirty = 30, sixty = 60};

static sqlite3* OpenDataBaseConnection(){
    sqlite3* db;
    // 数据库文件名为 pomodoro.db，如果不存在会自动创建
    int rc = sqlite3_open("pomodoro.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "无法打开数据库" << sqlite3_errmsg(db) << "\n";
        return nullptr;
    }
    return db;
}

bool InitDataBase(){
    sqlite3* db = OpenDataBaseConnection();
    if (!db) {
        return false;
    }
    const char* createTableSQL = 
        "CREATE TABLE IF NOT EXISTS focus_records ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "complete_time TEXT NOT NULL, "
        "duration_minutes INTEGER NOT NULL);";
    
    char* ErrMsg = 0;
    int rc = sqlite3_exec(db, createTableSQL, 0, 0, &ErrMsg);

    bool success = true;
    if(rc != SQLITE_OK){
        std::cerr << "创建表失败；" << ErrMsg << "\n";
        sqlite3_free(ErrMsg);
        success = false;
    } else {
        std::cout << ">数据库已初始化完成！\n"; 
    }

    sqlite3_close(db);
    return success;
}

bool SaveRecordToSQLite(const char *timeStr, int duration){
    sqlite3* db = OpenDataBaseConnection();
    if (!db) {
        return false;
    }
    sqlite3_stmt* stmt = nullptr;
    const char* insertSQL = "INSERT INTO focus_records (complete_time, duration_minutes) VALUES (? , ? );";
    if (sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "插入语句准备失败: " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, timeStr, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, duration);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "插入失败: " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }

    std::cout << "插入成功！\n记录时间: " << timeStr << "时长: " << duration << "分钟\n";
    sqlite3_close(db);
    return true;
}

void PrintAllRecord(){
    sqlite3* db = OpenDataBaseConnection();
    if (!db) {
        return;
    }
    const char* selectSQL = "SELECT id, complete_time, duration_minutes FROM focus_records ORDER BY id;";

    sqlite3_stmt * stmt = nullptr;
    if (sqlite3_prepare_v2(db, selectSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "查询失败！" << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);                    // 第 0 列：id
        const unsigned char* t = sqlite3_column_text(stmt, 1);   // 第 1 列：时间
        int minutes = sqlite3_column_int(stmt, 2);               // 第 2 列：时长

        std::cout << id << ".\t" << (t != nullptr ? reinterpret_cast<const char*>(t) : "") << "\t" << minutes << " 分钟\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

bool DeleterRecord(int id){
    sqlite3* db = OpenDataBaseConnection();
    if (!db) {
        return false;
    }
    
    const char* DeleteSQL = "DELETE FROM focus_records WHERE id = ?;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, DeleteSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "准备删除语句失败: " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "删除失败" << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }
    int affect = sqlite3_changes(db);
    if(affect == 0){
        std::cout << "没有找到 id = " << id << "的记录\n";
        sqlite3_close(db);
        return false;
    }
    std::cout << "成功删除 id = " << id << "的记录\n";
    sqlite3_close(db);
    return true;
}