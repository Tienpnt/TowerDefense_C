// NOM prénom étudiant 1 : SAFI Noorsaba
// NOM prénom étudiant 2 : PHAM Nguyen Thuy Tien

#include "SDL.h"
#include "maSDL.h" // bibliothèque avec des fonctions d'affichage utilisant la SDL
#include "towerdefend.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


/*--------- Main ---------------------*/

int main(int argc, char *argv[])
{
    SDL_Window *pWindow;
    SDL_Init(SDL_INIT_VIDEO);

    pWindow = SDL_CreateWindow(
        "Appuyez sur ECHAP pour quitter, S/C ET D/V les gerer les sauvegardes",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        LARGEURJEU * 40,
        HAUTEURJEU * 40,
        SDL_WINDOW_SHOWN);

    // SDL_Renderer * renderer = SDL_CreateRenderer(pWindow, -1, 0);  // non utilisé, pour mémoire

    SDL_Surface *pWinSurf = SDL_GetWindowSurface(pWindow);
    SDL_Surface *pSpriteTourSol = SDL_LoadBMP("./data/TourSol.bmp");
    SDL_Surface *pSpriteTourAir = SDL_LoadBMP("./data/TourAir.bmp");
    SDL_Surface *pSpriteTourRoi = SDL_LoadBMP("./data/TourRoi.bmp");
    SDL_Surface *pSpriteArcher = SDL_LoadBMP("./data/Archer.bmp");
    SDL_Surface *pSpriteChevalier = SDL_LoadBMP("./data/Chevalier.bmp");
    SDL_Surface *pSpriteDragon = SDL_LoadBMP("./data/Dragon.bmp");
    SDL_Surface *pSpriteGargouille = SDL_LoadBMP("./data/Gargouille.bmp");
    SDL_Surface *pSpriteEau = SDL_LoadBMP("./data/Eau.bmp");
    SDL_Surface *pSpriteHerbe = SDL_LoadBMP("./data/Herbe.bmp");
    SDL_Surface *pSpritePont = SDL_LoadBMP("./data/Pont.bmp");
    SDL_Surface *pSpriteTerre = SDL_LoadBMP("./data/Terre.bmp");

    SDL_Surface *TabSprite[11] = {
        pSpriteTourSol, pSpriteTourAir, pSpriteTourRoi,
        pSpriteArcher, pSpriteChevalier, pSpriteDragon,
        pSpriteGargouille, pSpriteEau, pSpriteHerbe,
        pSpritePont, pSpriteTerre};

    srand(time(NULL)); // ajouter
    int **tabParcours = initChemin();

    if (pSpriteTourSol) // si le premier sprite a bien été chargé, on suppose que les autres aussi
    {
        TplateauJeu jeu = AlloueTab2D(LARGEURJEU, HAUTEURJEU);
        initPlateauAvecNULL(jeu, LARGEURJEU, HAUTEURJEU);

        prepareAllSpriteDuJeu(jeu, tabParcours, LARGEURJEU, HAUTEURJEU, TabSprite, pWinSurf);
        maj_fenetre(pWindow);

        message("Welcome in TowerDfend", "Ceci est un point de depart pour votre future interface de votre jeu TowerDefend");
        message("et fin", "ECHAP->quitter, S/C ET D/V les gerer les sauvegardes");

        /**********************************************************************/
        /*                                                                    */
        /*              DEFINISSEZ/INITIALISER ICI VOS VARIABLES              */
        /*                                                                    */

        TListePlayer playerRoi = NULL;   // Joueur défenseur
        TListePlayer playerHorde = NULL; // Joueur attaquant

        AjouterUnite(&playerRoi, creeTourRoi(tabParcours[NBCOORDPARCOURS - 1][X],
                                             tabParcours[NBCOORDPARCOURS - 1][Y]));
        AjouterUnite(&playerHorde, creeUniteHordeAleatoire(tabParcours[0][X],
                                                           tabParcours[0][Y]));

        /**********************************************************************/

        // boucle principale du jeu
        int cont = 1;
        int tours = 0;

        while (cont != 0)
        {
            SDL_PumpEvents();
            efface_fenetre(pWinSurf);
            prepareAllSpriteDuJeu(jeu, tabParcours, LARGEURJEU, HAUTEURJEU, TabSprite, pWinSurf);

            /***********************************************************************/
            /*                                                                     */
            /*                  APPELEZ ICI VOS FONCTIONS DU JEU                   */
            /*                                                                     */
            // 0. Reconstruction du plateau depuis les listes
           initPlateauAvecNULL(jeu, LARGEURJEU, HAUTEURJEU);
            PositionnePlayerOnPlateau(playerRoi, jeu);
            PositionnePlayerOnPlateau(playerHorde, jeu);

            // 1) Remise à 1 de peutAttaquer pour toutes les unités
            reinitialiseAttaque(playerRoi);
            reinitialiseAttaque(playerHorde);

            // 2) Phase combat
            phaseCombat(pWinSurf, jeu, playerRoi, &playerHorde);
            phaseCombat(pWinSurf, jeu, playerHorde, &playerRoi);

            // 3) Phase déplacement
            phaseDeplacement(playerHorde, jeu, tabParcours);

            // 4) Phase création
            phaseCreation(jeu, tabParcours, &playerRoi, &playerHorde, tours);

            tours++;

            // 5) Fin du jeu : la Horde gagne si la tour du Roi est détruite
            if (tourRoiDetruite(playerRoi))
            {
                cont = 0;
                message("Fin de partie!", "La Horde a detruit la Tour du Roi !");
            }

            /***********************************************************************/

            // affichage du jeu à chaque tour
           maj_fenetre(pWindow);
            SDL_Delay(300);

            // LECTURE DE CERTAINES TOUCHES POUR LANCER LES RESTAURATIONS ET SAUVEGARDES
            const Uint8 *pKeyStates = SDL_GetKeyboardState(NULL);

            if (pKeyStates[SDL_SCANCODE_V])
            {
                /* Ajouter vos appels de fonctions ci-dessous qd le joueur appyue sur V */

                // APPELEZ ICI VOTRE FONCTION DE SAUVEGARDE/RESTAURATION DEMANDEE

               if (restaurationSequentiel(PARTIESEQ, jeu, LARGEURJEU, HAUTEURJEU) == EXIT_SUCCESS)
                {
                    updatePlayerListsDepuisPlateau(jeu, &playerRoi, &playerHorde);
                    message("Restauration sequentielle", "Partie chargee depuis 'partieseq.tds'");
                }
                else
                {
                    message("Restauration sequentielle", "Echec du chargement");
                }

                // Ne pas modifiez les 4 lignes ci-dessous
                efface_fenetre(pWinSurf);
                prepareAllSpriteDuJeu(jeu, tabParcours, LARGEURJEU, HAUTEURJEU, TabSprite, pWinSurf);
                maj_fenetre(pWindow);
                SDL_Delay(300);
            }

            if (pKeyStates[SDL_SCANCODE_C])
            {
                /* Ajouter vos appels de fonctions ci-dessous qd le joueur appyue sur C */

                // APPELEZ ICI VOTRE FONCTION DE SAUVEGARDE/RESTAURATION DEMANDEE
               if (restaurationBinaire(PARTIEBIN, jeu, LARGEURJEU, HAUTEURJEU) == EXIT_SUCCESS)
                {
                    updatePlayerListsDepuisPlateau(jeu, &playerRoi, &playerHorde);
                    message("Restauration binaire", "Partie chargee depuis 'partiebin.tdb'");
                }
                else
                {
                    message("Restauration binaire", "Echec du chargement");
                }

                // Ne pas modifiez les 4 lignes ci-dessous
                efface_fenetre(pWinSurf);
                prepareAllSpriteDuJeu(jeu, tabParcours, LARGEURJEU, HAUTEURJEU, TabSprite, pWinSurf);
                maj_fenetre(pWindow);
                SDL_Delay(300);
            }

            if (pKeyStates[SDL_SCANCODE_D])
            {
                /* Ajouter vos appels de fonctions ci-dessous qd le joueur appyue sur D */

                // APPELEZ ICI VOTRE FONCTION DE SAUVEGARDE/RESTAURATION DEMANDEE
               if (sauvegardeSequentiel(PARTIESEQ, jeu, LARGEURJEU, HAUTEURJEU) == EXIT_SUCCESS)
                {
                    message("Sauvegarde sequentielle", "Partie sauvegardee dans 'partieseq.tds'");
                }
                else
                {
                    message("Sauvegarde sequentielle", "Echec de la sauvegarde");
                }
                // Ne pas modifiez les 4 lignes ci-dessous
                efface_fenetre(pWinSurf);
                prepareAllSpriteDuJeu(jeu, tabParcours, LARGEURJEU, HAUTEURJEU, TabSprite, pWinSurf);
                maj_fenetre(pWindow);
                SDL_Delay(300);
            }

            if (pKeyStates[SDL_SCANCODE_S])
            {
                /* Ajouter vos appels de fonctions ci-dessous qd le joueur appyue sur S */

                // APPELEZ ICI VOTRE FONCTION DE SAUVEGARDE/RESTAURATION DEMANDEE
              if (sauvegardeBinaire(PARTIEBIN, jeu, LARGEURJEU, HAUTEURJEU) == EXIT_SUCCESS)
                {
                    message("Sauvegarde binaire", "Partie sauvegardee dans 'partiebin.tdb'");
                }
                else
                {
                    message("Sauvegarde binaire", "Echec de la sauvegarde");
                }
                // Ne pas modifiez les 4 lignes ci-dessous
                efface_fenetre(pWinSurf);
                prepareAllSpriteDuJeu(jeu, tabParcours, LARGEURJEU, HAUTEURJEU, TabSprite, pWinSurf);
                maj_fenetre(pWindow);
                SDL_Delay(300);
            }

            if (pKeyStates[SDL_SCANCODE_ESCAPE])
            {
                cont = 0;
            }
        }

        // Libération de la mémoire
        libererListePlayer(&playerRoi, 0);
        libererListePlayer(&playerHorde, 0);
        libererPlateau(jeu, LARGEURJEU, HAUTEURJEU);

        SDL_FreeSurface(pSpriteTourSol);
        SDL_FreeSurface(pSpriteTourAir);
        SDL_FreeSurface(pSpriteTourRoi);
        SDL_FreeSurface(pSpriteArcher);
        SDL_FreeSurface(pSpriteChevalier);
        SDL_FreeSurface(pSpriteDragon);
        SDL_FreeSurface(pSpriteGargouille);
        SDL_FreeSurface(pSpriteEau);
        SDL_FreeSurface(pSpriteHerbe);
        SDL_FreeSurface(pSpritePont);
        SDL_FreeSurface(pWinSurf);
    }
    else
    {
        fprintf(stdout, "Échec de chargement du sprite (%s)\n", SDL_GetError());
    }

    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    freeChemin(tabParcours);

    return 0;
}
