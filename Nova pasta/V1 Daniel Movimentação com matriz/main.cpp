
#include <stdio.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "constantes.h"  //Variaveis constantes globais
#include "arrays.h"     //Matrizes importantes
#include "structures.h"  //Estruturas

int init_fail (ALLEGRO_DISPLAY *janela, ALLEGRO_FONT *fonte, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_BITMAP *imagem, ALLEGRO_TIMER *timer); //Funçao falha na inicializaçao
void destroy_al(ALLEGRO_DISPLAY *janela,ALLEGRO_FONT *fonte, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_BITMAP *imagem, ALLEGRO_TIMER *timer);
void init_system(Sistema &torre); //Carrega informaçoes das torres
void draw_tower(ALLEGRO_BITMAP *imagem, int pos_x, int pos_y); //desenha a torre
void coor_matrix(int mapa[A][B], ALLEGRO_FONT *fonte); //Desenha a matriz para fins de debug
void a_coord(Coord coordenada[], ALLEGRO_FONT *fonte);

//Funçoes dos montros
void init_horda(Monstro monstro[], int n_mostros);
void start_horda(Monstro monstro[], int n_monstros);
void update_horda(Monstro monstro[], int n_monstros);
void draw_horda(Monstro monstro[], int n_mostros, ALLEGRO_BITMAP *imagem);

int main(int argc, char const *argv[]) {

    int i = 0;
    int pos_x = 0;
    int pos_y = 0;
    int n_mostros = 10;
    bool nova_horda = true;
    bool GameOver = false;
    bool render = false;

    Sistema torre;
    Monstro monstro[n_mostros];
    Coord coordenada[A*B];

    //Declaraçao vairáveis allegro
    ALLEGRO_DISPLAY *janela = NULL;	            //Variável para a janela
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
    init_system(torre);

    //Atribui atributos às variáveis allegro
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    fila_eventos = al_create_event_queue();
    imagem = al_load_bitmap("virus.png");
    timer = al_create_timer(1.0 / fps);
    fonte = al_load_font("arial.ttf", 12, 0);    //Fonte DejaVu

    //Inicializa o mouse e tempo
    al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
    al_start_timer(timer);
    init_fail(janela, fonte, fila_eventos, imagem, timer); //Funçao de teste

    //Regista os eventos da janela, mouse e timer na variável de eventos (fila_eventos)
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));

    al_clear_to_color(al_map_rgb(235, 235, 235));   //Limpa a tela
    al_flip_display();                              //Atualiza a tela

    //Loop principal
    while (!GameOver)
    {
        ALLEGRO_EVENT evento;                         //Variavel para eventos
        al_wait_for_event(fila_eventos, &evento);

        if(evento.type == ALLEGRO_EVENT_TIMER){ //Evento de renderizaçao
            i++;
            render = true;

            update_horda(monstro, n_mostros);
        }


        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            GameOver = true;
        }

        else if(evento.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			pos_x = evento.mouse.x;
			pos_y = evento.mouse.y;
		}

        else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
            start_horda(monstro, n_mostros);
        }

		else if(render && al_is_event_queue_empty(fila_eventos))
		{
			render = false;

            coor_matrix(mapa, fonte);
            al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/4, 50, ALLEGRO_ALIGN_CENTRE, "Taxa de Frames: %i", i);
            al_draw_textf(fonte, al_map_rgb(0, 0, 0), pos_x, pos_y, ALLEGRO_ALIGN_LEFT, "   x:%i y:%i", pos_x, pos_y);

            draw_horda(monstro, n_mostros, imagem);

            al_draw_textf(fonte, al_map_rgb(255, 0, 0), 5*l_celula, 7*a_celula + 15, ALLEGRO_ALIGN_LEFT, "%c%i", coordenada[5].letra, coordenada[7+1].numero);

            al_flip_display();
			al_clear_to_color(al_map_rgb(255,255,255));
		}
    }

    destroy_al(janela, fonte, fila_eventos, imagem, timer); //Destroi as variáveis allegro

    return 0;
}

void init_system(Sistema &torre) {
    torre.x = 400;
    torre.y = 400;
    torre.lives = 10;
    torre.boundx = 405;
    torre.boundy = 390;
    torre.score = 0;
}

int init_fail(ALLEGRO_DISPLAY *janela, ALLEGRO_FONT *fonte, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_BITMAP *imagem, ALLEGRO_TIMER *timer){
    if (!al_init()){
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return -1;
    }
    if (!al_install_mouse()){
        fprintf(stderr, "Falha ao inicializar o mouse.\n");
        al_destroy_display(janela);
        return -1;
    }
    if (!janela){
        fprintf(stderr, "Falha ao criar janela.\n");
        return -1;
    }
    if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT)){
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        al_destroy_display(janela);
        return -1;
    }
    if (!imagem){
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
    if(!fonte){
        fprintf(stderr, "Falha ao inicializar a fonte.\n");
        al_destroy_font(fonte);
        al_destroy_display(janela);
        return -1;
    }
    if(!timer){
        fprintf(stderr, "Falha ao inicializar o timer.\n");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        return -1;
    }
}

void destroy_al(ALLEGRO_DISPLAY *janela,ALLEGRO_FONT *fonte, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_BITMAP *imagem, ALLEGRO_TIMER *timer){
    al_destroy_display(janela);
    al_destroy_font(fonte);
    al_destroy_event_queue(fila_eventos);
    al_destroy_bitmap(imagem);
    al_destroy_timer(timer);
}

