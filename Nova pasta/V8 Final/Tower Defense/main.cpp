#include <stdio.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "structures.h"  //Estruturas - Deve vir antes das constantes
#include "constantes.h"  //Variaveis constantes globais
#include "arrays.h"      //Matrizes importantes

int init_fail (ALLEGRO_DISPLAY *janela, ALLEGRO_FONT *fonte, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_BITMAP *imagem, ALLEGRO_TIMER *timer, ALLEGRO_BITMAP *trilha, ALLEGRO_BITMAP *fundao, ALLEGRO_BITMAP *spawn, ALLEGRO_BITMAP *the_end, ALLEGRO_BITMAP *monstro2, ALLEGRO_BITMAP *monstro3, ALLEGRO_BITMAP *torre1, ALLEGRO_BITMAP *upgrade, ALLEGRO_BITMAP *sell );
void destroy_al(ALLEGRO_DISPLAY *janela,ALLEGRO_FONT *fonte, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_BITMAP *imagem, ALLEGRO_TIMER *timer);
    void init_system(Sistema &sistema); //Carrega informaçoes das torres
    void draw_background(int mapa[A][B], ALLEGRO_FONT *fonte, ALLEGRO_BITMAP *trilha, ALLEGRO_BITMAP *fundao, ALLEGRO_BITMAP *spawn, ALLEGRO_BITMAP *the_end, ALLEGRO_BITMAP *planofundo); //Desenha a matriz para fins de debug

    //Funçoes dos montros
    void init_horda(Monstro monstro[tipos_monstros][n_monstros], int n_monstros, int n_hordas, int tipos_monstros);
    void start_horda(Monstro monstro[tipos_monstros][n_monstros], int n_monstros, int n_hordas, int tipos_monstros);
    void update_horda(Monstro monstro[tipos_monstros][n_monstros], Sistema &sistema, int mapa[A][B], int n_monstros, int tipos_monstros);
    void draw_horda(Monstro monstro[tipos_monstros][n_monstros], int n_monstros, ALLEGRO_BITMAP *imagem, int tipos_monstros, ALLEGRO_BITMAP *monstro2, ALLEGRO_BITMAP *monstro3);
    void colisao_horda(Torre torre[], Monstro monstro[tipos_monstros][n_monstros], int t, int n_monstros, Sistema &sistema, int *resposta, int tipos_monstros);

    //Funçoes das torres
    void setup_tower(Torre torre[], Tipo tipo[], int t, int r, int l);
    void draw_mouse_tower(int r, int l, int tmouse, int t, Torre torre[], Tipo tipo1[], Tipo tipo2[], ALLEGRO_BITMAP *torre1, ALLEGRO_BITMAP *torre2);
    void draw_towers(int mapa[A][B], Sistema &sistema, ALLEGRO_FONT *fonte, ALLEGRO_BITMAP *the_end, ALLEGRO_BITMAP *torre1, ALLEGRO_BITMAP *upgrade, ALLEGRO_BITMAP *sell, ALLEGRO_BITMAP *torre2, ALLEGRO_FONT *fonte2);
    void show_tower_information(Torre torre[], int t, ALLEGRO_FONT *fonte);
    void buy_tower(Tipo tipo[], ALLEGRO_FONT *fonte20);
    void tower1_upgrades(Torre torre[], int t, Tipo tipo1[]);
    void tower2_upgrades(Torre torre[], int t, Tipo tipo2[]);
    void show_upgrade_information(Torre torre[], Tipo tipo[], int t, ALLEGRO_FONT *fonte);
    int find_tower_ID(Torre torre[], int t, int r, int l);
    void sell_tower(Sistema &sistema, Torre torre[], int ID);

    void setup_torre1(Tipo tipo1[]);
    void setup_torre2(Tipo tipo2[]);

    //Funçoes dos tiros
    void draw_tiro(Torre torre[], int t);
    void fire_tiro(Torre torre[], Monstro monstro[tipos_monstros][n_monstros], int t, int n_monstros, int tipos_monstros);
    void update_tiro(Torre torre[], Monstro monstro[tipos_monstros][n_monstros], int t, int n_monstros, int tipos_monstros);

    //restart functions
    void setup_array(int mapa[A][B]);
    void restart_tower(Torre torre[], int t);

