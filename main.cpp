#include "MonteCarloIntegrator.hpp"
#include "YIndependantedoMonteCarloIntegrator.hpp"
#include "GeneralEDOMonteCarloIntegrator.hpp"

#include <QApplication>

#include "mainwindow.h"
#include "qcustomplot.h"

#include <QtDebug>
#include <QString>
#include <QFile>
#include <QVector>
#include <QtMath> // M_PI



//function est la fonction de n variables à intégrer
double function(std::vector<double>& x)
{
    return x[0];
}

//Permet de resize et de remplir deux vecteurs, x_min et x_max
//Le fichier doit respecter un certain format cependant
void SetBornes(std::vector<double>& x_min,
               std::vector<double>& x_max,
               QString filename)
{
//trouver comment lire dans un fichier en c++
//on ecrit d'abord la dimension de la fonction
//on lit ensuite le min puis le max, un par ligne
     QFile file(filename);
     if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
     {
         qDebug() << "fail to open filename" << endl;
         exit(-1);
     }

    int dim = 0;

    QTextStream in(&file);

    QString line = in.readLine();

    dim = line.toInt();

    //logiquement, il y a dim*2 elements à lire dans le fichier
    for(int i = 0 ; i < dim ; i++)
    {
        line = in.readLine();
        double min = line.toDouble();
        x_min.push_back(min);

        line = in.readLine();
        double max = line.toDouble();
        x_max.push_back(max);
    }
    file.close();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow* win = new MainWindow();
    QCustomPlot *customPlot = new QCustomPlot(win);
    //trop petit sur la mainwindow, il faut reussir à l'agrandir.

    win->show();

    std::vector<double> x_min;
    std::vector<double> x_max;
    double y_0 = 1;
    //utilisation de reference pour des questions de non-recopie
    auto& ref_x_min = x_min;
    auto& ref_x_max = x_max;

    //possible de remplir les vecteurs à la main
    //   std::vector<double> x_min(2,0);
    //   std::vector<double> x_max(2,2);

    //possible d'utiliser un fichier de configuration pour remplir les vecteurs
    //SetBornes(ref_x_min,
    //          ref_x_max,
    //          QString(argv[1]));

    x_min.push_back(0);
    x_max.push_back(1);


    //on doit faire ça pour mettre à jour le champs "dimension"
    MonteCarloIntegrator* TestMonteCarloIntegrator = new MonteCarloIntegrator(ref_x_min,
                                                                              ref_x_max);

    //on cree un integrateur d'EDO
    //yIndependantEDOMonteCarloIntegrator* testEDOIntegrator = new yIndependantEDOMonteCarloIntegrator(0, 1, 0.05);


    //c'est ameliorable, un utilisateur peut sans le faire exprès, mettre des aleurs dans le vecteur passé à l'integrator,
    //et en meme temps donner des valeurs differentes dans EDOintegrator, voire meme des dimensions de vecteurs differentes (supposé )


    //on integre avec l'integrateur precedemment configuré
    //testEDOIntegrator->IntegreEDO(function,
    //                              TestMonteCarloIntegrator);

    GeneralEDOMonteCarloIntegrator* test_genedo = new GeneralEDOMonteCarloIntegrator(x_min[0], x_max[0], 0.05, y_0);
    test_genedo->IntegreGeneralEDO(function, TestMonteCarloIntegrator);
    test_genedo->makePlot(customPlot);


    delete test_genedo;
    delete TestMonteCarloIntegrator;

    return app.exec();
}
