#include <stdio.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "structures.h"  //Estruturas - Deve vir antes das constantes
#include "constantes.h"  //Variaveis constantes globais
#include "arrays.h"      //Matrizes importantes

int init_fail (ALLEGRO_DISPLAY *janela, ALLEGRO_FONT *fonte, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_BITMAP *imagem, ALLEGRO_TIMER *timer); //Fun�ao falha na inicializa�ao
void destroy_al(ALLEGRO_DISPLAY *janela,ALLEGRO_FONT *fonte, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_BITMAP *imagem, ALLEGRO_TIMER *timer);
void init_system(Sistema &sistema); //Carrega informaçoes das torres
void draw_background(int mapa[A][B], Coord coordenada[], ALLEGRO_FONT *fonte); //Desenha a matriz para fins de debug
void a_coord(Coord coordenada[]);

//Funçoes dos montros
void init_horda(Monstro monstro[], int n_monstros);
void start_horda(Monstro monstro[], int n_monstros, int n_hordas);
void update_horda(Monstro monstro[], Sistema &sistema, int mapa[A][B], int n_monstros);
void draw_horda(Monstro monstro[], int n_monstros, ALLEGRO_BITMAP *imagem);
void colisao_horda(Torre torre[], Monstro monstro[], int t, int n_monstros, Sistema &sistema);

//Funçoes das torres
void setup_tower(Sistema &sistema, Torre torre[], int t, int r, int l);
void draw_mouse_tower(int r, int l, Torre torre[], int t);
void draw_towers(int mapa[A][B], Coord coordenada[], Sistema &sistema, ALLEGRO_FONT *fonte);
void show_tower_information(Torre torre[], int t, int r, int l, ALLEGRO_FONT *fonte);
void buy_tower(Torre torre[], ALLEGRO_FONT *fonte);
void upgrade_tower(Torre torre[], int t);

//Funçoes dos tiros
void draw_tiro(Torre torre[], int t);
void fire_tiro(Torre torre[], Monstro monstro[], int t, int n_monstros);
void update_tiro(Torre torre[], Monstro monstro[], int t, int n_monstros);