int main(int argc, char const *argv[])
{
    int n_hordas = 0;             //Numero de hordas chamadas
    int r;                        //Variável para colunas
    int l;                        //Variável para linhas
    int torre_ID;                 //Identifica as torres

    bool torre_mouse = false;     //Se a torre está no mouse
    bool info_torre = false;      //Chama a funçao de informaçoes da torre
    bool info_ups1 = false;        //Chama a funçao de exibiçao dos upgrades
    bool info_ups2 = false;
    bool compra_torre = false;    //Exibe as informaçoes da torre a ser comprada
    bool render = false;          //Renderizaçao

    int t = 1, i = 0, s = 0;
    int resposta = 0;
    int gamestate = 0;
    int desespero = 0;
    int tmouse = 0;
    bool sell_t = false;

    //Setup inicial
    Sistema sistema;

    Monstro monstro[tipos_monstros][n_monstros];

    Tipo tipo_torre[num_upgrades];
    Tipo tipo1[num_upgrades];
    Tipo tipo2[num_upgrades];
    Torre torre[100];

    //Declaracao vairaveis allegro
    ALLEGRO_DISPLAY *janela = NULL;	            //Vari�vel para a janela
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;   //  ''     para eventos
    ALLEGRO_TIMER *timer = NULL;                //  ''     para o tempo (fps)
    ALLEGRO_FONT *fonte = NULL;                 //  ''     para fonte
    ALLEGRO_BITMAP *imagem = NULL;              //  ''     para imagem
    ALLEGRO_BITMAP *trilha = NULL;
    ALLEGRO_BITMAP *fundao = NULL;
    ALLEGRO_BITMAP *spawn = NULL;
    ALLEGRO_BITMAP *the_end = NULL;
    ALLEGRO_BITMAP *monstro2 = NULL;
    ALLEGRO_BITMAP *monstro3 = NULL;
    ALLEGRO_BITMAP *torre1 = NULL;
    ALLEGRO_BITMAP *upgrade = NULL;
    ALLEGRO_BITMAP *telainicial = NULL;
    ALLEGRO_BITMAP *torre2 = NULL;
    ALLEGRO_BITMAP *planoescrita = NULL;
    ALLEGRO_BITMAP *planofundo = NULL;
    ALLEGRO_BITMAP *background = NULL;
    ALLEGRO_BITMAP *warning = NULL;
    ALLEGRO_BITMAP *sell = NULL;
    ALLEGRO_BITMAP *loadvirus = NULL;
    ALLEGRO_BITMAP *theend = NULL;
    ALLEGRO_FONT *fonte40 = NULL;
    ALLEGRO_FONT *fonte30 = NULL;
    ALLEGRO_FONT *fonte20 = NULL;
    ALLEGRO_SAMPLE *musica = NULL;

    //Inicializa o allegro, mouse e add-ons
    al_init();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    //Setup inicial do sistema, monstros e torres
    init_horda(monstro, n_monstros, n_hordas, tipos_monstros);
    init_system(sistema);

    //Setup inicial dos tipos de torre
    setup_torre1(tipo1);
    setup_torre2(tipo2);

    //Atribui atributos às variáveis allegro
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    fila_eventos = al_create_event_queue();
    imagem = al_load_bitmap("virus.png");
    trilha = al_load_bitmap("fundoc.jpg");
    fundao = al_load_bitmap("fundod.jpg");
    spawn = al_load_bitmap("Spawn.jpg");
    the_end = al_load_bitmap("The_end.jpg");
    monstro2 = al_load_bitmap("Virus1.png");
    monstro3 = al_load_bitmap("virus3.png");
    torre1 = al_load_bitmap("torre1.png");
    telainicial = al_load_bitmap("PlanoFundo.jpg");
    planoescrita = al_load_bitmap("PlanoEscrita.png");
    planofundo = al_load_bitmap("fundo.png");
    torre2 = al_load_bitmap("torre2.png");
    loadvirus = al_load_bitmap("Baidu.png");
    upgrade = al_load_bitmap("green_button.png");
    sell = al_load_bitmap("yellow_button.png");
    theend = al_load_bitmap("theend.JPG");
    background = al_load_bitmap("background.jpg");
    warning = al_load_bitmap("Warning.jpg");
    timer = al_create_timer(1.0 / fps);
    fonte = al_load_font("arial.ttf", 12, 0);
    fonte40 = al_load_font("arial.ttf", 40, 0);
    fonte30 = al_load_font("isocpeur.ttf", 30, 0);
    fonte20 = al_load_font("arial.ttf", 20, 0);

    al_convert_mask_to_alpha(imagem, al_map_rgb(255,255,255));
    al_convert_mask_to_alpha(monstro2, al_map_rgb(255,255,255));
    al_convert_mask_to_alpha(monstro3, al_map_rgb(255,255,255));

    //Inicializa o mouse e tempo
    al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
    al_start_timer(timer);
    al_install_keyboard();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(3);
    init_fail(janela, fonte, fila_eventos, imagem, timer, trilha, fundao, spawn, the_end, monstro2, monstro3, torre1, upgrade, sell); //Fun�ao de teste de inicializaçao do allegro

    musica = al_load_sample("SkyrimT.wav");

    //Regista os eventos da janela, mouse e timer na vari�vel de eventos (fila_eventos)
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));

    al_clear_to_color(al_map_rgb(235, 235, 235));       //Limpa a tela
    al_flip_display();                                  //Atualiza a tela

    //Loop principal
    while (!GameOver)
    {
        ALLEGRO_EVENT evento;                           //Variavel para eventos
        al_wait_for_event(fila_eventos, &evento);       //Espera por eventos

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            GameOver = true;
        }

        switch(gamestate)
        {
        case 0:  //Menu inicial
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
                    gamestate = 3;
                    break;
                case ALLEGRO_KEY_SPACE:
                    gamestate = 3;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    gamestate = 2;
                    break;
                }
            }
            break;
        }
        case 1:  //Jogo
        {
            al_play_sample(musica, 0.7, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
            if(evento.type == ALLEGRO_EVENT_TIMER)
            {
                update_horda(monstro, sistema, mapa, n_monstros, tipos_monstros);
                fire_tiro(torre, monstro, t, n_monstros, tipos_monstros);
                update_tiro(torre, monstro, t, n_monstros, tipos_monstros);
                colisao_horda(torre, monstro, t, n_monstros, sistema, &resposta, tipos_monstros);

                i++;
                if(i >= 60)
                {
                    s++;
                    i = 0;
                }
                render = true;
                if(sistema.lives <= 0)
                    gamestate = 2;
            }
            if(evento.type == ALLEGRO_EVENT_MOUSE_AXES)
            {
                pos_x = evento.mouse.x; //Armazena a posiçao x do mouse
                pos_y = evento.mouse.y; //Armazena a posiçao y do mouse

                r = pos_x/l_celula; // Atribui uma celula de coluna
                l = pos_y/a_celula; // Atribui uma celula de linha

                switch (mapa[l][r]) {
                    case 10:
                        compra_torre = true;
                        info_torre = false;
                        info_ups1 = false;
                        info_ups2 = false;
                        tipo_torre[0] = tipo1[0];
                        mapa[19][27] = 0;
                        break;
                    case 20:
                        compra_torre = true;
                        info_torre = false;
                        info_ups1 = false;
                        info_ups2 = false;
                        tipo_torre[0] = tipo2[0];
                        mapa[19][27] = 0;
                        break;
                    case 12:
                        sell_t = false;
                        info_ups1 = true;
                        break;
                    case 13:
                        info_ups1 = false;
                        info_ups2 = false;
                        sell_t = true;
                        break;
                    case 22:
                        sell_t = false;
                        info_ups2 = true;
                        break;
                    default:
                        info_ups1 = false;
                        info_ups2 = false;
                }
            }

            else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                switch (mapa[l][r])
                {
                case 11:
                    torre_ID = find_tower_ID(torre, t, r, l);
                    info_torre = true;
                case 12:
                    if(torre[torre_ID].upgrade < 4){
                        mapa[19][27] = 12;
                        mapa[19][28] = 12;
                    }
                    else{
                        mapa[19][27] = 0;
                        mapa[19][28] = 0;
                    }
                        mapa[20][28] = 13;
                        mapa[20][27] = 13;
                    break;
                case 13: //vende as torres
                    torre[torre_ID].live = false;
                    mapa[torre[torre_ID].l][torre[torre_ID].r] = 0;
                    mapa[19][27] = 0;
                    mapa[19][28] = 0;
                    mapa[20][27] = 0;
                    mapa[20][28] = 0;
                    sistema.money += (75 * torre[torre_ID].upgrade_price)/100;
                    sell_t = false;
                    break;
                case 21:
                    torre_ID = find_tower_ID(torre, t, r, l);
                    info_torre = true;
                    mapa[19][27] = 0;
                    mapa[19][28] = 0;
                case 22:
                    if(torre[torre_ID].upgrade < 4){
                        mapa[19][27] = 22;
                        mapa[19][28] = 22;
                    }
                    else{
                        mapa[19][27] = 0;
                        mapa[19][28] = 0;
                    }
                        mapa[20][28] = 13;
                        mapa[20][27] = 13;
                    break;
                default:
                    info_torre = false;
                    mapa[19][27] = 0;
                    mapa[19][28] = 0;
                    mapa[20][28] = 0;
                    mapa[20][27] = 0;
                }
                if(!torre_mouse && mapa[l][r] == 10){
                    info_torre = false;
                    compra_torre = true;
                    tipo_torre[0] = tipo1[0];
                    mapa[19][27] = 0;
                    mapa[19][28] = 0;
                    if(sistema.money >= tipo_torre[0].price && evento.mouse.button & 1){
                        torre_mouse = true;
                        tmouse = 1;
                    }
                }
                if(!torre_mouse && mapa[l][r] == 20){
                    info_torre = false;
                    compra_torre = true;
                    tipo_torre[0] = tipo2[0];
                    mapa[19][27] = 0;
                    mapa[19][28] = 0;
                    if(sistema.money >= tipo_torre[0].price && evento.mouse.button & 1){
                        torre_mouse = true;
                        tmouse = 2;
                    }
                }
                if(torre_mouse && (mapa[l][r] == 0 || mapa[l][r] == 5) && evento.mouse.button & 1) //Posicionamento da torre enquanto ela estiver no mouse
                {
                    setup_tower(torre, tipo_torre, t, r, l);
                    sistema.money -= tipo_torre[0].price;      //Pagamento da torre
                    torre_ID = find_tower_ID(torre, t, r, l);
                    mapa[19][27] = 12;
                    mapa[19][28] = 12;
                    mapa[20][28] = 13;
                    mapa[20][27] = 13;
                    info_torre = true;
                    torre_mouse = false;
                    compra_torre = false;
                    t++;
                }
                if(torre_mouse && evento.mouse.button & 2)  //Cancela compra
                {
                    torre_mouse = false;
                    compra_torre = false;
                }
                if(compra_torre && mapa[l][r] != 10 && mapa[l][r] != 20)  //Termina a exibiçao da torre a ser comprada
                {
                    compra_torre = false;
                }
                if(mapa[l][r] == 12 && sistema.money >= torre[torre_ID].upgrade_price){
                    info_torre = true;
                    sistema.money -= torre[torre_ID].upgrade_price;
                    tower1_upgrades(torre, torre_ID, tipo1);
                }
                if(mapa[l][r] == 22 && sistema.money >= torre[torre_ID].upgrade_price){
                    info_torre = true;
                    sistema.money -= torre[torre_ID].upgrade_price;
                    tower2_upgrades(torre, torre_ID, tipo2);
                }
            }
            else if(evento.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                if(resposta == 1)
                {
                    switch(evento.keyboard.keycode)
                    {
                    case ALLEGRO_KEY_SPACE: //Inicializa uma nova horda
                        start_horda(monstro, n_monstros, n_hordas, tipos_monstros);
                        n_hordas++;
                        break;
                    }
                }
            }
            break;
        }
        case 2: //Fim de jogo
        {
            al_destroy_sample(musica);
            if(evento.type == ALLEGRO_EVENT_TIMER)
            {
                render = true;
            }
            if(evento.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                desespero++;
            }
            if(desespero < 6)
            {
                al_clear_to_color(al_map_rgb(255,255,255));
                al_draw_bitmap(loadvirus, 0, 0, 0);
                al_flip_display();
            }
            if(desespero >= 6)
            {
                if(evento.type == ALLEGRO_EVENT_KEY_DOWN)
                {
                    switch(evento.keyboard.keycode)
                    {
                    case ALLEGRO_KEY_R:
                        init_system(sistema);
                        init_horda(monstro, n_monstros, n_hordas, tipos_monstros);
                        restart_tower(torre, t);
                        n_hordas = 0;
                        setup_array(mapa);
                        s = 0;
                        desespero = 0;
                        gamestate = 1;
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        GameOver = true;
                        break;
                    }
                }
                al_clear_to_color(al_map_rgb(255,255,255));
                al_draw_bitmap(theend,0,0,0);
                al_flip_display();
            }
            break;
        }
        case 3:
            {
                al_clear_to_color(al_map_rgb(255,255,255));
                al_draw_bitmap(warning, 0, 0, 0);
                al_flip_display();
                if(evento.type == ALLEGRO_EVENT_KEY_DOWN)
                {
                    switch(evento.keyboard.keycode)
                    {
                    case ALLEGRO_KEY_ENTER:
                        gamestate = 1;
                        break;
                    case ALLEGRO_KEY_SPACE:
                        gamestate = 1;
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        gamestate = 2;
                        break;
                }
            }
            break;
        }
        }
        if(render && al_is_event_queue_empty(fila_eventos))
        {
            render = false;

            if(gamestate == 0)
            {
                al_clear_to_color(al_map_rgb(255,255,255));
                al_draw_bitmap(telainicial, 0 , 0 , 0);
                al_draw_bitmap(planoescrita, (LARGURA_TELA/2) -250 , (ALTURA_TELA/2) - 75 , 0);

            }
            if(gamestate == 1)
            {

                al_draw_bitmap(background, 0, 0, 0);

                draw_background(mapa, fonte, trilha, fundao, spawn, the_end, planofundo); //Desenha o plano de fundo

                draw_towers(mapa, sistema, fonte, the_end, torre1, upgrade, sell, torre2, fonte20); //Desenha as torres

                al_draw_textf(fonte20, al_map_rgb(255, 255, 255), 800, 15, ALLEGRO_ALIGN_LEFT, "Vidas do sistema %i", sistema.lives);
                al_draw_textf(fonte20, al_map_rgb(255, 255, 255), 800, 35, ALLEGRO_ALIGN_LEFT, "Bitcoins %.2f", sistema.money);
                al_draw_textf(fonte20, al_map_rgb(255, 255, 255), 30, 15, ALLEGRO_ALIGN_LEFT, "Monstros mortos: %i  Wave: %i", sistema.score, n_hordas);

                if(resposta == 1)
                {
                        al_draw_textf(fonte20, al_map_rgb(255,0,255), LARGURA_TELA/2, 690, 0, "Pressione ESPAÇO para iniciar a horda %d", n_hordas + 1);
                }

                //al_draw_textf(fonte, al_map_rgb(0, 0, 0), pos_x, pos_y, ALLEGRO_ALIGN_LEFT, "l:%i r:%i", l, r);
                //al_draw_textf(fonte, al_map_rgb(0, 0, 0), pos_x, pos_y + 15, ALLEGRO_ALIGN_CENTRE, "mapa[l][r]: %i", mapa[l][r]);

                draw_horda(monstro, n_monstros, imagem, tipos_monstros, monstro2, monstro3); //Desenha os montros

                if(torre_mouse)
                {
                    draw_mouse_tower(r, l, tmouse, torre_ID, torre, tipo1, tipo2, torre1, torre2); //Desenha a torre somente enquanto ela estiver no mouse
                }
                if(info_torre)
                {
                    show_tower_information(torre, torre_ID, fonte); //info torres
                }
                if(compra_torre)
                {
                    buy_tower(tipo_torre, fonte20); //Exibe as informaçoes da torre a ser comprada
                }
                if(info_ups1){
                    show_upgrade_information(torre, tipo1, torre_ID, fonte);
                }
                if(info_ups2){
                    show_upgrade_information(torre, tipo2, torre_ID, fonte);
                }
                if(sell_t){
                    al_draw_textf(fonte, al_map_rgb(255, 255, 255), 9 * l_celula, 23 * a_celula, ALLEGRO_ALIGN_LEFT, "Selling price: %i", ((75 * torre[torre_ID].upgrade_price)/100));
                }
                al_draw_textf(fonte20, al_map_rgb(255, 255, 255), 800, 55, ALLEGRO_ALIGN_LEFT, "Segundos %i", s);

                draw_tiro(torre, t); //Desenha os tiros

            }

            al_flip_display();
        }
    }
    destroy_al(janela, fonte, fila_eventos, imagem, timer); //Destroi as vari�veis allegro
    al_destroy_sample(musica);

    return 0;
}


