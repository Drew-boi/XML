#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <string>
#include <iostream>
#include <bitset>
#include <cstring>
#include <unordered_set>
#include <algorithm>

using std::string;
using std::iostream;
using std::ostream;
using std::istream;
using std::cout;
using std::endl;
using std::bitset;

extern char OPENCHEVRON;
extern char CLOSECHEVRON;
extern char SLASH;

class Employee {
public:
    string name;
    int id;
    string address;
    string city;
    string state;
    string country;
    string phone;
    double salary;
    
    Employee() = default;
    
    void display(ostream& os) const; // Write a readable Employee representation to a stream
    void write(ostream& os) const; // Write a fixed-length record to current file position
    void store(iostream& ios) const; // Overwrite (or append) record in (to) file
    void toXML(ostream& os) const; // Write XML record for employee
    static Employee* read(istream& is); // Read record from current file positon
    static Employee* retrieve(istream& is, int id); // Search file for record by id
    static Employee* fromXML(istream& is) { // Read the XML record from a stream
        // Initialize default employee variables
        string name = ""; // if default, don't create object
        int id = -1; // if default, don't create object
        string address = "";
        string city = "";
        string state = "";
        string country = "";
        string phone = "";
        double salary = 0.0;
        char c;
        bitset<6> boolSet; // 5 is the front bit, 0 is the back in terms of str representation
        // front three bits used for employee tag. 5, 4 used for open close tags 3 is used for when we have found a '<' and are storing the contents of a tag.
        string tagStorage = ""; // temporary tag text storage
        // comparison strings for tags
        string employeeTag = "employee";
        string nameTag = "name";
        string idTag = "id";
        string addressTag = "address";
        string cityTag = "city";
        string stateTag = "state";
        string countryTag = "country";
        string phoneTag = "phone";
        string salaryTag = "salary";
        std::unordered_set<string> tags = {
            nameTag,
            idTag,
            addressTag,
            cityTag,
            stateTag,
            countryTag,
            phoneTag,
            salaryTag
        };
        while (is.get(c)) {
            cout << c;
            if (!boolSet.test(5)) { // Opening employee tag not yet found
                if (c == OPENCHEVRON and !boolSet.test(3)) {
                    boolSet.set(3);
                }
//                else if (!boolSet.test(5) and c != ' ') {
//                    cout << "throw (no opening tag)"; // see if we need this later
//                }
                else if (c == CLOSECHEVRON and boolSet.test(3)) { // end of xml statement
                    if (strcasecmp(tagStorage.c_str(), employeeTag.c_str()) == 0) {
                        boolSet.reset(3);
                        boolSet.set(5);
                        tagStorage = "";
                    }
                    else {
                        cout << "throw (no opening employee tag)" << "tag = " << tagStorage << endl;
                        tagStorage = "";
                        boolSet.reset(3);
                    }
                }
                else if (boolSet.test(3)) {
                    tagStorage.push_back(c); // store whats in the tag
                }
                else if (c == OPENCHEVRON) {
                    cout << "already set" << endl; // probably throw here, will see
                }
            }
            
            else if (!boolSet.test(4)) { // open employee tag found, not end tag
                if (c == OPENCHEVRON and !boolSet.test(0)) {
                    boolSet.set(0);
                }
                else if (boolSet.test(2)) {
                    cout << "opening tag found";
                }
                else if (c == CLOSECHEVRON and boolSet.test(0)) { // end of xml statement
                    std::transform(tagStorage.begin(), tagStorage.end(), tagStorage.begin(), ::tolower);
                    if (tags.find(tagStorage) != tags.end()) { // testing if correct tag is present
                        boolSet.reset(0);
                        boolSet.set(2);
                    }
                    else {
                        cout << "throw (incorrect tag name)" << "tag = " << tagStorage << endl;
                        tagStorage = "";
                        boolSet.reset(0);
                    }
                }
                else if (boolSet.test(0)) {
                    tagStorage.push_back(c); // store whats in the tag
                }
                else if (c == OPENCHEVRON) {
                    cout << "already set" << endl; // probably throw here, will see
                }
            }
            
            else {
                break; // opening and closing employee tag found
            }
        }
        if (name == "") {
            cout << "\nthrow (Missing <Name> tag)" << endl;
        }
        if (id == -1) {
            cout << "throw (Missing <Id> tag)" << endl;
        }
        Employee* newEmployee = new Employee();
        return newEmployee;
    }
};

#endif
