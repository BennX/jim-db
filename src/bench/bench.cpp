#include "bench.h"
#include "benchmark.h"
#include "../log/logger.h"

Bench::Bench(const int& i): m_id(i), m_cycleStart(std::chrono::high_resolution_clock::now())
{
}

Bench::~Bench()
{
    Benchmark::getInstance().add( m_id, std::chrono::duration_cast<std::chrono::microseconds>
                                  (std::chrono::high_resolution_clock::now() -
                                   m_cycleStart).count());
}
