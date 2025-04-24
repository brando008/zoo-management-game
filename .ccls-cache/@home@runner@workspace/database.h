// database.h
// Declaration of Database wrapper around SQLite3 C API
// Provides basic open/close functionality and SQL execution interface

#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h> // SQLite3 C API definitions
#include <string>    // std::string for file paths and SQL commands

class Database {
private:
  sqlite3 *db; // Internal handle to the SQLite database connection

public:
  // Constructor: attempts to open (or create) the database file at 'filename'
  // On failure, logs error in implementation and sets 'db' to nullptr
  Database(const std::string &filename);

  // Destructor: closes the database connection if open
  ~Database();

  // get: exposes the raw sqlite3* pointer for direct API calls
  //  - Useful for prepare/step/finalize operations in other modules
  sqlite3 *get() { return db; }

  // execute: runs a non-query SQL statement (CREATE, INSERT, UPDATE, DELETE)
  // - Returns true on success, false on failure (and logs error)
  bool execute(const std::string &sql);
};

#endif // DATABASE_H
