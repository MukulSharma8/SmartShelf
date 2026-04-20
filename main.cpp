#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

struct Book {
    int id;
    string title;
    string author;
    bool issued;
};

vector<Book> library;

// ---------------- FILE HANDLING ----------------
void saveToFile() {
    ofstream file("books.txt");
    for (auto &b : library) {
        file << b.id << "|" << b.title << "|" << b.author << "|" << b.issued << "\n";
    }
}

void loadFromFile() {
    ifstream file("books.txt");
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string id, title, author, issued;

        getline(ss, id, '|');
        getline(ss, title, '|');
        getline(ss, author, '|');
        getline(ss, issued, '|');

        library.push_back({stoi(id), title, author, stoi(issued)});
    }
}

// ---------------- FUZZY SEARCH ----------------
// simple similarity check
bool fuzzyMatch(string a, string b) {
    transform(a.begin(), a.end(), a.begin(), ::tolower);
    transform(b.begin(), b.end(), b.begin(), ::tolower);

    if (a.find(b) != string::npos) return true;

    int match = 0;
    for (char c : b) {
        if (a.find(c) != string::npos) match++;
    }

    return match >= b.length() / 2;
}

// ---------------- RECOMMENDATION ----------------
void recommendBooks(string keyword) {
    cout << "\n📚 Recommended Books:\n";
    for (auto &b : library) {
        if (fuzzyMatch(b.title, keyword)) {
            cout << "- " << b.title << " by " << b.author << endl;
        }
    }
}

// ---------------- FEATURES ----------------
void addBook() {
    Book b;
    cout << "Enter ID: ";
    cin >> b.id;
    cin.ignore();

    cout << "Enter Title: ";
    getline(cin, b.title);

    cout << "Enter Author: ";
    getline(cin, b.author);

    b.issued = false;

    library.push_back(b);
    saveToFile();
    cout << "Book added!\n";
}

void displayBooks() {
    cout << "\n===== BOOK LIST =====\n";
    for (auto &b : library) {
        cout << b.id << " | " << b.title << " | " << b.author
             << " | " << (b.issued ? "Issued" : "Available") << endl;
    }
}

void searchBook() {
    string query;
    cin.ignore();
    cout << "Enter search keyword: ";
    getline(cin, query);

    cout << "\n🔍 Results:\n";
    for (auto &b : library) {
        if (fuzzyMatch(b.title, query)) {
            cout << b.id << " | " << b.title << " | " << b.author << endl;
        }
    }

    recommendBooks(query);
}

void issueBook() {
    int id;
    cout << "Enter Book ID: ";
    cin >> id;

    for (auto &b : library) {
        if (b.id == id) {
            if (!b.issued) {
                b.issued = true;
                saveToFile();
                cout << "Book issued!\n";
            } else {
                cout << "Already issued\n";
            }
            return;
        }
    }
    cout << "Book not found\n";
}

void returnBook() {
    int id;
    cout << "Enter Book ID: ";
    cin >> id;

    for (auto &b : library) {
        if (b.id == id) {
            if (b.issued) {
                b.issued = false;
                saveToFile();
                cout << "Book returned!\n";
            } else {
                cout << "Not issued\n";
            }
            return;
        }
    }
    cout << "Book not found\n";
}

// ---------------- MENU ----------------
int main() {
    loadFromFile();

    int choice;
    do {
        cout << "\n===== SMART LIBRARY SYSTEM =====\n";
        cout << "1. Add Book\n";
        cout << "2. Display Books\n";
        cout << "3. Smart Search (Fuzzy + Recommend)\n";
        cout << "4. Issue Book\n";
        cout << "5. Return Book\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchBook(); break;
            case 4: issueBook(); break;
            case 5: returnBook(); break;
        }
    } while (choice != 6);

    return 0;
}