		Intégration par une méthode de Monte Carlo
		------------------------------------------

1 - Introduction
----------------

Nous proposons dans ce projet une classe appelé " MonteCarloIntegrator ", permettant de calculer des intégrales définies, pour des fonctions de plusieurs variables. Nous nous sommes basés sur les document "montecarlointegrale.pdf" et https://jotterbach.github.io/content/posts/mc_ode/2018-08-08-MonteCarloODE  pour l'implémentation.

2 - Utilisation
---------------

La classe comporte différents champs et méthodes, en nombre restreint.
* Il est nécessaire de remplir des vecteurs contenant les bornes inférieures et supérieures pour chacune des variables. Il faut également connaitre le nombre de variable de la fonction à intégrer. Ce dernier est appelé la dimension dans la suite.
* Il est nécessaire d'instancier un objet de type MonteCarloIntegrator avant de lancer l'intégration.
* La fonction à intégrer est donner à la méthode "ComputeIntegral()" via un pointeur de fonction.

3 - Le fonctionnement de la méthode d'intégration
-------------------------------------------------

La methode d'intégration calcule une approximation en utilisant la formule suivante :  I = (1/N) * SOMME( f(X_n) / p(X_n) ) pour n = 0:N-1
* N est le nombre de poins de l'espace, tiré au hasard
* p(x) est la distribution de probabilité utilisé pour le calcul
* f(x) est la fonction à intégrer
* X_n est le n-ème points tirés

Le nombre N est fixé à 10 000 dans le code source, plus il est élevé, plus la précision sera bonne.

ATTENTION : X_n est un vecteur de R_d (on tire donc d nombres au hasard pour créer 1 point de R_d  )

4 - Estimation de l'erreur sur la valeur exacte
-----------------------------------------------

//trouver comment faire


5 - Ouverture
-------------

Ce calcul d'intégrale peut être utilisé pour intégrer certaines EDO. Nous créerons une classe appelé " IntegrableEdoIntegrator " très bientôt.




Gui114ume