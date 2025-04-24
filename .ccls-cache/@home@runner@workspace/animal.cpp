// animal.cpp
// Implementation of the Animal class, responsible for tracking individual
// animal details. Relies on ExhibitManager to verify that an animal's assigned
// exhibit exists.

#include "animal.h"
#include "exhibitManager.h" // Provides static methods to query existing exhibits
#include <iostream>         // Used for console I/O (warnings, printing)

// Constructor
// - Initializes all member variables: name, species, id, age, exhibit
// - If the specified exhibit does not exist in ExhibitManager, logs a warning
Animal::Animal(const string &n, const string &s, int i, int a, const string &ex)
    : name(n), species(s), id(i), age(a), exhibit(ex) {
  if (!ExhibitManager::exhibitExists(ex)) {
    std::cerr << "[Warning] Exhibit '" << ex << "' does not exist."
              << std::endl;
    // Optionally, one could assign a default exhibit here or throw an exception
  }
}

// printAnimalInformation
// Displays all stored details about this animal to the console.
void Animal::printAnimalInformation() const {
  std::cout << "Name: " << name << std::endl;
  std::cout << "Species: " << species << std::endl;
  std::cout << "ID: " << id << std::endl;
  std::cout << "Age: " << age << std::endl;
  std::cout << "Exhibit: " << exhibit << std::endl;
}

// Setter: update the animal's age
void Animal::setAnimalAge(int a) { age = a; }

// Setter: change the animal's exhibit assignment (no validation here)
void Animal::setAnimalExhibit(const string &ex) { exhibit = ex; }

// ——— Getters ——————————————————————————————————————
// Provide read-only access to private members
int Animal::getId() const { return id; }

const string &Animal::getName() const { return name; }

const string &Animal::getSpecies() const { return species; }

int Animal::getAge() const { return age; }

const string &Animal::getExhibit() const { return exhibit; }

// ——— Additional Setters —————————————————————————————————
// Allows renaming or changing species independently
void Animal::setName(const string &n) { name = n; }

void Animal::setSpecies(const string &s) { species = s; }
