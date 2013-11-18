#include <stdio.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "desafios.h"
#include "../../geral/cards.h"
#include "..//descricao/descricao.h"

typedef struct {
    int v[15];
    int f;
} card_pile;

void card_pile_init(card_pile *cp);
int card_pull(card_pile *cp, int n);
int card_pop(card_pile *cp);

void click_prog_buttons(clk_flag flags, challenger_rule cr, card_pile *cp);
void click_condition_buttons(clk_flag flags, challenger_rule cr, card_pile *cp);
void click_action_buttons(clk_flag flags, challenger_rule cr, card_pile *cp);

void execute_event(int ev_type, clk_flag flags, challenger_rule cr, card_pile *cp);

// Variável externa
extern ALLEGRO_DISPLAY *janela;

// Arrays globais da área de seleção do usuário
static int img_size = 6;
static ALLEGRO_BITMAP *im_prog_set[6];
static ALLEGRO_BITMAP *im_cond_set[6];
static ALLEGRO_BITMAP *im_act_set[6];

/* Cria o desafio de acordo com a regra de desafio (challenger_rule). */
int create_desafio(challenger_rule cr){
    // Guarda o resultado da nossa operação.
    int resultado = 0, i;
    // Cria flag para controle dos eventos de clique.
    clk_flag ev_flag = {-1, -1, -1, -1};   
    // Cria pilha de cartas.
    card_pile *card_set;

    // Nova event queue para registrar os eventos desta janela.
    ALLEGRO_EVENT_QUEUE *ev_queue = NULL;

   	// Evento para os novos cliques.
	ALLEGRO_EVENT ev_click;

	// Background.
	ALLEGRO_BITMAP *bg = NULL;

	// Botões.
	ALLEGRO_BITMAP *im_help = NULL;
	ALLEGRO_BITMAP *im_memory = NULL;
	ALLEGRO_BITMAP *im_compile = NULL;  
   	
   	bg = al_load_bitmap("res/img/bg/bg_test.png");
    al_draw_bitmap(bg, 0, 0, 0);

 	im_help = al_load_bitmap("res/img/gen/help.png");
	im_memory = al_load_bitmap("res/img/gen/memory.png");
	im_compile = al_load_bitmap("res/img/prog/compile_symbol.png");
	
    im_prog_set[0] = al_load_bitmap("res/img/prog/direct_symbol.png");
    im_prog_set[1] = al_load_bitmap("res/img/prog/if_symbol.png");    
    im_prog_set[2] = al_load_bitmap("res/img/prog/while_symbol.png");
    im_prog_set[3] = al_load_bitmap("res/img/prog/array_symbol.png");
    im_prog_set[4] = im_prog_set[5] = NULL;

    im_cond_set[0] = al_load_bitmap("res/img/prog_condition/if_condition.png");    
    im_cond_set[1] = al_load_bitmap("res/img/prog_condition/if_elseif.png");
    im_cond_set[2] = al_load_bitmap("res/img/prog_condition/if_else.png");
    im_cond_set[3] = al_load_bitmap("res/img/prog_condition/while_break.png");
    im_cond_set[4] = im_cond_set[5] = NULL;
	
    im_act_set[0] = al_load_bitmap("res/img/act/atk.png");    
    im_act_set[1] = al_load_bitmap("res/img/act/atk_double.png");
    im_act_set[2] = al_load_bitmap("res/img/act/def.png");
    im_act_set[3] = al_load_bitmap("res/img/act/def_magic.png");
    im_act_set[4] = al_load_bitmap("res/img/act/def_projectile.png");
    im_act_set[5] = NULL;

	// Desenha as cartas na tela.
    draw_help_card(im_help);
    draw_memory_card(im_memory);
    draw_compile_card(im_compile);

    draw_prog_cards(im_prog_set, cr);    
    draw_conditional_cards(im_cond_set, cr);
    draw_action_cards(im_act_set, cr);

    al_flip_display();

    // Inicializa a struct de set de card.
    card_pile_init(card_set);

    // Cria uma event queue diferente da tela principal.
	ev_queue = al_create_event_queue();
	// Registra a interação com o mouse novamente.
	al_register_event_source(ev_queue, al_get_mouse_event_source());
    

    while(1){    
    	int ev_type = -1;	
    	al_wait_for_event(ev_queue, &ev_click);

        if (ev_click.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
        	// detecta o tipo de evento e seta as flags.
        	ev_type = detect_click_pos(ev_click, &ev_flag, cr);

        	//executa o evento especificado.
        	execute_event(ev_type, ev_flag, cr, card_set);
            al_flip_display();  
            printf("\n\nEvento executado!\n\n");
        	
        	// Se o usuário clicou em compile ou reset, por enquanto finalizamos os eventos.
        	if(ev_type == 6 || ev_type == 7){
        		al_unregister_event_source(ev_queue, al_get_mouse_event_source());
                printf("O erro esta aqui no compile ou reset!\n");
        		break;
        	}
        }
    }

    // Destrói esta instância de eventos para não interferir nos outros eventos do jogo.
    al_destroy_event_queue(ev_queue);
    return resultado;
}

