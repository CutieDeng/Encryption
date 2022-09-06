import <iostream>; 
import <fstream>; 
import <iomanip>; 
import <cassert>; 
import <chrono>; 
import <optional>; 
import <array>; 

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
    using time_point_t = decltype(std::chrono::system_clock::now()); 
    std::optional<time_point_t> start; 
    if (argc >= 3 && std::string_view(argv[2]) == "-t") {
        start = std::chrono::system_clock::now(); 
    }
    auto r = sha1::hash(std::ifstream(argv[1], std::fstream::binary)); 

    std::optional<std::chrono::microseconds> d; 
    if (start) {
        d = std::chrono::duration_cast<typename decltype(d) :: value_type>(std::chrono::system_clock::now() - *start); 
    }
    for (size_t i = 0; i < 5; ++i) {
        std::cout << std::hex << std::setw(8) << std::setfill('0') << r[i] ; // << (i < 4 ? "_" : ""); 
    }
    std::cout << '\n'; 
    if (d) {
        std::cout << "\nCost time: " << d->count() / 1e3 << " ms. \n"; 
    }
    flush(std::cout); 
    return 0; 
}