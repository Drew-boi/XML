/*
 This project helped me a learn a lot about strings and io.
 There were definitely some struggles, such as with converting the employee record to binary.
 Even in these struggles I learned cool things that I can use elsewhere. I found it most interesting
 that you can convert a fixed length record directly to fixed length binary. I also found it very helpful
 to learn how to move pointers in a stream.
 */
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "employee.h"
#include <vector>
#include <memory>
using std::cout;
using std::ifstream;
using std::unique_ptr;

char OPENCHEVRON = '<';
char CLOSECHEVRON = '>';
char SLASH = '/';

int main(int argc, const char * argv[]) {
    ifstream ifs;
    try { // open the file
        ifs.open(argv[1]);
        if (!ifs.is_open()) {
            throw std::runtime_error("Please enter a valid file.");
        }
    }
    catch (std::runtime_error& err) {
        cout << err.what() << endl;
        return 0;
    }
    std::vector<unique_ptr<Employee>> employees;
    Employee* currEmployee;
    try { // populate the vector with employees from the xml file
        currEmployee = Employee::fromXML(ifs);
        while (currEmployee != nullptr){
            employees.push_back(unique_ptr<Employee>(currEmployee));
            currEmployee = Employee::fromXML(ifs);
        }
    }
    catch (std::runtime_error& err) {
        cout << err.what() << endl;
    }
    ifs.clear();
    std::fstream file("employee.bin", std::fstream::in | std::fstream::out | std::ios::binary | std::ios::trunc); // Create binary file
    for (auto& emp : employees) {
        emp->display(cout); // Display each employee record
        cout << endl;
        emp->write(file); // Write to the fixed length file
    }
    employees.clear(); // Empty vector (unique_ptr will delete all of employees)
    file.seekg(0, std::ios::beg);
    currEmployee = Employee::read(file);
    file.clear();
    while (currEmployee != nullptr) { // create employees from binary file
        employees.push_back(unique_ptr<Employee>(currEmployee));
        currEmployee = Employee::read(file);
    }
    file.clear();
    for (auto& emp : employees) { // print xml representations of each employee
        emp->toXML(cout);
    }
    unique_ptr<Employee> newEmployee = unique_ptr<Employee>(Employee::retrieve(file, 12345)); // search binary file for employee object
    newEmployee->salary = 15000.00; // change the salary of that employee
    newEmployee->store(file); // write over the old employee with that id
    file.clear();
    unique_ptr<Employee> checkEmployee = unique_ptr<Employee>(Employee::retrieve(file, 12345)); // retrieve it again
    file.clear();
    cout << "Salary: "<< checkEmployee->salary << endl << endl; // check that the salary of employee has changed
    Employee myEmployee("Big Joey", 1234567, "1010 Hello Street", "Atlantis", "Bermuda Triangle", "Ocean", "801-888-1234", 1000.00);
    myEmployee.store(file); // store a new employee in the file
    file.clear();
    unique_ptr<Employee> finalEmployee = unique_ptr<Employee>(Employee::retrieve(file, 1234567)); // retrieve that employee
    finalEmployee->display(cout); // print the employee
    return 0;
}
