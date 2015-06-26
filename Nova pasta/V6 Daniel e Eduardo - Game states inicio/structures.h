//Object IDs
enum IDS{PLAYER, BULLET, ENEMY, TOWER};

//Parametros do sistema
struct Sistema
{
	int ID;
	int x;
	int y;
	int lives;			//Vidas restantes do sistema
	int boundx;
	int boundy;
	float money;		//Poder de compra do sistema
	int score;			//Score (po enquanto monstros mortos)
};

//Monstros
struct Monstro{
    int ID;
    int health;         	//VIDA DO MONSTRO
    float speed;          	//VELOCIDADE DO MONSTRO
    float xlocation;      	//LOCALIZA��O X DO MONSTRO
    float ylocation;      	//LOCALIZA��O Y DO MONSTRO
    bool stillalive;    	//TRUE = VIVO    FALSE = MORTO
    int boundx;
    int boundy;
    float mov_y;
    float mov_x;
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

struct Tipo{
	int n;
	int fire_power;		//Poder de fogo da torre
	float fire_rate;	//Período de disparo
	int range;			//Alcance da torre
	int price;			//Preço deste tipo de torre
	int upgrade;		//Upgrades realizados
	int mapa;			//Numero a ser inserido na matriz
	struct Tiro tiro;
};

//Torres; utiliza a struct dos tiros
struct Torre{
	int n;				//Tipo da torre
	int ID;				//Identificaçao particular da torre
	int fire_power;		//Poder de fogo da torre
	float fire_rate;	//Período de disparo
	int range;			//Alcance da torre
	int price;
	int live;
	int upgrade;
	int x;
	int y;
	bool in_mouse;		//Se ela está no mouse
	struct Tiro tiro;
};