int main(int argc, char const *argv[])
{
    int i = 0;
    int t = 1;

    int n_monstros = 10; //Numero de monstros de cada horda
    int n_hordas = 0;    //Numero de hordas chamadas
    bool nova_horda = true;    //Chama nova horda
    bool render = false;       //Renderizaçao
    bool torre_mouse = false;  //Se a torre está no mouse
    bool info_torre = false;   //Chama a funçao de informaçoes da torre
    bool compra_torre = false;
    bool upgrade_torre;
    int tower_posx = 0;        //Posiçao x de determinada torre
    int tower_posy = 0;        //Posiçao y de determinada torre
    int torre_n;
    int gamestate = 0;

    int r; //Variável para colunas
    int l; //Variável para linhas

    //Iniciaçao das estruturas
    Sistema sistema;
    Torre torre[100];
    Monstro monstro[n_monstros];
    Coord coordenada[A*B];

    //Declaracao vairaveis allegro
    ALLEGRO_DISPLAY *janela = NULL;	            //Vari�vel para a janela
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;   //  ''     para eventos
    ALLEGRO_BITMAP *imagem = NULL;              //  ''     para imagem
    ALLEGRO_TIMER *timer = NULL;                //  ''     para o tempo (fps)
    ALLEGRO_FONT *fonte = NULL;                 //  ''     para fonte
    ALLEGRO_FONT *FonteMenu = NULL;
    ALLEGRO_FONT *fonte40 = NULL;
    //Inicializa o allegro, mouse e add-ons
    al_init();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    //Setup inicial da matriz, monstros e sistema
    a_coord(coordenada);
    init_horda(monstro, n_monstros);
    init_system(sistema);
    //******************* -> Temporário!
    torre[0].price = 40;
    torre[0].range = 80;
    torre[0].fire_rate = 0.3;
    torre[0].fire_power = 25;
    //*******************

    //Atribui atributos às variáveis allegro
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    fila_eventos = al_create_event_queue();
    imagem = al_load_bitmap("virus.png");
    timer = al_create_timer(1.0 / fps);
    fonte = al_load_font("arial.ttf", 12, 0);
    FonteMenu = al_load_font("coure.fon", 18, 0);
    fonte40 = al_load_font("arial.ttf", 40, 0);

    //Inicializa o mouse e tempo
    al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
    al_start_timer(timer);
    al_install_keyboard();

    init_fail(janela, fonte, fila_eventos, imagem, timer); //Fun�ao de teste

    //Regista os eventos da janela, mouse e timer na vari�vel de eventos (fila_eventos)
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));

    al_clear_to_color(al_map_rgb(235, 235, 235));   //Limpa a tela
    al_flip_display();                              //Atualiza a tela

    //Loop principal
    while (!GameOver)
    {
        ALLEGRO_EVENT evento;                         //Variavel para eventos
        al_wait_for_event(fila_eventos, &evento);

        if(gamestate == 0)
        {
            if(evento.type == ALLEGRO_EVENT_TIMER)
            {
                render = true;
            }
            if(evento.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                switch(evento.keyboard.keycode)
                {
                case ALLEGRO_KEY_ENTER:
                    gamestate = 1;
                    break;
                case ALLEGRO_KEY_BACKSPACE:
                    gamestate = 2;
                    break;
                }
            }
        }

        if(gamestate == 1)
        {

            if(evento.type == ALLEGRO_EVENT_TIMER)
            {
                for(int j =0; j<t+1; j++)  //Loop para o disparo das torres
                {
                    if(torre[j].live)
                    {
                        if(i >= fps*(torre[j].fire_rate))
                        {
                            fire_tiro(torre, monstro, t, n_monstros); //Dispara tiros
                            i = 0;
                        }
                    }
                }
                //****Upgrade beta
                if(info_torre && !upgrade_torre){
                    mapa[21][3] = 11;
                }
                if(upgrade_torre){
                    mapa[21][3] = 0;
                }
                ///*******
                i++;

                update_horda(monstro, sistema, mapa, n_monstros);
                update_tiro(torre, monstro, t, n_monstros);
                colisao_horda(torre, monstro, t, n_monstros, sistema);

                render = true;
                if(sistema.lives <= 0)
                    gamestate = 2;
            }

            if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                GameOver = true;
            }

            else if(evento.type == ALLEGRO_EVENT_MOUSE_AXES)
            {
                pos_x = evento.mouse.x; //Armazena a posiçao x do mouse
                pos_y = evento.mouse.y; //Armazena a posiçao y do mouse

                r = coordenada[(pos_x/l_celula)].numero[0]; // Atribui uma celula de coluna
                l = coordenada[(pos_y/a_celula)].numero[1]; // Atribui uma celula de linha
            }

            else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                {
                    if (mapa[l][r] == 9 && evento.mouse.button & 1)
                    {
                        compra_torre = true;
                        if(sistema.money >= torre[t-1].price && evento.mouse.button & 1)
                            torre_mouse = true;
                    }

                    if(torre_mouse && mapa[l][r] == 0 && evento.mouse.button & 1) //Posicionamento da torre
                    {
                        setup_tower(sistema, torre, t, r, l);
                        sistema.money -= torre[t].price;
                        torre_mouse = false;
                        compra_torre = false;
                        mapa[l][r] = 10;
                        coordenada[l*r].torre = t;     //Atribui um numero especifico para determinada torre
                        t++;
                    }

                    if(torre_mouse && evento.mouse.button & 2)  //Cancela compra
                    {
                        torre_mouse = false;
                        compra_torre = false;
                    }

                    if (mapa[l][r] == 10)  //Exibiçao das infomaçoes da torre
                    {
                        tower_posx = r;
                        tower_posy = l;
                        torre_n = coordenada[l*r].torre;
                        info_torre = true;
                        upgrade_torre = false;
                        mapa[21][3] = 11;
                    }
                    if(mapa[l][r] != 10 && mapa[l][r] != 11) //Termina a exibiçao de informaçoes da torre comprada
                    {
                        info_torre = false; //Fecha a exibiçao
                        mapa[21][3] = 0;
                    }
                    if(mapa[l][r] != 9 && compra_torre){ //Termina a exibiçao da torre a ser comprada
                        compra_torre = false;
                    }
                    if(info_torre && mapa[l][r] == 11 && sistema.money >= 60){
                        sistema.money -= 60;
                        upgrade_tower(torre, torre_n);
                        upgrade_torre = true;
                        mapa[21][3] = 0;
                    }
            }


            else if(evento.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                switch(evento.keyboard.keycode)
                {
                case ALLEGRO_KEY_SPACE: //Inicializa uma nova horda
                    start_horda(monstro, n_monstros, n_hordas);
                    n_hordas++;
                    break;
                }
            }
        }
        if(gamestate == 2)
        {
            if(evento.type == ALLEGRO_EVENT_TIMER)
            {
                render = true;
            }
            if(evento.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                switch(evento.keyboard.keycode)
                {
                case ALLEGRO_KEY_R:
                    gamestate = 1;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    GameOver = true;
                    break;
                }
            }
        }

        if(render && al_is_event_queue_empty(fila_eventos))
        {
            render = false;

            if(gamestate == 0)
            {
                al_clear_to_color(al_map_rgb(255,255,255));
                al_draw_textf(fonte, al_map_rgb(0, 0, 255), LARGURA_TELA/2, (ALTURA_TELA/2) - 20, 0, "Pressione ENTER para Jogar");
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, (ALTURA_TELA/2) + 20, 0, "Pressione BACKSPACE para Sair");
            }
            if(gamestate == 1)
            {
                al_clear_to_color(al_map_rgb(61, 10, 10));

                draw_background(mapa, coordenada, fonte); //Desenha o plano de fundo
                draw_towers(mapa, coordenada, sistema, fonte); //Desenha as torres

                draw_background(mapa, coordenada, fonte); //Desenha o plano de fundo
                draw_towers(mapa, coordenada, sistema, fonte); //Desenha as torres


                al_draw_textf(fonte, al_map_rgb(0, 0, 0), 900, 15, ALLEGRO_ALIGN_LEFT, "Vidas do sistema %i", sistema.lives);
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), 900, 35, ALLEGRO_ALIGN_LEFT, "Bitcoins %.2f", sistema.money);
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), 100, 15, ALLEGRO_ALIGN_LEFT, "Monstros mortos: %i  Wave: %i", sistema.score, n_hordas);

                al_draw_textf(fonte, al_map_rgb(0, 0, 0), pos_x, pos_y, ALLEGRO_ALIGN_LEFT, "x:%i y:%i", r, l);

                draw_horda(monstro, n_monstros, imagem); //Desenha os montros

                if(torre_mouse)
                {
                    draw_mouse_tower(r, l, torre, t-1); //Desenha a torre somente enquanto ela estiver no mouse
                }
                if(info_torre)
                {
                    show_tower_information(torre, torre_n, tower_posx, tower_posy, fonte); //info torres
                }
                if(compra_torre)
                {
                    buy_tower(torre, fonte);
                }
                draw_tiro(torre, t-1); //Desenha os tiros
            }
            if(gamestate == 2)
                {
                    al_clear_to_color(al_map_rgb(255,255,255));
                    al_draw_textf(fonte40, al_map_rgb(255, 0, 0), LARGURA_TELA/2, (ALTURA_TELA/2) - 100, 0, "Game Over");
                    al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, (ALTURA_TELA/2) - 20, 0, "Pressione R para Jogar Novamente");
                    al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, (ALTURA_TELA/2) + 20, 0, "Pressione ESC para Sair");
                }
                al_flip_display();
        }
    }

    destroy_al(janela, fonte, fila_eventos, imagem, timer); //Destroi as vari�veis allegro

    return 0;
}

