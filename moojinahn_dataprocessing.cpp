#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <string>
using namespace std;

class InMemoryDB {
private:
    unordered_map<string, int> mainState;
    unordered_map<string, int> transactionState;
    bool inTransaction;

public:
    InMemoryDB() : inTransaction(false) {}

    void begin_transaction() {
        if (inTransaction) {
            throw runtime_error("(Begin) Transaction already in progress.");
        }
        inTransaction = true;
        transactionState.clear();
    }

    void commit() {
        if (!inTransaction) {
            throw runtime_error("(Commit) No transaction in progress.");
        }
        for (const auto& entry : transactionState)
            mainState[entry.first] = entry.second;
        inTransaction = false;
    }

    void rollback() {
        if (!inTransaction) {
            throw runtime_error("(Rollback) No transaction in progress.");
        }
        transactionState.clear();
        inTransaction = false;
    }

    void put(string key, int val) {
        if (!inTransaction) {
            throw runtime_error("(Put) No transaction in progress.");
        }
        transactionState[key] = val;
    }

    int get(string key) {
        if (mainState.find(key) != mainState.end()) {
            return mainState[key];
        }
        return 0; // Return 0 if key doesn't exist
    }
};

int main() {
    InMemoryDB inmemoryDB;

    cout << "Welcome to the In-Memory Key-Value Database!" << endl << endl;
    cout << "Available commands:" << endl;
    cout << "begin: Start a new transaction" << endl;
    cout << "commit: Commit the current transaction" << endl;
    cout << "rollback: Rollback the current transaction" << endl;
    cout << "put [key] [value]: Add or update a key-value pair" << endl;
    cout << "get [key]: Retrieve the value for a given key" << endl;
    cout << "exit: Exit the program" << endl << endl;

    string input;
    while (true) {
        cout << "> ";
        getline(cin, input);

        if (input == "exit") {
            break;
        }

        istringstream iss(input);
        string command;
        iss >> command;

        try {
            if (command == "begin") {
                inmemoryDB.begin_transaction();
                cout << "Transaction started." << endl;
            } else if (command == "commit") {
                inmemoryDB.commit();
                cout << "Transaction committed." << endl;
            } else if (command == "rollback") {
                inmemoryDB.rollback();
                cout << "Transaction rolled back." << endl;
            } else if (command == "put") {
                string key;
                int value;
                iss >> key >> value;
                inmemoryDB.put(key, value);
                cout << "Key-value pair added/updated." << endl;
            } else if (command == "get") {
                string key;
                iss >> key;
                int value = inmemoryDB.get(key);
                if (value == 0) {
                    cout << "null: Key not found." << endl;
                } else {
                    cout << key << "'s value: " << value << endl;
                }
            } else {
                cout << "Invalid command." << endl;
            }
        } catch (const runtime_error& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    cout << "Exiting program." << endl;
    return 0;
}
