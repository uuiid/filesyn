#include "sqlConnect.h"
#include <sqlite3.h>

#include <iostream>
#include <exception>

//#include <sqlite3ext.h>

sqlConnect::sqlConnect( )
{
    db = nullptr;
    ZErrMsg = "ok";
}




sqlConnect::~sqlConnect( )
{
    sqlite3_close(db);
    //delete ZErrMsg;
}

bool sqlConnect::openSqlDB( )
{
    int rc;
    if (db == nullptr)
    {
        rc = sqlite3_open("doodle_syn.db", &db);
        if (rc)
        {
            throw std::runtime_error("�޷������ݿ�");
            return false;
        }
    }
    
    return true;
}

void sqlConnect::subObj( )
{
}

bool sqlConnect::exeStmtNoReturn(sqlite3_stmt * stmt )
{
    int err = sqlite3_step(stmt);
    if (err != SQLITE_OK){
        return false;
        std::cout << err << std::endl;
    }
    return true;
}

sqlite3* sqlConnect::getDB( )
{
    return db;
}

sqlConnect& sqlConnect::GetSqlCommect( )
{
    static sqlConnect instance;
    return instance;
}
