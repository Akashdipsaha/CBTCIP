#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <sstream>

using namespace std;

class Student {
public:
    string rollNo;
    string name;
    string surname;
    string course;
    int year;
    float cgpa;

    Student() : year(0), cgpa(0.0f) {}

    void input() {
        cout << "Enter roll number: ";
        cin >> rollNo;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        cout << "Enter first name: ";
        getline(cin, name);
        cout << "Enter surname: ";
        getline(cin, surname);
        cout << "Enter course: ";
        getline(cin, course);
        cout << "Enter year: ";
        cin >> year;
        cout << "Enter CGPA: ";
        cin >> cgpa;
    }

    void display() const {
        cout << "Roll Number: " << rollNo << "\n"
             << "Name: " << name << " " << surname << "\n"
             << "Course: " << course << "\n"
             << "Year: " << year << "\n"
             << "CGPA: " << cgpa << endl;
    }

    string serialize() const {
        return rollNo + "," + name + "," + surname + "," + course + "," + to_string(year) + "," + to_string(cgpa) + "\n";
    }

    void deserialize(const string& data) {
        stringstream ss(data);
        getline(ss, rollNo, ',');
        getline(ss, name, ',');
        getline(ss, surname, ',');
        getline(ss, course, ',');
        ss >> year;
        ss.ignore(1);
        ss >> cgpa;
    }
};

void addRecord() {
    ofstream outFile("students.txt", ios::app);
    Student student;
    student.input();
    outFile << student.serialize();
    outFile.close();
}

void displayRecords() {
    ifstream inFile("students.txt");
    string line;
    Student student;
    while (getline(inFile, line)) {
        student.deserialize(line);
        student.display();
    }
    inFile.close();
}

void searchRecord(const string& rollNo) {
    ifstream inFile("students.txt");
    string line;
    Student student;
    bool found = false;
    while (getline(inFile, line)) {
        student.deserialize(line);
        if (student.rollNo == rollNo) {
            student.display();
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "No record found." << endl;
    }
    inFile.close();
}

void deleteRecord(const string& rollNo) {
    ifstream inFile("students.txt");
    ofstream outFile("temp.txt", ios::trunc);
    string line;
    Student student;
    bool found = false;
    while (getline(inFile, line)) {
        student.deserialize(line);
        if (student.rollNo != rollNo) {
            outFile << line << "\n";
        } else {
            found = true;
        }
    }
    inFile.close();
    outFile.close();
    remove("students.txt");
    rename("temp.txt", "students.txt");
    if (found) {
        cout << "Record deleted successfully." << endl;
    } else {
        cout << "Record not found." << endl;
    }
}

void modifyRecord(const string& rollNo) {
    ifstream inFile("students.txt");
    ofstream outFile("temp.txt", ios::trunc);
    string line;
    Student student;
    bool found = false;
    while (getline(inFile, line)) {
        student.deserialize(line);
        if (student.rollNo == rollNo) {
            cout << "Enter new details for the record:\n";
            student.input();
            outFile << student.serialize();
            found = true;
        } else {
            outFile << line << "\n";
        }
    }
    inFile.close();
    outFile.close();
    remove("students.txt");
    rename("temp.txt", "students.txt");
    if (found) {
        cout << "Record modified successfully." << endl;
    } else {
        cout << "Record not found." << endl;
    }
}

int main() {
    int choice;
    string rollNo;

    do {
        cout << "\n1. Add Record\n2. Display Records\n3. Search Records\n4. Delete Record\n5. Modify Record\n0. Exit\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            addRecord();
            break;
        case 2:
            displayRecords();
            break;
        case 3:
            cout << "Enter roll number to search: ";
            cin >> rollNo;
            searchRecord(rollNo);
            break;
        case 4:
            cout << "Enter roll number to delete: ";
            cin >> rollNo;
            deleteRecord(rollNo);
            break;
        case 5:
            cout << "Enter roll number to modify: ";
            cin >> rollNo;
            modifyRecord(rollNo);
            break;
        case 0:
            cout << "Exiting program." << endl;
            break;
        default:
            cout << "Invalid choice, please try again." << endl;
        }
    } while (choice != 0);

    return 0;
}
