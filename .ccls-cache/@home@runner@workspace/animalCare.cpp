// animalCare.cpp
// Implements AnimalCareManager for recording, displaying, and persisting animal
// care records. Dependencies:
//  - animalCare.h: Class definitions for care records
//  - database.h: Provides Database abstraction for SQLite operations

#include "animalCare.h"
#include "database.h" // Abstraction over sqlite3 for executing SQL
#include <ctime>      // time() and localtime
#include <iostream>   // std::cout, std::cerr
#include <sstream>    // std::ostringstream

// recordFeeding
//  - Creates a FeedingRecord with current timestamp, food type, and amount
//  - Stores it in the in-memory 'records' map for quick retrieval
void AnimalCareManager::recordFeeding(int id, const std::string &food,
                                      double amount) {
  records[id].emplace_back(
      std::make_unique<FeedingRecord>(std::time(nullptr), food, amount));
}

// recordHealthCheck
//  - Creates a HealthRecord with current timestamp, vet name, notes, and
//  diagnosis
//  - Stores it similarly to feeding records
void AnimalCareManager::recordHealthCheck(int id, const std::string &vet,
                                          const std::string &notes,
                                          const std::string &diagnosis) {
  records[id].emplace_back(std::make_unique<HealthRecord>(
      std::time(nullptr), vet, notes, diagnosis));
}

// displayCareRecords
//  - Prints all care records for a given animal ID
//  - If no records exist, notifies user
void AnimalCareManager::displayCareRecords(int id) const {
  auto it = records.find(id);
  if (it == records.end()) {
    std::cout << "No records for animal " << id << std::endl;
    return;
  }
  std::cout << "Care records for animal " << id << ":\n";
  for (const auto &rptr : it->second) {
    std::cout << formatTime(rptr->getTime()) << " - " << rptr->getDetails()
              << std::endl;
  }
}

// formatTime
//  - Helper to convert time_t to a human-readable string
std::string AnimalCareManager::formatTime(time_t t) {
  char buf[64];
  std::tm *tm = std::localtime(&t);
  std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm);
  return buf;
}

// ===== Persistence Layer =====
// saveFeedingToDatabase
//  - Constructs an INSERT SQL statement for feeding records and executes it
void AnimalCareManager::saveFeedingToDatabase(int id, const std::string &food,
                                              double amount, Database &db) {
  std::ostringstream sql;
  sql << "INSERT INTO CareRecords (animal_id, type, details, timestamp) VALUES "
         "("
      << id << ", 'feeding', '" << amount << "kg of " << food
      << "', datetime('now'));";
  db.execute(sql.str());
}

// saveHealthToDatabase
//  - Similar to saveFeeding, but for health checks
void AnimalCareManager::saveHealthToDatabase(int id, const std::string &vet,
                                             const std::string &notes,
                                             const std::string &diagnosis,
                                             Database &db) {
  std::ostringstream sql;
  sql << "INSERT INTO CareRecords (animal_id, type, details, timestamp) VALUES "
         "("
      << id << ", 'health', '" << diagnosis << " by " << vet << ": " << notes
      << "', datetime('now'));";
  db.execute(sql.str());
}

// loadFromDatabase
//  - Loads all care records from the CareRecords table
//  - Prints them, demonstrating retrieval; could instead populate 'records'
void AnimalCareManager::loadFromDatabase(Database &db) {
  sqlite3_stmt *stmt;
  const std::string sql =
      "SELECT animal_id, type, details, timestamp FROM CareRecords;";
  int rc = sqlite3_prepare_v2(db.get(), sql.c_str(), -1, &stmt, nullptr);
  if (rc != SQLITE_OK) {
    std::cerr << "[Error] Failed to prepare statement: "
              << sqlite3_errmsg(db.get()) << std::endl;
    return;
  }

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    int animalId = sqlite3_column_int(stmt, 0);
    std::string type =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    std::string details =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    std::string timeStr =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));

    std::cout << "[DB] " << type << " for animal " << animalId << " at "
              << timeStr << ": " << details << "\n";
  }

  sqlite3_finalize(stmt);
}
