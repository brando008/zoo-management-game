// exhibitManager.h
// Declaration of ExhibitManager: manages global list of exhibits and their
// persistence.

#ifndef EXHIBIT_MANAGER_H
#define EXHIBIT_MANAGER_H

#include "database.h" // SQLite database wrapper
#include "exhibit.h"  // Exhibit class definition
#include <string>
#include <vector>

class ExhibitManager {
private:
  // Static in-memory list of all Exhibit instances
  static std::vector<Exhibit> exhibits;

public:
  // Constructor: can load exhibits from database if desired
  ExhibitManager();

  // In-memory operations:
  void addExhibit(const Exhibit &ex);     // Add to list
  void viewExhibits() const;              // Print all exhibits
  int selectExhibit() const;              // Interactive selection
  void viewSelectedExhibit(int ex) const; // Print animals in selected exhibit
  static bool exhibitExists(const std::string &ex); // Check existence by name
  int getExhibitCount() const;                      // Return count of exhibits
  void viewAnimalsInExhibit(int idx) const; // Print animals in exhibit idx

  // Accessors:
  Exhibit &getExhibitByIndex(int index); // Return modifiable exhibit
  const Exhibit &getExhibitByIndex(int index) const;   // Return const reference
  int findExhibitIndex(const std::string &name) const; // Find index by name
  Exhibit &getExhibitByName(
      const std::string &name); // Return by name (throws if not found)

  // Database persistence:
  void loadFromDatabase(Database &db); // Load exhibits at startup
  void saveExhibitToDatabase(const Exhibit &ex,
                             Database &db); // Persist an exhibit
};

#endif // EXHIBIT_MANAGER_H
