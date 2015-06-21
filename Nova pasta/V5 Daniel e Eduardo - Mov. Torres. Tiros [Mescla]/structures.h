//Object IDs
enum IDS{PLAYER, BULLET, ENEMY, TOWER};

//Parametros do sistema
struct Sistema
{
	int ID;
	int x;
	int y;
	int lives;		//Vidas restantes do sistema
	int boundx;
	int boundy;
	int money;		//Poder de compra do sistema
	int score;		//Score
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
	int fire_power;		//Poder de fogo do tiro - Dano causado no inimigo
	float speed;		//Velocidade de locomoçao do tiro [estático]
	bool live;
	struct Monstro monstro;
};

//Torres; utiliza a struct dos tiros
struct Torre{
	int ID;
	int xlocation;
	int ylocation;
	int fire_power;		//Poder de fogo da torre
	float fire_rate;	//Período de disparo
	int range;			//Alcance da torre
	int live;
	bool in_mouse;		//Se ela está no mouse
	struct Tiro tiro;
};


struct Coord{
	int numero[2];    	//Coordenada da matriz
};