void init_system(Sistema &sistema)
{
    sistema.lives = 10;
    sistema.score = 0;
    sistema.money = 100;
    sistema.boundx = l_celula;
    sistema.boundy = a_celula;
}

int init_fail(ALLEGRO_DISPLAY *janela, ALLEGRO_FONT *fonte, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_BITMAP *imagem, ALLEGRO_TIMER *timer)
{
    if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return -1;
    }
    if (!al_install_mouse())
    {
        fprintf(stderr, "Falha ao inicializar o mouse.\n");
        al_destroy_display(janela);
        return -1;
    }
    if (!janela)
    {
        fprintf(stderr, "Falha ao criar janela.\n");
        return -1;
    }
    if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
    {
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        al_destroy_display(janela);
        return -1;
    }
    if (!imagem)
    {
        fprintf(stderr, "Falha ao carregar imagem.\n");
        al_destroy_bitmap(imagem);
        al_destroy_display(janela);
        return -1;
    }
    if (!fila_eventos)
    {
        fprintf(stderr, "Falha ao inicializar o fila de eventos.\n");
        al_destroy_display(janela);
        return -1;
    }
    if(!fonte)
    {
        fprintf(stderr, "Falha ao inicializar a fonte.\n");
        al_destroy_font(fonte);
        al_destroy_display(janela);
        return -1;
    }
    if(!timer)
    {
        fprintf(stderr, "Falha ao inicializar o timer.\n");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        return -1;
    }
}

