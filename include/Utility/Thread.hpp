/**
 * @File Thread.hpp
 * @brief Threading specific helpers.
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <atomic>
#include <chrono>
#include <thread>
#include <mutex>
#include <shared_mutex>


template <typename SharedMutex>
class shared_lock_guard {
  public:
    explicit shared_lock_guard(SharedMutex& shared_mutex) : m_mutex(shared_mutex)
    { m_mutex.lock_shared(); }

    ~shared_lock_guard()
    { m_mutex.unlock_shared(); }

  private:
    SharedMutex& m_mutex;
};

namespace this_thread = std::this_thread;

inline void
sleep_for_sec(float sec)
{ this_thread::sleep_for(std::chrono::duration<float>(sec)); }

inline void
sleep_for_ms(float ms)
{ this_thread::sleep_for(std::chrono::duration<float, std::milli>(ms)); }

inline void
sleep_for_us(float us)
{ this_thread::sleep_for(std::chrono::duration<float, std::micro>(us)); }

inline void
sleep_for_ns(float ns)
{ this_thread::sleep_for(std::chrono::duration<float, std::nano>(ns)); }

