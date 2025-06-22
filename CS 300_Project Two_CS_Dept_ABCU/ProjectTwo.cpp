/*
===============================================================================
  Program:      Course Planner
  Author:       Shelby Joiner
  Date:         06-22-2025
  Description:  A console application to load, display, and query course data
                from a CSV file for the ABCU Advising Program.
===============================================================================
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <ctime> // For optional timestamp

using namespace std;

// Struct to store course information
struct Course {
    string number;
    string title;
    vector<string> prerequisites;
};

// Map to store all courses by course number
map<string, Course> courses;

// Helper to trim whitespace from strings
string trim(const string& str) {
    const auto start = str.find_first_not_of(" \t\r\n");
    const auto end = str.find_last_not_of(" \t\r\n");
    return (start == string::npos) ? "" : str.substr(start, end - start + 1);
}

// Function to load course data from CSV file
void loadCourses(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Warning: Unable to open file." << endl;
        return;
    }

    courses.clear(); // Clear any previously loaded data
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string field;
        vector<string> tokens;

        while (getline(ss, field, ',')) {
            tokens.push_back(trim(field));
        }

        if (tokens.size() < 2) continue;

        Course course;
        course.number = tokens[0];
        course.title = tokens[1];

        for (size_t i = 2; i < tokens.size(); ++i) {
            if (!tokens[i].empty()) {
                course.prerequisites.push_back(tokens[i]);
            }
        }

        courses[course.number] = course;
    }

    cout << "Courses loaded successfully!" << endl;
    file.close();
}

// Function to print all courses in alphabetical order
void printCourseList() {
    if (courses.empty()) {
        cout << "Warning: No course data loaded. Please load the data first." << endl;
        return;
    }

    vector<string> sortedCourses;
    for (const auto& pair : courses) {
        sortedCourses.push_back(pair.first);
    }

    sort(sortedCourses.begin(), sortedCourses.end());

    cout << "\n------------------------------------------" << endl;
    cout << "           Course Schedule         " << endl;
    cout << "------------------------------------------" << endl;
    for (const string& courseNum : sortedCourses) {
        cout << courseNum << ", " << courses[courseNum].title << endl;
    }
}

// Function to print details of a specific course
void printCourse(const string& inputCourseNum) {
    if (courses.empty()) {
        cout << "Warning: No course data loaded. Please load the data first." << endl;
        return;
    }

    string courseNum = inputCourseNum;
    transform(courseNum.begin(), courseNum.end(), courseNum.begin(), ::toupper);

    if (courses.find(courseNum) == courses.end()) {
        cout << "Course not found. Please check the course number and try again." << endl;
        return;
    }

    const Course& course = courses[courseNum];

    cout << "\n------------------------------------------" << endl;
    cout << "             Course Information           " << endl;
    cout << "------------------------------------------" << endl;
    cout << course.number << ", " << course.title << endl;

    if (!course.prerequisites.empty()) {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            cout << course.prerequisites[i];
            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
    else {
        cout << "Prerequisites: None" << endl;
    }
}

// Main function with menu
int main() {
    string input;

    time_t now = time(0);
    char buf[26];
    ctime_s(buf, sizeof(buf), &now);

    cout << "==========================================" << endl;
    cout << "         Welcome to the Course Planner    " << endl;
    cout << "==========================================" << endl;
    cout << "Session started on: " << buf;

    // Main menu loop
    while (true) {
        cout << "\n------------------------------------------" << endl;
        cout << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "------------------------------------------" << endl;
        cout << "What would you like to do? ";

        getline(cin, input);

        if (input == "1") {
            loadCourses("CS 300 ABCU_Advising_Program_Input.csv");
        }
        else if (input == "2") {
            printCourseList();
        }
        else if (input == "3") {
            cout << "Enter course number (e.g., CSCI100): ";
            string courseInput;
            getline(cin, courseInput);
            printCourse(courseInput);
        }
        else if (input == "9") {
            cout << "\nThank you for using the course planner!" << endl;
            break;
        }
        else {
            cout << "Invalid option. Please choose 1, 2, 3, or 9." << endl;
        }
    }

    return 0;
}
