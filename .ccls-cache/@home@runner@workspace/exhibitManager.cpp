// exhibitManager.cpp
// Implements ExhibitManager: manages a global list of Exhibits, handles I/O
// menus, and synchronizes with the database. Dependencies:
//  - exhibitManager.h: Class declaration and interface
//  - database.h: For persistence methods (load/save)
//  - sqlite3.h: For direct SQLite prepare/step/finalize

#include "exhibitManager.h"
#include "database.h" // Database wrapper for executing SQL
#include <iostream>   // std::cout, std::cerr
#include <sstream>    // std::ostringstream for SQL statement building
#include <stdexcept>  // std::out_of_range

// Definition of the static member that holds all Exhibit objects
std::vector<Exhibit> ExhibitManager::exhibits;

// Constructor: currently no initialization needed
ExhibitManager::ExhibitManager() {}

// addExhibit
//  - Adds a new Exhibit to the global list (both in-memory and later saved to
//  DB)
void ExhibitManager::addExhibit(const Exhibit &ex) { exhibits.push_back(ex); }

// viewExhibits
//  - Prints all exhibits with index, name, type, and capacity
void ExhibitManager::viewExhibits() const {
  for (int idx = 0; idx < static_cast<int>(exhibits.size()); ++idx) {
    const Exhibit &exhibit = exhibits[idx];
    std::cout << idx << ") Exhibit Name: " << exhibit.getExhibitName()
              << std::endl;
    std::cout << "   Type: " << exhibit.getExhibitType() << std::endl;
    std::cout << "   Capacity: " << exhibit.getExhibitCapacity() << std::endl;
    std::cout << "-----------------------------" << std::endl;
  }
}

// selectExhibit
//  - Lists exhibits by index and prompts the user to choose one
//  - Validates input and loops until a valid index is entered
int ExhibitManager::selectExhibit() const {
  int num = exhibits.size();
  for (int i = 0; i < num; ++i) {
    std::cout << "(" << i << ") " << exhibits[i].getExhibitName() << std::endl;
  }

  int ans;
  while (true) {
    std::cout << "What numbered Exhibit would you like to see? ";
    if (!(std::cin >> ans)) {
      std::cerr << "[Error] Invalid input. Please enter a number." << std::endl;
      std::cin.clear();
      std::cin.ignore(1000, '\n');
      continue;
    }
    if (ans < 0 || ans >= num) {
      std::cerr << "[Error] Invalid exhibit number. Enter between 0 and "
                << (num - 1) << "." << std::endl;
      continue;
    }
    std::cin.ignore(1000, '\n'); // clear newline
    return ans;
  }
}

// viewSelectedExhibit
//  - Displays all animals within the exhibit at index 'ex'
void ExhibitManager::viewSelectedExhibit(int ex) const {
  if (ex < 0 || ex >= static_cast<int>(exhibits.size())) {
    std::cerr << "[Error] Invalid exhibit number. Please try again."
              << std::endl;
    return;
  }
  exhibits[ex].viewAnimals();
}

// exhibitExists
//  - Checks if an exhibit with the given name exists
bool ExhibitManager::exhibitExists(const std::string &ex) {
  for (const Exhibit &e : exhibits) {
    if (e.getExhibitName() == ex)
      return true;
  }
  return false;
}

// getExhibitCount
//  - Returns the number of exhibits in memory
int ExhibitManager::getExhibitCount() const {
  return static_cast<int>(exhibits.size());
}

// viewAnimalsInExhibit
//  - Lists animals in the exhibit at index 'idx'
void ExhibitManager::viewAnimalsInExhibit(int idx) const {
  if (idx < 0 || idx >= getExhibitCount()) {
    std::cerr << "[Error] Invalid exhibit index" << std::endl;
    return;
  }
  const Exhibit &e = exhibits[idx];
  int n = e.getAnimalCount();
  std::cout << "\n--- Animals in '" << e.getExhibitName() << "' ---"
            << std::endl;
  if (n == 0) {
    std::cout << "(none)" << std::endl;
  } else {
    for (int i = 0; i < n; ++i) {
      std::cout << "  " << i << ") " << e.getAnimal(i) << std::endl;
    }
  }
  std::cout << "-------------------------------" << std::endl;
}

// getExhibitByIndex
//  - Returns a reference to exhibit at 'index'; throws if out of range
Exhibit &ExhibitManager::getExhibitByIndex(int index) {
  return exhibits.at(index);
}

const Exhibit &ExhibitManager::getExhibitByIndex(int index) const {
  return exhibits.at(index);
}

// findExhibitIndex
//  - Returns the index of an exhibit by name or -1 if not found
int ExhibitManager::findExhibitIndex(const std::string &name) const {
  for (int i = 0; i < getExhibitCount(); ++i) {
    if (exhibits[i].getExhibitName() == name)
      return i;
  }
  return -1;
}

// getExhibitByName
//  - Returns a reference to the exhibit with matching name; throws if not found
Exhibit &ExhibitManager::getExhibitByName(const std::string &name) {
  int idx = findExhibitIndex(name);
  if (idx < 0) {
    throw std::out_of_range("Exhibit not found: " + name);
  }
  return exhibits[idx];
}

// loadFromDatabase
//  - Loads exhibits from the database table 'Exhibits'
void ExhibitManager::loadFromDatabase(Database &db) {
  sqlite3_stmt *stmt;
  std::string sql = "SELECT name, type, capacity FROM Exhibits;";
  sqlite3_prepare_v2(db.get(), sql.c_str(), -1, &stmt, nullptr);

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    std::string name = (const char *)sqlite3_column_text(stmt, 0);
    std::string type = (const char *)sqlite3_column_text(stmt, 1);
    int capacity = sqlite3_column_int(stmt, 2);

    // ✅ This constructor allocates the animals array
    Exhibit ex(name, type, capacity);

    exhibits.push_back(ex);
  }

  sqlite3_finalize(stmt);
}

// saveExhibitToDatabase
//  - Persists a single Exhibit object to the 'Exhibits' table
void ExhibitManager::saveExhibitToDatabase(const Exhibit &ex, Database &db) {
  std::ostringstream sql;
  sql << "INSERT INTO Exhibits (name, type, capacity) VALUES ('"
      << ex.getExhibitName() << "', '" << ex.getExhibitType() << "', "
      << ex.getExhibitCapacity() << ");";
  db.execute(sql.str());
}
