#ifndef IMONTECARLOINTEGRATOR_HPP
#define IMONTECARLOINTEGRATOR_HPP

#include <cstdlib>
#include <iostream>
#include <QString>

#include <vector>


class IMonteCarloIntegrator
{
public:
    virtual ~IMonteCarloIntegrator() {};

    virtual void setDimension(unsigned int dimension) = 0;
    virtual unsigned int getDimension() const = 0;

    virtual double ComputeIntegrale(double (*function)(std::vector<double>& ),
                                   std::vector<double>& x_min,
                                   std::vector<double>& x_max)  = 0;

    virtual double ComputeIntegrale(double (*function)(std::vector<double>& ),
                                    double y,
                                    std::vector<double>& x_min,
                                    std::vector<double>& x_max)  = 0;

    virtual std::pair<double, double> ComputeIntegraleAndVariance(double (*function)(std::vector<double>&),
                                                                  std::vector<double>& x_min,
                                                                  std::vector<double>& x_max) = 0;

    virtual std::pair<double, double> ComputeIntegraleAndVariance(double (*function)(std::vector<double>&),
                                                                  double y,
                                                                  std::vector<double>& x_min,
                                                                  std::vector<double>& x_max) = 0;

    virtual void SetDistributionParameter(std::vector<double>& min,
                                          std::vector<double>& max) = 0;
protected:
    IMonteCarloIntegrator() {};
};

#endif
