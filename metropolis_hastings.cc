#include <iostream>
#include <memory>
#include <cmath>
#include <map>
#include <random>
#include <functional>
#include <exception>

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

std::function<float(float)> target(float mu, float sigma)
{
    return [mu, sigma](float x) {
        float a = (x - mu) / sigma;
        return std::exp(-a * a / 2) / (sigma * std::sqrt(2 * M_PI));
    };
}

void print_estimation(const std::map<int, int> &estimation, size_t i)
{
    const double scaling = 200;
    std ::cout << "Estimation at iteration " << i << ":" << std::endl;
    for (auto x : estimation)
    {
        std::cout << x.first << "\t"
                  << std::string(std::round(x.second * scaling / (i + 1)), '*')
                  << std::endl
                  << std::endl;
    }
}

void metropolis_hastings(size_t n_iterations)
{
    auto gen = std::make_shared<std::mt19937>(std::random_device{}());
    auto target_distribution = target(-60, 3);
    auto uniform =
        Distribution<std::uniform_real_distribution<float>>(gen, 0, 1);

    std::map<int, int> estimated_distribution;
    float teta = 7;
    auto proposal_distribution =
        Distribution<std::normal_distribution<float>>(gen, teta, 1);
    for (size_t i = 0; i < n_iterations; ++i)
    {
        proposal_distribution =
            Distribution<std::normal_distribution<float>>(gen, teta, 1);
        auto teta_ = proposal_distribution();
        float a = target_distribution(teta_) / target_distribution(teta);
        if (a >= 1 || a >= uniform())
            teta = teta_;

        ++estimated_distribution[std::round(teta)];
        if (i % (n_iterations / 10) == 0 && i != 0)
        {
            std::cout << "Current teta: " << teta << std::endl;
            print_estimation(estimated_distribution, i);
        }
    }
}

int main(int argc, char **argv)
{
    int return_code = 0;
    if (argc != 3)
    {
        std::cerr << "There should be 2 arguments: in.txt and out.txt."
                  << std::endl;
        return_code = 1;
    }
    else
    {
        std::cout << "in: " << argv[1] << " out: " << argv[2] << std::endl;
        metropolis_hastings(10000000);
    }
    return return_code;
}