// userInterface.cpp
// Console-based UI orchestrating interactions between AnimalManager,
// ExhibitManager, AnimalCareManager, and the SQLite-backed Database. Presents
// menus for CRUD operations.

#include "userInterface.h"
#include "animal.h"         // Animal model
#include "animalCare.h"     // AnimalCareManager and related records
#include "animalManager.h"  // CRUD in-memory and DB persistence for Animals
#include "database.h"       // Database wrapper for SQLite
#include "exhibit.h"        // Exhibit model
#include "exhibitManager.h" // CRUD and persistence for Exhibits

#include <iostream> // I/O streams
#include <string>   // std::string

using std::cin;
using std::cout;
using std::endl;
using std::string;

// Helper: prompt for integer within [min, max], with validation loop
static int readInt(const string &prompt, int min, int max) {
  int x;
  while (true) {
    cout << prompt;
    cin >> x;
    if (cin.fail() || x < min || x > max) {
      cout << "  ▶ Please enter an integer between " << min << " and " << max
           << "." << endl;
      cin.clear();            // clear error state
      cin.ignore(1000, '\n'); // skip bad input
      continue;
    }
    cin.ignore(1000, '\n'); // remove trailing newline
    return x;
  }
}

// Helper: prompt for double within [minV, maxV], with validation
static double readDouble(const string &prompt, double minV, double maxV) {
  double x;
  while (true) {
    cout << prompt;
    cin >> x;
    if (cin.fail() || x < minV || x > maxV) {
      cout << "  ▶ Please enter a number between " << minV << " and " << maxV
           << "." << endl;
      cin.clear();
      cin.ignore(1000, '\n');
      continue;
    }
    cin.ignore(1000, '\n');
    return x;
  }
}

