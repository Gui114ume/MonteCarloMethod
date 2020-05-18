#ifndef yINDEPENDANTEDOMONTECARLOINTEGRATOR_HPP
#define yINDEPENDANTEDOMONTECARLOINTEGRATOR_HPP

#include <cstdlib>
#include <vector>

#include "IMonteCarloIntegrator.hpp"

//La classe EDOMonteCarloIntegrator permet d'intégrer certaines EDO qui peuvent s'écrire sous la forme suivant :
//F(x) - F(x_0) = intégrale(x_0, x) {  f(x) } , avec f(x) connue explicitement. F(x) est la fonction que l'on cherche à connaitre

class yIndependantEDOMonteCarloIntegrator
{
public:
    yIndependantEDOMonteCarloIntegrator(double BorneInf,
                                        double BorneSup,
                                        double ecart)
    {
        this->EcartEntreValeur = ecart;
        this->MinIntervalleDIntegration.push_back(BorneInf);
        this->MaxIntervalleDIntegration.push_back(BorneSup);
    };
    ~yIndependantEDOMonteCarloIntegrator() {};

    void IntegreEDO(double (*function)(std::vector<double>&),
                    IMonteCarloIntegrator* MCIntegrator)
    {
        std::vector<double> tmp;
        auto& rtmp = tmp;
        tmp.push_back(this->MinIntervalleDIntegration[0]);

        double resultat = function(rtmp); //mettre ici function(x_0) à la place de 0, c'est y_0 en fait ici

        std::vector<double> vect_min;
        vect_min.push_back(this->MinIntervalleDIntegration[0]);
        auto& rvect_min = vect_min;

        std::vector<double> vect_max;
        vect_max.push_back(rvect_min[0] + this->EcartEntreValeur);
        auto& rvect_max = vect_max;

        MCIntegrator->SetDistributionParameter(rvect_min,
                                               rvect_max);

        for(int i = 0 ; i < static_cast<int> ( (  this->MaxIntervalleDIntegration[0] - this->MinIntervalleDIntegration[0]) / this->EcartEntreValeur); i++)
        {
            resultat += MCIntegrator->ComputeIntegrale(function,
                                                       rvect_min,
                                                       rvect_max); //ici il faut plutot mettre min + ecart
            EDOSolution.push_back(resultat);

            rvect_min[0] = rvect_max[0];
            rvect_max[0] += this->EcartEntreValeur;

            MCIntegrator->SetDistributionParameter(rvect_min,
                                                   rvect_max);

            std::cout << resultat << std::endl;
        }
    }

private:
    std::vector<double> EDOSolution;
    std::vector<double> MinIntervalleDIntegration;
    std::vector<double> MaxIntervalleDIntegration;
    double EcartEntreValeur;
};

#endif // yINDEPENDANTEDOMONTECARLOINTEGRATOR_HPP
