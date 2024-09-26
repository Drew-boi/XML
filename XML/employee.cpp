#include "employee.h"

Employee::Employee(string name, int id, string address, string city, string state, string country, string phone, double salary) : name(name), id(id), address(address), city(city), state(state), country(country), phone(phone), salary(salary)
{};

void Employee::display(ostream& os) const {
    cout << "id: " << id << endl;
    cout << "name: " << name << endl;
    cout << "address: " << address << endl;
    cout << "city: " << city << endl;
    cout << "state: " << state << endl;
    cout << "country: " << country << endl;
    cout << "phone: " << phone << endl;
    cout << "salary: " << salary << endl;
}

void Employee::write(ostream& os) const {
    EmployeeRec emprec;
    emprec.id = id;
    stobuf(name, emprec.name, sizeof(emprec.name)-1);
    stobuf(address, emprec.address, sizeof(emprec.address)-1);
    stobuf(city, emprec.city, sizeof(emprec.city)-1);
    stobuf(state, emprec.state, sizeof(emprec.state)-1);
    stobuf(country, emprec.country, sizeof(emprec.country)-1);
    stobuf(phone, emprec.phone, sizeof(emprec.phone)-1);
    emprec.salary = salary;
    os.write(reinterpret_cast<const char*>(&emprec), sizeof emprec);
}

void Employee::store(iostream& ios) const { // Overwrite (or append) record in (to) file
    EmployeeRec emprec;
    emprec.id = id;
    stobuf(name, emprec.name, sizeof(emprec.name)-1);
    stobuf(address, emprec.address, sizeof(emprec.address)-1);
    stobuf(city, emprec.city, sizeof(emprec.city)-1);
    stobuf(state, emprec.state, sizeof(emprec.state)-1);
    stobuf(country, emprec.country, sizeof(emprec.country)-1);
    stobuf(phone, emprec.phone, sizeof(emprec.phone)-1);
    emprec.salary = salary;
    ios.write(reinterpret_cast<const char*>(&emprec), sizeof emprec);
}

void Employee::toXML(ostream& os) const { // Write XML record for employee
    os << "<Employee>\n";
    os << "\t<Name>" << name << "</Name>\n";
    os << "\t<ID>" << id << "</ID>\n";
    if (address != "") {
        os << "\t<Address>" << address << "</Address>\n";
    }
    if (city != "") {
        os << "\t<City>" << city << "</City>\n";
    }
    if (state != "") {
        os << "\t<State>" << state << "</State>\n";
    }
    if (country != "") {
        os << "\t<Country>" << country << "</ID>\n";
    }
    if (phone != "") {
        os << "\t<Phone>" << phone << "</Phone>\n";
    }
    if (salary != 0.0) {
        os << "\t<Salary>" << salary << "</Salary>\n";
    }
    os << "</Employee>\n\n";
}