void init_system(Sistema &sistema) //Inicializa as variáveis iniciais do sistema

{
    sistema.lives = 10;
    sistema.score = 0;
    sistema.money = 110;
    sistema.boundx = l_celula;
    sistema.boundy = a_celula;
}

int init_fail (ALLEGRO_DISPLAY *janela, ALLEGRO_FONT *fonte, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_BITMAP *imagem, ALLEGRO_TIMER *timer,
               ALLEGRO_BITMAP *trilha, ALLEGRO_BITMAP *fundao, ALLEGRO_BITMAP *spawn, ALLEGRO_BITMAP *the_end, ALLEGRO_BITMAP *monstro2, ALLEGRO_BITMAP *monstro3, ALLEGRO_BITMAP *torre1, ALLEGRO_BITMAP *upgrade, ALLEGRO_BITMAP *sell )
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
    if (!trilha)
    {
        fprintf(stderr, "Falha ao carregar trilha.\n");
        al_destroy_bitmap(imagem);
        al_destroy_display(janela);
        return -1;
    }
    if (!spawn)
    {
        fprintf(stderr, "Falha ao carregar spawn.\n");
        al_destroy_bitmap(imagem);
        al_destroy_display(janela);
        return -1;
    }
    if (!the_end)
    {
        fprintf(stderr, "Falha ao carregar the_end.\n");
        al_destroy_bitmap(imagem);
        al_destroy_display(janela);
        return -1;
    }
    if (!monstro2)
    {
        fprintf(stderr, "Falha ao carregar monstro2.\n");
        al_destroy_bitmap(imagem);
        al_destroy_display(janela);
        return -1;
    }
    if (!monstro3)
    {
        fprintf(stderr, "Falha ao carregar monstro3.\n");
        al_destroy_bitmap(imagem);
        al_destroy_display(janela);
        return -1;
    }
    if (!torre1)
    {
        fprintf(stderr, "Falha ao carregar torre1.\n");
        al_destroy_bitmap(imagem);
        al_destroy_display(janela);
        return -1;
    }
    if (!upgrade)
    {
        fprintf(stderr, "Falha ao carregar upgrade.\n");
        al_destroy_bitmap(imagem);
        al_destroy_display(janela);
        return -1;
    }
    if (!sell)
    {
        fprintf(stderr, "Falha ao carregar sell.\n");
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

void draw_background(int mapa[A][B], ALLEGRO_FONT *fonte, ALLEGRO_BITMAP *trilha, ALLEGRO_BITMAP *fundao, ALLEGRO_BITMAP *spawn, ALLEGRO_BITMAP *the_end, ALLEGRO_BITMAP *planofundo) //Setup e desenho o ultimo plano da matriz
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
                drawbut = false;
                break;
            case 1:
                al_draw_bitmap(trilha,m_x,m_y,ALLEGRO_FLIP_VERTICAL);
                break;
            case 2:
                al_draw_bitmap(trilha,m_x,m_y,ALLEGRO_FLIP_VERTICAL);
                break;
            case 3:
                al_draw_bitmap(trilha,m_x,m_y,ALLEGRO_FLIP_VERTICAL);
                break;
            case 4:
                al_draw_bitmap(trilha,m_x,m_y,ALLEGRO_FLIP_VERTICAL);
                break;
            case 6:
                al_draw_bitmap(spawn,m_x,m_y,0);
                break;
            }
            m_x += l_celula;
        }
        m_x = 0;
        m_y += a_celula;
    }
    al_draw_filled_rectangle(0, 18 * a_celula, LARGURA_TELA, ALTURA_TELA, al_map_rgb(0, 0 , 0));
}

