// animalCare.h
// Declaration of classes and manager for tracking and persisting animal care
// records.

#ifndef ANIMAL_CARE_H
#define ANIMAL_CARE_H

#include "database.h" // Provides Database handle and execute/get functions
#include <ctime>      // time_t
#include <map>        // std::map for grouping records by animal ID
#include <memory>     // std::unique_ptr for polymorphic CareRecord storage
#include <sstream>    // std::ostringstream for formatting details
#include <string>     // std::string
#include <vector>     // std::vector for per-animal record lists

// Base class for any care-related record (feeding, health checks, etc.)
class CareRecord {
protected:
  time_t timestamp; // Unix timestamp when record was created
  CareRecord(time_t t) : timestamp(t) {}

public:
  virtual ~CareRecord() = default;

  // Returns a formatted string describing this record
  virtual std::string getDetails() const = 0;

  // Retrieve raw timestamp (for sorting or formatting)
  time_t getTime() const { return timestamp; }
};

// FeedingRecord: represents a feeding event
class FeedingRecord : public CareRecord {
public:
  std::string foodType; // Type of food given
  double amount;        // Amount in kilograms

  FeedingRecord(time_t t, std::string f, double a)
      : CareRecord(t), foodType(std::move(f)), amount(a) {}

  std::string getDetails() const override {
    std::ostringstream oss;
    oss << "[FEEDING] " << amount << "kg of " << foodType;
    return oss.str();
  }
};

// HealthRecord: represents a veterinary health check
class HealthRecord : public CareRecord {
public:
  std::string vetName;   // Name of veterinarian
  std::string notes;     // Summary of health observations
  std::string diagnosis; // Diagnosis provided

  HealthRecord(time_t t, std::string v, std::string n, std::string d)
      : CareRecord(t), vetName(std::move(v)), notes(std::move(n)),
        diagnosis(std::move(d)) {}

  std::string getDetails() const override {
    std::ostringstream oss;
    oss << "[HEALTH] " << diagnosis << " by " << vetName << ": " << notes;
    return oss.str();
  }
};

// AnimalCareManager: orchestrates creation, display, and database persistence
// of care records
class AnimalCareManager {
  // Mapping from animalID to a list of care record pointers
  std::map<int, std::vector<std::unique_ptr<CareRecord>>> records;

public:
  AnimalCareManager() = default;

  // In-memory operations:
  void recordFeeding(int animalID, const std::string &food, double amount);
  void recordHealthCheck(int animalID, const std::string &vet,
                         const std::string &notes,
                         const std::string &diagnosis);
  void displayCareRecords(int animalID) const;

  // Database operations:
  void loadFromDatabase(Database &db);
  void saveFeedingToDatabase(int animalId, const std::string &food,
                             double amount, Database &db);
  void saveHealthToDatabase(int animalId, const std::string &vet,
                            const std::string &notes,
                            const std::string &diagnosis, Database &db);

private:
  // Helper to format a time_t into human-readable string
  static std::string formatTime(time_t t);
};

#endif // ANIMAL_CARE_H
