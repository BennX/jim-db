#include "benchmark.h"
#include <mutex>
#include <sstream>
#include "../log/logger.h"

Benchmark& Benchmark::getInstance()
{
    static Benchmark instance;
    return instance;
}

Benchmark& Benchmark::init(const int count, const std::string& filename)
{
    m_doneValue = count;
    m_filename = filename;
    m_values.reserve(m_doneValue);//prealocate
    return *this;
}

Benchmark& Benchmark::setType(const Type t)
{
    m_logType = t;
    return *this;
}

void Benchmark::add(const Type t, const unsigned long long& time)
{
    std::lock_guard<jimdb::tasking::SpinLock> lock(m_spin);
    //else we skip
    if (t == m_logType)
    {
        m_values.push_back(time);
        if (m_values.size() == m_doneValue)
            LOG_DEBUG << *this;
    }
}

Benchmark::Benchmark(): m_counter(0), m_logType(INSERT), m_filename("bench.dat"),
    m_doneValue(1000000)
{
    m_values.reserve(m_doneValue);//prealocate
}

std::ostream& operator<<(std::ostream& os, Benchmark& obj)
{
    std::stringstream ss;
    ss << obj.m_values[0];
    for (auto i = 1; i < obj.m_values.size(); i++)
    {
        ss << ";" << obj.m_values[i];
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