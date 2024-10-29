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
    struct EmployeeRec { // fixed length record of employee
        int id;
        char name[31];
        char address[26];
        char city[21];
        char state[21];
        char country[21];
        char phone[21];
        double salary;
    };
    
    static void stobuf(const string& s, char* buf, size_t limit) { // helper function to populate emprec
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
    static Employee* read(istream& is) { // Read record from current file positon (binary file)
        EmployeeRec inbuf;
        is.read(reinterpret_cast<char*>(&inbuf), sizeof inbuf); // grab fixed length binary and reinterpret it to a fixed length record
        if (is) {
            Employee* emp = new Employee(inbuf.name, inbuf.id, inbuf.address, inbuf.city, inbuf.state, inbuf.country, inbuf.phone, inbuf.salary); // create new employee object from record
            return emp;
        }
        else {
            return nullptr;
        }
    }
    
    static Employee* retrieve(istream& is, int findID) { // Search file for record by id
        size_t record_size = sizeof(EmployeeRec); // this is the size of an employee record
        int value = 0;
        size_t i = 0;
        size_t currPoint = i * record_size;
        while (value != findID and is) { // search the file for the id
            currPoint = i * record_size;
            is.seekg(currPoint, std::ios::beg);
            is.read(reinterpret_cast<char*>(&value), sizeof(value));
            i++;
        }
        is.clear();
        if (value == findID) {
            is.seekg(currPoint, std::ios::beg); // go to where id was found
            EmployeeRec inbuf;
            is.read(reinterpret_cast<char*>(&inbuf), sizeof inbuf); // create fixed length record from binary
            if (is) {
                Employee* emp = new Employee(inbuf.name, inbuf.id, inbuf.address, inbuf.city, inbuf.state, inbuf.country, inbuf.phone, inbuf.salary); // create new employee
                return emp;
            }
            else {
                return nullptr;
            }
        }
        else {
            return nullptr; // if not found, return nullptr
        }
    }
        
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
        // front three bits used for employee tag. 5, 4 used for open close tags 3 is used for when we have found a '<' and are storing the contents of a tag. same thing for 2 and 0. 1 is used to check if a character was read before eof.
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
                    boolSet.set(3); // open chevron found
                }
                else if (c == CLOSECHEVRON and boolSet.test(3)) { // end of xml statement
                    if (strcasecmp(tagStorage.c_str(), employeeTag.c_str()) == 0) { // check if the tag contents match employee tag
                        boolSet.reset(3);
                        boolSet.set(5); // Employee tag found bit set
                        tagStorage = "";
                    }
                    else { // Tags don't match
                        string str = "Missing <Employee> tag";
                        throw std::runtime_error(str);
                    }
                }
                else if (boolSet.test(3)) { // Open chevron found, close chevron not yet found
                    tagStorage.push_back(c); // store whats in the tag
                }
            }
            
            else if (!boolSet.test(4)) { // open employee tag found, not close employee tag
                if (c == OPENCHEVRON and !boolSet.test(0)) { // Start of an opening tag or end tag
                    boolSet.set(0);
                }
                else if (boolSet.test(2)) { // opening tag found
                    if (!boolSet.test(0)) { // store the contents between tags
                        contentStorage.push_back(c);
                    }
                    else if (c == CLOSECHEVRON and boolSet.test(0)) { // close tag found
                        if (endStorage.at(0) == '/') { // make sure its actually a close tag, they must start with /
                            std::transform(endStorage.begin(), endStorage.end(), endStorage.begin(), ::tolower); // lowercase end tag
                            if (endStorage.substr(1) == tagStorage) { // correct close tag found
                                boolSet.reset(0);
                                boolSet.reset(2);
                                if (tagStorage == nameTag) { // check to figure out where to store the tag contents
                                    if (tempName != "") { // make sure there are no duplicate tags
                                        string str = "Multiple <" + tagStorage + "> tags";
                                        throw std::runtime_error(str);
                                    }
                                    tempName = contentStorage; // store tag contetnts
                                }
                                else if (tagStorage == idTag) {
                                    if (tempId != -1) { // make sure there are no duplicate tags
                                        string str = "Multiple <" + tagStorage + "> tags";
                                        throw std::runtime_error(str);
                                    }
                                    tempId = std::stoi(contentStorage);
                                }
                                else if (tagStorage == addressTag) {
                                    if (tempAddress != "") { // make sure there are no duplicate tags
                                        string str = "Multiple <" + tagStorage + "> tags";
                                        throw std::runtime_error(str);
                                    }
                                    tempAddress = contentStorage;
                                }
                                else if (tagStorage == cityTag) {
                                    if (tempCity != "") { // make sure there are no duplicate tags
                                        string str = "Multiple <" + tagStorage + "> tags";
                                        throw std::runtime_error(str);
                                    }
                                    tempCity = contentStorage;
                                }
                                else if (tagStorage == stateTag) {
                                    if (tempState != "") { // make sure there are no duplicate tags
                                        string str = "Multiple <" + tagStorage + "> tags";
                                        throw std::runtime_error(str);
                                    }
                                    tempState = contentStorage;
                                }
                                else if (tagStorage == countryTag) {
                                    if (tempCountry != "") { // make sure there are no duplicate tags
                                        string str = "Multiple <" + tagStorage + "> tags";
                                        throw std::runtime_error(str);
                                    }
                                    tempCountry = contentStorage;
                                }
                                else if (tagStorage == phoneTag) {
                                    if (tempPhone != "") { // make sure there are no duplicate tags
                                        string str = "Multiple <" + tagStorage + "> tags";
                                        throw std::runtime_error(str);
                                    }
                                    tempPhone = contentStorage;
                                }
                                else if (tagStorage == salaryTag) {
                                    if (tempSalary != 0.0) { // make sure there are no duplicate tags
                                        string str = "Multiple <" + tagStorage + "> tags";
                                        throw std::runtime_error(str);
                                    }
                                    tempSalary = std::stod(contentStorage);
                                }
                                tagStorage = "";
                                contentStorage = "";
                                endStorage = "";
                            }
                            else if (endStorage.substr(1) == employeeTag) { // end employee tag found
                                boolSet.set(4); // set end tag bool
                            }
                            else { // end tag not found
                                string str = "Missing </" + tagStorage + "> tag";
                                throw std::runtime_error(str);
                            }
                        }
                        else { // missing tag
                            string str = "Missing </" + tagStorage + "> tag";
                            throw std::runtime_error(str);
                        }
                    }
                    else if (boolSet.test(0)) {
                        endStorage.push_back(c); // store the end tag
                    }
                }
                else if (c == CLOSECHEVRON and boolSet.test(0)) { // end of xml opening statement
                    std::transform(tagStorage.begin(), tagStorage.end(), tagStorage.begin(), ::tolower);
                    if (tags.find(tagStorage) != tags.end()) { // testing if a correct tag is present
                        boolSet.reset(0);
                        boolSet.set(2);
                    }
                    else if (tagStorage.at(0) == '/') {
                        if (tagStorage.substr(1) == employeeTag) { // End employee tag found
                            break;
                        }
                    }
                    else { // tag is not equal to a valid tag
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
        if (!boolSet.test(1)) { // test if eof was read before a character
            return nullptr;
        }
        if (tempName == "") { // checking for required tags, if not found throw an error
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
