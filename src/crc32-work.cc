import <sstream>; 
import <iostream>; 
import <iomanip>; 
import <syncstream>; 

import crc32; 

int main (int argc, char *argv[]) {
    if (argc == 1) {
        std::osyncstream(std::clog) << "\e[31;1m[Error 1] usage: " << argv[0] << " (string without blank). \n\e[0m";
        return 1; 
    }
    auto p = crc32::hash(std::stringstream (argv[1])); 
    std::cout << std::hex << std::setw(8) << std::setfill('0') << p << '\n'; 
}