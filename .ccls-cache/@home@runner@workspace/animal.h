// animal.h
// Declaration of the Animal class: represents a single animal with its
// attributes

#ifndef ANIMAL_H
#define ANIMAL_H

#include <string>
using std::string;

class Animal {
private:
  // Basic identifying attributes
  string name;    // Animal's given name
  string species; // Species (e.g., "Lion", "Elephant")
  int id;         // Unique identifier
  int age;        // Age in years
  string exhibit; // Name of the exhibit this animal belongs to

public:
  // Constructor: initializes all fields
  // 'ex' is validated at runtime in the .cpp implementation
  Animal(const string &n, const string &s, int i, int a, const string &ex);

  // ——— Getters —————————————————————————————————
  // Provide read-only access to private members
  int getId() const;
  const string &getName() const;
  const string &getSpecies() const;
  int getAge() const;
  const string &getExhibit() const;

  // ——— Setters —————————————————————————————————
  // Allow controlled modification of fields
  void setName(const string &n);
  void setSpecies(const string &s);
  void setAnimalAge(int a);
  void setAnimalExhibit(const string &ex);

  // Print all animal information to console
  void printAnimalInformation() const;
};

#endif // ANIMAL_H
