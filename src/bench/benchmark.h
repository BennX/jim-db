#pragma once
#include <iostream>
#include "../thread/spinlock.h"
#include <vector>

class Benchmark
{
public:

    enum Type
    {
        INSERT, FIND
    };

    static Benchmark& getInstance();
    Benchmark& init(const int count, const std::string& filename);
    Benchmark& setType(const Type t);
    //only add the type which get logged atm
    //@param[in] t Type to add
    //@param[in] time the time in µs it took
    void add(const Type t, const unsigned long long& time);
    friend std::ostream& operator<<(std::ostream& os, Benchmark& obj);

private:
    Benchmark();
    ~Benchmark() {};

    jimdb::tasking::SpinLock m_spin;
    std::vector<unsigned long long> m_values;
    int m_counter;

    Type m_logType;

    std::string m_filename;
    int m_doneValue;
};

