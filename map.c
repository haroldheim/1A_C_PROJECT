#include "prototypes.h"


Map map;

void initMaps() {
    map.background = loadImage("graphics/background.jpg");
    
    map.tileSetNumber = 0;
    
    map.health1 = NULL;
    map.health2 = NULL;
}

SDL_Texture *getBackground() {
    return map.background;
}

SDL_Texture *getHealth1() {
    return map.health1;
}

SDL_Texture *getHealth2() {
    return map.health2;
}

void setHealth1(SDL_Texture *message) {
    map.health1 = message;
}

void setHealth2(SDL_Texture *message) {
    map.health2 = message;
}

void cleanMaps() {
    if(map.background != NULL) {
        SDL_DestroyTexture(map.background);
        map.background = NULL;
    }
    
    if(map.tileSet != NULL) {
        SDL_DestroyTexture(map.tileSet);
        map.tileSet = NULL;
    }
}

void loadMap(char *name) {
    int x, y;
    FILE *fp;
    
    fp = fopen(name, "rb");
    
    if(fp == NULL) {
        printf("Impossible d'ouvrir la map %s\n", name);
        exit(1);
    }
   
    fscanf(fp, "%d", &map.begin_player1x);
    fscanf(fp, "%d", &map.begin_player1y);
    
    fscanf(fp, "%d", &map.begin_player2x);
    fscanf(fp, "%d", &map.begin_player2y);

    
    fscanf(fp, "%d", &map.tileSetAffiche);
    
    map.maxX = 0;
    map.maxY = 0;
    
    
    for(y = 0; y < MAX_MAP_Y; y++) {
        for(x = 0; x < MAX_MAP_X; x++ ) {
            fscanf(fp, "%d", &map.tile[y][x]);
        }
    }
    
    map.maxX = 1280;
    map.maxY = 800;
    
    fclose(fp);
    
}

void drawMap() {
    int x, y, mapX, x1, x2, mapY, y1, y2, xsource, ysource, a;
    
    mapX = map.startX / TILE_SIZE;
    x1 = (map.startX % TILE_SIZE) * -1;
    x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);
    
    mapY = map.startY / TILE_SIZE;
    y1 = (map.startY % TILE_SIZE) * -1;
    y2 = y1 + SCREEN_HEIGHT + (y1 == 0 ? 0 : TILE_SIZE);
    
    for(y = y1; y < y2; y += TILE_SIZE) {
        mapX = map.startX / TILE_SIZE;
        for(x = x1; x < x2; x += TILE_SIZE) {
            a = map.tile[mapY][mapX];
                
            ysource = a / 28 * TILE_SIZE;
            xsource = a % 28 * TILE_SIZE;
            
            drawTile(map.tileSet, x, y, xsource, ysource);
            
            mapX++;
        }
        mapY++;
    }
    
}

void changeLevel(void)
{
 
	char file[200];
	  

	sprintf(file, "map/map%d.txt", getLevel());
	
	loadMap(file);
	
	//Charge le tileset
	if (map.tileSet != NULL)
	{
	SDL_DestroyTexture(map.tileSet);
	}
	 
	sprintf(file, "graphics/tileset%d.png", map.tileSetAffiche);
	
	map.tileSet = loadImage(file); 
	
}

int getStartX() {
    return map.startX;
}

int getStartY() {
    return map.startY;
}

void setStartX(int valeur) {
    map.startX = valeur;
}

void setStartY(int valeur) {
    map.startY = valeur;
}

int getMaxX() {
    return map.maxX;
}

int getMaxY() {
    return map.maxY;
}

int getBeginPlayer1X() {
    return map.begin_player1x;
}

int getBeginPlayer1Y() {
    return map.begin_player1y;
}

int getBeginPlayer2X() {
    return map.begin_player2x;
}

int getBeginPlayer2Y() {
    return map.begin_player2y;
}

