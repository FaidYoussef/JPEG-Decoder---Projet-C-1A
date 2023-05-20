# Décodeur JPEG

Projet C à réaliser en 1A à l'Ensimag.
Réalisation d'un décodeur JPEG.

![JPEG encoder/decoder architecture](http://JonathanMAROTTA.github.io/JPEG_encoder_decoder_architecture.png?raw=true)

## Authors

- [@Youssef FAID](https://gitlab.ensimag.fr/faidy)
- [@Jonathan MAROTTA](https://github.com/JonathanMAROTTA)
- [@Gwénolé MARTIN](https://gitlab.ensimag.fr/martigwe)

## TODO

- apparemment il nous faut un Gantt chart ou équivalent dégueulasse ... cf. fin de cette page
- refaire fonctionner les tests avec toutes les fonctions !!!
- thread par composante pour accélérer le traitement pour toutes les étapes après decoding_bitstream
- voir pour améliorer la gestion mémoire en forçant int16_t[64] et [8][8] plutôt que de ne pas annoncer la taille
- Vérifier que la longueur lue dans chaque segment correspond bien à la longueur annoncée du segment
- ajouter l'upsampling dans le schéma du README.MD
- Ajouter une musique de victoire + défaite (et un gif de chatons)
- prévoir de reformater nos sorties du mode verbose pour rendre la lecture plus facile&jolie ... s'inspirer de jpeg2blabla ?

## Features

- Décodeur JPEG `Mode baseline DCT`  ✨Done✨
    - grayscale et couleurs (Note : on ne gère que 3 composantes au maximum, sinon on détecte et annonce la non implémentation)
    
    - Options (Note : on peut placer les options dans n'importe quel sens dans la ligne de commande)  
        `-h` &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; affiche l'aide
        `-v` &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; mode verbose  
        `-hv` &nbsp;&nbsp;&nbsp;&nbsp; mode highly verbose  
        `--force-grayscale` &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; force la conversion en niveau de gris  

    - Optimisations temps d'exécution et utilisation mémoire
        - fast_IDCT d'après [PRACTICAL FAST 1-D DCT ALGORITHMS WITH 11 MULTIPLICATIONS (Loeffler *et al.*)](https://formationc.pages.ensimag.fr/projet/jpeg/jpeg/distrib/loeffler.pdf)
        - multiprocessing (vérification de la possibilité via Makefile)
        - utilisation des instructions SIMD via AVX et AVX2 si disponibles (vérification de la possibilité via Makefile)
        - optimisation utilisation mémoire (écriture et accès)

    - gestion des erreurs
        - vérification de la validité du fichier JPEG (via magic number JPEG classique FFD8FF & via présence de l'APP0 JFIF)
        - génération d'un message d'erreur à chacune des étapes où l'on catch un problème
    -> cf. fichiers de tests forgés pour tester les erreurs
        - Note : on ne gère pas les fichiers polyglotes (sauf si les données jpeg sont en début de fichier)

    - Tests unitaires
        - mode standard et mode verbose (utiliser `-hv`) pour voir les détails
        - coloration sympa pour voir rapidement les tests qui passent et ceux qui ne passent pas

- Décodeur JPEG `Mode progressive` #TODO

## Usage

```sh
jpeg2ppm [-h] [-v|-hv] [--force-grayscale] <jpeg_file>
```
![struct JPEG architecture](http://JonathanMAROTTA.github.io/jpeg2ppm-usage.png?raw=true)


## Architecture du code
- schéma modules ou directement visible dans VSCode avec les différents dossiers et sous-dossiers
- schéma "sitemap" de la struct JPEG
![struct JPEG architecture](http://JonathanMAROTTA.github.io/jpeg2ppm_sitemap_graph_6_layer-1.png?raw=true)



## Blabla init

Bienvenue sur la page d'accueil de _votre_ projet JPEG, un grand espace de liberté, sous le regard bienveillant de vos enseignants préférés.
Le sujet sera disponible dès le mardi 2 mai à l'adresse suivante : [https://formationc.pages.ensimag.fr/projet/jpeg/jpeg/](https://formationc.pages.ensimag.fr/projet/jpeg/jpeg/).

Comme indiqué lors de l'amphi de présentation, vous devrez organiser un point d'étape avec vos enseignants pour valider cette architecture logicielle.
Cette page d'accueil servira de base à cette discussion. En pratique, vous pouvez reprendre son contenu comme bon vous semble, mais elle devra au moins comporter les infos suivantes :

1. des informations sur le découpage des fonctionnalités du projet en modules, en spécifiant les données en entrée et sortie de chaque étape ;
2. (au moins) un dessin des structures de données de votre projet (format libre, ça peut être une photo d'un dessin manuscrit par exemple) ;
3. une répartition des tâches au sein de votre équipe de développement, comportant une estimation du temps consacré à chacune d'elle (là encore, format libre, du truc cracra fait à la main, au joli Gantt chart).

Rajouter **régulièrement** des informations sur l'avancement de votre projet est aussi **une très bonne idée** (prendre 10 min tous les trois chaque matin pour résumer ce qui a été fait la veille, établir un plan d'action pour la journée qui commence et reporter tout ça ici, par exemple).

# Liens utiles

- Bien former ses messages de commits : [https://www.conventionalcommits.org/en/v1.0.0/](https://www.conventionalcommits.org/en/v1.0.0/) ;
- Problème relationnel au sein du groupe ? Contactez [Pascal](https://fr.wikipedia.org/wiki/Pascal,_le_grand_fr%C3%A8re) !
- Besoin de prendre l'air ? Le [Mont Rachais](https://fr.wikipedia.org/wiki/Mont_Rachais) est accessible à pieds depuis la salle E301 !
- Un peu juste sur le projet à quelques heures de la deadline ? Le [Montrachet](https://www.vinatis.com/achat-vin-puligny-montrachet) peut faire passer l'envie à vos profs de vous mettre une tôle !
