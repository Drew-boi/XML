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
    ifstream ifs(argv[1]);
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
    std::fstream file("/Users/andrewbuckland/Desktop/School/Current Semester/CS3370/XML/XML/XML/employee.bin", std::fstream::in | std::fstream::out | std::fstream::trunc | std::ios::binary);
    for (Employee* emp : employees) {
        emp->display(cout);
        cout << endl;
        emp->write(file);
    }
    for (Employee* emp : employees) {
        emp->toXML(cout);
    }
//    size_t record_size = 160; // this is the size of an employee record
//    file.seekg(2 * record_size, std::ios::beg); // for each 32 bits in memory, you go up 4 on seekg
//    int value;
//    file.read(reinterpret_cast<char*>(&value), sizeof(value));
//
//    // Output the bits of the integer
//    std::bitset<32> bid(value);
//    cout << bid.to_string() << endl;
    
    return 0;
}
