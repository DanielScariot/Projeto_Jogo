//Object IDs
enum IDS{PLAYER, BULLET, ENEMY, TOWER};

//Parametros do sistema
struct Sistema
{
	int x, y;			//Posiçao
	int lives;			//Vidas restantes do sistema
	float money;		//Poder de compra do sistema
	int score;			//Score (por enquanto monstros mortos)
	int boundx;			//Para colisao
	int boundy;
};

//Monstros
struct Monstro{
    int ID;
    int health;         //VIDA DO MONSTRO
    float speed;        //VELOCIDADE DO MONSTRO
    float xlocation;    //LOCALIZA��O X DO MONSTRO
    float ylocation;    //LOCALIZA��O Y DO MONSTRO
    bool stillalive;    //TRUE = VIVO    FALSE = MORTO
    int boundx;
    int boundy;
    float mov_y;
    float mov_x;
};

//Tiros das torres; utiliza a struct Monstro para alvo
struct Tiro{
	int fire_power;		//Dano
	float speed;		//Velocidade de locomoçao do tiro -> fixo
	bool live;			//Tiro em movimento
	int xlocation;
	int ylocation;
	struct Monstro monstro;
};

struct Tipo{
	int n;				//Identificaçao do tipo
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
	int price;			//Preço (bc)
	int fire_power;		//Poder de fogo da torre  -> Maior melhor
	float fire_rate;	//Período de disparo (s)  -> Menor melhor
	int range;			//Alcance da torre	(px)  -> Maior melhor
	int time_to_shot;	//Contagem para o disparo
	bool live;			//Torre ativa
	int upgrade;		//upgrades
	int x, y;			//Localizaçao
	bool in_mouse;		//Se ela está no mouse
	struct Tiro tiro;
};
