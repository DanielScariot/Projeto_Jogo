//Object IDs
enum IDS{PLAYER, BULLET, ENEMY};

//Our Player
struct Sistema
{
	int x;
	int y;
	int lives;
	int boundx;
	int boundy;
	int score;
};

struct Monstro
{
    int ID;
    int health;         //VIDA DO MONSTRO
    int speed;          //VELOCIDADE DO MONSTRO
    int xlocation;      //LOCALIZA��O X DO MONSTRO
    int ylocation;      //LOCALIZA��O Y DO MONSTRO
    bool stillalive;    //TRUE = VIVO    FALSE = MORTO
    int boundx;
    int boundy;
};

struct Coord{
	int numero;
	char letra[2];
};
