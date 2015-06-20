//Object IDs
enum IDS{PLAYER, BULLET, ENEMY, TOWER};

//Parametros do sistema
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

//Monstros
struct Monstro{
    int ID;
    int health;         	//VIDA DO MONSTRO
    int speed;          	//VELOCIDADE DO MONSTRO
    float xlocation;      	//LOCALIZA��O X DO MONSTRO
    float ylocation;      	//LOCALIZA��O Y DO MONSTRO
    bool stillalive;    	//TRUE = VIVO    FALSE = MORTO
    int boundx;
    int boundy;
};

//Tiros das torres; utiliza a struct Monstro para alvo
struct Tiro{
	int xlocation;
	int ylocation;
	int fire_power;
	float speed;
	bool live;
	struct Monstro monstro;
};

//Torres; utiliza a struct dos tiros
struct Torre{
	int ID;
	int xlocation;
	int ylocation;
	int fire_power;
	float fire_rate;
	int range;
	int live;
	bool in_mouse;
	struct Tiro tiro;
};


struct Coord{
	int numero;
	char letra[2];
};
