export module generator; 

import <optional>; 
import <coroutine>; 

using std::optional; 

export namespace generator {
    template <typename T = void> 
    class Generator; 
}

namespace generator {
    template <typename T> 
    class Generator {
        public: 
            struct promise_type {
                optional<T> value_; 
                Generator get_return_object() {
                    return {std::coroutine_handle<promise_type>::from_promise(*this)}; 
                }
                std::suspend_always initial_suspend() noexcept {
                    return {}; 
                }
                std::suspend_always final_suspend() noexcept {
                    return {}; 
                }
                void return_void() {
                }
                void unhandled_exception() {
                    std::terminate(); 
                }
                std::suspend_always yield_value(T v) {
                    value_ = std::move(v);    
                    return {}; 
                }
            }; 
            ~Generator() {
                h_.destroy(); 
            }
            bool has_next() {
                if (h_.promise().value_) 
                    return true; 
                if (h_.done()) 
                    return false; 
                h_.resume(); 
                return true; 
            }
            optional<T> const &peek() const {
                return h_.promise().value_; 
            }
            explicit(true) operator bool() {
                return has_next(); 
            }
            T next() {
                has_next(); 
                T ret = std::move(h_.promise().value_.value()); 
                h_.promise().value_ = std::nullopt; 
                return ret; 
            }
            Generator (std::coroutine_handle<promise_type> h): h_(h) {} 
            Generator (Generator const &) = delete; 
        private: 
            std::coroutine_handle<promise_type> h_; 
    }; 

    template <> 
    class Generator<void> {
    }; 
}