#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

struct Monstro
{
    int health;         //VIDA DO MONSTRO
    int speed;          //VELOCIDADE DO MONSTRO
    int xlocation;      //LOCALIZAÇÃO X DO MONSTRO
    int ylocation;      //LOCALIZAÇÃO Y DO MONSTRO
    bool stillalive;    //TRUE = VIVO    FALSE = MORTO
}Monstro[10];

const int LARGURA_TELA = 640;
const int ALTURA_TELA = 480;


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


int main(void)
{
    bool GameOver = false;
    ALLEGRO_DISPLAY *screen = NULL;
    ALLEGRO_BITMAP *imagem = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;

     if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return -1;
    }

    if (!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
        return -1;
    }

    screen = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!screen)
    {
        fprintf(stderr, "Falha ao criar janela.\n");
        return -1;
    }

    imagem = al_load_bitmap("poring.png");
    if (!imagem)
    {
        fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
        al_destroy_display(screen);
        return -1;
    }

    fila_eventos = al_create_event_queue();
    if (!fila_eventos)
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(screen);
        return -1;
    }

    al_init_primitives_addon();
    al_draw_line(640,480, 0, 0, al_map_rgb(0, 255, 0), 1000);



    /*rectfill(screen,0,0,70,200,6);
    rectfill(screen,0,200,230,250,6);
    rectfill(screen,230,100,275,250,6);
    rectfill(screen,275,100,400,150,6);
    rectfill(screen,400,100,450,350,6);
    rectfill(screen,450,300,550,350,6);

    //Draw the castle
    rectfill(screen,550,290,625,365,0);
    circlefill(screen,545,285,18,0);
    circlefill(screen,545,370,18,0);
    circlefill(screen,630,285,18,0);
    circlefill(screen,630,370,18,0);

    //Draw the rectangle that will contain the citizens
    rectfill(screen,560,300,615,350,9);

    //Draw the tower build section of the screen
    rectfill(screen,0,400,200,435,9);
    textprintf_ex(screen,font,5,415,15,9,"Click this area");
    textprintf_ex(screen,font,5,425,15,9,"to build a tower");*/

    al_register_event_source(fila_eventos, al_get_display_event_source(screen));



    al_flip_display();

    while (1)
    {
        ALLEGRO_EVENT evento;
        ALLEGRO_TIMEOUT timeout;
        al_init_timeout(&timeout, 0.05);

        int tem_eventos = al_wait_for_event_until(fila_eventos, &evento, &timeout);

        if (tem_eventos && evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }

        while(!GameOver)
    {
    //This is the main game loop

         //Create the Monstro
         for(int x = 1; x < 11; x++)
         {
                 createmonster(x);
         }

         //Internal game loop to handle monster movement, shooting, etc.
         for(int walkcount = 0; walkcount < 1500; walkcount ++)
         {
//Did the player click on 'Build a tower'?


                 //Move the Monstro
                 for (int x = 0; x < 11; x++)
                 {

                     if (Monstro[x].stillalive)
                     {
                          erasemonster(x);
                          movemonster(x);
                          drawmonster(x);
                          if (Monstro[x].xlocation > 529)
                          {
                               erasemonster(x);
                               Monstro[x].stillalive = false;
                               Monstro[x].xlocation = 0;
                               Monstro[x].ylocation = 0;
                               CitizenCounter--;
                               UpdateCitizens();
                               if (CitizenCounter == 0)
                               {
                                    while(!key[KEY_ESC])
                                    {
                                         textprintf_ex(screen,font,310,240,15,10,"GAME OVER!! Press ESCape to Exit");
                                    }
//                                    GameOver = true;
                               }
                          }
                      }
                  }
         }
     }

        al_flip_display();
    }

    al_destroy_display(screen);
    al_destroy_event_queue(fila_eventos);

    return 0;
}
