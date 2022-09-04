import <iostream>; 
import <fstream>; 
import <iomanip>; 

import sha1; 

int main() {
    auto r = sha1(std::fstream("test.txt")); 
    for (size_t i = 0; i < 5; ++i) {
        std::cout << std::hex << std::setw(8) << std::setfill('0') << r[i] ; // << (i < 4 ? "_" : ""); 
    }
    std::cout << '\n'; 
}