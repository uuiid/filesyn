#include "sqlConnect.h"
#include <sqlite3.h>

#include <iostream>
#include <exception>

//#include <sqlite3ext.h>

sqlConnect::sqlConnect( )
{
    int rc;
    rc = sqlite3_open("test.db", &db);
    if (rc)
    {
        throw std::runtime_error("�޷������ݿ�");
    }
}




sqlConnect::~sqlConnect( )
{
    sqlite3_close(db);
}