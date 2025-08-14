# Zoo Management Game (C++ + SQLite)

Welcome to the **Zoo Management Game**, a terminal-based project built with C++ and SQLite3. This game lets you manage animals, exhibits, and caretaking tasks in a simplified zoo simulation. It was built as a hands-on project using Replit and demonstrates concepts like data persistence, object-oriented programming, and simple game logic.

---

## Features

- Add, update, and remove animals
- Create and manage exhibits
- Record animal care and feeding logs
- Save and load data using **SQLite3**
- Built using `Makefile` and Replit’s custom configuration (`.replit`, `replit.nix`)

---

## Built With

- **C++**
- **SQLite3** – for storing animal, exhibit, and caretaker records
- **Makefile** – for compilation and build automation
- **Replit** – as the development environment

---

## Getting Started

### 1. Clone the repository

```bash
git clone https://github.com/brando008/zoo-management-game.git
cd zoo-management-game
```

### 2. Install dependencies (if not using Replit)
Make sure you have sqlite3 installed:

```bash
sudo apt-get install sqlite3 libsqlite3-dev
```

### 3. Compile and Run

```bash
make
./main
```

## Project Structure

```bash
.
├── animal.cpp / animal.h         # Animal class and logic
├── exhibit.cpp / exhibit.h       # Exhibit class and management
├── animalManager.cpp             # Handles animal storage and actions
├── exhibitManager.cpp            # Exhibit logic and interface
├── animalCare.cpp                # Care logs and caretaker tasks
├── userInterface.cpp             # Text-based menu and input
├── database.db                   # (Optional) Your SQLite3 DB file
├── Makefile                      # Build configuration
├── .replit / replit.nix          # Replit-specific settings
```

