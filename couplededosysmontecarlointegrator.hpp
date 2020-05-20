#ifndef COUPLEDEDOSYSMONTECARLOINTEGRATOR_HPP
#define COUPLEDEDOSYSMONTECARLOINTEGRATOR_HPP

#include "MonteCarloIntegrator.hpp"
#include "qcustomplot.h"

//On a copier le code de GeneralEDOMonteCarloIntegrator,
//En bref, on resoud dX/dt = F(X,t) avec X un vecteur, F une fonction vectorielle, de memes dimensions

class CoupledEDOSysMonteCarloIntegrator
{
public:
    CoupledEDOSysMonteCarloIntegrator(double BorneInf,
                                      double BorneSup,
                                      double ecart,
                                      std::vector<double> y_0)
    {
        this->EcartEntreValeur = ecart;
        this->MinIntervalleDIntegration.push_back(BorneInf);
        this->MaxIntervalleDIntegration.push_back(BorneSup);
        this->EDOSolution.resize(y_0.size());
        for(unsigned int i = 0 ; i < y_0.size() ; i++)
        {
            this->EDOSolution[i].push_back(y_0[i]);
        }
    };
    ~CoupledEDOSysMonteCarloIntegrator() {};

    void IntegreCoupledEDO(std::vector<double(*)(std::vector<double>&)> function,
                            IMonteCarloIntegrator* MCIntegrator)
    {
        std::vector<double> resultat(this->EDOSolution.size());
        std::vector<double> tmp_resultat(this->EDOSolution.size());
        for(unsigned int i = 0 ; i < this->EDOSolution.size() ; i++)
        {
            resultat[i] = this->EDOSolution[i][0];
        }

        std::vector<double> vect_min;
        vect_min.push_back(this->MinIntervalleDIntegration[0]);
        auto& rvect_min = vect_min;

        std::vector<double> vect_max;
        vect_max.push_back(rvect_min[0] + this->EcartEntreValeur);
        auto& rvect_max = vect_max;

        MCIntegrator->SetDistributionParameter(rvect_min,
                                               rvect_max);

        //i est le "pas de temps"
        for(int i = 0 ; i < static_cast<int> ( (  this->MaxIntervalleDIntegration[0] - this->MinIntervalleDIntegration[0]) / this->EcartEntreValeur) ; i++)
        {
            tmp_resultat = MCIntegrator->ComputeIntegrale(function,
                                                          resultat,
                                                          rvect_min,
                                                          rvect_max);
            //j est la taille du vecteur
            for(unsigned int j = 0 ; j < tmp_resultat.size() ; j++)
            {
                resultat[j] += tmp_resultat[j]; // y_i = y_i-1 + ESTIMATEUR_MC
                EDOSolution[j].push_back(resultat[j]); // on stocke chaque composante de y_i
            }


            rvect_min[0] = rvect_max[0];
            rvect_max[0] += this->EcartEntreValeur;

            MCIntegrator->SetDistributionParameter(rvect_min,
                                                   rvect_max);

        }
        std::cout << EDOSolution[0][EDOSolution[0].size() - 1] << std::endl; //premiere composante du vecteur solution au temps "i"
        std::cout << EDOSolution[1][EDOSolution[1].size() - 1] << std::endl;


    };

    void makePlot(QCustomPlot* QCP)
    {
        //faire une boucle qui addgraph pour chaque composte de EDOSolution
        QVector<QVector<double>> y(EDOSolution.size());
        for(unsigned int i = 0 ; i < EDOSolution.size() ; i++)
            y[i] = QVector<double>::fromStdVector(this->EDOSolution[i]);
        QVector<double> x;
        for(int i = 0 ; i < y[0].size() ; i++)
            x.push_back(this->MinIntervalleDIntegration[0] + i * this->EcartEntreValeur);

        for(int i = 0 ; i < y.size() ; i++)
        {
            QCP->addGraph();
            QCP->graph(i)->setData(x, y[i]);
            QCP->replot();

        }
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
    std::vector<std::vector<double>> EDOSolution;
    std::vector<double> MinIntervalleDIntegration;
    std::vector<double> MaxIntervalleDIntegration;
    double EcartEntreValeur; //doit être faible !
};

#endif // COUPLEDEDOSYSMONTECARLOINTEGRATOR_HPP