void destroy_al(ALLEGRO_DISPLAY *janela,ALLEGRO_FONT *fonte, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_BITMAP *imagem, ALLEGRO_TIMER *timer)
{
    al_destroy_display(janela);
    al_destroy_font(fonte);
    al_destroy_event_queue(fila_eventos);
    al_destroy_bitmap(imagem);
    al_destroy_timer(timer);
}

void draw_background(int mapa[A][B], Coord coordenada[], ALLEGRO_FONT *fonte) //Setup e desenho o ultimo plano da matriz
{
    int i = 0;
    int j = 0;
    int m_x = 0;
    int m_y = 0;

    for (i=0;  i<A; i++)
    {
        for(j=0; j<B; j++)
        {
            switch (mapa[i][j])
            {
            case 0:
                al_draw_filled_rectangle(m_x, m_y, m_x + l_celula, m_y + a_celula, fundo_azulado);
                break;
            case 1:
                al_draw_filled_rectangle(m_x, m_y, m_x + l_celula, m_y + a_celula, al_map_rgb(0, 0, 200));
                break;
            case 2:
                al_draw_filled_rectangle(m_x, m_y, m_x + l_celula, m_y + a_celula, al_map_rgb(0, 0, 200));
                break;
            case 3:
                al_draw_filled_rectangle(m_x, m_y, m_x + l_celula, m_y + a_celula, al_map_rgb(0, 0, 200));
                break;
            case 4:
                al_draw_filled_rectangle(m_x, m_y, m_x + l_celula, m_y + a_celula, al_map_rgb(0, 0, 200));
                break;
            case 5:
                al_draw_filled_rectangle(m_x, m_y, m_x + l_celula, m_y + a_celula, al_map_rgb(0, 200, 0));
                break;
            case 6:
                al_draw_filled_rectangle(m_x, m_y, m_x + l_celula, m_y + a_celula, al_map_rgb(200, 200, 0));
                break;
            }
            m_x += l_celula;
        }
        m_x = 0;
        m_y += a_celula;
    }
}

void draw_towers(int mapa[A][B], Coord coordenada[], Sistema &sistema, ALLEGRO_FONT *fonte) //Desenha as torres; segundo plano
{
    int i = 0;
    int j = 0;
    int m_x = 0;
    int m_y = 0;

    for (i=0;  i<A; i++)
    {
        for(j=0; j<B; j++)
        {
            switch (mapa[i][j])
            {
            case 9:
                al_draw_filled_circle(m_x + (l_celula/2), m_y + (a_celula/2), l_celula/2, al_map_rgb(100, 0, 0));
                break;
            case 10:
                al_draw_filled_circle(m_x + (l_celula/2), m_y + (a_celula/2), l_celula/2, al_map_rgb(153, 0, 0));
                break;
            case 11:
                al_draw_filled_circle(m_x + (l_celula/2), m_y + (a_celula/2), l_celula/2, al_map_rgb(100, 0, 0));
                break;
            case 90:
                al_draw_filled_circle(m_x + (l_celula/2), m_y + (a_celula/2), l_celula/2, al_map_rgb(0, 0, 0));
                //Define as coordenadas do sistema de acordo com a posiçao 90 na matriz
                sistema.x = m_x;
                sistema.y = m_y;
                break;
            }
            m_x += l_celula;
        }
        m_x = 0;
        m_y += a_celula;
    }
}

void a_coord(Coord coordenada[])  //Nomeia as células da matriz, é executada apenas uma vez
{
    int i, j;

    for (i=0;  i<A; i++)
    {
        for(j=0; j<B; j++)
        {
            coordenada[j].numero[0] = j;
            coordenada[i].numero[1] = i;
        }
    }
}

void init_horda(Monstro monstro[], int n_monstros)
{
    int m = 0;
    for(m = 0; m < n_monstros; m++)
    {
        monstro[m].stillalive = false;
        monstro[m].health = 15;
        monstro[m].speed = 3;
        monstro[m].boundx = 35;
        monstro[m].boundy = 35;
    }
}