// Clique nas cartas de programação.
void click_prog_buttons(clk_flag flags, challenger_rule cr, card_pile *cp){
    // Verifica se o status está ok (== 1).
	if(flags.ev_status == 1){
        // verifica o "nível de carta" em programação.
        // para saber se o usuário pode utilizar a posição em que clicou.
        if(flags.card_pos < cr.prog){
            // Adiciona a carta à pilha de cartas do usuário.
            card_pull(cp, flags.card_num);            
    
            // Desenha ela na área de cartas selecionadas.
            draw_selected_cards(im_prog_set[flags.card_pos], flags.card_num);  
        }
	}
}

// Clique nas cartas de condição.
void click_condition_buttons(clk_flag flags, challenger_rule cr, card_pile *cp){
	if(flags.ev_status == 1){
        // verifica o "nível de carta" em programação.
        // para saber se o usuário pode utilizar a posição em que clicou.
        if(flags.card_pos < cr.cond){
            // Adiciona a carta à pilha de cartas do usuário.
            card_pull(cp, flags.card_num);
            // Desenha ela na área de cartas selecionadas.
            draw_selected_cards(im_cond_set[flags.card_pos], flags.card_num);
        }
	}
}

// Clique nas cartas de ação.
void click_action_buttons(clk_flag flags, challenger_rule cr, card_pile *cp){
	if(flags.ev_status == 1){      
        // verifica o "nível de carta" em programação.
        // para saber se o usuário pode utilizar a posição em que clicou.
        if(flags.card_pos < cr.act){
            // Adiciona a carta à pilha de cartas do usuário.
            card_pull(cp, flags.card_num);
            // Desenha ela na área de cartas selecionadas.
            draw_selected_cards(im_act_set[flags.card_pos], flags.card_num);  
        }
	}
}

// depois de detectar o evento, executa o mesmo de acordo com o tipo e as flags.
void execute_event(int ev_type, clk_flag flags, challenger_rule cr, card_pile *cp){
	switch(ev_type){
        // Cartas de programação.
		case 1:
			click_prog_buttons(flags, cr, cp);
			break;

        // Cartas de condição.
		case 2:
			click_condition_buttons(flags, cr, cp);
			break;

        // Cartas de ação.
		case 3:
			click_action_buttons(flags, cr, cp);
			break;

        // Carta de memória.
		case 4:
            // mostrar instruções.
			break;	

        // Carta de ajuda
		case 5:
            // mostrar ajuda.
			break;

        // Carta de compilar.
		case 6:
            if((validate_selection(cr.v, (*cp).v)) == -1){
                //GGWP return -1;
            } else {
                //deubosta return indice da carta errada == 0;
            }
            // validar 
			break;

        // Carta de reset.
		case 7:
            // criar
			break;
	}
}

/* Inicializa a pilha de cartas. */
void card_pile_init(card_pile *cp){
    int i;
    (*cp).f = 0;

    for(i = 0; i < 15; i++){
        (*cp).v[i] = -1;
    }
}

/* Adiciona novo valor à pilha de cartas.
Retorna 0 em erro ou 1 em sucesso. */
int card_pull(card_pile *cp, int n){
    // se estourar o vetor retorna erro.
    if((*cp).f >= 15){
        return 0;
    }

    // adiciona n ao vetor na posição final e incrementa o final.
    (*cp).v[(*cp).f++] = n;
    // retorna sucesso
    return 1;
}

/* Remove o valor no topo da pilha de cartas.
Retorna -1 em erro ou o valor em sucesso. 
*/
int card_pop(card_pile *cp){
    int i;
    // verifica se a pilha não está vazia.
    if((*cp).f == 0){
        // retorna -1 porque é garantido que os valores do vetor v serão naturais.
        return -1;
    }

    // armazena o valor no topo da pilha.
    i = (*cp).v[(*cp).f];
    // seta a posição que era topo para -1 e decrementa o topo.
    (*cp).v[(*cp).f--] = -1;
    // retorna o valor que era o topo da pilha.
    return i;
}