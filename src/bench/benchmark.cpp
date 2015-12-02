#include "benchmark.h"
#include <mutex>
#include <sstream>
#include "../log/logger.h"
#include "../tasking/taskqueue.h"


Benchmark& Benchmark::getInstance()
{
    static Benchmark instance;
    return instance;
}

void Benchmark::add(const int& i, const unsigned long long& time)
{
    std::lock_guard<jimdb::tasking::SpinLock> lock(m_spin);
    if(m_values.count(i))
    {
        auto l_cur = m_values[i];
        m_values[i] = l_cur + time;
    }
    else
    {
        m_values[i] = time;
        m_counter++;
    }
}

std::ostream& operator<<(std::ostream& os, Benchmark& obj)
{
    std::stringstream ss;
    for(auto it = obj.m_values.begin(); it != obj.m_values.end(); ++it)
    {
        ss << it->first << ";" << it->second;
        //linebreak after 10k
        if(it->first % 10000 == 0)
            ss << std::endl;
    }
    obj.m_values.clear();
    LOG_DEBUG << ss.str();
    return os;
}