void draw_horda(Monstro monstro[], int n_monstros, ALLEGRO_BITMAP *imagem)
{
    int m = 0;
    for(m = 0; m < n_monstros; m++)
    {
        if(monstro[m].stillalive)
        {
            al_draw_bitmap(imagem, monstro[m].xlocation, monstro[m].ylocation, 0);
        }
    }
}

void start_horda(Monstro monstro[], int n_monstros, int n_hordas)
{
    int m = 0;
    int i = 0;
    int j = 0;
    for (m = 0; m < n_monstros; m++)
    {
        if(!monstro[m].stillalive)
        {
            monstro[m].stillalive = true;
            for (i = 0; i < A; i++)
            {
                for(j = 0; j < B; j++ )
                {
                    switch (mapa[i][j])
                    {
                    case 6:
                        monstro[m].xlocation = 0 - ((m - 1) * 40);
                        monstro[m].ylocation = i * a_celula + 5;
                        monstro[m].health = 20 + (n_hordas*5 ) * 1.4;
                        monstro[m].speed = 3;
                        monstro[m].mov_x = 1;
                        monstro[m].mov_y = 0;
                        break;
                    }
                }
            }
        }
    }
}

void update_horda(Monstro monstro[], Sistema &sistema, int mapa[A][B], int n_monstros)
{
    int m = 0;
    int i = 0;
    int j = 0;
    for(m = 0; m < 10; m++)
    {
        if(monstro[m].stillalive)
        {
            int cx = (monstro[m].xlocation + 16.5)/l_celula;    //x = meio do monstro
            int dx = (monstro[m].xlocation + 33)/l_celula;      //x = direita do monstro
            int ex = monstro[m].xlocation/l_celula;             //x = esquerda do monstro
            int cy = (monstro[m].ylocation + 14.5)/a_celula;    //y = centro do monstro
            int ay = monstro[m].ylocation/a_celula;             //y = acima do monstro
            int by = (monstro[m].ylocation + 29)/a_celula;      //y = abaixo do monstro

            if (monstro[m].mov_x == 1)                          //se o monstro estiver se locomovendo para a direita
            {
                switch (mapa[cy][ex])
                {
                case 0:                                     //monstro continua com a movimentação anterior
                    monstro[m].mov_x = monstro[m].mov_x;
                    monstro[m].mov_y = monstro[m].mov_y;
                    break;
                case 1:                                     //monstro vai para baixo
                    monstro[m].mov_x = 0;
                    monstro[m].mov_y = 1;
                    break;
                case 2:                                     //monstro vai para cima
                    monstro[m].mov_x = 0;
                    monstro[m].mov_y = -1;
                    break;
                case 3:                                     //monstro vai para esquerda
                    monstro[m].mov_x = -1;
                    monstro[m].mov_y = 0;
                    break;
                case 4:                                     //monstro vai para direita
                    monstro[m].mov_x = 1;
                    monstro[m].mov_y = 0;
                    break;
                }
            }

            if (monstro[m].mov_x == -1)                         //se o monstro estiver se locomovendo para a esquerda
            {
                switch (mapa[cy][dx])
                {
                case 0:                                     //monstro continua com a movimentação anterior
                    monstro[m].mov_x = monstro[m].mov_x;
                    monstro[m].mov_y = monstro[m].mov_y;
                    break;
                case 1:                                     //monstro vai para baixo
                    monstro[m].mov_x = 0;
                    monstro[m].mov_y = 1;
                    break;
                case 2:                                     //monstro vai para cima
                    monstro[m].mov_x = 0;
                    monstro[m].mov_y = -1;
                    break;
                case 3:                                     //monstro vai para esquerda
                    monstro[m].mov_x = -1;
                    monstro[m].mov_y = 0;
                    break;
                case 4:                                     //monstro vai para direita
                    monstro[m].mov_x = 1;
                    monstro[m].mov_y = 0;
                    break;
                }
            }


            if (monstro[m].mov_y == 1)                          //se o monstro estiver se locomovendo para a baixo
            {
                switch (mapa[ay][cx])
                {
                case 0:                                     //monstro continua com a movimentação anterior
                    monstro[m].mov_x = monstro[m].mov_x;
                    monstro[m].mov_y = monstro[m].mov_y;
                    break;
                case 1:                                     //monstro vai para baixo
                    monstro[m].mov_x = 0;
                    monstro[m].mov_y = 1;
                    break;
                case 2:                                     //monstro vai para cima
                    monstro[m].mov_x = 0;
                    monstro[m].mov_y = -1;
                    break;
                case 3:                                     //monstro vai para esquerda
                    monstro[m].mov_x = -1;
                    monstro[m].mov_y = 0;
                    break;
                case 4:                                     //monstro vai para direita
                    monstro[m].mov_x = 1;
                    monstro[m].mov_y = 0;
                    break;
                }
            }

            if (monstro[m].mov_y == -1)     //se o monstro estiver se locomovendo para a cima
            {
                switch (mapa[by][cx])
                {
                case 0:                                     //monstro continua com a movimentação anterior
                    monstro[m].mov_x = monstro[m].mov_x;
                    monstro[m].mov_y = monstro[m].mov_y;
                    break;
                case 1:                                     //monstro vai para baixo
                    monstro[m].mov_x = 0;
                    monstro[m].mov_y = 1;
                    break;
                case 2:                                     //monstro vai para cima
                    monstro[m].mov_x = 0;
                    monstro[m].mov_y = -1;
                    break;
                case 3:                                     //monstro vai para esquerda
                    monstro[m].mov_x = -1;
                    monstro[m].mov_y = 0;
                    break;
                case 4:                                     //monstro vai para direita
                    monstro[m].mov_x = 1;
                    monstro[m].mov_y = 0;
                    break;
                }
            }

            if(monstro[m].health <= 0)
            {
                monstro[m].stillalive = false;
                sistema.score++;
                sistema.money += 1.5;
            }
            if(monstro[m].xlocation > sistema.x)
            {
                sistema.lives--;
                monstro[m].stillalive = false;
            }
            monstro[m].xlocation += (monstro[m].mov_x*monstro[m].speed);
            monstro[m].ylocation += (monstro[m].mov_y*monstro[m].speed);
        }
    }
}

