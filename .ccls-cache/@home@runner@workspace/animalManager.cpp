// animalManager.cpp
// Implements AnimalManager for in-memory and database-backed storage of Animal
// objects. Coordinates with ExhibitManager to place animals into exhibits, and
// with Database for persistence.

#include "animalManager.h" // Class declaration and dependencies
#include "database.h"      // Database wrapper for SQLite operations
#include <iostream>        // std::cout, std::cerr
#include <sqlite3.h>       // SQLite C API for direct DB queries (prepare/step)
#include <sstream>         // std::ostringstream for SQL string building

// addAnimal
//  - Adds an Animal to both the in-memory vector and the persistent database
//  - Also attempts to assign the animal to the given Exhibit
//  - Returns false if exhibit is full (setAnimal() failure)
bool AnimalManager::addAnimal(const Animal &a, Exhibit &homeExhibit,
                              Database &db) {
  std::cout << "addAnimal Test 1\n";
  // 1) Try to add to exhibit
  if (!homeExhibit.setAnimal(a.getName())) {
    std::cout << "[DEBUG] setAnimal returned FALSE for '" << a.getName()
              << "'\n";
    return false; // Exhibit full or invalid
  }
  std::cout << "[DEBUG] setAnimal returned TRUE\n";

  std::cout << "addAnimal Test 2\n";

  // 2) Store in-memory
  animals.push_back(a);

  std::cout << "addAnimal Test 2\n";

  // 3) Persist to database
  std::ostringstream sql;
  sql << "INSERT INTO Animals (id, name, species, age, exhibit) VALUES ("
      << a.getId() << ", '" << a.getName() << "', '" << a.getSpecies() << "', "
      << a.getAge() << ", '" << a.getExhibit() << "');";

  std::cout << "addAnimal Test 3\n";

  std::cout << "[DEBUG] Executing SQL: " << sql.str() << std::endl;

  std::cout << "addAnimal Test 4\n";

  if (!db.execute(sql.str())) {
    std::cerr << "[ERROR] Failed to insert animal into database!\n";
    return false;
  }
  // db.execute(sql.str()); // Executes SQL via wrapper
  std::cout << "[DB] Animal inserted: " << a.getName() << std::endl;

  return true;
}

// viewAnimals
//  - Prints a simple list of all animals currently in memory
void AnimalManager::viewAnimals() const {
  int num = 0;
  for (const auto &a : animals) {
    std::cout << num << ") Name: " << a.getName()
              << ", Species: " << a.getSpecies() << std::endl;
    ++num;
  }
}

// getAnimalCount
//  - Returns the count of animals in the in-memory list
int AnimalManager::getAnimalCount() const {
  return static_cast<int>(animals.size());
}

// updateAnimal
//  - Allows user to modify fields of an existing animal, including moving
//  exhibits
//  - Note: Reads some input with getline and others with >>; ensure input
//  buffer is flushed properly
void AnimalManager::updateAnimal(int idx, ExhibitManager &em) {
  if (idx < 0 || idx >= getAnimalCount()) {
    std::cerr << "[Error] Invalid animal index\n";
    return;
  }

  Animal &a = animals[idx];
  std::cout << "Current Animal Details:\n";
  a.printAnimalInformation();

  // Gather new values
  std::string tempString;
  int tempInt;
  std::cout << "Setting New Details...\n";

  std::cout << "New Name: ";
  std::getline(std::cin, tempString);
  a.setName(tempString);

  std::cout << "New Species: ";
  std::getline(std::cin, tempString);
  a.setSpecies(tempString);

  std::cout << "New Age: ";
  if (!(std::cin >> tempInt)) {
    std::cerr << "[Error] Invalid age input\n";
    std::cin.clear();
    std::cin.ignore(1000, '\n');
    return;
  }
  std::cin.ignore(1000, '\n');
  a.setAnimalAge(tempInt);

  // Change exhibit
  std::cout << "\nSelect a new exhibit (or the same to leave unchanged):\n";
  int newExIdx = em.selectExhibit();
  const std::string oldExName = a.getExhibit();
  if (newExIdx >= 0) {
    // Remove from old exhibit
    em.getExhibitByName(oldExName).removeAnimal(a.getName());

    // Add to new exhibit
    Exhibit &newEx = em.getExhibitByIndex(newExIdx);
    if (newEx.setAnimal(a.getName())) {
      a.setAnimalExhibit(newEx.getExhibitName());
    } else {
      std::cerr << "[Warning] Exhibit full—keeping old exhibit.\n";
      em.getExhibitByName(oldExName).setAnimal(a.getName());
    }
  }
}

// removeAnimal
//  - Erases the first animal matching the given ID from the in-memory list
bool AnimalManager::removeAnimal(int id) {
  for (auto it = animals.begin(); it != animals.end(); ++it) {
    if (it->getId() == id) {
      animals.erase(it);
      return true;
    }
  }
  return false; // Not found
}

// getAnimalByIndex
//  - Returns a reference to an animal at a specific index, throws if invalid
Animal &AnimalManager::getAnimalByIndex(int idx) {
  if (idx < 0 || idx >= getAnimalCount()) {
    throw std::out_of_range("Animal index out of range");
  }
  return animals[idx];
}

// loadFromDatabase
//  - Reads all Animal records from the DB, reconstructs objects in memory,
//    and places them into exhibits via ExhibitManager
void AnimalManager::loadFromDatabase(ExhibitManager &em, Database &db) {
  sqlite3_stmt *stmt = nullptr;
  const std::string sql =
      "SELECT id, name, species, age, exhibit FROM Animals;";

  int rc = sqlite3_prepare_v2(db.get(), sql.c_str(), -1, &stmt, nullptr);
  if (rc != SQLITE_OK) {
    std::cerr << "[Error] Failed to prepare Animals query: "
              << sqlite3_errmsg(db.get()) << std::endl;
    return;
  }

  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    int id = sqlite3_column_int(stmt, 0);
    std::string name =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    std::string species =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    int age = sqlite3_column_int(stmt, 3);
    std::string exhibit =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));

    // Reconstruct Animal in memory
    Animal a(name, species, id, age, exhibit);
    animals.push_back(a);

    // Place into exhibit if exists
    if (em.exhibitExists(exhibit)) {
      em.getExhibitByName(exhibit).setAnimal(name);
    } else {
      std::cerr << "[Warning] Exhibit '" << exhibit
                << "' not found for animal '" << name
                << "'. Skipping placement.\n";
    }
  }

  // Finalize and clean up
  sqlite3_finalize(stmt);
}
