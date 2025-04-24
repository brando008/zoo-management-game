// animalManager.h
// Declaration of AnimalManager: handles in-memory collection and database
// persistence of Animal objects

#ifndef ANIMAL_MANAGER_H
#define ANIMAL_MANAGER_H

#include "animal.h"         // Definition of Animal class
#include "database.h"       // Database wrapper for SQLite operations
#include "exhibitManager.h" // ExhibitManager for exhibit assignments
#include <string>
#include <vector>

class AnimalManager {
public:
  // In-memory storage of Animal objects
  std::vector<Animal> animals;

  // Next ID to assign if auto-generating; currently unused since ID comes from
  // SQLite
  int nextId;

  // Constructor: initialize nextId if needed
  AnimalManager() : nextId(1) {}

  // Adds a new Animal both to the specified Exhibit and to the database
  //  - 'a' must have a valid ID, name, species, age, and exhibit
  //  - Returns false if exhibit is full or invalid
  bool addAnimal(const Animal &a, Exhibit &homeExhibit, Database &db);

  // Loads all Animal records from the database, reconstructs Animal objects,
  // and places them into the appropriate Exhibit via ExhibitManager
  void loadFromDatabase(ExhibitManager &em, Database &db);

  // Displays all currently loaded animals to console
  void viewAnimals() const;

  // Returns the number of animals currently in memory
  int getAnimalCount() const;

  // Updates an existing animal's fields and potentially moves it to a new
  // exhibit
  //  - idx: index in the 'animals' vector
  void updateAnimal(int idx, ExhibitManager &em);

  // Removes an animal by its unique ID (not vector index)
  //  - Returns true if an Animal with that ID was found and erased
  bool removeAnimal(int id);

  // Returns a reference to an Animal by its vector index; throws if out of
  // range
  Animal &getAnimalByIndex(int idx);
};

#endif // ANIMAL_MANAGER_H