void draw_mouse_tower(int r, int l, Torre torre[], int t)  //Desenha a torre enquanto ela estiver no mouse, nao atribui parametros
{
    al_draw_filled_circle(r*l_celula + (l_celula/2), l*a_celula + (a_celula/2), l_celula/2, al_map_rgb(150, 50, 0));
    al_draw_circle(r*l_celula + (l_celula/2), l*a_celula + (a_celula/2), torre[t].range, al_map_rgb(10, 110, 10), 0);
}

void show_tower_information(Torre torre[], int t, int r, int l, ALLEGRO_FONT *fonte)
{
    al_draw_circle(torre[t].xlocation, torre[t].ylocation, torre[t].range, al_map_rgb(10, 110, 10), 0);
    al_draw_textf(fonte, al_map_rgb(0, 0, 0), 50, 600, ALLEGRO_ALIGN_LEFT, "Tower fire power: %i", torre[t].fire_power);
    al_draw_textf(fonte, al_map_rgb(0, 0, 0), 50, 620, ALLEGRO_ALIGN_LEFT, "Tower range: %i", torre[t].range);
    al_draw_textf(fonte, al_map_rgb(0, 0, 0), 50, 640, ALLEGRO_ALIGN_LEFT, "Tower fire rate: %.2fs", torre[t].fire_rate);
    al_draw_textf(fonte, al_map_rgb(0, 0, 0), 40, 660, ALLEGRO_ALIGN_LEFT, "Upgrade:");

}

void setup_tower(Sistema &sistema, Torre torre[], int t, int r, int l)  //Define os parametros da torre quando ela é criada
{
    torre[t].price = 40;
    torre[t].xlocation = r*l_celula + (l_celula/2);
    torre[t].ylocation = l*a_celula + (a_celula/2);
    torre[t].range = 80;                             //Alcance de até 80 pixel
    torre[t].fire_rate = 0.3;                         //1 tiro a cada 0,3 segundos
    torre[t].fire_power = 25;                         //25 de dano nos monstros
    torre[t].tiro.speed = 7;                         //Velocidade do tiro
    torre[t].tiro.live = false;
    torre[t].live = true;
}

