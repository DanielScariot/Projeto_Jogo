
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "constantes.h" //Variaveis constantes globais
#include "player.h"     //Informaçoes do player
#include "Monsters.h"   //Informaçoes dos mostros
#include "arrays.h"     //Matrizes importantes

int init_fail (ALLEGRO_DISPLAY *janela, ALLEGRO_BITMAP *imagem, ALLEGRO_EVENT_QUEUE *fila_eventos); //Funçao falha na inicializaçao
void init_system(Sistema &torre); //Carrega informaçoes das torres
void draw_tower(ALLEGRO_BITMAP *imagem, int pos_x, int pos_y); //desenha a torre
void coor_matrix(ALLEGRO_FONT *fonte); //Desenha a matriz para fins de debug

//Funçoes dos montros
void createmonster(int n);
void movemonster(int n);
void erasemonster(int n);
void drawmonster(int n);


int main(int argc, char const *argv[]) {

    int i = 0;
    int pos_x = 0;
    int pos_y = 0;
    bool click = false;
    bool GameOver = false;

    Sistema torre;


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

    //Atribui atributos às variáveis allegro
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    fila_eventos = al_create_event_queue();
    imagem = al_load_bitmap("porquinho.jpg");
    timer = al_create_timer(1.0 / fps);
    fonte = al_load_font("/usr/share/fonts/truetype/dejavu/DejaVuSerif-Bold.ttf", 12, 0);    //Fonte DejaVu

    //Inicializa o mouse e tempo
    al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
    al_start_timer(timer);

    init_fail(janela, imagem, fila_eventos); //Funçao de teste

    //Regista os eventos da janela, mouse e timer na variável de eventos (fila_eventos)
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));

    init_system(torre);

    for(int x = 1; x < 11; x++)
        {
            createmonster(x);
        }

    al_clear_to_color(al_map_rgb(235, 235, 235));   //Limpa a tela
    al_draw_line(640,480, 0, 0, al_map_rgb(255, 255, 255), 1000);
    al_flip_display();                              //Atualiza a tela

    //Loop principal
    while (!GameOver)
    {
        al_clear_to_color(al_map_rgb(235, 235, 235)); //Limpa a tela
        coor_matrix(fonte);
        ALLEGRO_EVENT evento;                         //Variavel para eventos
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }

        else if(evento.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			pos_x = evento.mouse.x;
			pos_y = evento.mouse.y;
		}

        else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
            click = !click;
            al_draw_textf(fonte, al_map_rgb(0, 0, 0), pos_x, pos_y, ALLEGRO_ALIGN_LEFT, "%i%i%i",pos_x, pos_y, click);

        }

            //Internal game loop to handle monster movement, shooting, etc.
                //Move the Monstro
                for (int x = 0; x < 11; x++)
                {

                    if (Monstro[x].stillalive == true)
                    {
                        erasemonster(x);
                        movemonster(x);
                        drawmonster(x);

                        if (Monstro[x].xlocation > 529)
                        {
                            //erasemonster(x);
                            Monstro[x].stillalive = false;
                            Monstro[x].xlocation = 0;
                            Monstro[x].ylocation = 0;
                        }
                    }

                }
        if(evento.type == ALLEGRO_EVENT_TIMER){ //Evento de renderizaçao
            al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/4, 50, ALLEGRO_ALIGN_CENTRE, "Taxa de Frames: %i, click: %i",i, click);
            i++;
            al_flip_display();
        }

    }

    //Destroi as variáveis allegro
    al_destroy_display(janela);
    al_destroy_font(fonte);
    al_destroy_event_queue(fila_eventos);
    al_destroy_bitmap(imagem);
    al_destroy_timer(timer);

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

    void draw_tower(ALLEGRO_BITMAP *imagem, int pos_x, int pos_y){
        al_draw_bitmap(imagem, pos_x, pos_y, 0);
    }

    void coor_matrix(ALLEGRO_FONT *fonte){
        int i = 0;
        int j = 0;
        for (i=0;  i<A; i++) {
            for(j=0; j<B; j++){
                al_draw_line(0 + (LARGURA_TELA / B) * j, 0, 0 + (LARGURA_TELA / B) * j, ALTURA_TELA, al_map_rgb(255, 0, 90  ), 1); //Linhas verticais
                al_draw_line( 0,  0 + (ALTURA_TELA / A) * i, LARGURA_TELA, (ALTURA_TELA / A) * i, al_map_rgb(0, 255, 0), 1);       //Linhas horizontais
                al_draw_textf(fonte, al_map_rgb(0, 0, 0),(LARGURA_TELA / B)*j, (ALTURA_TELA / A)*i, ALLEGRO_ALIGN_LEFT  , "%c%i",letras[j], i+1);
            }
        }
    }


int init_fail(ALLEGRO_DISPLAY *janela, ALLEGRO_BITMAP *imagem, ALLEGRO_EVENT_QUEUE *fila_eventos){
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

}

void createmonster(int n)
{
    Monstro[n].xlocation = 45;
    Monstro[n].ylocation = 0 - ((n - 1) * 50);
    Monstro[n].health = 5;
    Monstro[n].speed = 1;
    Monstro[n].stillalive = true;
}
void movemonster(int n)
{
    if ((Monstro[n].xlocation < 250) && (Monstro[n].ylocation < 225))
    {
        Monstro[n].ylocation++;
    }
    if ((Monstro[n].xlocation < 250) && (Monstro[n].ylocation == 225))
    {
        Monstro[n].xlocation++;
    }
    if ((Monstro[n].xlocation == 250) && (Monstro[n].ylocation > 125))
    {
        Monstro[n].ylocation--;
    }
    if ((Monstro[n].xlocation < 425) && (Monstro[n].ylocation == 125))
    {
        Monstro[n].xlocation++;
    }
    if ((Monstro[n].xlocation == 425) && (Monstro[n].ylocation < 325))
    {
        Monstro[n].ylocation++;
    }
    if ((Monstro[n].xlocation < 530) && (Monstro[n].ylocation == 325))
    {
        Monstro[n].xlocation++;
    }
}
void erasemonster(int n)
{
    al_draw_filled_circle(Monstro[n].xlocation, Monstro[n].ylocation, 18, al_map_rgb(255, 255, 0));
}
void drawmonster(int n)
{
    al_draw_filled_circle(Monstro[n].xlocation, Monstro[n].ylocation, 18, al_map_rgb(0, 0, 255));
}
