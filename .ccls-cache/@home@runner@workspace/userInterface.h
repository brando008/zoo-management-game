// userInterface.h
// Declaration of the console UI entry point for the Zoo Management Application

#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "database.h" // Ensures Database type is available for UI initialization

// Starts the interactive console user interface.
// This function:
//  - Initializes the SQLite database and tables
//  - Loads existing data into managers
//  - Presents menus for Animals, Exhibits, and Health Care operations
void runUserInterface();

#endif // USER_INTERFACE_H
