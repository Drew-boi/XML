#include <iostream>
#include <fstream>
#include <stdexcept>
#include "employee.h"
#include <vector>
using std::cout;
using std::ifstream;

char OPENCHEVRON = '<';
char CLOSECHEVRON = '>';
char SLASH = '/';

int main(int argc, const char * argv[]) {
    ifstream ifs;
    try {
        ifs.open(argv[1]);
        if (!ifs.is_open()) {
            throw std::runtime_error("Please enter a valid file.");
        }
    }
    catch (std::runtime_error& err) {
        cout << err.what() << endl;
        return 0;
    }
    std::vector<Employee*> employees;
    Employee* currEmployee;
    try {
        currEmployee = Employee::fromXML(ifs);
        while (currEmployee != nullptr){
            employees.push_back(currEmployee);
            currEmployee = Employee::fromXML(ifs);
        }
    }
    catch (std::runtime_error& err) {
        cout << err.what() << endl;
    }
    ifs.clear();
    std::fstream file("/Users/andrewbuckland/Desktop/School/Current Semester/CS3370/XML/XML/XML/employee.bin", std::fstream::in | std::fstream::out | std::ios::binary | std::ios::trunc);
    for (Employee* emp : employees) {
        emp->display(cout); // Display each employee record
        cout << endl;
        emp->write(file); // Write to the fixed length file
    }
    employees.clear(); // Empty vector
    file.seekg(0, std::ios::beg);
    currEmployee = Employee::read(file);
    file.clear();
    while (currEmployee != nullptr) {
        employees.push_back(currEmployee);
        currEmployee = Employee::read(file);
    }
    file.clear();
    for (Employee* emp : employees) {
        emp->toXML(cout);
    }
    Employee* newEmployee = Employee::retrieve(file, 12345);
    newEmployee->salary = 15000.00;
    newEmployee->store(file);
    file.clear();
    Employee* checkEmployee = Employee::retrieve(file, 12345);
    file.clear();
    cout << "Salary: "<< checkEmployee->salary << endl << endl;
    Employee myEmployee("Big Joey", 1234567, "1010 Hello Street", "Atlantis", "Bermuda Triangle", "Ocean", "801-888-1234", 1000.00);
    myEmployee.store(file);
    file.clear();
    Employee* finalEmployee = Employee::retrieve(file, 1234567);
    finalEmployee->display(cout);
    return 0;
}
