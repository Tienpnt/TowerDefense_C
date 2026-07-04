# Projet Tower Defense en C avec SDL

## Introduction

Ce projet est un jeu de type **Tower Defense** développé en langage C avec la bibliothèque graphique SDL (:contentReference[oaicite:0]{index=0}).

Le jeu simule une confrontation entre deux camps :
- la Horde (unités ennemies qui avancent sur un chemin)
- le Royaume (tours défensives protégeant la Tour du Roi)

---

## Fonctionnement général

Le jeu est basé sur une grille 2D représentant le plateau. Chaque case peut contenir une unité ou être vide. Les unités sont manipulées via des listes chaînées.

---

## Phases du jeu

### Phase de création
Des unités apparaissent de manière aléatoire :
- la Horde génère des unités avec une probabilité fixe à chaque tour
- les tours du Roi peuvent être créées périodiquement avec une chance définie
- l’IA choisit automatiquement la meilleure position pour les tours

---

### Phase de déplacement
- les unités de la Horde suivent un chemin prédéfini ou généré aléatoirement
- chaque unité possède une vitesse de déplacement
- une case ne peut contenir qu’une seule unité

---

### Phase de combat
- les unités attaquent selon leur portée
- le type de cible (air ou sol) est pris en compte
- les cibles sont triées selon leurs points de vie
- chaque unité attaque une seule fois par tour

---

### Mise à jour
- suppression des unités mortes
- mise à jour des listes des joueurs
- réinitialisation des capacités d’attaque

---

## Types d’unités

### Tours
- Tour Sol
- Tour Air
- Tour du Roi

### Unités de la Horde
- Archer
- Chevalier
- Dragon
- Gargouille

---

## Structure du plateau

Le plateau est représenté par une structure de type :
- `TplateauJeu = Tunite***`
- chaque case contient un pointeur vers une unité ou NULL

---

## Génération du chemin

Le chemin de la Horde est généré aléatoirement :
- il respecte les limites du plateau
- il évite les collisions
- il relie le bas vers le haut de la carte

---

## Sauvegarde et restauration

Le jeu propose deux formats :

### Format texte
- sauvegarde lisible
- restauration avec lecture ligne par ligne

### Format binaire
- plus compact
- lecture et écriture plus rapides

---

## Intelligence du jeu

- choix des cibles selon la portée et le type
- priorité aux unités avec peu de points de vie
- placement optimal des tours selon un score de position
- la Horde vise en priorité la Tour du Roi

---

## Structures principales

```c
Tunite {
    nom;
    pointsDeVie;
    degats;
    portee;
    posX;
    posY;
    vitesseDeplacement;
    cibleAttaquable;
}