/*!=================================================================!*/
/*!= E.Incerti - eric.incerti@univ-eiffel.fr                       =!*/
/*!= Université Gustave Eiffel                                     =!*/
/*!= Code "squelette" pour prototypage avec libgfl.7e              =!*/
/*!=================================================================!*/

/* le seul #include nécessaire a priori
 * contient les libs C standards et OpenGl */
#include <gfl.h>

#include <vector>
#include <iostream>

#include "../include/PMat.hpp"
#include "../include/Link.hpp"

/* tailles initiales de la fenêtre graphique (en pixels)     */
#define WWIDTH 720
#define WHEIGHT 540

/* -----------------------------------------------------------------------
 * ici, en général pas mal de variables GLOBALES
 * - les variables de données globales (points, vecteurs....)
 * - les FLAGS de dialogues
 * - les paramètres de dialogue
 * - ......
 * Pas trop le choix, puisque TOUT passe par des fonctions <void f(void)>
 * ----------------------------------------------------------------------- */

double Fe = 500;

std::vector<PMat> pmats;
std::vector<Link> links;
PMat ref = PMat(0, Point(0, 0, 0), Vect(0, 0, 0), 0);
std::vector<Link> gravity;
double viscosite = 0.1f;
std::vector<Link> poutre;
double gravity_value = 9.8;

/* la fonction d'initialisation : appelée 1 seule fois, au début     */
static void init(void)
{
  /*!  Ici, pas de dessin, pas de calcul dynamique, rien que des initialisations
   *   -> allocation(s) de mémoire, init. des paramètres globaux,
   *      construction de modèles....
   *      ouverture de fichiers, pré-chargement de données....
   *
   *   Tout ce qu'il y a ici pourrait être écrit directement dans le main()
   *   juste après l'appel à gfl_InitWindow()
  !*/
  int rows = 30;                // Nombre de lignes
  int cols = 45;                // Nombre de colonnes
  double spacing = 0.5;         // Espacement entre les points
  double k = 0.05f * Fe * Fe;   // Constante de raideur
  double amor = viscosite * Fe; // Constante d'amortissement

  // Création des points
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      double x = j * spacing - (cols - 1) * spacing / 2.0;
      double y = -i * spacing;
      double z = 0;
      int type = (j == 0) ? 0 : 2; // Fixer uniquement le premier point de chaque ligne
      pmats.push_back(PMat(1, Point(x, y, z), Vect(0, 0, 0), type));
    }
  }

  // Création des liens horizontaux
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols - 1; j++)
    {
      int idx = i * cols + j;
      links.push_back(Link(&pmats[idx], &pmats[idx + 1], k, amor));
    }
  }

  // Création des liens verticaux
  for (int i = 0; i < rows - 1; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      int idx = i * cols + j;
      links.push_back(Link(&pmats[idx], &pmats[idx + cols], k, amor));
    }
  }

  // Création des liens diagonaux
  for (int i = 0; i < rows - 1; i++)
  {
    for (int j = 0; j < cols - 1; j++)
    {
      int idx = i * cols + j;
      links.push_back(Link(&pmats[idx], &pmats[idx + cols + 1], k, amor)); // Diagonale droite
      links.push_back(Link(&pmats[idx + 1], &pmats[idx + cols], k, amor)); // Diagonale gauche
    }
  }

  // Création des liens ponts horizontaux
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols - 2; j++)
    { // Sauter un point
      int idx = i * cols + j;
      links.push_back(Link(&pmats[idx], &pmats[idx + 2], k, amor));
    }
  }

  // Création des liens ponts verticaux
  for (int i = 0; i < rows - 2; i++)
  { // Sauter une ligne
    for (int j = 0; j < cols; j++)
    {
      int idx = i * cols + j;
      links.push_back(Link(&pmats[idx], &pmats[idx + 2 * cols], k, amor));
    }
  }

  // Gravité
  for (int i = 0; i < cols; i++)
  {
    for (int j = 0; j < rows; j++)
    {
      int idx = j * cols + i;
      gravity.push_back(Link(&pmats[idx], &ref, k, amor));
    }
  }
}

