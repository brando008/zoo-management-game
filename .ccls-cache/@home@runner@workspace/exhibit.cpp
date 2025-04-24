// exhibit.cpp
#include "exhibit.h"
#include <iostream>

Exhibit::Exhibit() : name(""), type(""), capacity(0), animals() {
  std::cerr << "[WARNING] Default Exhibit constructor called — animals array "
               "not allocated!\n";
}

Exhibit::Exhibit(const string &n, const string &t, int c)
    : name(n), type(t), capacity(c), animals(c, "") {}

Exhibit::~Exhibit() {}

string Exhibit::getExhibitName() const { return name; }
string Exhibit::getExhibitType() const { return type; }
int Exhibit::getExhibitCapacity() const { return capacity; }
int Exhibit::getAnimalCount() const { return static_cast<int>(animals.size()); }

string Exhibit::getAnimal(int index) const {
  if (index >= 0 && index < static_cast<int>(animals.size())) {
    return animals[index];
  }
  return "";
}

bool Exhibit::setAnimal(const string &a) {
  for (const auto &existing : animals) {
    if (existing == "") {
      for (auto &slot : animals) {
        if (slot == "") {
          slot = a;
          return true;
        }
      }
    }
  }
  std::cerr << "[Error] Exhibit '" << name << "' is full! Cannot add '" << a
            << "'.\n";
  return false;
}

void Exhibit::viewAnimals() const {
  bool any = false;
  for (int i = 0; i < static_cast<int>(animals.size()); ++i) {
    if (!animals[i].empty()) {
      if (!any) {
        std::cout << "Animals in exhibit '" << name << "':" << std::endl;
        any = true;
      }
      std::cout << "  " << i << ") " << animals[i] << std::endl;
    }
  }
  if (!any) {
    std::cout << "(No animals in exhibit '" << name << "')" << std::endl;
  }
}

bool Exhibit::removeAnimal(const string &animalName) {
  for (auto &slot : animals) {
    if (slot == animalName) {
      slot = "";
      return true;
    }
  }
  std::cout << "[Warning] Animal '" << animalName << "' not found in exhibit '"
            << name << "'." << std::endl;
  return false;
}
