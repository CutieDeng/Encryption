import <iostream>; 
import <fstream>; 
import <iomanip>; 
import <cassert>; 

import sha1; 

int main(int argc, char *argv[]) {
    assert (argc >= 2); 
    auto r = sha1(std::fstream(argv[1])); 
    for (size_t i = 0; i < 5; ++i) {
        std::cout << std::hex << std::setw(8) << std::setfill('0') << r[i] ; // << (i < 4 ? "_" : ""); 
    }
    std::cout << '\n'; 
    flush(std::cout); 
}