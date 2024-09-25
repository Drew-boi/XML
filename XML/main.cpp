#include <iostream>
#include <fstream>
#include "employee.h"
using std::cout;
using std::ifstream;

char OPENCHEVRON = '<';
char CLOSECHEVRON = '>';
char SLASH = '/';

int main(int argc, const char * argv[]) {
    ifstream ifs(argv[1]);
    Employee::fromXML(ifs);
    return 0;
}

