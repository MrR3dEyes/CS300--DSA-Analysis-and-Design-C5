/*
 * ProjectTwo.cpp
 * Author: Filip Arghir
 * Date: 2025
 * Course: CS-300
 * Description: Course planner that loads courses from a CSV file
 *              and stores them in a Binary Search Tree.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;

// Structure for course info
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;

    Course() {}
    Course(string number, string title) {
        courseNumber = number;
        courseTitle = title;
    }
};

// Tree node structure
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node() {
        left = nullptr;
        right = nullptr;
    }
    Node(Course aCourse) : Node() {
        course = aCourse;
    }
};

// Binary Search Tree class
class BinarySearchTree {
private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    void destroyTree(Node* node);
    Node* removeNode(Node* node, string courseNumber);
    Node* search(Node* node, string courseNumber);

public:
    BinarySearchTree();
    ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    void Remove(string courseNumber);
    Course Search(string courseNumber);
    bool isEmpty();
};

// Constructor
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

// Destructor
BinarySearchTree::~BinarySearchTree() {
    destroyTree(root);
}

// Delete all nodes in the tree
void BinarySearchTree::destroyTree(Node* node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

// Check if tree is empty
bool BinarySearchTree::isEmpty() {
    return root == nullptr;
}

// Print all courses in order
void BinarySearchTree::InOrder() {
    inOrder(root);
}

// Insert course into BST
void BinarySearchTree::Insert(Course course) {
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        addNode(root, course);
    }
}

// Remove course from BST
void BinarySearchTree::Remove(string courseNumber) {
    transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);
    root = removeNode(root, courseNumber);
}

// Search for a course
Course BinarySearchTree::Search(string courseNumber) {
    transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);
    Node* result = search(root, courseNumber);
    if (result != nullptr) return result->course;
    return Course();
}

// Recursive search
Node* BinarySearchTree::search(Node* node, string courseNumber) {
    if (node == nullptr) return nullptr;

    string nodeCourseUpper = node->course.courseNumber;
    transform(nodeCourseUpper.begin(), nodeCourseUpper.end(), nodeCourseUpper.begin(), ::toupper);

    if (nodeCourseUpper == courseNumber) return node;
    if (courseNumber < nodeCourseUpper) return search(node->left, courseNumber);
    return search(node->right, courseNumber);
}

// Add node recursively
void BinarySearchTree::addNode(Node* node, Course course) {
    if (course.courseNumber < node->course.courseNumber) {
        if (node->left == nullptr)
            node->left = new Node(course);
        else
            addNode(node->left, course);
    }
    else if (course.courseNumber > node->course.courseNumber) {
        if (node->right == nullptr)
            node->right = new Node(course);
        else
            addNode(node->right, course);
    }
}

// Print courses alphabetically
void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);
        cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
        inOrder(node->right);
    }
}

// Remove node from BST
Node* BinarySearchTree::removeNode(Node* node, string courseNumber) {
    if (node == nullptr) return node;

    string nodeCourseUpper = node->course.courseNumber;
    transform(nodeCourseUpper.begin(), nodeCourseUpper.end(), nodeCourseUpper.begin(), ::toupper);

    if (courseNumber < nodeCourseUpper) {
        node->left = removeNode(node->left, courseNumber);
    }
    else if (courseNumber > nodeCourseUpper) {
        node->right = removeNode(node->right, courseNumber);
    }
    else {
        // Node to remove found
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
        }
        else if (node->left != nullptr && node->right == nullptr) {
            Node* temp = node;
            node = node->left;
            delete temp;
        }
        else if (node->right != nullptr && node->left == nullptr) {
            Node* temp = node;
            node = node->right;
            delete temp;
        }
        else {
            Node* temp = node->right;
            while (temp->left != nullptr)
                temp = temp->left;
            node->course = temp->course;
            node->right = removeNode(node->right, temp->course.courseNumber);
        }
    }
    return node;
}

// Display course info
void displayCourse(Course course) {
    cout << course.courseNumber << ", " << course.courseTitle << endl;
    if (!course.prerequisites.empty()) {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course.prerequisites.size(); i++) {
            cout << course.prerequisites[i];
            if (i < course.prerequisites.size() - 1) cout << ", ";
        }
        cout << endl;
    }
    else {
        cout << "Prerequisites: None" << endl;
    }
}

// Load CSV data into BST
bool loadCourses(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;
    ifstream file(csvPath);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << csvPath << endl;
        return false;
    }

    string line;
    int courseCount = 0;

    while (getline(file, line)) {
        if (line.empty()) continue;
        Course course;
        stringstream ss(line);
        string field;
        int fieldCount = 0;

        while (getline(ss, field, ',')) {
            field.erase(0, field.find_first_not_of(" \t\r\n"));
            field.erase(field.find_last_not_of(" \t\r\n") + 1);

            if (fieldCount == 0) course.courseNumber = field;
            else if (fieldCount == 1) course.courseTitle = field;
            else if (!field.empty()) course.prerequisites.push_back(field);
            fieldCount++;
        }

        if (!course.courseNumber.empty() && !course.courseTitle.empty()) {
            bst->Insert(course);
            courseCount++;
        }
    }

    file.close();
    cout << courseCount << " courses read" << endl;
    return courseCount > 0;
}

// Display main menu
void displayMenu() {
    cout << "\n1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
    cout << "\nWhat would you like to do? ";
}

// Main program
int main() {
    BinarySearchTree* bst = nullptr;
    string csvPath, courseKey;
    Course course;
    int choice = 0;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        displayMenu();
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input." << endl;
            continue;
        }

        switch (choice) {
        case 1:
            if (bst != nullptr) delete bst;
            bst = new BinarySearchTree();
            cout << "Enter filename: ";
            cin.ignore();
            getline(cin, csvPath);
            if (!loadCourses(csvPath, bst)) {
                cout << "Failed to load course data." << endl;
                delete bst;
                bst = nullptr;
            }
            break;
        case 2:
            if (bst != nullptr && !bst->isEmpty()) {
                cout << "\nHere is a sample schedule:\n" << endl;
                bst->InOrder();
            }
            else {
                cout << "Error: No data loaded." << endl;
            }
            break;
        case 3:
            if (bst != nullptr && !bst->isEmpty()) {
                cout << "Enter course number: ";
                cin >> courseKey;
                course = bst->Search(courseKey);
                if (!course.courseNumber.empty()) displayCourse(course);
                else cout << "Course " << courseKey << " not found." << endl;
            }
            else {
                cout << "Error: No data loaded." << endl;
            }
            break;
        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;
        default:
            cout << "Invalid choice." << endl;
            break;
        }
    }

    if (bst != nullptr) delete bst;
    return 0;
}
