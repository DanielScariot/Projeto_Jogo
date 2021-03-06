//Object IDs
enum IDS{PLAYER, BULLET, ENEMY, TOWER};

//Our Player
struct Sistema
{
	int ID;
	int x;
	int y;
	int lives;
	int boundx;
	int boundy;
	int score;
};

struct Torre{
	int ID;
	int xlocation;
	int ylocation;
	int fire_power;
	int fire_rate;
	int range;
};

struct Tiro{
	int xlocation;
	int ylocation;
	int fire_power;
	int speed;
	bool live;
};

struct Monstro
{
    int ID;
    int health;         //VIDA DO MONSTRO
    int speed;          //VELOCIDADE DO MONSTRO
    float xlocation;      //LOCALIZA��O X DO MONSTRO
    float ylocation;      //LOCALIZA��O Y DO MONSTRO
    bool stillalive;    //TRUE = VIVO    FALSE = MORTO
    int boundx;
    int boundy;
};

struct Coord{
	int numero;
	char letra[2];
};