void draw_towers(int mapa[A][B], Sistema &sistema, ALLEGRO_FONT *fonte, ALLEGRO_BITMAP *the_end, ALLEGRO_BITMAP *torre1, ALLEGRO_BITMAP *upgrade, ALLEGRO_BITMAP *sell, ALLEGRO_BITMAP *torre2, ALLEGRO_FONT *fonte20) //Desenha as torres; segundo plano
{
    int i = 0;
    int j = 0;
    int m_x = 0;
    int m_y = 0;
    int butx;
    int buty;
    for (i=0;  i<A; i++)
    {
        for(j=0; j<B; j++)
        {
            switch (mapa[i][j])
            {
            case 10:
                al_draw_bitmap(torre1,m_x,m_y, 0 );
                break;
            case 11:
                al_draw_bitmap(torre1,m_x,m_y, 0 );
                break;
            case 12:
            case 22:
                butx = m_x;
                buty = m_y;
                drawbut = true;
                break;
            case 20:
                al_draw_bitmap(torre2,m_x,m_y, 0 );
                break;
            case 21:
                al_draw_bitmap(torre2,m_x,m_y, 0 );
                break;
            case 90:
                al_draw_bitmap(the_end,m_x,m_y,ALLEGRO_FLIP_HORIZONTAL);
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
    if(drawbut){
    al_draw_bitmap(upgrade, butx - l_celula, buty, ALLEGRO_FLIP_HORIZONTAL);
    al_draw_textf(fonte, al_map_rgb(0, 0, 0), butx - l_celula, buty + a_celula/4, ALLEGRO_ALIGN_LEFT, " Upgrade");
    al_draw_bitmap(sell, butx - l_celula, buty + a_celula, ALLEGRO_FLIP_HORIZONTAL);
    al_draw_textf(fonte, al_map_rgb(0, 0, 0), butx - l_celula, buty + a_celula + a_celula/4, ALLEGRO_ALIGN_LEFT, " Sell");
    }
}

void init_horda(Monstro monstro[tipos_monstros][n_monstros], int n_monstros, int n_hordas, int tipos_monstros) //Setup inicial da horda
{
    int m = 0;
    int t = 0;
    for(t = 0; t < tipos_monstros; t++)
    {
        for(m = 0; m < n_monstros; m++)
        {
            if(t == 0)
            {
                monstro[t][m].stillalive = false;
                monstro[t][m].health = 15;
                monstro[t][m].speed = 2.7;
                monstro[t][m].boundx = 35;
                monstro[t][m].boundy = 35;
            }
            if(t == 1)
            {
                monstro[t][m].stillalive = false;
                monstro[t][m].health = 40;
                monstro[t][m].speed = 1.8;
                monstro[t][m].boundx = 35;
                monstro[t][m].boundy = 35;
            }
            if(t == 2)
            {
                monstro[t][m].stillalive = false;
                monstro[t][m].health = 40;
                monstro[t][m].speed = 1.3;
                monstro[t][m].boundx = 35;
                monstro[t][m].boundy = 35;
            }
        }
    }
}

void draw_horda(Monstro monstro[tipos_monstros][n_monstros], int n_monstros, ALLEGRO_BITMAP *imagem, int tipos_monstros, ALLEGRO_BITMAP *monstro2, ALLEGRO_BITMAP *monstro3)
{
    int m = 0;
    int t = 0;
    for(t = 0; t < tipos_monstros; t++)
    {
        for(m = 0; m < n_monstros; m++)
        {
            if(monstro[t][m].stillalive)
            {
                if(t == 0)
                {
                    al_draw_bitmap(imagem, monstro[t][m].xlocation, monstro[t][m].ylocation, 0);
                }
                else if(t == 1)
                {
                    al_draw_bitmap(monstro2, monstro[t][m].xlocation, monstro[t][m].ylocation, 0);
                }
                else if(t == 2)
                {
                    al_draw_bitmap(monstro3, monstro[t][m].xlocation, monstro[t][m].ylocation, 0 );
                }
            }
        }
    }
}

void start_horda(Monstro monstro[tipos_monstros][n_monstros], int n_monstros, int n_hordas, int tipos_monstros)
{
    // Monstro 1: Vida = 50 ;
    // Monstro 2: Vida = 100 ;
    // Monstro 3: Vida = 200 ;
    int m = 0;
    int t = 0;
    int i = 0;
    int j = 0;
    if(n_hordas == 0)     //diferentes hordas
    {
        for (m = 0; m < 5; m++)
        {
            if(!monstro[t][m].stillalive)
            {
                monstro[t][m].stillalive = true;
                for (i = 0; i < A; i++)
                {
                    for(j = 0; j < B; j++ )
                    {
                        switch (mapa[i][j])
                        {
                        case 6:
                            monstro[t][m].xlocation = - 0 - ((m - 1) * 34);
                            monstro[t][m].ylocation = i * a_celula;
                            monstro[t][m].health = 20 + (n_hordas * 5 ) * 1.6;
                            monstro[t][m].mov_x = 1;
                            monstro[t][m].mov_y = 0;
                            break;
                        }
                    }
                }
            }
        }
    }
    if(n_hordas == 1)
    {
        for (m = 0; m < 5; m++)
        {
            if(!monstro[1][m].stillalive)
            {
                monstro[1][m].stillalive = true;
                for (i = 0; i < A; i++)
                {
                    for(j = 0; j < B; j++ )
                    {
                        switch (mapa[i][j])
                        {
                        case 6:
                            monstro[1][m].xlocation = - 10 - ((m - 1) * 34);
                            monstro[1][m].ylocation = i * a_celula;
                            monstro[1][m].health = 50 + (n_hordas * 5 ) * 1.6;
                            monstro[1][m].mov_x = 1;
                            monstro[1][m].mov_y = 0;
                            break;
                        }
                    }
                }
            }
        }
    }
    if(n_hordas == 2)
    {
        for (m = 0; m < 5; m++)
        {
            if(!monstro[2][m].stillalive)
            {
                monstro[2][m].stillalive = true;
                for (i = 0; i < A; i++)
                {
                    for(j = 0; j < B; j++ )
                    {
                        switch (mapa[i][j])
                        {
                        case 6:
                            monstro[2][m].xlocation = - 10 - ((m - 1) * 34);
                            monstro[2][m].ylocation = i * a_celula;
                            monstro[2][m].health = 100 + (n_hordas * 5 ) * 1.6;
                            monstro[2][m].mov_x = 1;
                            monstro[2][m].mov_y = 0;
                            break;
                        }
                    }
                }
            }
        }
    }
    if(n_hordas == 3)
    {
        for(t = 0; t < 2; t++)
        {
            for(m = 0; m < 5; m++)
            {
                if(!monstro[t][m].stillalive)
                {
                    monstro[t][m].stillalive = true;
                    for(i = 0; i < A; i++)
                    {
                        for(j = 0; j < B; j++)
                        {
                            if(t == 0)
                            {
                                switch (mapa[i][j])
                                {
                                case 6:
                                    monstro[t][m].xlocation = - 10 - ((m - 1) * 34);
                                    monstro[t][m].ylocation = i * a_celula;
                                    monstro[t][m].health = 20 + (n_hordas * 5 ) * 1.6;
                                    monstro[t][m].mov_x = 1;
                                    monstro[t][m].mov_y = 0;
                                    break;
                                }
                            }
                            if(t == 1)
                            {
                                switch (mapa[i][j])
                                {
                                case 6:
                                    monstro[t][m].xlocation = - 200 - ((m - 1) * 34);
                                    monstro[t][m].ylocation = i * a_celula;
                                    monstro[t][m].health = 50 + (n_hordas * 5 ) * 1.6;
                                    monstro[t][m].mov_x = 1;
                                    monstro[t][m].mov_y = 0;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if(n_hordas == 4)
    {
        for(t = 1; t < 3; t++)
        {
            for(m = 0; m < 5; m++)
            {
                if(!monstro[t][m].stillalive)
                {
                    monstro[t][m].stillalive = true;
                    for(i = 0; i < A; i++)
                    {
                        for(j = 0; j < B; j++)
                        {
                            if(t == 1)
                            {
                                switch (mapa[i][j])
                                {
                                case 6:
                                    monstro[t][m].xlocation = - 10 - ((m - 1) * 34);
                                    monstro[t][m].ylocation = i * a_celula;
                                    monstro[t][m].health = 50 + (n_hordas * 5 ) * 1.6;
                                    monstro[t][m].mov_x = 1;
                                    monstro[t][m].mov_y = 0;
                                    break;
                                }
                            }
                            if(t == 2)
                            {
                                switch (mapa[i][j])
                                {
                                case 6:
                                    monstro[t][m].xlocation = - 200 - ((m - 1) * 34);
                                    monstro[t][m].ylocation = i * a_celula;
                                    monstro[t][m].health = 100 + (n_hordas * 5 ) * 1.6;
                                    monstro[t][m].mov_x = 1;
                                    monstro[t][m].mov_y = 0;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if(n_hordas == 5)
    {
        for(t = 0; t < 3; t++)
        {
            for(m = 0; m < 5; m++)
            {
                if(!monstro[t][m].stillalive)
                {
                    monstro[t][m].stillalive = true;
                    for(i = 0; i < A; i++)
                    {
                        for(j = 0; j < B; j++)
                        {
                            if(t == 0)
                            {
                                switch (mapa[i][j])
                                {
                                case 6:
                                    monstro[t][m].xlocation = - 10 - ((m - 1) * 34);
                                    monstro[t][m].ylocation = i * a_celula;
                                    monstro[t][m].health = 20 + (n_hordas * 5) * 1.6;
                                    monstro[t][m].mov_x = 1;
                                    monstro[t][m].mov_y = 0;
                                    break;
                                }
                            }
                            if(t == 1)
                            {
                                switch (mapa[i][j])
                                {
                                case 6:
                                    monstro[t][m].xlocation = - 200 - ((m - 1) * 34);
                                    monstro[t][m].ylocation = i * a_celula;
                                    monstro[t][m].health = 50 + (n_hordas * 5 ) * 1.6;
                                    monstro[t][m].mov_x = 1;
                                    monstro[t][m].mov_y = 0;
                                    break;
                                }
                            }
                            if(t == 2)
                            {
                                switch (mapa[i][j])
                                {
                                case 6:
                                    monstro[t][m].xlocation = - 400 - ((m - 1) * 34);
                                    monstro[t][m].ylocation = i * a_celula;
                                    monstro[t][m].health = 100 + (n_hordas * 5 ) * 1.6;
                                    monstro[t][m].mov_x = 1;
                                    monstro[t][m].mov_y = 0;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if(n_hordas == 6)
    {
        for(t = 0; t < 3; t++)
        {
            for(m = 0; m < 7; m++)
            {
                if(!monstro[t][m].stillalive)
                {
                    monstro[t][m].stillalive = true;
                    for(i = 0; i < A; i++)
                    {
                        for(j = 0; j < B; j++)
                        {
                            if(t == 0)
                            {
                                switch (mapa[i][j])
                                {
                                case 6:
                                    monstro[t][m].xlocation = - 10 - ((m - 1) * 34);
                                    monstro[t][m].ylocation = i * a_celula;
                                    monstro[t][m].health = 20 + (n_hordas * 5) * 1.6;
                                    monstro[t][m].mov_x = 1;
                                    monstro[t][m].mov_y = 0;
                                    break;
                                }
                            }
                            if(t == 1)
                            {
                                switch (mapa[i][j])
                                {
                                case 6:
                                    monstro[t][m].xlocation = - 200 - ((m - 1) * 34);
                                    monstro[t][m].ylocation = i * a_celula;
                                    monstro[t][m].health = 50 + (n_hordas * 5 ) * 1.6;
                                    monstro[t][m].mov_x = 1;
                                    monstro[t][m].mov_y = 0;
                                    break;
                                }
                            }
                            if(t == 2)
                            {
                                switch (mapa[i][j])
                                {
                                case 6:
                                    monstro[t][m].xlocation = - 350 - ((m - 1) * 34);
                                    monstro[t][m].ylocation = i * a_celula;
                                    monstro[t][m].health = 100 + (n_hordas * 5 ) * 1.6;
                                    monstro[t][m].mov_x = 1;
                                    monstro[t][m].mov_y = 0;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if(n_hordas == 7)
    {
        for(t = 0; t < 3; t++)
        {
            for(m = 0; m < 8; m++)
            {
                if(!monstro[t][m].stillalive)
                {
                    monstro[t][m].stillalive = true;
                    for(i = 0; i < A; i++)
                    {
                        for(j = 0; j < B; j++)
                        {
                            if(t == 0)
                            {
                                switch (mapa[i][j])
                                {
                                case 6:
                                    monstro[t][m].xlocation = - 10 - ((m - 1) * 34);
                                    monstro[t][m].ylocation = i * a_celula;
                                    monstro[t][m].health = 20 + (n_hordas * 7) * 1.6;
                                    monstro[t][m].mov_x = 1;
                                    monstro[t][m].mov_y = 0;
                                    break;
                                }
                            }
                            if(t == 1)
                            {
                                switch (mapa[i][j])
                                {
                                case 6:
                                    monstro[t][m].xlocation = - 230 - ((m - 1) * 34);
                                    monstro[t][m].ylocation = i * a_celula;
                                    monstro[t][m].health = 50 + (n_hordas * 7 ) * 1.6;
                                    monstro[t][m].mov_x = 1;
                                    monstro[t][m].mov_y = 0;
                                    break;
                                }
                            }
                            if(t == 2)
                            {
                                switch (mapa[i][j])
                                {
                                case 6:
                                    monstro[t][m].xlocation = - 330 - ((m - 1) * 34);
                                    monstro[t][m].ylocation = i * a_celula;
                                    monstro[t][m].health = 100 + (n_hordas * 7 ) * 1.6;
                                    monstro[t][m].mov_x = 1;
                                    monstro[t][m].mov_y = 0;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if(n_hordas >= 8)
    {
        for(t = 0; t < 3; t++)
        {
            for(m = 0; m < 8; m++)
            {
                if(!monstro[t][m].stillalive)
                {
                    monstro[t][m].stillalive = true;
                    for(i = 0; i < A; i++)
                    {
                        for(j = 0; j < B; j++)
                        {
                            if(t == 0)
                            {
                                switch (mapa[i][j])
                                {
                                case 6:
                                    monstro[t][m].xlocation = - 10 - ((m - 1) * 34);
                                    monstro[t][m].ylocation = i * a_celula;
                                    monstro[t][m].health = 20 + (n_hordas * 11) * 1.6;
                                    monstro[t][m].mov_x = 1;
                                    monstro[t][m].mov_y = 0;
                                    break;
                                }
                            }
                            if(t == 1)
                            {
                                switch (mapa[i][j])
                                {
                                case 6:
                                    monstro[t][m].xlocation = - 230 - ((m - 1) * 34);
                                    monstro[t][m].ylocation = i * a_celula;
                                    monstro[t][m].health = 50 + (n_hordas * 11) * 1.6;
                                    monstro[t][m].mov_x = 1;
                                    monstro[t][m].mov_y = 0;
                                    break;
                                }
                            }
                            if(t == 2)
                            {
                                switch (mapa[i][j])
                                {
                                case 6:
                                    monstro[t][m].xlocation = - 330 - ((m - 1) * 34);
                                    monstro[t][m].ylocation = i * a_celula;
                                    monstro[t][m].health = 100 + (n_hordas * 11) * 1.6;
                                    monstro[t][m].mov_x = 1;
                                    monstro[t][m].mov_y = 0;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void update_horda(Monstro monstro[tipos_monstros][n_monstros], Sistema &sistema, int mapa[A][B], int n_monstros, int tipos_monstros)
{
    int m = 0;
    int t = 0;
    for(t = 0; t< tipos_monstros; t++)
    {
        for(m = 0; m < n_monstros; m++)
        {
            if(monstro[t][m].stillalive)
            {
                int cx = (monstro[t][m].xlocation + 16.5)/l_celula;    //x = meio do monstro
                int dx = (monstro[t][m].xlocation + 33)/l_celula;      //x = direita do monstro
                int ex = monstro[t][m].xlocation/l_celula;             //x = esquerda do monstro
                int cy = (monstro[t][m].ylocation + 14.5)/a_celula;    //y = centro do monstro
                int ay = monstro[t][m].ylocation/a_celula;             //y = acima do monstro
                int by = (monstro[t][m].ylocation + 29)/a_celula;      //y = abaixo do monstro

                if (monstro[t][m].mov_x == 1)                          //se o monstro estiver se locomovendo para a direita
                {
                    switch (mapa[cy][ex])
                    {
                    case 0:                                     //monstro continua com a movimentação anterior
                        monstro[t][m].mov_x = monstro[t][m].mov_x;
                        monstro[t][m].mov_y = monstro[t][m].mov_y;
                        break;
                    case 1:                                     //monstro vai para baixo
                        monstro[t][m].mov_x = 0;
                        monstro[t][m].mov_y = 1;
                        break;
                    case 2:                                     //monstro vai para cima
                        monstro[t][m].mov_x = 0;
                        monstro[t][m].mov_y = -1;
                        break;
                    case 3:                                     //monstro vai para esquerda
                        monstro[t][m].mov_x = -1;
                        monstro[t][m].mov_y = 0;
                        break;
                    case 4:                                     //monstro vai para direita
                        monstro[t][m].mov_x = 1;
                        monstro[t][m].mov_y = 0;
                        break;
                    }
                }
                if (monstro[t][m].mov_x == -1)                         //se o monstro estiver se locomovendo para a esquerda
                {
                    switch (mapa[cy][dx])
                    {
                    case 0:                                     //monstro continua com a movimentação anterior
                        monstro [t][m].mov_x = monstro[t][m].mov_x;
                        monstro[t][m].mov_y = monstro[t][m].mov_y;
                        break;
                    case 1:                                     //monstro vai para baixo
                        monstro[t][m].mov_x = 0;
                        monstro[t][m].mov_y = 1;
                        break;
                    case 2:                                     //monstro vai para cima
                        monstro[t][m].mov_x = 0;
                        monstro[t][m].mov_y = -1;
                        break;
                    case 3:                                     //monstro vai para esquerda
                        monstro[t][m].mov_x = -1;
                        monstro[t][m].mov_y = 0;
                        break;
                    case 4:                                     //monstro vai para direita
                        monstro[t][m].mov_x = 1;
                        monstro[t][m].mov_y = 0;
                        break;
                    }
                }
                if (monstro[t][m].mov_y == 1)                          //se o monstro estiver se locomovendo para a baixo
                {
                    switch (mapa[ay][cx])
                    {
                    case 0:                                     //monstro continua com a movimentação anterior
                        monstro[t][m].mov_x = monstro[t][m].mov_x;
                        monstro[t][m].mov_y = monstro[t][m].mov_y;
                        break;
                    case 1:                                     //monstro vai para baixo
                        monstro[t][m].mov_x = 0;
                        monstro[t][m].mov_y = 1;
                        break;
                    case 2:                                     //monstro vai para cima
                        monstro[t][m].mov_x = 0;
                        monstro[t][m].mov_y = -1;
                        break;
                    case 3:                                     //monstro vai para esquerda
                        monstro[t][m].mov_x = -1;
                        monstro[t][m].mov_y = 0;
                        break;
                    case 4:                                     //monstro vai para direita
                        monstro[t][m].mov_x = 1;
                        monstro[t][m].mov_y = 0;
                        break;
                    }
                }
                if (monstro[t][m].mov_y == -1)     //se o monstro estiver se locomovendo para a cima
                {
                    switch (mapa[by][cx])
                    {
                    case 0:                                     //monstro continua com a movimentação anterior
                        monstro[t][m].mov_x = monstro[t][m].mov_x;
                        monstro[t][m].mov_y = monstro[t][m].mov_y;
                        break;
                    case 1:                                     //monstro vai para baixo
                        monstro[t][m].mov_x = 0;
                        monstro[t][m].mov_y = 1;
                        break;
                    case 2:                                     //monstro vai para cima
                        monstro[t][m].mov_x = 0;
                        monstro[t][m].mov_y = -1;
                        break;
                    case 3:                                     //monstro vai para esquerda
                        monstro[t][m].mov_x = -1;
                        monstro[t][m].mov_y = 0;
                        break;
                    case 4:                                     //monstro vai para direita
                        monstro[t][m].mov_x = 1;
                        monstro[t][m].mov_y = 0;
                        break;
                    }
                }
                if(monstro[t][m].health <= 0)
                {
                    monstro[t][m].stillalive = false;
                    sistema.score++;
                    sistema.money += 3.5;
                }
                if(monstro[t][m].xlocation > sistema.x)
                {
                    sistema.lives--;
                    monstro[t][m].stillalive = false;
                }
                monstro[t][m].xlocation += (monstro[t][m].mov_x * monstro[t][m].speed);
                monstro[t][m].ylocation += (monstro[t][m].mov_y * monstro[t][m].speed);
            }
        }
    }
}

void draw_mouse_tower(int r, int l, int tmouse, int t, Torre torre[], Tipo tipo1[], Tipo tipo2[], ALLEGRO_BITMAP *torre1, ALLEGRO_BITMAP *torre2)  //Desenha a torre enquanto ela estiver no mouse, nao atribui parametros
{
    if(tmouse == 1){
        al_draw_tinted_bitmap(torre1, al_map_rgba(150, 150, 150, 10), r*l_celula, l*a_celula, 0);
        al_draw_circle(r*l_celula + (l_celula/2), l*a_celula + (a_celula/2), tipo1[0].range, al_map_rgb(255, 110, 10), 2);
    }
    else if(tmouse == 2){
        al_draw_tinted_bitmap(torre2, al_map_rgba(150, 150, 150, 0), r*l_celula, l*a_celula, 0);
        al_draw_circle(r*l_celula + (l_celula/2), l*a_celula + (a_celula/2), tipo2[0].range, al_map_rgb(255, 110, 10), 2);
    }

}

void show_tower_information(Torre torre[], int t, ALLEGRO_FONT *fonte) //Monstra as informaçoes da torre selecionada
{
    if(torre[t].live){
    al_draw_circle(torre[t].x, torre[t].y, torre[t].range, al_map_rgb(110, 11, 11), 3);
    al_draw_textf(fonte, al_map_rgb(255, 255, 255), 5 * l_celula, 19 * a_celula, ALLEGRO_ALIGN_LEFT, "Tower type: %i", torre[t].n);
    al_draw_textf(fonte, al_map_rgb(255, 255, 255), 5 * l_celula, 20 * a_celula, ALLEGRO_ALIGN_LEFT, "Tower fire power: %i", torre[t].fire_power);
    al_draw_textf(fonte, al_map_rgb(255, 255, 255), 5 * l_celula, 21 * a_celula, ALLEGRO_ALIGN_LEFT, "Tower range: %i", torre[t].range);
    al_draw_textf(fonte, al_map_rgb(255, 255, 255), 5 * l_celula, 22 * a_celula, ALLEGRO_ALIGN_LEFT, "Tower fire rate: %.2fs", torre[t].fire_rate);
    al_draw_textf(fonte, al_map_rgb(255, 255, 255), 5 * l_celula, 23 * a_celula, ALLEGRO_ALIGN_LEFT, "Upgrade: %i", torre[t].upgrade);
    al_draw_textf(fonte, al_map_rgb(255, 255, 255), 5 * l_celula, 24 * a_celula, ALLEGRO_ALIGN_LEFT, "ID: %i", torre[t].ID);
    }
}

void setup_torre1(Tipo tipo1[])  //Setup da torre do tipo 1
{
    tipo1[0].n = 1;
    tipo1[0].mapa = 11;
    tipo1[0].tiro.speed = 6;
    for (int i = 0; i < num_upgrades; i++){
        tipo1[i].price = 30 + (i * i * 20);
        tipo1[i].range = 65 + (9 * i);
        tipo1[i].fire_rate = 0.6 - (i*0.3)/3;
        tipo1[i].fire_power = 25 + (15 * i);
    }
}

void setup_torre2(Tipo tipo2[])  //Setup da torre do tipo 2
{
    tipo2[0].n = 2;
    tipo2[0].mapa = 21;
    tipo2[0].tiro.speed = 5;
    for (int i = 0; i < num_upgrades; i++){
        tipo2[i].price = 55 + (i * i * 30);
        tipo2[i].range = 75 + (15 * i);
        tipo2[i].fire_rate = 1 - (i*0.2)/2 ;
        tipo2[i].fire_power = 70 + (25 * i);
    }
}

void setup_tower(Torre torre[], Tipo tipo[], int t, int r, int l)  //Setup da torre quando ela é comprada
{
    torre[t].n = tipo[0].n;
    torre[t].ID = t;
    torre[t].upgrade = 0;
    torre[t].price = tipo[torre[t].upgrade].price;
    torre[t].range = tipo[torre[t].upgrade].range;
    torre[t].fire_rate = tipo[torre[t].upgrade].fire_rate;
    torre[t].fire_power = tipo[torre[t].upgrade].fire_power;
    torre[t].tiro.speed = tipo[torre[t].upgrade].tiro.speed;
    torre[t].upgrade_price = tipo[torre[t].upgrade].price;
    torre[t].tiro.live = false;
    torre[t].time_to_shot = 0;
    torre[t].r = r;
    torre[t].l = l;
    torre[t].x = r * l_celula + l_celula/2;
    torre[t].y = l * a_celula + a_celula/2;
    torre[t].live = true;
    mapa[l][r] = tipo[torre[t].upgrade].mapa;
}

void buy_tower(Tipo tipo[], ALLEGRO_FONT *fonte20) //Info compra
{
    al_draw_textf(fonte20, al_map_rgb(255, 255, 255), l_celula, 14 * a_celula, ALLEGRO_ALIGN_LEFT, "Tower price: %i bitcoins", tipo[0].price);
    al_draw_textf(fonte20, al_map_rgb(255, 255, 255), l_celula, 15 * a_celula, ALLEGRO_ALIGN_LEFT, "Tower fire power: %i", tipo[0].fire_power);
    al_draw_textf(fonte20, al_map_rgb(255, 255, 255), l_celula, 16 * a_celula, ALLEGRO_ALIGN_LEFT, "Tower range: %i", tipo[0].range);
    al_draw_textf(fonte20, al_map_rgb(255, 255, 255), l_celula, 17 * a_celula, ALLEGRO_ALIGN_LEFT, "Tower fire rate: %.2fs", tipo[0].fire_rate);
}

void tower1_upgrades(Torre torre[], int t, Tipo tipo1[])  //Em desenvolvimento
{
    if(torre[t].live){
    torre[t].upgrade++;
    torre[t].upgrade_price = tipo1[torre[t].upgrade].price;
    torre[t].range = tipo1[torre[t].upgrade].range;
    torre[t].fire_rate = tipo1[torre[t].upgrade].fire_rate;
    torre[t].fire_power = tipo1[torre[t].upgrade].fire_power;
    }
}

void tower2_upgrades(Torre torre[], int t, Tipo tipo2[])  //Em desenvolvimento
{
    if(torre[t].live){
    torre[t].upgrade++;
    torre[t].upgrade_price = tipo2[torre[t].upgrade].price;
    torre[t].range = tipo2[torre[t].upgrade].range;
    torre[t].fire_rate = tipo2[torre[t].upgrade].fire_rate;
    torre[t].fire_power = tipo2[torre[t].upgrade].fire_power;
    }
}

void show_upgrade_information(Torre torre[], Tipo tipo[], int t, ALLEGRO_FONT *fonte){
    if(torre[t].live){
        int ups = torre[t].upgrade;
        al_draw_textf(fonte, al_map_rgb(255, 255, 255), 9 * l_celula, 23 * a_celula, ALLEGRO_ALIGN_LEFT, "Price: %i", tipo[ups].price);
        ups++;
        al_draw_circle(torre[t].x, torre[t].y, tipo[ups].range, al_map_rgb(100, 10, 10), 2);
        al_draw_textf(fonte, al_map_rgb(255, 255, 255), 9 * l_celula, 20 * a_celula, ALLEGRO_ALIGN_LEFT, "Upgrade: %i", tipo[ups].fire_power);
        al_draw_textf(fonte, al_map_rgb(255, 255, 255), 9 * l_celula, 21 * a_celula, ALLEGRO_ALIGN_LEFT, "Upgrade: %i", tipo[ups].range);
        al_draw_textf(fonte, al_map_rgb(255, 255, 255), 9 * l_celula, 22 * a_celula, ALLEGRO_ALIGN_LEFT, "Upgrade: %fs", tipo[ups].fire_rate);
    }
}

int find_tower_ID(Torre torre[], int t, int r, int l)  //Encontra o ID da torre selecionada
{
    for(int i = 0; i < t; i++)
    {
        if(torre[i].x == r * l_celula + l_celula/2 && torre[i].y == l * a_celula + a_celula/2)
            return torre[i].ID;
    }
}

void sell_tower(Sistema &sistema, Torre torre[], int ID){
    if(torre[ID].live){
    sistema.money += 20;
    torre[ID].live = false;
    mapa[torre[ID].l][torre[ID].r] = 1;
    mapa[19][27] = 0;
    mapa[19][28] = 0;
    mapa[20][27] = 0;
    mapa[20][28] = 0;
    }
}

void fire_tiro(Torre torre[], Monstro monstro[tipos_monstros][n_monstros], int t, int n_monstros, int tipos_monstros)  //Verifica os montros e atira casa algum esteja vivo
{
    int m = 0;
    int j = 0;
    for(j = 0; j < tipos_monstros; j++)
    {
        for(m = 0; m < n_monstros; m++)
        {
            if(monstro[j][m].stillalive)
            {
                for(int i = 0; i < t+1; i++)
                {
                    if(torre[i].live && !torre[i].tiro.live && torre[i].time_to_shot <= 0 &&
                            torre[i].x - monstro[j][m].xlocation <= torre[i].range &&
                            torre[i].x - monstro[j][m].xlocation >= - torre[i].range &&
                            torre[i].y - monstro[j][m].ylocation <= torre[i].range &&
                            torre[i].y - monstro[j][m].ylocation >= - torre[i].range)
                    {
                        torre[i].tiro.xlocation = torre[i].x;
                        torre[i].tiro.ylocation = torre[i].y;
                        torre[i].tiro.fire_power = torre[i].fire_power;
                        torre[i].tiro.monstro = monstro[j][m];
                        torre[i].tiro.live = true;
                        torre[i].time_to_shot = fps * torre[i].fire_rate;
                    }
                }
            }
        }
    }
}

void update_tiro(Torre torre[], Monstro monstro[tipos_monstros][n_monstros], int t, int n_monstros, int tipos_monstros)   //Atualiza a posiçao do tiro
{
    for(int i = 0; i < t; i++)
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
        torre[i].time_to_shot--;
    }
}

void colisao_horda(Torre torre[], Monstro monstro[tipos_monstros][n_monstros], int t, int n_monstros, Sistema &sistema, int *resposta, int tipos_monstros)  //Detecta se um tiro atingiu algum monstro
{
    int vivos = 0;
    int j = 0;
    for(j = 0; j < tipos_monstros; j++)
    {
        for(int m = 0; m < n_monstros; m++)
        {
            if(monstro[j][m].stillalive)
            {
                vivos++;
                for(int i = 0; i < t+1; i++)
                {
                    if(torre[i].tiro.live)  //Verificacao da localizaçao
                    {
                        if( torre[i].tiro.xlocation > (torre[i].tiro.monstro.xlocation - torre[i].tiro.monstro.boundx) &&
                                torre[i].tiro.xlocation < (torre[i].tiro.monstro.xlocation + torre[i].tiro.monstro.boundx) &&
                                torre[i].tiro.ylocation > (torre[i].tiro.monstro.ylocation - torre[i].tiro.monstro.boundy) &&
                                torre[i].tiro.ylocation < (torre[i].tiro.monstro.ylocation + torre[i].tiro.monstro.boundy))
                        {
                            monstro[j][m].health -= torre[i].tiro.fire_power;
                            torre[i].tiro.live = false;
                        }
                    }
                }
            }
        }
    }
    if(vivos == 0)
    {
        *resposta = 1;
    }
    else
    {
        *resposta = 0;
    }
}

void draw_tiro(Torre torre[], int t)  //Desenha o tiro durante a trajetória
{
    for(int i = 0; i < t; i++)
    {
        if(torre[i].tiro.live)
            al_draw_filled_circle(torre[i].tiro.xlocation, torre[i].tiro.ylocation, 5, al_map_rgb(255, 255, 255));
    }

}

void setup_array(int mapa[A][B])
{
    int a = 0;
    int b = 0;
    for(a = 0; a < A; a++)
    {
        for(b = 0; b < B; b++)
        {
            if(mapa[a][b] == 11)
            {
                mapa[a][b] = 0;
            }
            else if(mapa[a][b] == 21)
            {
                mapa[a][b] = 0;
            }
        }
    }
}

void restart_tower(Torre torre[], int t)
{
    int i = 0;
    for(i = 0; i < t; i++)
    {
        torre[i].live = false;
    }
}