void mapCollision(GameObject *entity)
{
    
    int i, x1, x2, y1, y2;
    
    /* D'abord, on considère le joueur en l'air jusqu'à temps
     d'être sûr qu'il touche le sol */
    entity->onGround = 0;
    
    /* Ensuite, on va tester les mouvements horizontaux en premier
     (axe des X). On va se servir de i comme compteur pour notre boucle.
     En fait, on va découper notre sprite en blocs de tiles pour voir
     quelles tiles il est susceptible de recouvrir.
     On va donc commencer en donnant la valeur de Tile_Size à i pour qu'il
     teste la tile où se trouve le x du joueur mais aussi la suivante SAUF
     dans le cas où notre sprite serait inférieur à la taille d'une tile.
     Dans ce cas, on lui donnera la vraie valeur de la taille du sprite
     Et on testera ensuite 2 fois la même tile. Mais comme ça notre code
     sera opérationnel quelle que soit la taille de nos sprites ! */
    
    if (entity->h > TILE_SIZE)
        i = TILE_SIZE;
    else
        i = entity->h;
    
    
    //On lance alors une boucle for infinie car on l'interrompra selon
    //les résultats de nos calculs
    for (;;)
    {
        //On va calculer ici les coins de notre sprite à gauche et à
        //droite pour voir quelle tile ils touchent.
        x1 = (entity->x + entity->dirX) / TILE_SIZE;
        x2 = (entity->x + entity->dirX + entity->w - 1) / TILE_SIZE;
        
        //Même chose avec y, sauf qu'on va descendre au fur et à mesure
        //pour tester toute la hauteur de notre sprite, grâce à notre
        //fameuse variable i.
        y1 = (entity->y) / TILE_SIZE;
        y2 = (entity->y + i - 1) / TILE_SIZE;
        
        //De là, on va tester les mouvements initiés dans updatePlayer
        //grâce aux vecteurs dirX et dirY, tout en testant avant qu'on
        //se situe bien dans les limites de l'écran.
        if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
        {
            //Si on a un mouvement à droite
            if (entity->dirX > 0)
            {
                //On vérifie si les tiles recouvertes sont solides
                if (map.tile[y1][x2] > BLANK_TILE || map.tile[y2][x2] > BLANK_TILE)
                {
                    // Si c'est le cas, on place le joueur aussi près que possible
                    // de ces tiles, en mettant à jour ses coordonnées. Enfin, on
                    //réinitialise son vecteur déplacement (dirX).
                    
                    entity->x = x2 * TILE_SIZE;
                    entity->x -= entity->w + 1;
                    entity->dirX = 0;
                    
                }
            }
            
            //Même chose à gauche
            else if (entity->dirX < 0)
            {
                if (map.tile[y1][x1] > BLANK_TILE || map.tile[y2][x1] > BLANK_TILE)
                {
                    entity->x = (x1 + 1) * TILE_SIZE;
                    entity->dirX = 0;
                }
                
            }
            
        }
        
        //On sort de la boucle si on a testé toutes les tiles le long de la hauteur du sprite.
        if (i == entity->h)
        {
            break;
        }
        
        //Sinon, on teste les tiles supérieures en se limitant à la heuteur du sprite.
        i += TILE_SIZE;
        
        if (i > entity->h)
        {
            i = entity->h;
        }
    }
    
    
    //On recommence la même chose avec le mouvement vertical (axe des Y)
    if (entity->w > TILE_SIZE)
        i = TILE_SIZE;
    else
        i = entity->w;
  
    
    for (;;)
    {
        x1 = (entity->x) / TILE_SIZE;
        x2 = (entity->x + i) / TILE_SIZE;
        
        y1 = (entity->y + entity->dirY) / TILE_SIZE;
        y2 = (entity->y + entity->dirY + entity->h) / TILE_SIZE;
        
        if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
        {
            if (entity->dirY > 0)
            {
                
                //Gestion des plateformes traversables : elles se situent juste avant
                //les tiles bloquantes dans notre tileset (dont la valeur butoire est
                //BLANK_TILE). Il suffit donc d'utiliser le numéro de la première tile
                //traversable au lieu de BLANK_TILE pour bloquer le joueur,
                //seulement quand il tombe dessus (sinon, il passe au-travers
                //et le test n'est donc pas effectué dans les autres directions
                if (map.tile[y2][x1] > TILE_TRANSVERSABLE || map.tile[y2][x2] > TILE_TRANSVERSABLE)
                {
                    //Si la tile est une plateforme ou une tile solide, on y colle le joueur et
                    //on le déclare sur le sol (onGround).
                    entity->y = y2 * TILE_SIZE;
                    entity->y -= entity->h;
                    entity->dirY = 0;
                    entity->onGround = 1;
                }
                
            }
            
            else if (entity->dirY < 0)
            {
                
     
                if (map.tile[y1][x1] > BLANK_TILE || map.tile[y1][x2] > BLANK_TILE)
                {
                    entity->y = (y1 + 1) * TILE_SIZE;
                    entity->dirY = 0;
                }
                
            }
        }
        
        //On teste la largeur du sprite (même technique que pour la hauteur précédemment)
        if (i == entity->w)
        {
            break;
        }
        
        i += TILE_SIZE;
        
        if (i > entity->w)
        {
            i = entity->w;
        }
    }

    /* Maintenant, on applique les vecteurs de mouvement si le sprite n'est pas bloqué */
    entity->x += entity->dirX;
    entity->y += entity->dirY;
   
    //Et on contraint son déplacement aux limites de l'écran.
    if (entity->x < 0)
    {
        entity->x = 0;
    }
    
    else if (entity->x + entity->w >= map.maxX)
    {
        //Si on touche le bord droit de l'écran, on annule
        //et on limite le déplacement du joueur
        entity->x = map.maxX - entity->w - 1;
    }
}

void mondeSuivant(double victoires) {
    if(victoires != 3 || victoires != -3)
        reinitializePlayers();
    if(victoires == 0) {
        setValeurDuNiveau(1);
        map.background = loadImage("graphics/background.jpg");
    }
    else if(victoires == 1 || victoires == -1) {
        setValeurDuNiveau(2);
        map.background = loadImage("graphics/background2.png");
        changeLevel();
    } else if(victoires == 2 || victoires == -2) {
        setValeurDuNiveau(3);
        map.background = loadImage("graphics/background3.jpeg");
        changeLevel();
    }
}
