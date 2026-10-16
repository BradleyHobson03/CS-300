#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Stores all information associated with one course.
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Removes leading and trailing spaces from a string.
string trim(const string& text) {
    size_t first = text.find_first_not_of(" \t\r\n");
    if (first == string::npos) {
        return "";
    }

    size_t last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

// Converts a string to uppercase so course searches are case-insensitive.
string toUpper(string text) {
    transform(text.begin(), text.end(), text.begin(),
              [](unsigned char c) { return static_cast<char>(toupper(c)); });
    return text;
}

// Loads course data from a CSV file into a map keyed by course number.
bool loadCourses(const string& fileName, map<string, Course>& courses) {
    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cout << "Error: Could not open file " << fileName << "." << endl;
        return false;
    }

    map<string, Course> loadedCourses;
    string line;

    while (getline(inputFile, line)) {
        if (trim(line).empty()) {
            continue;
        }

        stringstream lineStream(line);
        vector<string> fields;
        string field;

        while (getline(lineStream, field, ',')) {
            fields.push_back(trim(field));
        }

        // Every valid row must contain at least a course number and title.
        if (fields.size() < 2 || fields[0].empty() || fields[1].empty()) {
            cout << "Error: Invalid course data found in file." << endl;
            return false;
        }

        Course course;
        course.courseNumber = toUpper(fields[0]);
        course.courseTitle = fields[1];

        // Any remaining non-empty fields are prerequisite course numbers.
        for (size_t i = 2; i < fields.size(); ++i) {
            if (!fields[i].empty()) {
                course.prerequisites.push_back(toUpper(fields[i]));
            }
        }

        loadedCourses[course.courseNumber] = course;
    }

    inputFile.close();

    // Validate that every listed prerequisite exists in the course file.
    for (const auto& entry : loadedCourses) {
        for (const string& prerequisite : entry.second.prerequisites) {
            if (loadedCourses.find(prerequisite) == loadedCourses.end()) {
                cout << "Error: Prerequisite " << prerequisite
                     << " for " << entry.first << " was not found in the file." << endl;
                return false;
            }
        }
    }

    courses = loadedCourses;
    cout << "Data loaded successfully." << endl;
    return true;
}

// Prints all courses in alphanumeric order.
void printCourseList(const map<string, Course>& courses) {
    cout << "Here is a sample schedule:" << endl;

    for (const auto& entry : courses) {
        const Course& course = entry.second;
        cout << course.courseNumber << ", " << course.courseTitle << endl;
    }
}

// Prints one course and its prerequisites.
void printCourseInformation(const map<string, Course>& courses, const string& requestedCourse) {
    string courseNumber = toUpper(trim(requestedCourse));
    auto courseIterator = courses.find(courseNumber);

    if (courseIterator == courses.end()) {
        cout << courseNumber << " is not a valid course." << endl;
        return;
    }

    const Course& course = courseIterator->second;
    cout << course.courseNumber << ", " << course.courseTitle << endl;

    cout << "Prerequisites: ";
    if (course.prerequisites.empty()) {
        cout << "None" << endl;
        return;
    }

    for (size_t i = 0; i < course.prerequisites.size(); ++i) {
        cout << course.prerequisites[i];
        if (i < course.prerequisites.size() - 1) {
            cout << ", ";
        }
    }
    cout << endl;
}

// Displays the main course planner menu.
void displayMenu() {
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
}

int main() {
    map<string, Course> courses;
    bool dataLoaded = false;
    int menuChoice = 0;

    cout << "Welcome to the course planner." << endl;

    while (menuChoice != 9) {
        cout << endl;
        displayMenu();
        cout << "What would you like to do? ";

        string input;
        getline(cin, input);

        try {
            size_t processedCharacters = 0;
            menuChoice = stoi(input, &processedCharacters);

            // Reject values such as "2abc" that stoi would otherwise partially accept.
            if (processedCharacters != trim(input).length()) {
                throw invalid_argument("Invalid menu input");
            }
        }
        catch (...) {
            cout << input << " is not a valid option." << endl;
            menuChoice = 0;
            continue;
        }

        switch (menuChoice) {
        case 1: {
            cout << "What file would you like to load? ";
            string fileName;
            getline(cin, fileName);
            dataLoaded = loadCourses(trim(fileName), courses);
            break;
        }

        case 2:
            if (!dataLoaded) {
                cout << "Please load the course data first." << endl;
            }
            else {
                printCourseList(courses);
            }
            break;

        case 3:
            if (!dataLoaded) {
                cout << "Please load the course data first." << endl;
            }
            else {
                cout << "What course do you want to know about? ";
                string courseNumber;
                getline(cin, courseNumber);
                printCourseInformation(courses, courseNumber);
            }
            break;

        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:
            cout << menuChoice << " is not a valid option." << endl;
            break;
        }
    }

    return 0;
}