void buy_tower(Torre torre[], ALLEGRO_FONT *fonte)
{
    al_draw_textf(fonte, al_map_rgb(0, 0, 0), 700, 630, ALLEGRO_ALIGN_LEFT, "Tower price: %i bitcoins", torre[0].price);
    al_draw_textf(fonte, al_map_rgb(0, 0, 0), 700, 650, ALLEGRO_ALIGN_LEFT, "Tower fire power: %i", torre[0].fire_power);
    al_draw_textf(fonte, al_map_rgb(0, 0, 0), 700, 670, ALLEGRO_ALIGN_LEFT, "Tower range: %i", torre[0].range);
    al_draw_textf(fonte, al_map_rgb(0, 0, 0), 700, 690, ALLEGRO_ALIGN_LEFT, "Tower fire rate: %.2fs", torre[0].fire_rate);
}
void upgrade_tower(Torre torre[], int t){
    torre[t].range = 100;
    torre[t].fire_power = 60;
}


void fire_tiro(Torre torre[], Monstro monstro[], int t, int n_monstros)  //Verifica os montros e atira casa algum esteja vivo
{
    int m = 0;
    for(m = 0; m < n_monstros; m++)
    {
        if(monstro[m].stillalive)
        {
            for(int i = 0; i < t+1; i++)
            {
                if(torre[i].live && !torre[i].tiro.live &&
                        torre[i].xlocation - monstro[m].xlocation <= torre[i].range &&
                        torre[i].xlocation - monstro[m].xlocation >= - torre[i].range &&
                        torre[i].ylocation - monstro[m].ylocation <= torre[i].range &&
                        torre[i].ylocation - monstro[m].ylocation >= - torre[i].range)
                {
                    torre[i].tiro.xlocation = torre[i].xlocation;
                    torre[i].tiro.ylocation = torre[i].ylocation;
                    torre[i].tiro.fire_power = torre[i].fire_power;
                    torre[i].tiro.monstro = monstro[m];
                    torre[i].tiro.live = true;
                }
            }
        }
    }
}

void update_tiro(Torre torre[], Monstro monstro[], int t, int n_monstros)  //Atualiza a posiçao do tiro
{
    for(int i = 0; i < t+1; i++)
    {
        if(!torre[i].tiro.monstro.stillalive)
            torre[i].tiro.live = false;
        if(torre[i].tiro.monstro.stillalive)
        {
            if(torre[i].tiro.xlocation > torre[i].tiro.monstro.xlocation)
            {
                torre[i].tiro.xlocation -= torre[i].tiro.speed;
            }
            if(torre[i].tiro.xlocation < torre[i].tiro.monstro.xlocation)
            {
                torre[i].tiro.xlocation += torre[i].tiro.speed;
            }
            if(torre[i].tiro.ylocation > torre[i].tiro.monstro.ylocation)
            {
                torre[i].tiro.ylocation -= torre[i].tiro.speed;
            }
            if(torre[i].tiro.ylocation < torre[i].tiro.monstro.ylocation)
            {
                torre[i].tiro.ylocation += torre[i].tiro.speed;
            }
        }
    }
}

void colisao_horda(Torre torre[], Monstro monstro[], int t, int n_monstros, Sistema &sistema)  //Detecta se um tiro atingiu algum monstro
{
    for(int m = 0; m < n_monstros; m++)
    {
        if(monstro[m].stillalive)
        {
            for(int i = 0; i < t+1; i++)
            {
                if(torre[i].tiro.live)  //Verificacao da localizaçao
                {
                    if( torre[i].tiro.xlocation > (torre[i].tiro.monstro.xlocation - torre[i].tiro.monstro.boundx) &&
                            torre[i].tiro.xlocation < (torre[i].tiro.monstro.xlocation + torre[i].tiro.monstro.boundx) &&
                            torre[i].tiro.ylocation > (torre[i].tiro.monstro.ylocation - torre[i].tiro.monstro.boundy) &&
                            torre[i].tiro.ylocation < (torre[i].tiro.monstro.ylocation + torre[i].tiro.monstro.boundy))
                    {
                        monstro[m].health -= torre[i].tiro.fire_power;
                        torre[i].tiro.live = false;
                    }
                }
            }
        }
    }
}

void draw_tiro(Torre torre[], int t)  //Desenha o tiro durante a trajetória
{
    for(int i = 0; i < t+1; i++)
    {
        if(torre[i].tiro.live)
            al_draw_filled_circle(torre[i].tiro.xlocation, torre[i].tiro.ylocation, 5, al_map_rgb(255, 255, 255));
    }
}