// Entry point for the console UI
void runUserInterface() {
  // Initialize database (file: zoo.db)
  Database db("zoo.db");

  // Ensure tables exist
  db.execute("CREATE TABLE IF NOT EXISTS Animals ("
             "id INTEGER PRIMARY KEY, name TEXT, species TEXT, age INTEGER, "
             "exhibit TEXT);");
  db.execute("CREATE TABLE IF NOT EXISTS Exhibits ("
             "name TEXT PRIMARY KEY, type TEXT, capacity INTEGER);");
  db.execute("CREATE TABLE IF NOT EXISTS CareRecords ("
             "id INTEGER PRIMARY KEY AUTOINCREMENT, animal_id INTEGER, type "
             "TEXT, details TEXT, timestamp TEXT);");

  // Instantiate managers
  ExhibitManager exhibitMgr;
  AnimalManager animalMgr;
  AnimalCareManager careMgr;

  // Load persisted data
  exhibitMgr.loadFromDatabase(db);
  animalMgr.loadFromDatabase(exhibitMgr, db);
  careMgr.loadFromDatabase(db);

  // Add default exhibit if none loaded
  if (exhibitMgr.getExhibitCount() == 0) {
    Exhibit defaultEx("Default", "General", 20);
    exhibitMgr.addExhibit(defaultEx);
    exhibitMgr.saveExhibitToDatabase(defaultEx, db);
  }

  bool exitProgram = false;

  while (!exitProgram) {
    // Main menu
    cout << "\n=== Zoo Management Main Menu ===\n"
         << "1) Animals\n"
         << "2) Exhibits\n"
         << "3) Health Care\n"
         << "4) Exit\n";

    int choice = readInt("Choose an option: ", 1, 4);
    switch (choice) {
    case 1: { // ANIMALS MENU
      bool back = false;
      while (!back) {
        cout << "\n-- Animals Menu --\n"
             << "1) Add New Animal\n"
             << "2) View All Animals\n"
             << "3) Update Animal Information\n"
             << "4) Remove Animal\n"
             << "5) Back to Main Menu\n";
        int aopt = readInt("Choose: ", 1, 5);
        switch (aopt) {
        case 1: { // Add New Animal
          // Read name and species (allow spaces)
          cout << "Name: ";
          string name;
          std::getline(cin, name);
          cout << "Species: ";
          string species;
          std::getline(cin, species);

          int id = readInt("ID (integer): ", 1, 999999);
          int age = readInt("Age: ", 0, 200);

          // Select exhibit
          cout << "\nSelect Exhibit for this animal:\n";
          int exIdx = exhibitMgr.selectExhibit();
          Exhibit &ex = exhibitMgr.getExhibitByIndex(exIdx);

          Animal newA(name, species, id, age, ex.getExhibitName());

          if (!animalMgr.addAnimal(newA, ex, db)) {
            cout << "Sorry, that exhibit is full or invalid!\n";
          } else {
            cout << "Animal '" << name << "' added successfully!\n";
          }
          
          cout << "test 2\n";
          break;
        }
        case 2: // View All Animals
          animalMgr.viewAnimals();
          break;
        case 3: // Update Animal
          animalMgr.viewAnimals();
          if (animalMgr.getAnimalCount() == 0)
            break;
          {
            int idx = readInt(
                "Which animal number to update? (0-" +
                    std::to_string(animalMgr.getAnimalCount() - 1) + "): ",
                0, animalMgr.getAnimalCount() - 1);
            animalMgr.updateAnimal(idx, exhibitMgr);
          }
          break;
        case 4: // Remove Animal
          animalMgr.viewAnimals();
          if (animalMgr.getAnimalCount() == 0)
            break;
          {
            int idx = readInt(
                "Which animal number to remove? (0-" +
                    std::to_string(animalMgr.getAnimalCount() - 1) + "): ",
                0, animalMgr.getAnimalCount() - 1);
            Animal &a = animalMgr.getAnimalByIndex(idx);
            if (exhibitMgr.getExhibitByName(a.getExhibit())
                    .removeAnimal(a.getName())) {
              animalMgr.removeAnimal(a.getId());
              cout << "Animal removed successfully.\n";
            } else {
              cout << "Failed to remove animal from exhibit or manager.\n";
            }
          }
          break;
        case 5: // Back
          back = true;
          break;
        }
      }
      break;
    }
    case 2: { // EXHIBITS MENU
      bool back = false;
      while (!back) {
        cout << "\n-- Exhibits Menu --\n"
             << "1) Add New Exhibit\n"
             << "2) View All Exhibits\n"
             << "3) View Animals in Exhibit\n"
             << "4) Back to Main Menu\n";
        int eopt = readInt("Choose: ", 1, 4);
        switch (eopt) {
        case 1: { // Add Exhibit
          cout << "Exhibit Name: ";
          string exName;
          std::getline(cin, exName);
          cout << "Exhibit Type: ";
          string exType;
          std::getline(cin, exType);
          int cap = readInt("Capacity: ", 1, 10000);
          Exhibit newEx(exName, exType, cap);
          exhibitMgr.addExhibit(newEx);
          exhibitMgr.saveExhibitToDatabase(newEx, db);
          cout << "Exhibit '" << exName << "' added.\n";
          break;
        }
        case 2: // View Exhibits
          exhibitMgr.viewExhibits();
          break;
        case 3: { // View Animals in Exhibit
          exhibitMgr.viewExhibits();
          int idx = readInt(
              "Which exhibit index? (0-" +
                  std::to_string(exhibitMgr.getExhibitCount() - 1) + "): ",
              0, exhibitMgr.getExhibitCount() - 1);
          exhibitMgr.viewAnimalsInExhibit(idx);
          break;
        }
        case 4:
          back = true;
          break;
        }
      }
      break;
    }
    case 3: { // HEALTH CARE MENU
      bool backHC = false;
      while (!backHC) {
        cout << "\n-- Health Care Menu --\n"
             << "1) Record Feeding\n"
             << "2) Record Health Check\n"
             << "3) View Care Records\n"
             << "4) Back to Main Menu\n";
        int hopt = readInt("Choose: ", 1, 4);
        switch (hopt) {
        case 1: { // Feeding
          animalMgr.viewAnimals();
          if (animalMgr.getAnimalCount() == 0)
            break;
          int aidx =
              readInt("Select animal: ", 0, animalMgr.getAnimalCount() - 1);
          Animal &a = animalMgr.getAnimalByIndex(aidx);
          cout << "Food type: ";
          string food;
          std::getline(cin, food);
          double amt = readDouble("Amount (kg): ", 0.0, 1000.0);
          careMgr.recordFeeding(a.getId(), food, amt);
          careMgr.saveFeedingToDatabase(a.getId(), food, amt, db);
          cout << "Feeding record added for '" << a.getName() << "'.\n";
          break;
        }
        case 2: { // Health Check
          animalMgr.viewAnimals();
          if (animalMgr.getAnimalCount() == 0)
            break;
          int aidx =
              readInt("Select animal: ", 0, animalMgr.getAnimalCount() - 1);
          Animal &a = animalMgr.getAnimalByIndex(aidx);
          cout << "Vet name: ";
          string vet;
          std::getline(cin, vet);
          cout << "Diagnosis: ";
          string diag;
          std::getline(cin, diag);
          cout << "Notes: ";
          string notes;
          std::getline(cin, notes);
          careMgr.recordHealthCheck(a.getId(), vet, notes, diag);
          careMgr.saveHealthToDatabase(a.getId(), vet, notes, diag, db);
          cout << "Health record added for '" << a.getName() << "'.\n";
          break;
        }
        case 3: { // View Care
          animalMgr.viewAnimals();
          if (animalMgr.getAnimalCount() == 0)
            break;
          int aidx =
              readInt("Select animal: ", 0, animalMgr.getAnimalCount() - 1);
          Animal &a = animalMgr.getAnimalByIndex(aidx);
          cout << "\nCare Records for '" << a.getName() << "':\n";
          careMgr.displayCareRecords(a.getId());
          break;
        }
        case 4:
          backHC = true;
          break;
        }
      }
    } break;
    case 4:
      cout << "Goodbye!\n";
      exitProgram = true;
      break;
    }
  }
}
