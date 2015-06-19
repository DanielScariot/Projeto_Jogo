#include <stdio.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "constantes.h"  //Variaveis constantes globais
#include "structures.h"  //Estruturas
#include "arrays.h"      //Matrizes importantes

int init_fail (ALLEGRO_DISPLAY *janela, ALLEGRO_FONT *fonte, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_BITMAP *imagem, ALLEGRO_TIMER *timer); //Fun�ao falha na inicializa�ao
void destroy_al(ALLEGRO_DISPLAY *janela,ALLEGRO_FONT *fonte, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_BITMAP *imagem, ALLEGRO_TIMER *timer);
void init_system(Sistema &sistema); //Carrega informa�oes das torres
void draw_tower(int r, int l, Torre torre[], int t);
void coor_matrix(int mapa[A][B], Coord coordenada[], ALLEGRO_FONT *fonte); //Desenha a matriz para fins de debug
void a_coord(Coord coordenada[], ALLEGRO_FONT *fonte);
int conversao_coordenadas(Coord coordenada[], char a, char b);

bool GameOver = false;
//Funçoes dos montros
void init_horda(Monstro monstro[], int n_mostros);
void start_horda(Monstro monstro[], int n_monstros);
void update_horda(Monstro monstro[], int mapa[A][B], int n_monstros);
void draw_horda(Monstro monstro[], int n_mostros, ALLEGRO_BITMAP *imagem);
void colisao_horda(Tiro tiro[], Monstro monstro[], int n_monstros);

void initTorre(Torre torre[], Tiro tiro[], int t);

void drawTiro(Tiro tiro[], int t);
void FireTiro(Tiro tiro[], Torre torre[], Monstro monstro[], int t);
void UpdateTiro(Tiro tiro[], Monstro monstro[], int t);


int main(int argc, char const *argv[])
{
    int i = 0;
    int t = 1;

    int n_mostros = 10;
    bool click = false;
    bool nova_horda = true;
    bool render = false;
    bool torre_mouse = false;

    char a;
    char b;
    int r;
    int l;

    Sistema sistema;
    Torre torre[10];
    Tiro tiro[10];
    Monstro monstro[n_mostros];
    Coord coordenada[A*B];

    //Declara�ao vair�veis allegro
    ALLEGRO_DISPLAY *janela = NULL;	            //Vari�vel para a janela
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;   //  ''     para eventos
    ALLEGRO_BITMAP *imagem = NULL;              //  ''     para imagem
    ALLEGRO_TIMER *timer = NULL;                //  ''     para o tempo (fps)
    ALLEGRO_FONT *fonte = NULL;                 //  ''     para fonte

    //Inicializa o allegro, mouse e add-ons
    al_init();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    a_coord(coordenada, fonte);
    init_horda(monstro, n_mostros);
    init_system(sistema);
    initTorre(torre, tiro, t-1);



    //Atribui atributos às variáveis allegro

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    fila_eventos = al_create_event_queue();
    imagem = al_load_bitmap("virus.png");
    timer = al_create_timer(1.0 / fps);
    fonte = al_load_font("arial.ttf", 12, 0);    //Fonte DejaVu

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

        if(evento.type == ALLEGRO_EVENT_TIMER)  //Evento de renderiza�ao
        {
            i++;
            render = true;

            FireTiro(tiro, torre, monstro, t-1);
            UpdateTiro(tiro, monstro, t-1);
            update_horda(monstro, mapa, n_mostros);
            colisao_horda(tiro, monstro, n_mostros);

        }

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            GameOver = true;
        }

        else if(evento.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            pos_x = evento.mouse.x;
            pos_y = evento.mouse.y;

            a =  coordenada[(pos_x/l_celula)].letra[0];
            b =  coordenada[(pos_x/l_celula)].letra[1];
            l =  coordenada[(pos_y/a_celula)].numero;

            r = conversao_coordenadas(coordenada, a, b);
        }

        else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            if (mapa[pos_y/a_celula][pos_x/l_celula] == 9)
            {
                torre_mouse = true;
                torre[t].in_mouse = true;
            }
            if(torre_mouse && !click)
            {
                if (mapa[pos_y/a_celula][pos_x/l_celula] != 9)
                {
                    mapa[pos_y/a_celula][pos_x/l_celula] = 10;
                    torre_mouse = false;
                    torre[t].in_mouse = false;
                    t++;
                }
            }
            click = !click;
        }

        else if(evento.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch(evento.keyboard.keycode)
            {
            case ALLEGRO_KEY_SPACE:
                start_horda(monstro, n_mostros);
                break;
            }
        }
        else if(render && al_is_event_queue_empty(fila_eventos))
        {
            render = false;

            coor_matrix(mapa, coordenada, fonte);

            al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/4, 50, ALLEGRO_ALIGN_CENTRE, "Taxa de Frames: %i", i);
            al_draw_textf(fonte, al_map_rgb(0, 0, 0), pos_x, pos_y, ALLEGRO_ALIGN_LEFT, "   x:%i y:%i", pos_x, pos_y);

            draw_horda(monstro, n_mostros, imagem);

            if(torre_mouse)
            {
                draw_tower(r, l, torre, t);
            }
            drawTiro(tiro, t-1);

            al_flip_display();
            al_clear_to_color(al_map_rgb(255,255,255));
        }
    }

    destroy_al(janela, fonte, fila_eventos, imagem, timer); //Destroi as vari�veis allegro

    return 0;
}


