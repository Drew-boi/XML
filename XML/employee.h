#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <string>
#include <iostream>
#include <bitset>
#include <cstring>
#include <unordered_set>
#include <algorithm>
#include <stdexcept>

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
    struct EmployeeRec {
        int id;
        char name[31];
        char address[26];
        char city[21];
        char state[21];
        char country[21];
        char phone[21];
        double salary;
    };
    
    union doublelong {
      double d;
      unsigned long long u;
    };
    
    static void stobuf(const string& s, char* buf, size_t limit) {
        // Compute the numbers of chars to transfer (can't exceed limit-1)
        size_t nchars{std::min(limit-1,s.size())};
        // Copy and null-terminate
        s.copy(buf,nchars);
        buf[nchars] = '\0';
    }

public:
    string name;
    int id;
    string address;
    string city;
    string state;
    string country;
    string phone;
    double salary;
    
    Employee(string name, int id, string address = "", string city = "", string state = "", string country = "", string phone = "", double salary = 0.0);
    
    void display(ostream& os) const; // Write a readable Employee representation to a stream
    void write(ostream& os) const; // Write a fixed-length record to current file position
    void store(iostream& ios) const; // Overwrite (or append) record in (to) file
    void toXML(ostream& os) const; // Write XML record for employee
    static Employee* read(istream& is); // Read record from current file positon
    static Employee* retrieve(istream& is, int id); // Search file for record by id
    static Employee* fromXML(istream& is) { // Read the XML record from a stream
        // Initialize default employee variables
        string tempName = ""; // if default, don't create object
        int tempId = -1; // if default, don't create object
        string tempAddress = "";
        string tempCity = "";
        string tempState = "";
        string tempCountry = "";
        string tempPhone = "";
        double tempSalary = 0.0;
        char c;
        bitset<6> boolSet; // 5 is the front bit, 0 is the back in terms of str representation
        // front three bits used for employee tag. 5, 4 used for open close tags 3 is used for when we have found a '<' and are storing the contents of a tag.
        string tagStorage = ""; // temporary tag text storage
        string contentStorage = "";
        string endStorage = "";
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
        is.get(c);
        while (!is.eof()) {
            if (!boolSet.test(5)) { // Opening employee tag not yet found
                if (c == OPENCHEVRON and !boolSet.test(3)) {
                    if (!boolSet.test(1)) { // check if we read a valid character before eof
                        boolSet.set(1);
                    }
                    boolSet.set(3);
                }
                else if (c == CLOSECHEVRON and boolSet.test(3)) { // end of xml statement
                    if (strcasecmp(tagStorage.c_str(), employeeTag.c_str()) == 0) {
                        boolSet.reset(3);
                        boolSet.set(5);
                        tagStorage = "";
                    }
                    else {
                        string str = "Missing <Employee> tag";
                        throw std::runtime_error(str);
                    }
                }
                else if (boolSet.test(3)) {
                    tagStorage.push_back(c); // store whats in the tag
                }
            }
            
            else if (!boolSet.test(4)) { // open employee tag found, not emp end tag
                if (c == OPENCHEVRON and !boolSet.test(0)) { // Start of an opening tag or end tag
                    boolSet.set(0);
                }
                else if (boolSet.test(2)) { // opening tag found
                    if (!boolSet.test(0)) {
                        contentStorage.push_back(c);
                    }
                    else if (c == CLOSECHEVRON and boolSet.test(0)) {
                        if (endStorage.at(0) == '/') {
                            std::transform(endStorage.begin(), endStorage.end(), endStorage.begin(), ::tolower);
                            if (endStorage.substr(1) == tagStorage) {
                                boolSet.reset(0);
                                boolSet.reset(2);
                                if (tagStorage == nameTag) {
                                    if (tempName != "") {
                                        string str = "Multiple <" + tagStorage + "> tags";
                                        throw std::runtime_error(str);
                                    }
                                    tempName = contentStorage;
                                }
                                else if (tagStorage == idTag) {
                                    if (tempId != -1) {
                                        string str = "Multiple <" + tagStorage + "> tags";
                                        throw std::runtime_error(str);
                                    }
                                    tempId = std::stoi(contentStorage);
                                }
                                else if (tagStorage == addressTag) {
                                    if (tempAddress != "") {
                                        string str = "Multiple <" + tagStorage + "> tags";
                                        throw std::runtime_error(str);
                                    }
                                    tempAddress = contentStorage;
                                }
                                else if (tagStorage == cityTag) {
                                    if (tempCity != "") {
                                        string str = "Multiple <" + tagStorage + "> tags";
                                        throw std::runtime_error(str);
                                    }
                                    tempCity = contentStorage;
                                }
                                else if (tagStorage == stateTag) {
                                    if (tempState != "") {
                                        string str = "Multiple <" + tagStorage + "> tags";
                                        throw std::runtime_error(str);
                                    }
                                    tempState = contentStorage;
                                }
                                else if (tagStorage == countryTag) {
                                    if (tempCountry != "") {
                                        string str = "Multiple <" + tagStorage + "> tags";
                                        throw std::runtime_error(str);
                                    }
                                    tempCountry = contentStorage;
                                }
                                else if (tagStorage == phoneTag) {
                                    if (tempPhone != "") {
                                        string str = "Multiple <" + tagStorage + "> tags";
                                        throw std::runtime_error(str);
                                    }
                                    tempPhone = contentStorage;
                                }
                                else if (tagStorage == salaryTag) {
                                    if (tempSalary != 0.0) {
                                        string str = "Multiple <" + tagStorage + "> tags";
                                        throw std::runtime_error(str);
                                    }
                                    tempSalary = std::stod(contentStorage);
                                }
                                tagStorage = "";
                                contentStorage = "";
                                endStorage = "";
                            }
                            else if (endStorage.substr(1) == employeeTag) {
                                boolSet.set(4);
                            }
                            else {
                                string str = "Missing </" + tagStorage + "> tag";
                                throw std::runtime_error(str);
                            }
                        }
                        else {
                            string str = "Missing </" + tagStorage + "> tag";
                            throw std::runtime_error(str);
                        }
                    }
                    else if (boolSet.test(0)) {
                        endStorage.push_back(c); // store the end tag
                    }
                }
                else if (c == CLOSECHEVRON and boolSet.test(0)) { // end of xml statement
                    std::transform(tagStorage.begin(), tagStorage.end(), tagStorage.begin(), ::tolower);
                    if (tags.find(tagStorage) != tags.end()) { // testing if correct tag is present
                        boolSet.reset(0);
                        boolSet.set(2);
                    }
                    else if (tagStorage.at(0) == '/') {
                        if (tagStorage.substr(1) == employeeTag) { // End employee tag found
                            break;
                        }
                    }
                    else {
                        string str = "Invalid tag: <" + tagStorage + ">";
                        throw std::runtime_error(str);
                    }
                }
                else if (boolSet.test(0)) {
                    tagStorage.push_back(c); // store whats in the tag
                }
            }
            
            else {
                break; // opening and closing employee tag found
            }
            is.get(c);
        }
        if (!boolSet.test(1)) {
            return nullptr;
        }
        if (tempName == "") {
            throw std::runtime_error("Missing <Name> tag");
        }
        if (tempId == -1) {
            throw std::runtime_error("Missing <Id> tag");
        }
        Employee* newEmployee = new Employee(tempName, tempId, tempAddress, tempCity, tempState, tempCountry, tempPhone, tempSalary);
        return newEmployee;
    }
};

#endif
