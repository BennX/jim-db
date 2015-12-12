#include "bench.h"
#include "benchmark.h"
#include "../log/logger.h"

Bench::Bench(): m_cycleStart(std::chrono::high_resolution_clock::now()), m_type(Benchmark::Type::INSERT)
{
}

void Bench::setType(const Benchmark::Type t)
{
    m_type = t;
}

Bench::~Bench()
{
    Benchmark::getInstance().add(m_type, std::chrono::duration_cast<std::chrono::microseconds>
                                 (std::chrono::high_resolution_clock::now() -
                                  m_cycleStart).count());
}
