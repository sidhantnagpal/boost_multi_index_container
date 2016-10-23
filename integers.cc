#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

#include <algorithm>
#include <iostream>
#include <random>
#include <chrono>

namespace tags {
struct asc {};
struct desc {};
}

using namespace boost::multi_index;

int main()
{
    boost::multi_index_container<
      int,
      indexed_by<
        ordered_unique<
          tag<tags::asc>,
          identity<int>,
          std::less<int>
        >,
        ordered_unique<
          tag<tags::desc>,
          identity<int>,
          std::greater<int>
        >
      >
    > mic;

    auto benchmark_op = [](auto&& m, auto&& operation)
    {
        static const int Iterations = 1e6;

        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < Iterations; ++i)
            operation(m);
        auto end = std::chrono::steady_clock::now();

        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
    };

    auto benchmark = [benchmark_op](auto&& m)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> rng;

        benchmark_op(m, [&](auto&& m) { m.insert(rng(gen)); });

        int found = 0; // its only reason is to avoid the compiler to optimize all the lookups
        benchmark_op(m, [&](auto&& m) { found += m.find(rng(gen)) != m.end(); });
        std::cout << found << std::endl;
    };

    benchmark(mic);

#if 0
    auto&& v = m.get<tags::desc>();

    for (auto&& i : v)
        std::cout << i << std::endl;
#endif
}
