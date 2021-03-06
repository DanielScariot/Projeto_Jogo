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

struct Tiro{
	int xlocation;
	int ylocation;
	int fire_power;
	float speed;
	bool live;
};

struct Torre{
	int ID;
	int xlocation;
	int ylocation;
	int fire_power;
	int fire_rate;
	int range;
	int live;
	bool in_mouse;
	struct Tiro tiro;
};

struct Monstro{
    int ID;
    int health;         //VIDA DO MONSTRO
    int speed;          //VELOCIDADE DO MONSTRO
    float xlocation;      //LOCALIZA��O X DO MONSTRO
    float ylocation;      //LOCALIZA��O Y DO MONSTRO
    bool stillalive;    //TRUE = VIVO    FALSE = MORTO
    int boundx;
    int boundy;
    float movimentoy;
    float movimentox;
};

struct Coord{
	int numero;
	char letra[2];
};
