#include <stdio.h>
#include <allegro5/allegro.h>

const int LARGURA_TELA = 640;
const int ALTURA_TELA = 480;

int init_fail (ALLEGRO_DISPLAY *janela, ALLEGRO_BITMAP *botao_sair, ALLEGRO_BITMAP *area_central, ALLEGRO_EVENT_QUEUE *fila_eventos);


int main(int argc, char const *argv[]) {

    ALLEGRO_DISPLAY *janela = NULL;	//Variável para a janela
    ALLEGRO_BITMAP *botao_sair = NULL, *area_central = 0; // Retangulos
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;  //Variável para eventos

    al_init();
    al_install_mouse();

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    area_central = al_create_bitmap(LARGURA_TELA / 2, ALTURA_TELA / 2);
    botao_sair = al_create_bitmap(100, 50);
    fila_eventos = al_create_event_queue();

    al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);

    init_fail(janela, botao_sair, area_central, fila_eventos); //Funçao de teste

    al_register_event_source(fila_eventos, al_get_mouse_event_source());

    al_flip_display();	//Atualiza a tela

    return 0;
}

    int init_fail(ALLEGRO_DISPLAY *janela, ALLEGRO_BITMAP *botao_sair, ALLEGRO_BITMAP *area_central, ALLEGRO_EVENT_QUEUE *fila_eventos){
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

    if (!botao_sair)
    {
        fprintf(stderr, "Falha ao criar botão de saída.\n");
        al_destroy_bitmap(area_central);
        al_destroy_display(janela);
        return -1;
    }

    if (!area_central)
    {
        fprintf(stderr, "Falha ao criar bitmap.\n");
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
