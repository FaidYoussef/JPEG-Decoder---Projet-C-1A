# Décodeur JPEG

Réalisé dans le cadre du projet C en 1ère année à l'Ensimag (2022-2023)

![JPEG encoder/decoder architecture](http://JonathanMAROTTA.github.io/JPEG_encoder_decoder_architecture_color_enforced.png?raw=true)

## Authors

- [@Youssef FAID](https://gitlab.ensimag.fr/faidy)
- [@Jonathan MAROTTA](https://github.com/JonathanMAROTTA)
- [@Gwénolé MARTIN](https://gitlab.ensimag.fr/martigwe)

## TODO

- upsampling (partiel ~ok + complet)    // Youssef + Jon
- refaire fonctionner les tests avec toutes les fonctions !!!   // Jon
- apparemment il nous faut un Gantt chart ou équivalent dégueulasse ... cf. fin de cette page   // Gwen
- thread par composante pour accélérer le traitement pour toutes les étapes après decoding_bitstream et avant YCbCr2RGB  // Jon
- Vérifier que la longueur lue dans chaque segment correspond bien à la longueur annoncée du segment    // Gwen
- voir pour améliorer la gestion mémoire en forçant int16_t[64] et [8][8] plutôt que de ne pas annoncer la taille   // tout le monde pour re-checker ?
- Ajouter une musique de victoire + défaite (et un gif de chatons)

## Features

- Décodeur JPEG `Mode baseline DCT`  ✨Done✨
    - Niveaux de gris et couleurs (on ne gère que 3 composantes au maximum, sinon on détecte et annonce la non-implémentation)
    
    - Options (on peut placer les options dans n'importe quel sens dans la ligne de commande)  
        `-h` &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; affiche l'aide  
        `-v` &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; mode verbose  
        `-hv` &nbsp;&nbsp;&nbsp;&nbsp; mode highly verbose  
        `--force-grayscale` &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; force la conversion en niveau de gris  

        ![--force-grayscale printscreen](http://JonathanMAROTTA.github.io/--force-grayscale.png?raw=true)

    - Optimisations temps d'exécution et utilisation mémoire
        - fast_IDCT d'après [PRACTICAL FAST 1-D DCT ALGORITHMS WITH 11 MULTIPLICATIONS (Loeffler *et al.*)](https://formationc.pages.ensimag.fr/projet/jpeg/jpeg/distrib/loeffler.pdf)
        - multiprocessing (vérification de la possibilité via Makefile)
        - vectorisation via utilisation des instructions SIMD AVX et AVX2 si disponibles (vérification de la possibilité via Makefile)
        - optimisation utilisation mémoire (écriture et accès)  

        ![meme Asterix&Obélix FREE](http://JonathanMAROTTA.github.io/Asterix30GalereObelixRep-1024x1010.jpg?raw=true){width=50%}

    - gestion des erreurs
        - vérification de la validité du fichier JPEG (via magic number JPEG classique FFD8FF & via présence de l'APP0 JFIF)
        - génération d'un message d'erreur à chacune des étapes où l'on catch un problème  
        -> cf. fichiers de tests forgés pour tester les erreurs
        - Note : on gère les fichiers polyglotes (Spécifications JPEG forcent les données jpeg en début de fichier)

    - Tests unitaires
        - mode standard et mode verbose (utiliser `-hv`) pour voir les détails
        
        ![tests unitaires printscreen](http://JonathanMAROTTA.github.io/tests_unitaires.png?raw=true)


## Usage

```sh
make
jpeg2ppm [-h] [-v|-hv] [--force-grayscale] <jpeg_file>

make tests
extract-test
huffman-test [-hv]
IDCT-test
IQ-test [-hv]
IZZ-test [-hv]
ppm-test
ycbcr2rgb-test [-hv]

```
![jpeg2ppm usage printscreen](http://JonathanMAROTTA.github.io/jpeg2ppm-usage.png?raw=true)


## Architecture du code
- Architecture en modules avec tests unitaires séparés.
    - extract.c  
	    - IN &nbsp;&nbsp;&nbsp;: [FILE *]	// input_file  
	    - OUT : [struct JPEG * jpeg | NULL]	// NULL si erreur lors de l'extraction des données  
	   ```
        > vérification conformité fichier
		    > présence SOI + APPO
	        > présence de toutes les informations nécessaires au décodage
	    > extraction des données du header et de l'image compressée avec stockage dans une super structure (struct JPEG)
		    > Start Of Frame
		    > DHT (conversion des tables en arbres binaires de recherche)
		    > DQT
		    > Start Of Scan
		    > EOI
        ```

    - huffman.c  
	    - IN &nbsp;&nbsp;&nbsp;: [struct JPEG *]
	    - OUT : [int8_t]	// EXIT_SUCCESS = 0 : pas d'erreur lors de l'exécution de la fonction  
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;// EXIT_FAILURE = 1 : erreur lors de l'exécution de la fonction
         ```
    	> décode le bitstream
    	> prise en charge de l'upsampling
    	> utilisation des arbres binaires de recherche pour récupérer les symboles (Run/Size) associés
    	> lecture du bon nombre de bits pour récupérer les valeurs des coefficients DC/AC
    	> remplissage des MCUs de chaque composante présente
    	 ```

    - IQ.c  
	    - IN &nbsp;&nbsp;&nbsp;: [struct JPEG *]
	    - OUT : [int8_t]	// EXIT_SUCCESS = 0 : pas d'erreur lors de l'exécution de la fonction  
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;// EXIT_FAILURE = 1 : erreur lors de l'exécution de la fonction
        ```
        > procède à la quantification inverse  
        > prise en charge de l'upsampling  
        > utilisation des tables de quantification associée aux composantes  
        > modification en place des valeurs des MCUs de chaque composante présente
        ```

    - IZZ.c  
	    - IN &nbsp;&nbsp;&nbsp;: [struct JPEG *]
	    - OUT : [int8_t]	// EXIT_SUCCESS = 0 : pas d'erreur lors de l'exécution de la fonction  
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;// EXIT_FAILURE = 1 : erreur lors de l'exécution de la fonction
        ```
        > procède au zig-zag inverse de chacun des MCUs  
        > modification des valeurs des MCUs de chaque composante présente
        ```

    - IDCT.c  
	    - IN &nbsp;&nbsp;&nbsp;: [struct JPEG *]
	    - OUT : [int8_t]	// EXIT_SUCCESS = 0 : pas d'erreur lors de l'exécution de la fonction  
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;// EXIT_FAILURE = 1 : erreur lors de l'exécution de la fonction  
        ```
        > procède à la transformée en cosinus discrète inverse  
        > prise en charge de l'upsampling  
        > fast IDCT via algorithme de Loeffler et al.  
        > modification des valeurs des MCUs de chaque composante présente
        ```

    - YCbCr2RGB.c  
	    - IN &nbsp;&nbsp;&nbsp;: [struct JPEG *], [int8_t]
	    - OUT : [int8_t]	// EXIT_SUCCESS = 0 : pas d'erreur lors de l'exécution de la fonction  
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;// EXIT_FAILURE = 1 : erreur lors de l'exécution de la fonction
        ```
        > procède à la conversion en RGB  
        > prise en charge de l'upsampling  
        > possibilité de forcer la conversion en niveau de gris via la ligne de commande qui modifie la valeur du paramètre d'entrée `force_grayscale`  
        > modification en place des valeurs des MCUs de chaque composante présente
        ```

    - ppm.c  
	    - IN &nbsp;&nbsp;&nbsp;: [struct JPEG *]
	    - OUT : [int8_t]	// EXIT_SUCCESS = 0 : pas d'erreur lors de l'exécution de la fonction  
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;// EXIT_FAILURE = 1 : erreur lors de l'exécution de la fonction
        ```
        > procède à l'écriture d'un fichier PGM (grayscale) ou PPM  
        > optimisation de la taille du fichier via écriture en binaire
        ```

    - free_JPEG_struct() in extract.c  
	    - IN &nbsp;&nbsp;&nbsp;: [struct JPEG *]
	    - OUT : [ ]
        ```
	    > procède à la libération de la mémoire dynamique allouée
	    ```

- schéma "sitemap" de la struct JPEG
![struct JPEG - sitemap visualization](http://JonathanMAROTTA.github.io/jpeg2ppm_sitemap_graph_6_layer-1.png?raw=true)


## Architecture fichier JPEG

![architecture fichier JPEG](http://JonathanMAROTTA.github.io/jpegrgb_dissected.png?raw=true)

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
