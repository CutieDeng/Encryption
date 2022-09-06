export module sha1; 

import <cassert>;
import <ios>; 
import <array>; 
import <bit>; 

using namespace std;

export namespace sha1 {
    using result_type = array<uint32_t, 5>; 
    auto hash(auto &&v) -> result_type {
        struct {
            uint32_t a, b, c, d, e; 
        } origin {.a = 0x67452301, .b = 0xEFCDAB89, .c = 0x98BADCFE, .d = 0x10325476, .e = 0xC3D2E1F0 },
            older, tmp; 

        // Deal with the bad bit. :)
        v.exceptions(v.badbit); 

        constexpr size_t CACHE_BLOCK = 512 / 8; 
        char cache [CACHE_BLOCK]; 

        constexpr size_t SUB_BLOCK = 80; 
        uint32_t blocks[SUB_BLOCK]; 

        uint64_t total_cnt {}; 
        enum class Eof : unsigned char {
            not_eof, eof, eof_too_long, 
        } the_last = Eof::not_eof; 

        static_assert(sizeof the_last == 1); 

        while (1) {
            constexpr size_t PARTIAL_BLOCK = {CACHE_BLOCK - 8}; 
            static_assert (PARTIAL_BLOCK < CACHE_BLOCK, "PARTIAL_BLOCK calculate overflow takes place. "); 
            
            if (the_last == Eof::not_eof) [[likely]] {
                if (!v) {
                    assert (total_cnt == 0); 
                    throw std::ios_base::failure("Open the related byte stream but meeting error. "); 
                }
                v.read(cache, CACHE_BLOCK); 
                {
                    size_t cnt = v.gcount(); 
                    total_cnt += cnt; 
                    // Find the eof situation, then never read any more. 
                    if (v.eof()) {
                        assert (cnt != CACHE_BLOCK); 
                        // cache [cnt++] = 0b1; 
                        cache[cnt++] = 0b1000'0000; 
                        // static_assert (448 == PARTIAL_BLOCK * 8); 
                        if (cnt <= PARTIAL_BLOCK) {
                            the_last = Eof::eof; 
                        } else {
                            the_last = Eof::eof_too_long;  
                        }
                        for (; cnt < CACHE_BLOCK; ++cnt) {
                            cache[cnt] = 0; 
                        }
                        if (the_last == Eof::eof) 
                            *(uint64_t *)&cache[PARTIAL_BLOCK] = byteswap(total_cnt * 8); 
                    } else {
                        assert (cnt == CACHE_BLOCK); 
                    }
                } 
            } else if (the_last == Eof::eof_too_long) {
                for (size_t cnt = 0; cnt < PARTIAL_BLOCK; ++cnt) {
                    cache[cnt] = 0; 
                }
                *(uint64_t *)&cache[PARTIAL_BLOCK] = byteswap(total_cnt * 8); 
                the_last = Eof::eof; 
            } else {
                assert (the_last == Eof::eof); 
                // Can I get in here? 
                break; 
            }
            // todo: sha-1      
            size_t i; 
            for (i = 0; i < CACHE_BLOCK / 4; ++i) {
                // blocks[i] = *(uint32_t *)&cache[i * 4]; 
                blocks[i] = byteswap(((uint32_t *)cache)[i]); 
            }
            static_assert (CACHE_BLOCK / 4 >= 16); 
            for (; i < SUB_BLOCK; ++i) {
                blocks[i] = rotl(
                        (blocks[i - 3] ^ blocks[i - 8] ^ blocks[i - 14] ^ blocks[i - 16]), 
                    1); 
            }
            older = origin; 
            // repeat sub-block times to calculate the result. 
            constexpr auto ft = [] (uint32_t b, uint32_t c, uint32_t d, size_t cnt) constexpr -> uint32_t {
                if (cnt < 20) {
                    return (b & c) | ((~b) & d); 
                } else if (cnt < 40) {
                    return b ^ c ^ d; 
                } else if (cnt < 60) {
                    return (b & c) | (b & d) | (c & d); 
                } else {
                    assert (cnt < 80); 
                    return b ^ c ^ d; 
                }
            }; 
            constexpr uint32_t k[] = {0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6, }; 
            static_assert (sizeof k / sizeof (uint32_t) >= SUB_BLOCK / 20); 
            for (i = 0; i < SUB_BLOCK; ++i) {
                tmp.b = origin.a; 
                tmp.c = rotl(origin.b, 30); 
                tmp.d = origin.c; 
                tmp.e = origin.d; 
                tmp.a = rotl(origin.a, 5) 
                    + ft(origin.b, origin.c, origin.d, i) + origin.e
                    + blocks[i] + k[i / 20]; 
                origin = tmp; 
            }
            origin.a += older.a; 
            origin.b += older.b; 
            origin.c += older.c; 
            origin.d += older.d; 
            origin.e += older.e; 
        }
        return {origin.a, origin.b, origin.c, origin.d, origin.e}; 
    }
}