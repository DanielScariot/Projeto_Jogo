//Arquivo das constantes e globais

const int fps = 60;
const int A = 23; //Linhas
const int B = 32; //Colunas
const int LARGURA_TELA = 1024;
const int ALTURA_TELA = 720;
const int a_celula = ALTURA_TELA / A;  //Altura da célula
const int l_celula = LARGURA_TELA / B; //Largura da célula
int pos_x = 0;      //Posiçao x do mouse
int pos_y = 0;      //Posiçao y do mouse
bool GameOver = false;

//*****Cores********

#define preto al_map_rgb(0, 0, 0)
#define fundo_azulado al_map_rgb(50, 50, 90)
