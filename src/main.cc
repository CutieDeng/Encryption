import <iostream>; 
import <fstream>; 
import <iomanip>; 
import <cassert>; 

import sha1; 

int main(int argc, char *argv[]) {
    if (argc == 1) {
        // std::cerr << "argc needs to larger than 1. \n"; 
        std::cerr << 
R"document(Sha-1 Manual

Usage: '(this-program) (the-related-file)' 

)document"; 
        return 0; 
    }
    auto r = sha1(std::fstream(argv[1])); 
    for (size_t i = 0; i < 5; ++i) {
        std::cout << std::hex << std::setw(8) << std::setfill('0') << r[i] ; // << (i < 4 ? "_" : ""); 
    }
    std::cout << '\n'; 
    flush(std::cout); 
}