void init_system(Sistema &sistema)
{
    sistema.x = 400;
    sistema.y = 400;
    sistema.lives = 10;
    sistema.boundx = 405;
    sistema.boundy = 390;
    sistema.score = 0;
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

void coor_matrix(int mapa[A][B], Coord coordenada[], ALLEGRO_FONT *fonte)
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
                al_draw_filled_rectangle(m_x, m_y, m_x + l_celula, m_y + a_celula, al_map_rgb(0, 0, 255));
                break;
            case 1:
                al_draw_filled_rectangle(m_x, m_y, m_x + l_celula, m_y + a_celula, al_map_rgb(0, 0, 255));
                break;
            case 2:
                al_draw_filled_rectangle(m_x, m_y, m_x + l_celula, m_y + a_celula, al_map_rgb(0, 0, 255));
                break;
            case 3:
                al_draw_filled_rectangle(m_x, m_y, m_x + l_celula, m_y + a_celula, al_map_rgb(0, 0, 255));
                break;
            case 4:
                al_draw_filled_rectangle(m_x, m_y, m_x + l_celula, m_y + a_celula, al_map_rgb(0, 0, 255));
                break;
            case 5:
                al_draw_filled_rectangle(m_x, m_y, m_x + l_celula, m_y + a_celula, al_map_rgb(0, 255, 0));
                break;
            case 6:
                al_draw_filled_rectangle(m_x, m_y, m_x + l_celula, m_y + a_celula, al_map_rgb(255, 255, 0));
                break;
            case 9:
                al_draw_filled_circle(m_x + (l_celula/2), m_y + (a_celula/2), l_celula/2, al_map_rgb(0, 200, 0));
                break;
            case 10:
                al_draw_filled_circle(m_x + (l_celula/2), m_y + (a_celula/2), l_celula/2, al_map_rgb(255, 0, 255));
                break;
            }
            al_draw_line(0 + l_celula * j, 0, 0 + l_celula * j, ALTURA_TELA, al_map_rgb(255, 0, 90  ), 0); //Linhas verticais
            al_draw_line( 0,  0 + a_celula * i, LARGURA_TELA, a_celula * i, al_map_rgb(0, 255, 0), 0);       //Linhas horizontais

            // al_draw_textf(fonte, al_map_rgb(0, 0, 0), l_celula * j, a_celula * i, ALLEGRO_ALIGN_LEFT, "%c%c%i", coordenada[j].letra[0], coordenada[j].letra[1], coordenada[i].numero + 1);
            m_x += l_celula;
        }
        m_x = 0;
        m_y += a_celula;
    }
}

void a_coord(Coord coordenada[], ALLEGRO_FONT *fonte)
{
    int i, j, l = 0;

    for (i=0;  i<A; i++)
    {
        for(j=0; j<B; j++)
        {
            coordenada[j].numero = j;
            if(j<26)
            {
                coordenada[j].letra[0] = letras[l];
                coordenada[j].letra[1] = letras[26];
                l++;
            }
            else if (j==26)
            {
                l=0;
                coordenada[j].letra[0] = letras[0];
                coordenada[j].letra[1] = letras[l];
            }
            else
            {
                l++;
                coordenada[j].letra[0] = letras[0];
                coordenada[j].letra[1] = letras[l];
            }
        }
        l = 0;
    }
}

int conversao_coordenadas(Coord coordenada[], char a, char b)
{
    int i;
    for(i=0; i<B; i++)
    {
        if(coordenada[i].letra[0] == a && coordenada[i].letra[1] == b)
            break;
    }
    return i;
}

void init_horda(Monstro monstro[], int n_mostros)
{
    int m = 0;
    for(m = 0; m < n_mostros; m++)
    {
        monstro[m].stillalive = false;
        monstro[m].health = 20;
        monstro[m].speed = 5;
        monstro[m].boundx = 18;
        monstro[m].boundy = 18;
    }
}

void draw_horda(Monstro monstro[], int n_mostros, ALLEGRO_BITMAP *imagem)
{
    int m = 0;
    for(m = 0; m < n_mostros; m++)
    {
        if(monstro[m].stillalive)
        {
            al_draw_bitmap(imagem, monstro[m].xlocation, monstro[m].ylocation, 0);
        }
    }
}

void start_horda(Monstro monstro[], int n_monstros)
{
    int m = 0;
    int i = 0;
    int j = 0;
    for (m = 0; m < n_monstros; m++){
        if(!monstro[m].stillalive){
            monstro[m].stillalive = true;
            for (i = 0; i < A; i++) {
                for(j = 0; j < B; j++ ){
                    switch (mapa[i][j]) {
                        case 6:
                        monstro[m].xlocation = 0 - ((m - 1) * 30);
                        monstro[m].ylocation = i * a_celula;
                        break;
                    }
                }
            }
        }
    }
}

