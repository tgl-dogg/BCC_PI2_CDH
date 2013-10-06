#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "fases/fases.h"
#include "fases/descricao/descricao.h"

// Atributos da tela
const int LARGURA_TELA = 800;
const int ALTURA_TELA = 600;
    
int main(){
    // Variável representando a janela principal
    ALLEGRO_DISPLAY *janela = NULL;
    // Variável representando a posição de tela
    ALLEGRO_DISPLAY_MODE disp_data;
    // Variável representando as imagens (menu)
    ALLEGRO_BITMAP *start = NULL, *leave = NULL;
    // Variável representando as interações de eventos
    ALLEGRO_EVENT_QUEUE *interacao = NULL;
    // Variável representando eventos
    ALLEGRO_EVENT evento;
    // Variável representando cor da fonte
    ALLEGRO_COLOR font_color;
    // Variável representando as fontes utilizadas
    ALLEGRO_FONT *font;

    // Inicializa a Allegro
    al_init();
    // Inicializa o add-on para utilização de imagens
    al_init_image_addon();
    // inicializa a font_addon
    al_init_font_addon();
    // inicializa a ttf_(True Type Font)_addon
    al_init_ttf_addon(); 
    // Atribui em disp_data as configurações de tela
    al_get_display_mode(0, &disp_data);

    // Configura o display
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela){
        fprintf(stderr, "Falha ao criar display!\n");
        return -1;
    }
    
    // Atribui onde o display será posicionado
    al_set_window_position(janela, (disp_data.width-LARGURA_TELA)/2, (disp_data.height-ALTURA_TELA)/2);
    // Configura o título do display
    al_set_window_title(janela, "CÓDIGO DE HONRA!");

    // Torna apto o uso de mouse na aplicação
    if (!al_install_mouse()){
        fprintf(stderr, "Falha ao inicializar o mouse.\n");
        al_destroy_display(janela);
        return -1;
    }

    // Atribui o cursor padrão do sistema para ser usado
    if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT)){
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        al_destroy_display(janela);
        return -1;
    }

    // carrega a font a ser usada
    font_color = al_map_rgb(0, 0, 0);
    font = al_load_ttf_font("res/font/architectsdaughter.ttf", 60, 0);
    if (!font){
        printf("Could not load architectsdaughter.ttf");
        return 0;
    }  
    
    // vetor de caracteres com o nome do jogo
    char txt[] = "CÓDIGO DE HONRA!";

    // Carrega as imagens
    start = al_load_bitmap("res/img/menu/start.png");
    leave = al_load_bitmap("res/img/menu/exit.png");

    // Desenha as imagens na tela
    al_clear_to_color(al_map_rgb(255, 255, 255)); // desenha fundo branco
    al_draw_text(font, font_color, 800/2, (600/4), ALLEGRO_ALIGN_CENTRE, txt); // desenha texto na tela
    al_draw_bitmap(start, 350, 250, 0); // x, y, z(inverte imagem)
    al_draw_bitmap(leave, 350, 400, 0); // x, y, z(inverte imagem)

    // Atualiza a tela
    al_flip_display();

    // Criando interações do mouse
    interacao = al_create_event_queue();
    if (!interacao){
        fprintf(stderr, "Falha ao inicializar a interacao.\n");
        al_destroy_display(janela);
        return -1;
    }

    // Inicialização das interações do mouse
    al_register_event_source(interacao, al_get_mouse_event_source());
    al_register_event_source(interacao, al_get_display_event_source(janela));
    
    while (1){
        // Passando parâmetros da função
        al_wait_for_event(interacao, &evento);
        
        // Se houver clique no [X] ele registra o evento e para a execução do jogo
        if (interacao && evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            break;
        }

        // Se for um evento do tipo clique, vê a posição do clique.
        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
            // Clique no botão leave.
            if (evento.mouse.x >= 350 && evento.mouse.x <= al_get_bitmap_width(leave) + 350 &&
                evento.mouse.y >= 400 && evento.mouse.y <= al_get_bitmap_height(leave) + 400) {
                printf("\nParando a execução...");
                break;
            } 
            // Clique no botão start.
            else if (evento.mouse.x >= 350  && evento.mouse.x <= al_get_bitmap_width(start) + 350 &&
                     evento.mouse.y >= 250  && evento.mouse.y <= al_get_bitmap_height(start) + 250) {
                printf("\nCarregando a primeira fase...");

                // Desregistra os eventos de mouse (novos eventos serão criados)
                al_unregister_event_source(interacao, al_get_mouse_event_source());

                // Carrega primeira fase
                fase1_init(janela, interacao);
                draw_text_fase1(janela);

            } else {
                printf("\nEvento não suportado.");
                continue;
            }
        }
        else if (evento.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
            printf("\nMouse saiu do display.");
        }
        else if (evento.type == ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY) {
            printf("\nMouse entrou no display.");
        }
        // Atualiza a tela
        al_flip_display();            
    }
    // Segura a execução
    al_rest(0.5);
    printf("\nFim da execução!\n");
 
    // Finaliza a display
    al_destroy_display(janela);
 
    return 0;
}

