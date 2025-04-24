// database.cpp
// Implements the Database class, a lightweight wrapper around SQLite's C API
// Provides methods to open/close the database and execute arbitrary SQL
// statements

#include "database.h" // Declaration of Database wrapper
#include <iostream>   // std::cerr for error logging
#include <sqlite3.h>  // SQLite3 C API

// Constructor: opens (or creates) the SQLite database file
// - filename: path to the .db file
// - On failure, logs an error and sets internal handle to nullptr
Database::Database(const std::string &filename) {
  if (sqlite3_open(filename.c_str(), &db)) {
    std::cerr << "[Error] Can't open database: " << sqlite3_errmsg(db)
              << std::endl;
    db = nullptr;
  }
}

// Destructor: closes the database connection if open
Database::~Database() {
  if (db) {
    sqlite3_close(db);
  }
}

// execute: runs a non-query SQL statement (e.g., CREATE, INSERT, UPDATE)
// - sql: the SQL command to execute
// - Returns true on success, false on error (and logs the error message)
bool Database::execute(const std::string &sql) {
  char *errMsg = nullptr;
  int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
  if (rc != SQLITE_OK) {
    std::cerr << "[SQL Error] " << errMsg << std::endl;
    sqlite3_free(errMsg);
    return false;
  }
  return true;
}
