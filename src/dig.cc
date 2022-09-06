import <sstream>; 
import <iostream>; 
import <iomanip>; 
import <chrono>; 
import <ranges>; 

import sha1; 

int main() {
    auto start = std::chrono::system_clock::now(); 
    std::stringstream buffer; 
    buffer << std::setfill('0');  
    int64_t a, b; 
    for (a = 0, b = 0; ; ) {
        buffer.str(""); 
        buffer.clear(); 
        buffer << std::setw(16) << a 
            << std::setw(16) << b; 
        auto r = sha1::hash(buffer); 
        if (r[0] & uint32_t { 0xffff'f000 }) {
        } else {
            auto cost_time = std::chrono::system_clock::now() - start; 
            std::stringstream buffer2; 
            buffer2 << std::setfill('0') 
                << std::setw(16) << a 
                << std::setw(16) << b; 
            std::cout << "Find result: " << buffer2.rdbuf() << "\nhash val: " << std::setfill('0') 
                << std::hex << std::setw(8) << r[0] 
                << std::setw(8) << r[1]
                << std::setw(8) << r[2] 
                << std::setw(8) << r[3] 
                << std::setw(8) << r[4] << '\n'
                << std::dec << "Cost time: " << (std::chrono::duration_cast<std::chrono::microseconds>(cost_time).count() / 1e3) << " ms. \n"; 
            break; 
        }
        if (b == 0x7fffffff'ffffffff) {
            b = 0; ++a; 
        } else [[likely]] {
            ++b; 
        }
    }
}