/* la fonction de contrôle : appelée 1 seule fois, juste APRES <init> */
static void ctrl(void)
{
  /*! Interface de dialogue (partie statique) : création des boutons, scrollbars....
   *  Tout ce qu'il y a ici pourrait être directement écrit dans la fonction init(),
   *  mais c'est plus 'propre' et plus pratique de séparer.
  !*/
  gfl_CreateScrollv_d("y", pmats[1].getPosPtr()->getYPtr(), -3, 3, "Position y du premier point");
  // gfl_CreateScrollv_d("y", pmats[pmats.size() - 2].getPosPtr()->getYPtr(), -3, 3, "Position y du dernier point");
  gfl_CreateScrollh_d("Fe", &Fe, 1, 100000, "Fe");
  gfl_CreateScrollh_d("viscosite", &viscosite, 0, 1, "viscosite");
  gfl_CreateScrollh_d("gravity", &gravity_value, 0, 500, "gravity");
}

/* la fonction de contrôle : appelée 1 seule fois, juste APRES <init> */
static void evts(void)
{
  /*! Interface de dialogue (partie dynamique) : les touches clavier, la souris ....
   *  Tout ce qu'il y a ici pourrait être directement écrit dans la fonction draw(),
   *  mais c'est plus 'propre' et plus pratique de séparer.
  !*/
}

/* la fonction de dessin : appelée en boucle (indispensable) */
static void draw(void)
{
  /*! C'est la fonction de dessin principale : elle ne réalise que de l'affichage
   *  sa mise à jour est automatique :
   *  - si un paramètre de contrôle est modifié
   *  - si la fonction <anim()> (calcul) est activée
   *  ATTENTION : surtout pas d'alloc. mémoire ici !!!
  !*/
  // gfl_Axes(); /* spécifique  : affiche le repère principal (O,x,y) */
  for (auto &link : links)
  {
    link.draw();
  }

  for (auto &pmat : pmats)
  {
    pmat.draw();
  }
}

/* la fonction d'animation : appelée en boucle draw/anim/draw/anim... (facultatif) */
static void anim(void)
{
  /*! C'est la fonction de 'calcul' qui va modifier les 'objets' affichés
   *  par la fonction de dessin (déplacement des objets, calculs divers...)
   *  Si elle n'est pas définie, c'est qu'il n'y a pas d'animation.
   *  ATTENTION : surtout pas d'alloc. mémoire ici !!!
  !*/
  for (auto &pmat : pmats)
  {
    pmat.update(1.f / Fe);
  }

  for (auto &link : links)
  {
    link.update();
  }
  for (auto &link : gravity)
  {
    link.update_gravity(gravity_value);
  }
  for (auto &link : poutre)
  {
    link.update();
  }
}

/* la fonction de sortie  (facultatif) */
static void quit(void)
{
  /*! Ici, les opérations à réaliser en sortie du programme
   *  - libération de la mémoire éventuellement alloueé dans <init()>
   *  - fermeture de fichiers ....
   *  - bilan et messages...
   *  Au final cette fonction est exécutée par un appel à <atexit()>
  !*/
}

/***************************************************************************/
/* La fonction principale : NE CHANGE (presque) JAMAIS                     */
/***************************************************************************/
int main(int argc, char **argv)
{
  /* 1°) creation de la fenetre - titre et tailles (pixels)  */
  gfl_InitWindow(*argv, WWIDTH, WHEIGHT);
  /* 2°) définition de la zone de travail en coord. réeelles *
   *     par défaut (pas d'initialisation) la zone 'réelle'  *
   *     est [(0.,0.),(WWIDTH,WHEIGHT)]                      */
  gfl_SetCenteredDrawZone(0., 0., 10., 0.);
  /* --- autre façon de faire ---
   *     ATTENTION : veiller à respecter les proportions
   *                 (wxmax-wxmin)/(wymax-wymin) = WWIDTH/WHEIGHT
   */
  // double wxmin = -10., wymin = -10.,
  // wxmax = +10., wymax = +10.;
  // gfl_SetWindowCoord(wxmin,wymin,wxmax,wymax);

  /* 3°) association des fonctions */
  gfl_SetInitFunction(init); /* fonction d'initialisation */
  gfl_SetCtrlFunction(ctrl); /* fonction de contrôle      */
  gfl_SetEvtsFunction(evts); /* fonction d'événements     */
  gfl_SetDrawFunction(draw); /* fonction de dessin        */
  gfl_SetAnimFunction(anim); /* fonction d'animation      */
  gfl_SetExitFunction(quit); /* fonction de sortie        */

  /* 4°) lancement de la boucle principale */
  return gfl_MainStart();
  /* RIEN APRES CA */
}
