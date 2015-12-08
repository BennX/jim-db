#pragma once
#include <iostream>
#include "../thread/spinlock.h"
#include <map>

class Benchmark
{
public:
    static Benchmark& getInstance();
    void init(const int count, const std::string& filename);
    //@param[in] i the task id
    //@param[in] time the time in µs it took
    void add(const int& i , const unsigned long long& time);
    friend std::ostream& operator<<(std::ostream& os, Benchmark& obj);

private:
    Benchmark();
    ~Benchmark() {};

    jimdb::tasking::SpinLock m_spin;
    std::map<int, unsigned long long> m_values;
    int m_counter;

	std::string m_filename;
	int m_doneValue;
};

