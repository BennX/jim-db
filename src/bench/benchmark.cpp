#include "benchmark.h"
#include <mutex>
#include <sstream>
#include "../log/logger.h"

Benchmark& Benchmark::getInstance()
{
    static Benchmark instance;
    return instance;
}

void Benchmark::init(const int count, const std::string& filename)
{
    m_doneValue = count;
    m_filename = filename;
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
    if(m_values.size() == 1000000)
        LOG_DEBUG << *this;
}

Benchmark::Benchmark(): m_counter(0), m_filename("bench.dat"), m_doneValue(1000000) {}

std::ostream& operator<<(std::ostream& os, Benchmark& obj)
{
    std::stringstream ss;
    for(auto it = obj.m_values.begin(); it != obj.m_values.end(); ++it)
    {
        ss << ";" << it->second;
    }
    obj.m_values.clear();

    std::ofstream of(obj.m_filename, std::ios::out | std::ios::app);
    of << ss.str();
    of << std::endl;
    LOG_DEBUG << "bench done";
    //here we can close this
    exit(EXIT_SUCCESS);
    return os;
}