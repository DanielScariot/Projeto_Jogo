/*   This is tower my defense game
    Copyright (C) 2015  DSG & EKB

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "player.h"

const int LARGURA_TELA = 640;
const int ALTURA_TELA = 480;
const int fps = 60;
const int A = 20;
const int B = 20;


int init_fail (ALLEGRO_DISPLAY *janela, ALLEGRO_BITMAP *imagem, ALLEGRO_EVENT_QUEUE *fila_eventos);
void init_system(Sistema &torre);
void draw_tower(ALLEGRO_BITMAP *imagem, int pos_x, int pos_y);
void draw_matrix(void);
void coor_matrix(ALLEGRO_FONT *fonte);


int main(int argc, char const *argv[]) {

    int i = 0;
    int pos_x = 0;
    int pos_y = 0;
    bool click = false;

    Sistema torre;
    char [27] = {'A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z'}

      int mapa [A][B] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    ALLEGRO_DISPLAY *janela = NULL;	//Variável para a janela
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;  //Variável para eventos
    ALLEGRO_BITMAP *imagem = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_FONT *fonte = NULL;

    al_init();
    al_install_mouse();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    fila_eventos = al_create_event_queue();
    imagem = al_load_bitmap("porquinho.jpg");
    timer = al_create_timer(1.0 / fps);
    fonte = al_load_font("/usr/share/fonts/truetype/dejavu/DejaVuSerif-Bold.ttf", 12, 0);    //Fonte DejaVu

    al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
    al_start_timer(timer);

    init_fail(janela, imagem, fila_eventos); //Funçao de teste

    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));

    init_system(torre);
    al_clear_to_color(al_map_rgb(235, 235, 235));
    al_flip_display();

    while (1)
    {
        al_clear_to_color(al_map_rgb(235, 235, 235));
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }

        else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if(evento.mouse.button & 1)
            click = !click;
        }
		else if(evento.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			pos_x = evento.mouse.x;
			pos_y = evento.mouse.y;
		}

        else if(evento.type == ALLEGRO_EVENT_TIMER){
            draw_matrix();
            coor_matrix(fonte);
            al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/4, 50, ALLEGRO_ALIGN_CENTRE, "Taxa de Frames: %i",i);
            i++;
            if(click == true){
                draw_tower(imagem, pos_x, pos_y);
            }

            al_flip_display();
        }
    }

    al_destroy_display(janela);
    al_destroy_font(fonte);
    al_destroy_event_queue(fila_eventos);

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

    void draw_matrix(void){
        int i;
        int j = 0;
        for (i=0;  i<24; i++) {
            // Linha: x1, y1, x2, y2, cor, espessura
            al_draw_line( 0,  0 + 20 * i, LARGURA_TELA, 20 * i, al_map_rgb(0, 255, 0), 1); // Linhas horizontais
        }

        /*
        for (j=0;  j<34; j++) {
            // Linha: x1, y1, x2, y2, cor, espessura
            al_draw_line(0 + 20 * j, 0, 0 + 20 * j, ALTURA_TELA, al_map_rgb(255, 0, 0), 1); // Linhas verticais
        }
        */

    }

    void coor_matrix(ALLEGRO_FONT *fonte){
        int i = 0;
        int j = 0;
        for (i=0;  i<1; i++) {
            for(j=0; j <21; j++){
        al_draw_line(0 + 34 * j, 0, 0 + 34 * j, ALTURA_TELA, al_map_rgb(255, 0, 90  ), 1);;
        al_draw_textf(fonte, al_map_rgb(0, 0, 0),34*j, 24*i, ALLEGRO_ALIGN_CENTRE, "       A%i",j+1);
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
