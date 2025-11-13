#  Projet Labyrinthe en C

##  Description

Ce projet est un petit jeu de **labyrinthe en terminal**, entièrement écrit en **C**.  
Le programme permet de **générer automatiquement un labyrinthe parfait**, d’y **déplacer un joueur**, de **sauvegarder** et **charger** des parties.

Le projet est divisé en deux étapes principales :
- **Étape 1** : génération et affichage du labyrinthe.
- **Étape 2** : interactions utilisateur, navigation, sauvegarde/chargement et menu.

---

##  Structure du projet

```
.
├── include     # Fichier source (.c)
├── src      # fichiers d’en-tête
├── makefile    # makefile de génération
├── 
```

---

##  Compilation

Pour compiler le projet, utilisez `gcc` :

```bash
gcc main.c etape1.c etape2.c -o labyrinthe
```

---

## ▶ Exécution

Lancez simplement le programme compilé :

```bash
./labyrinthe
```

Vous verrez apparaître un menu interactif :

```
Merci de choisir une option:
1. Nouveau Labyrinth
2. Commencer une partie
3. Charger une partie
4. Quitter
```

---

##  Commandes du jeu

Dans le mode de jeu :
- `z` → Haut  
- `s` → Bas  
- `q` → Gauche  
- `d` → Droite  

> il faut valider chaque mouvement avec la touche **Entrée**.

---

##  Sauvegarde et chargement

- Le jeu peut sauvegarder le labyrinthe dans un fichier texte (`save.txt`) grâce à `saveLabyrinthInFile()`.
- Une partie sauvegardée peut être rechargée via `loadLabyrinthFromFile()`.


##  Génération du labyrinthe

L’algorithme utilisé est une version simplifiée d’un **algorithme d’union de zones** (semblable à Kruskal ou Prim), où :
- Les cellules impaires représentent des **salles**.
- Les murs sont progressivement supprimés aléatoirement jusqu’à ce que toutes les salles soient connectées.
- L’entrée est fixée en haut à gauche, et la sortie en bas à droite.

---

##  Principales fonctions

### Dans `etape1.c`
- `initLabyrinth()` : initialise la structure du labyrinthe.
- `createLabyrinth()` : génère un labyrinthe aléatoire.
- `displayLabyrinth()` : affiche le labyrinthe en console.
- `freeLabyrinth()` : libère la mémoire allouée.

### Dans `etape2.c`
- `openMenu()` : menu principal du jeu.
- `newLabyrinth()` : crée un nouveau labyrinthe.
- `startGame()` : permet au joueur de se déplacer.
- `saveLabyrinthInFile()` / `loadLabyrinthFromFile()` : gestion de la sauvegarde/chargement.

---

##  Améliorations possibles

- Gestion des **entrées clavier sans valider avec Entrée**
- Ajout d’un **chronomètre**
- Génération de labyrinthes plus grands ou plus complexes.
- Ajout des monstres

---

##  Auteur

Projet développé en C dans un cadre pédagogique.  

**Auteur :** Hugo LAGAÜZÈRE
**Année :** 2025
