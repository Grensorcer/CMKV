#pragma once
#include <memory>
#include <random>

template <typename DistributionType>
class Distribution
{
public:
    Distribution(std::shared_ptr<std::mt19937> gen, float a, float b)
        : gen_{ gen }
    {
        distribution_ = DistributionType(a, b);
    }

    void update(float a, float b)
    {
        distribution_ = DistributionType(a, b);
    }

    float operator()()
    {
        return distribution_(*gen_);
    }

protected:
    std::shared_ptr<std::mt19937> gen_;
    DistributionType distribution_;
};