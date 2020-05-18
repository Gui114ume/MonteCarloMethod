#ifndef GENERALEDOMONTECARLOINTEGRATOR_HPP
#define GENERALEDOMONTECARLOINTEGRATOR_HPP

#include <cstdlib>
#include <vector>

#include "IMonteCarloIntegrator.hpp"

//La classe GeneralEDOMonteCarloIntegrator permet d'intégrer toutes les EDO de la forme suivante :
// dy/dt = f( y (x) , x ), avec y(x_0) donné à l'avance.

class GeneralEDOMonteCarloIntegrator
{
public:
    GeneralEDOMonteCarloIntegrator(double BorneInf,
                                   double BorneSup,
                                   double ecart,
                                   double y_0)
    {
        this->EcartEntreValeur = ecart;
        this->MinIntervalleDIntegration.push_back(BorneInf);
        this->MaxIntervalleDIntegration.push_back(BorneSup);
        this->EDOSolution.push_back(y_0);
    };
    ~GeneralEDOMonteCarloIntegrator() {};

    void IntegreGeneralEDO(double (*function)(std::vector<double>&),
                           IMonteCarloIntegrator* MCIntegrator)
    {
        double resultat = this->EDOSolution[0];

        std::vector<double> vect_min;
        vect_min.push_back(this->MinIntervalleDIntegration[0]);
        auto& rvect_min = vect_min;

        std::vector<double> vect_max;
        vect_max.push_back(rvect_min[0] + this->EcartEntreValeur);
        auto& rvect_max = vect_max;

        MCIntegrator->SetDistributionParameter(rvect_min,
                                               rvect_max);

        for(int i = 0 ; i < static_cast<int> ( (  this->MaxIntervalleDIntegration[0] - this->MinIntervalleDIntegration[0]) / this->EcartEntreValeur) ; i++)
        {
            resultat += MCIntegrator->ComputeIntegrale(function,
                                                       resultat,
                                                       rvect_min,
                                                       rvect_max);
            EDOSolution.push_back(resultat);

            rvect_min[0] = rvect_max[0];
            rvect_max[0] += this->EcartEntreValeur;

            MCIntegrator->SetDistributionParameter(rvect_min,
                                                   rvect_max);

        }
        std::cout << resultat << std::endl;
    };
private:
    std::vector<double> EDOSolution;
    std::vector<double> MinIntervalleDIntegration;
    std::vector<double> MaxIntervalleDIntegration;
    double EcartEntreValeur; //doit être faible ! L'approximation utilisé doit être la plus vrai possible : y(xi) ~ y(xi - 1)

};










#endif // GENERALEDOMONTECARLOINTEGRATOR_HPP
