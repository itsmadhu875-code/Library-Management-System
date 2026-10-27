
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
using namespace std;

class Book {
public:
    int id;
    string title;
    string author;
    bool issued;
};

void addBook() {
    Book b;

    cout << "Enter Book ID: ";
    cin >> b.id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter Book Title: ";
    getline(cin, b.title);

    cout << "Enter Author Name: ";
    getline(cin, b.author);

    b.issued = false;

    ofstream file("books.txt", ios::app);

    if (file.is_open()) {
        file << b.id << '\n'
             << b.title << '\n'
             << b.author << '\n'
             << b.issued << '\n';

        file.close();
        cout << "\nBook added successfully!\n";
    } else {
        cout << "Error opening file!\n";
    }
}

void displayBooks() {
    Book b;
    ifstream file("books.txt");

    if (!file.is_open()) {
        cout << "\nNo books available.\n";
        return;
    }

    bool found = false;

    cout << "\n===== BOOK LIST =====\n";

    while (file >> b.id) {
        file.ignore(numeric_limits<streamsize>::max(), '\n');

        if (!getline(file, b.title)) break;
        if (!getline(file, b.author)) break;
        if (!(file >> b.issued)) break;

        file.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "\nBook ID: " << b.id;
        cout << "\nTitle: " << b.title;
        cout << "\nAuthor: " << b.author;
        cout << "\nStatus: "
             << (b.issued ? "Issued" : "Available");
        cout << "\n----------------------\n";

        found = true;
    }

    if (!found)
        cout << "No books available.\n";

    file.close();
}

void searchBook() {
    int searchID;
    cout << "Enter Book ID to search: ";
    cin >> searchID;

    Book b;
    ifstream file("books.txt");
    bool found = false;

    while (file >> b.id) {
        file.ignore(numeric_limits<streamsize>::max(), '\n');

        if (!getline(file, b.title)) break;
        if (!getline(file, b.author)) break;
        if (!(file >> b.issued)) break;

        file.ignore(numeric_limits<streamsize>::max(), '\n');

        if (b.id == searchID) {
            cout << "\nBook Found!";
            cout << "\nTitle: " << b.title;
            cout << "\nAuthor: " << b.author;
            cout << "\nStatus: "
                 << (b.issued ? "Issued" : "Available");
            cout << '\n';

            found = true;
            break;
        }
    }

    if (!found)
        cout << "\nBook not found!\n";

    file.close();
}

void updateBookStatus(bool issue) {
    int searchID;

    cout << "Enter Book ID: ";
    cin >> searchID;

    ifstream file("books.txt");
    ofstream temp("temp.txt");

    if (!file.is_open() || !temp.is_open()) {
        cout << "Error opening file!\n";
        return;
    }

    Book b;
    bool found = false;
    bool changed = false;

    while (file >> b.id) {
        file.ignore(numeric_limits<streamsize>::max(), '\n');

        if (!getline(file, b.title)) break;
        if (!getline(file, b.author)) break;
        if (!(file >> b.issued)) break;

        file.ignore(numeric_limits<streamsize>::max(), '\n');

        if (b.id == searchID) {
            found = true;

            if (issue && !b.issued) {
                b.issued = true;
                changed = true;
            } else if (!issue && b.issued) {
                b.issued = false;
                changed = true;
            } else {
                cout << (issue
                    ? "Book is already issued.\n"
                    : "Book is not issued.\n");
            }
        }

        temp << b.id << '\n'
             << b.title << '\n'
             << b.author << '\n'
             << b.issued << '\n';
    }

    file.close();
    temp.close();

    if (remove("books.txt") != 0) {
        cout << "Could not replace the data file.\n";
        remove("temp.txt");
        return;
    }

    if (rename("temp.txt", "books.txt") != 0) {
        cout << "Could not rename the temporary file.\n";
        return;
    }

    if (!found)
        cout << "Book not found!\n";
    else if (changed)
        cout << (issue
            ? "Book issued successfully!\n"
            : "Book returned successfully!\n");
}

int main() {
    int choice;

    do {
        cout << "\n===== LIBRARY MANAGEMENT SYSTEM =====\n";
        cout << "1. Add Book\n";
        cout << "2. Display All Books\n";
        cout << "3. Search Book\n";
        cout << "4. Issue Book\n";
        cout << "5. Return Book\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Please enter a valid number.\n";
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(), '\n'
            );
            continue;
        }

        switch (choice) {
            case 1:
                addBook();
                break;

            case 2:
                displayBooks();
                break;

            case 3:
                searchBook();
                break;

            case 4:
                updateBookStatus(true);
                break;

            case 5:
                updateBookStatus(false);
                break;

            case 0:
                cout << "Thank you for using the system!\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }

    } while (choice != 0);

    return 0;
}
