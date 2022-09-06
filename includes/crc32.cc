export module crc32; 

import <cstdint>; 
import <ios>; 
import <array>; 

using namespace std; 

namespace crc32 {

    consteval auto cal_table() -> array<uint32_t, 256> {
        array<uint32_t, 256> result; 
        for (size_t i = 0; i < size(result); ++i) {
            uint32_t word = i; 
            for (uint32_t j = 0; j < 8; ++j) {
                if (word & 1) {
                    word >>= 1; 
                    word ^= 0xedb8'8320; 
                } else {
                    word >>= 1; 
                }
            }
            result.at(i) = word; 
        }
        return result; 
    }

}

export namespace crc32 {

    constinit array<uint32_t, 256> table = cal_table(); 

    using result_type = uint32_t; 
    auto hash(auto &&input_bytes) -> result_type {
        result_type result { 0xFFFFFFFF }; 
        input_bytes.exceptions(ios::badbit); 
        char c; 
        while (1) {
            input_bytes.get(c); 
            if (input_bytes.eof()) 
                return result ^ 0xFFFFFFFF; 
            size_t look = (result ^ c) & 0xff; 
            result >>= 8; 
            result ^= table[look]; 
        }
    }
}