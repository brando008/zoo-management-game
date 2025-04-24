// exhibit.h
// Declaration of the Exhibit class: manages a fixed-size collection of animal
// names.

#ifndef EXHIBIT_H
#define EXHIBIT_H

#include <string>
#include <vector>
using std::string;

class Exhibit {
private:
  // Identification
  string name; // Exhibit name (e.g., "Savannah")
  string type; // Exhibit type or habitat (e.g., "Grassland")

  // Storage
  int capacity;                // Maximum number of animals allowed
  std::vector<string> animals; // Dynamic array of animal names

public:
  // Default constructor: initializes an empty exhibit
  Exhibit();
  // Constructor: allocates 'capacity' slots and initializes count to 0
  Exhibit(const string &n, const string &t, int c);

  // Destructor: frees the dynamic array
  ~Exhibit();

  // ——— Accessors —————————————————————————————
  string getExhibitName() const;     // Returns name
  string getExhibitType() const;     // Returns type
  int getExhibitCapacity() const;    // Returns capacity
  int getAnimalCount() const;        // Returns current count
  string getAnimal(int index) const; // Returns animal at index i

  // ——— Mutators —————————————————————————————
  // Attempts to add an animal name; returns false if exhibit is full
  bool setAnimal(const string &a);

  // Removes the named animal if found; shifts remaining names; returns success
  bool removeAnimal(const string &animalName);

  // ——— Utility —————————————————————————————
  // Prints a list of current animal names to the console
  void viewAnimals() const;
};

#endif // EXHIBIT_H
