#ifndef GENERALEDOMONTECARLOINTEGRATOR_HPP
#define GENERALEDOMONTECARLOINTEGRATOR_HPP

#include <cstdlib>
#include <vector>

#include "qcustomplot.h"
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

    void makePlot(QCustomPlot* QCP)
    {
        QVector<double> y = QVector<double>::fromStdVector(this->EDOSolution);
        QVector<double> x;
        for(int i = 0 ; i < y.size() ; i++)
            x.push_back(this->MinIntervalleDIntegration[0] + i * this->EcartEntreValeur);

        QCP->addGraph();
        QCP->graph(0)->setData(x, y);
        // give the axes some labels:
        QCP->xAxis->setLabel("x");
        QCP->yAxis->setLabel("y");
        // set axes ranges, so we see all data:
        QCP->xAxis->setRange(this->MinIntervalleDIntegration[0], this->MaxIntervalleDIntegration[0]);
        QCP->yAxis->setRange(0, 1); //il faudrait connnaitre y_min et y_max
        QCP->rescaleAxes();
        QCP->resize(400,400);
        QCP->replot();
    }

private:
    std::vector<double> EDOSolution;
    std::vector<double> MinIntervalleDIntegration;
    std::vector<double> MaxIntervalleDIntegration;
    double EcartEntreValeur; //doit être faible ! L'approximation utilisé doit être la plus vrai possible : y(xi) ~ y(xi - 1)

};










#endif // GENERALEDOMONTECARLOINTEGRATOR_HPP
