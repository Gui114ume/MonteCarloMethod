#ifndef MONTECARLOINTEGRATOR_HPP
#define MONTECARLOINTEGRATOR_HPP

#include <cstdlib>
#include <iostream>
#include <random>
#include <QString>

#include <omp.h>

#include "IMonteCarloIntegrator.hpp"

//Intègre une fonction de "dimension" variables, entre x_max et x_min (pour chacune des variables)
//La méthode utilisé est une méthode de Monte Carlo utilisant un RNG de loi uniforme
class MonteCarloIntegrator : public IMonteCarloIntegrator
{
public:
    MonteCarloIntegrator(std::vector<double>& x_min,
                         std::vector<double>& x_max)
    {
        this->distribution.resize(x_min.size());
        for(unsigned int i = 0 ; i < x_min.size() ; i++)
            this->distribution[i] = std::uniform_real_distribution<double>(x_min[i],
                                                                           x_max[i]);
        this->setDimension(x_min.size());
    };

    virtual ~MonteCarloIntegrator() {};


    virtual void SetDistributionParameter(std::vector<double>& min,
                                          std::vector<double>& max) override;

    virtual void setDimension(unsigned int dimension) override
    {
        this->dimension = dimension;
    }
    virtual unsigned int getDimension() const override
    {
        std::cout<< this->dimension << std::endl;
        return this->dimension;
    };


    //Calcule l'intégrale définie, d'une fonction de plusieurs variables
    //Utilise plusieurs loi uniforme (car les bornes sont différentes pour chaque variable)
    virtual double ComputeIntegrale(double (*function)(std::vector<double>&),
                                   std::vector<double>& x_min,
                                   std::vector<double>& x_max) override;

    virtual double ComputeIntegrale(double (*function)(std::vector<double>& ),
                                    double y,
                                    std::vector<double>& x_min,
                                    std::vector<double>& x_max)  override;

    virtual std::pair<double, double> ComputeIntegraleAndVariance(double (*function)(std::vector<double>&),
                                                                  std::vector<double>& x_min,
                                                                  std::vector<double>& x_max) override;

    virtual std::pair<double, double> ComputeIntegraleAndVariance(double (*function)(std::vector<double>&),
                                                                  double y,
                                                                  std::vector<double>& x_min,
                                                                  std::vector<double>& x_max) override;
private:
    //On a besoin d'un vecteur de generator, pour s'assurer que les nombres aleatoires ne se repetent pas, et qu'il n'y ait pas de race condition
    //D'après les test ça marche quand meme cependant ...
    std::default_random_engine generator;
    std::vector<std::uniform_real_distribution<double>> distribution;
    unsigned int dimension;
};





void MonteCarloIntegrator::SetDistributionParameter(std::vector<double> &min, std::vector<double> &max)
{
    this->distribution[0] = std::uniform_real_distribution<double>(min[0], max[0]);
}

double MonteCarloIntegrator::ComputeIntegrale(double (*function)(std::vector<double>&), std::vector<double> &x_min, std::vector<double> &x_max)
{
    double resultat = 0;

    std::vector<double> random_value;
    random_value.resize(this->dimension);

    omp_set_num_threads(4);
    #pragma omp parallel for reduction(+:resultat)
    for(int i = 0 ; i < 100000 ; i++)
    {
        for(unsigned int j = 0 ; j < this->dimension  ; j++)
        {
            random_value[j] = this->distribution[j](this->generator);
        }
        resultat += function(random_value);
    }

    for(unsigned int i = 0 ; i < this->dimension ; i++)
    {
        resultat *= (x_max[i] - x_min[i]);
    }
    resultat *= 0.00001;
    //std :: cout << resultat << std::endl;
    return resultat;
}

double MonteCarloIntegrator::ComputeIntegrale(double (*function)(std::vector<double> &), double y, std::vector<double> &x_min, std::vector<double> &x_max)
{
    double resultat = 0;

    std::vector<double> random_value;
    random_value.resize(this->dimension + 1);
    random_value[0] = y;

    omp_set_num_threads(4);
    #pragma omp parallel for reduction(+:resultat)
    for(int i = 0 ; i < 100000 ; i++)
    {
        for(unsigned int j = 1 ; j < this->dimension + 1 ; j++)
        {
            random_value[j] = this->distribution[j - 1](this->generator);
        }
        resultat += function(random_value);
    }

    for(unsigned int i = 0 ; i < this->dimension ; i++)
    {
        resultat *= (x_max[i] - x_min[i]);
    }
    resultat *= 0.00001;

    return resultat;
}

std::pair<double, double> MonteCarloIntegrator::ComputeIntegraleAndVariance(double (*function)(std::vector<double> &), std::vector<double> &x_min, std::vector<double> &x_max)
{
    double resultat = 0;
    double variance = 0;
    std::pair<double, double> res_pair;

    std::vector<double> random_value;
    random_value.resize(this->dimension );

    omp_set_num_threads(4);
    #pragma omp parallel for reduction(+:resultat) reduction(+:variance)
    for(unsigned int i = 0 ; i < 100000 ; i++)
    {
        for(unsigned int j = 0 ; j < this->dimension ; j++)
        {
            random_value[j] = this->distribution[j](this->generator);
        }
        double tmp = function(random_value);
        resultat += tmp;
        variance += tmp * tmp;

    }

    for(unsigned int i = 0 ; i < this->dimension ; i++)
    {
        //la distribution de probabilité utilisé est forcément uniforme, d'ou l'expression ci-dessous
        resultat *= (x_max[i] - x_min[i]);
        variance *= (x_max[i] - x_min[i]) * (x_max[i] - x_min[i]);
    }

    resultat *= 0.00001;
    variance *= 0.00001;

    variance -= resultat*resultat;

    res_pair.first = resultat;
    res_pair.second = variance;
    // intervalles de confiance : min = resultat - ( (1.96 * sqrt(variance))/ sqrt(10000))
    //                            max = resultat + ( (1.96 * sqrt(variance))/ sqrt(10000))
    return res_pair;
}

std::pair<double, double> MonteCarloIntegrator::ComputeIntegraleAndVariance(double (*function)(std::vector<double> &), double y, std::vector<double> &x_min, std::vector<double> &x_max)
{
    double resultat = 0;
    double variance = 0;
    std::pair<double, double> res_pair;

    std::vector<double> random_value;
    random_value.resize(this->dimension + 1);
    random_value[0] = y;

    omp_set_num_threads(4);
    #pragma omp parallel for reduction(+:resultat) reduction(+:variance)
    for(unsigned int i = 0 ; i < 100000 ; i++)
    {
        for(unsigned int j = 1 ; j < this->dimension + 1; j++)
        {
            random_value[j] = this->distribution[j - 1](this->generator);
        }
        double tmp = function(random_value);
        resultat += tmp;
        variance += tmp * tmp;

    }

    for(unsigned int i = 0 ; i < this->dimension ; i++)
    {
        //la distribution de probabilité utilisé est forcément uniforme, d'ou l'expression ci-dessous
        resultat *= (x_max[i] - x_min[i]);
        variance *= (x_max[i] - x_min[i]) * (x_max[i] - x_min[i]);
    }

    resultat *= 0.00001;
    variance *= 0.00001;

    variance -= resultat*resultat;

    res_pair.first = resultat;
    res_pair.second = variance;
    // intervalles de confiance : min = resultat - ( (1.96 * sqrt(variance))/ sqrt(10000))
    //                            max = resultat + ( (1.96 * sqrt(variance))/ sqrt(10000))
    return res_pair;
}

#endif