void update_horda(Monstro monstro[], int mapa[A][B], int n_monstros)
{
    int m = 0;
    int i = 0;
    int j = 0;
    for(m = 0; m < n_monstros; m++)
    {
        float movimentox = 0;
        float movimentoy = 0;
        float xmo, ymo;
        int xn = ceil(monstro[m].xlocation /(l_celula + 1));
        int yn = ceil(monstro[m].ylocation/(a_celula + 1));
        int xm = monstro[m].xlocation/l_celula;
        int ym = monstro[m].ylocation/a_celula;
            if(mapa[ym][xm] == 6)
            {
                monstro[m].xlocation += monstro[m].speed;
                xmo = monstro[m].speed;
                ymo = 0;
            }
            if(mapa[ym][xm] == 0)
            {
                monstro[m].xlocation += xmo;
                monstro[m].ylocation += ymo;
            }
            if(mapa[ym][xm] == 1)
            {
                monstro[m].ylocation += monstro[m].speed;
                ymo = monstro[m].speed;
                xmo = 0;
            }
            if(mapa[ym][xm] == 2)
            {
                monstro[m].ylocation -= monstro[m].speed;
                ymo = (-monstro[m].speed);
                xmo = 0;
            }
            if(mapa[ym][xm] == 3)
            {
                monstro[m].xlocation -= monstro[m].speed;
                xmo= (-monstro[m].speed);
                ymo = 0;
            }
            if(mapa[ym][xm] == 4)
            {
                monstro[m].xlocation += monstro[m].speed;
                xmo=monstro[m].speed;
                ymo = 0;
            }
            if(monstro[m].xlocation >= LARGURA_TELA)
            {
                monstro[m].stillalive = false;
            }
        }
}

void draw_tower(int r, int l, Torre torre[], int t)
{
    al_draw_filled_circle(r*l_celula + (l_celula/2), l*a_celula + (a_celula/2), l_celula/2, al_map_rgb(150, 50, 0));
    torre[t].xlocation = r*l_celula + (l_celula/2);
    torre[t].ylocation = l*a_celula + (a_celula/2);
    torre[t].live = true;
}


void initTorre(Torre torre[], Tiro tiro[], int t){
    torre[t].ID;
    torre[t].xlocation;
    torre[t].ylocation;
	torre[t].fire_power = 10;
	torre[t].fire_rate = 2;
	torre[t].range = 20;
    torre[t].live = false;
    torre[t].in_mouse = false;
    for(int t = 0; t < 10; t++){
        tiro[t].live = false;
        tiro[t].speed = 15;
    }
}

void drawTiro(Tiro tiro[], int t){
    for(int t = 0 ; t < 10; t++){
        if(tiro[t].live)
            al_draw_filled_circle(tiro[t].xlocation, tiro[t].ylocation, 15, al_map_rgb(255, 255, 255));
    }
}

void FireTiro(Tiro tiro[], Torre torre[], Monstro monstro[], int t){
    for(int t = 0 ; t < 10; t++){
        if(torre[t].live && !torre[t].in_mouse && !tiro[t].live){
            tiro[t].xlocation = torre[t].xlocation;
            tiro[t].ylocation = torre[t].ylocation;
            tiro[t].fire_power = torre[t].fire_power;
            tiro[t].live = true;
            break;
        }
    }
}

void UpdateTiro(Tiro tiro[], Monstro monstro[], int t){
    for(int t = 0 ; t < 10 ; t++){
        if(tiro[t].live){
            if(tiro[t].xlocation > monstro[t].xlocation)
            {
                tiro[t].xlocation -= tiro[t].speed;
            }
            if(tiro[t].xlocation < monstro[t].xlocation)
            {
                tiro[t].xlocation += tiro[t].speed;
            }
            if(tiro[t].ylocation > monstro[t].ylocation)
            {
                tiro[t].ylocation -= tiro[t].speed;
            }
            if(tiro[t].ylocation < monstro[t].ylocation)
            {
                tiro[t].ylocation += tiro[t].speed;
            }
            if(!monstro[t].stillalive){
                tiro[t].live = false;
            }
        }
    }
}

void colisao_horda(Tiro tiro[], Monstro monstro[], int n_monstros){
    for(int i = 0; i < 10; i++){
        if(tiro[i].live){
            for (int j = 0; j < n_monstros; j++){
                if(monstro[j].stillalive){
                    if(tiro[i].xlocation > (monstro[j].xlocation - monstro[j].boundx) &&
						tiro[i].xlocation < (monstro[j].xlocation + monstro[j].boundx) &&
						tiro[i].ylocation > (monstro[j].ylocation - monstro[j].boundy) &&
						tiro[i].ylocation < (monstro[j].ylocation + monstro[j].boundy))
					{
						tiro[i].live = false;
						monstro[j].stillalive = false;
					}
                }
            }
        }
    }
}