void draw_tower(ALLEGRO_BITMAP *imagem, int pos_x, int pos_y){
    al_draw_bitmap(imagem, pos_x, pos_y, 0);
}

void coor_matrix(int mapa[A][B], ALLEGRO_FONT *fonte){
    int i = 0;
    int j = 0;
    int m_x = 0;
    int m_y = 0;

    for (i=0;  i<A; i++) {
        for(j=0; j<B; j++){

            switch (mapa[i][j]){
                case 0:
                    al_draw_filled_rectangle(m_x, m_y, m_x + l_celula, m_y + a_celula, al_map_rgb(255, 255, 255));
                    break;
                case 1:
                    al_draw_filled_rectangle(m_x, m_y, m_x + l_celula, m_y + a_celula, al_map_rgb(0, 255, 0));
                    break;
            }
            al_draw_line(0 + (LARGURA_TELA / B) * j, 0, 0 + (LARGURA_TELA / B) * j, ALTURA_TELA, al_map_rgb(255, 0, 90  ), 0); //Linhas verticais
            al_draw_line( 0,  0 + (ALTURA_TELA / A) * i, LARGURA_TELA, (ALTURA_TELA / A) * i, al_map_rgb(0, 255, 0), 0);       //Linhas horizontais
            al_draw_textf(fonte, al_map_rgb(0, 0, 0),(LARGURA_TELA / B)*j, (ALTURA_TELA / A)*i, ALLEGRO_ALIGN_LEFT, "%c%i",letras[j], i+1);

            m_x += l_celula;
        }
    m_x = 0;
    m_y += a_celula;
    }
}

void a_coord(Coord coordenada[], ALLEGRO_FONT *fonte){
    int i, j;

    for (i=0;  i<A; i++){
        for(j=0; j<B; j++){
            coordenada[j].letra = letras[j];
            coordenada[j].numero = j;
        }
    }
}
void init_horda(Monstro monstro[], int n_mostros){
    for(int i=0; i < n_mostros; i++){
        monstro[i].stillalive = false;
        monstro[i].health = 20;
        monstro[i].speed = 2;
        monstro[i].boundx = 18;
        monstro[i].boundy = 18;
    }
}

void draw_horda(Monstro monstro[], int n_mostros, ALLEGRO_BITMAP *imagem){
    for(int n=0; n < n_mostros; n++){
        if(monstro[n].stillalive){
            al_draw_bitmap(imagem, monstro[n].xlocation, monstro[n].ylocation, 0);
            al_draw_filled_circle(monstro[n].xlocation, monstro[n].ylocation, 18, al_map_rgb(0, 0, 255));
        }
    }
}

void start_horda(Monstro monstro[], int n_monstros){
    for(int n = 0; n < n_monstros; n++){
        if(!monstro[n].stillalive){
            monstro[n].stillalive = true;
            for(int x=0;x<A;x++)
            {
                for(int y=0;y<B;y++)
                {
                    if (mapa[x][y] == 6)
                    {
                        monstro[n].xlocation = 0 - ((n - 1) *50);
                        monstro[n].ylocation = y*Tamanho_y;
                        break;
                    }
                }
            }
            /*monstro[n].xlocation = 45;
            monstro[n].ylocation = -40 - ((n - 1) * 50);*/
            monstro[n].health = 5;
        }
    }
}

void update_horda(Monstro monstro[], int n_monstros){
    for(int n = 0; n < n_monstros; n++){
        float movimento;
        for(int x = 0; x < A ; x++)
        {
            for(int y = 0; y < B ; y++)
            {
                float xmf = monstro[n].xlocation/Tamanho_x;
                float ymf = monstro[n].ylocation/Tamanho_y;
                int xm = roundf(xmf);
                int ym = roundf(ymf);
                if(mapa[xm][ym] == 6)
                {
                        movimento = monstro[n].xlocation+=20;
                        break;
                }
                if(mapa[xm][ym] == 0)
                {
                        movimento = movimento;
                        break;
                }
                if(mapa[xm][ym] == 1)
                {
                        movimento = monstro[n].ylocation+=20;
                        break;
                }
                if(mapa[xm][ym] == 2)
                {
                        movimento = monstro[n].ylocation-=20;
                        break;
                }
                if(mapa[xm][ym] == 3)
                {
                        movimento = monstro[n].xlocation-=20;
                        break;
                }
                if(mapa[xm][ym] == 4)
                {
                        movimento = monstro[n].xlocation+=20;
                        break;
                }

            }

        }
/*
    if ((monstro[n].xlocation < 250) && (monstro[n].ylocation < 225))
        monstro[n].ylocation += monstro[n].speed;

    if ((monstro[n].xlocation < 250) && (monstro[n].ylocation > 225))
        monstro[n].xlocation += monstro[n].speed;

    if ((monstro[n].xlocation == 250) && (monstro[n].ylocation > 125))
        monstro[n].ylocation += monstro[n].speed;

    if ((monstro[n].xlocation < 425) && (monstro[n].ylocation > 125))
        monstro[n].xlocation += monstro[n].speed;

    if ((monstro[n].xlocation == 425) && (monstro[n].ylocation < 325))
        monstro[n].ylocation += monstro[n].speed;

    if ((monstro[n].xlocation < LARGURA_TELA) && (monstro[n].ylocation > 325))
        monstro[n].xlocation += monstro[n].speed;*/
    }
}
