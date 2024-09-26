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
    bitset<32> bid(emprec.id);
    bitset<248> bname;
    for (int i = 0; i < 31; ++i) {
        char c = emprec.name[i];
        for (int j = 7; j >= 0 && c; --j) {
            if (c & 0x1) {
                bname.set(8 * i + j);
            }
            c >>= 1;
        }
    }
    bitset<208> baddress;
    for (int i = 0; i < 26; ++i) {
        char c = emprec.address[i];
        for (int j = 7; j >= 0 && c; --j) {
            if (c & 0x1) {
                baddress.set(8 * i + j);
            }
            c >>= 1;
        }
    }
    bitset<168> bcity;
    for (int i = 0; i < 21; ++i) {
        char c = emprec.city[i];
        for (int j = 7; j >= 0 && c; --j) {
            if (c & 0x1) {
                bcity.set(8 * i + j);
            }
            c >>= 1;
        }
    }
    bitset<168> bstate;
    for (int i = 0; i < 21; ++i) {
        char c = emprec.state[i];
        for (int j = 7; j >= 0 && c; --j) {
            if (c & 0x1) {
                bstate.set(8 * i + j);
            }
            c >>= 1;
        }
    }
    bitset<168> bcountry;
    for (int i = 0; i < 21; ++i) {
        char c = emprec.country[i];
        for (int j = 7; j >= 0 && c; --j) {
            if (c & 0x1) {
                bcountry.set(8 * i + j);
            }
            c >>= 1;
        }
    }
    bitset<168> bphone;
    for (int i = 0; i < 21; ++i) {
        char c = emprec.phone[i];
        for (int j = 7; j >= 0 && c; --j) {
            if (c & 0x1) {
                bphone.set(8 * i + j);
            }
            c >>= 1;
        }
    }
    doublelong d; // use a union so you can pass the double bits as a long to bitset
    d.d = emprec.salary;
    bitset<64> bsalary(d.u);
    int value = static_cast<int>(bid.to_ulong());
    cout << bid << endl;
    os.write(reinterpret_cast<const char*>(&value), sizeof(value));
}
