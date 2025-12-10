#pragma once
#include "sqlite3.h"

void createDatabase(sqlite3* DB);

void importData(sqlite3* DB);

void simpleSelect(sqlite3* DB);

void complexSelect(sqlite3* DB);

void userSelect(sqlite3* DB);

static int displayData(void* data, int argc, char** argv, char** azColName);

