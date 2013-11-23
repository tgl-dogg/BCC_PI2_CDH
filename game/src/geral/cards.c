#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "cards.h"

// Posição e tamanho da área "programação"
#define PROG_X 47
#define PROG_Y 390
#define PROG_W 134
#define PROG_H 204

// Posição e tamanho da área "condição"
#define COND_X 228
#define COND_Y 390
#define COND_W 134
#define COND_H 204

// Posição e tamanho da área "ações"
#define ACT_X 395
#define ACT_Y 390
#define ACT_W 134
#define ACT_H 204

// Posição e tamanho do botão memória
#define MEM_X 608
#define MEM_Y 372
#define MEM_W 64
#define MEM_H 64

// Posição e tamanho do botão help
#define HELP_X 678
#define HELP_Y 372
#define HELP_W 64
#define HELP_H 64

// Posição e tamanho do botão compilar
#define COMP_X 610
#define COMP_Y 442
#define COMP_W 128
#define COMP_H 128

// Posição e tamanho do botão reset
#define UND_X 15
#define UND_Y 230
#define UND_W 64
#define UND_H 64

// Posição e tamanho para área de "cartas selecionadas"
#define SLCT_X 570
#define SLCT_Y 10
#define SLCT_W 210
#define SLCT_H 350

// Espaçamento entre os botões
#define BTN_MARGIN 70

void draw_cards(ALLEGRO_BITMAP *img[], int n, int x, int y){
	int i = 0;
	int start_x = x;
	int start_y = y;
	int lim_x = x+128;
	int lim_y = y+172;

	if(n >= 15){
		return;
	}

	for(i; i < n; i++){	
		if(img[i] != NULL){
			if(x > lim_x){
				x = start_x;
				y += BTN_MARGIN;
			}

			al_draw_bitmap(img[i], x, y, 0);
			x += BTN_MARGIN;
		}
	}
}


void draw_undo_card(ALLEGRO_BITMAP *imagem){
	ALLEGRO_BITMAP *img[] = { imagem };
	draw_cards(img, 1, UND_X, UND_Y);
}

void draw_compile_card(ALLEGRO_BITMAP *imagem){
	ALLEGRO_BITMAP *img[] = { imagem };
	draw_cards(img, 1, COMP_X, COMP_Y);
}

void draw_memory_card(ALLEGRO_BITMAP *imagem){
	ALLEGRO_BITMAP *img[] = { imagem };
	draw_cards(img, 1, MEM_X, MEM_Y);
}

void draw_help_card(ALLEGRO_BITMAP *imagem){
	ALLEGRO_BITMAP *img[] = { imagem };
	draw_cards(img, 1, HELP_X, HELP_Y);
}

void draw_prog_cards(ALLEGRO_BITMAP *img[], challenger_rule cr){	
	draw_cards(img, cr.prog, PROG_X, PROG_Y);
}

void draw_conditional_cards(ALLEGRO_BITMAP *img[], challenger_rule cr){	
	draw_cards(img, cr.cond, COND_X, COND_Y);
}

void draw_action_cards(ALLEGRO_BITMAP *img[], challenger_rule cr){	
	draw_cards(img, cr.act, ACT_X, ACT_Y);
}

void draw_selected_cards(ALLEGRO_BITMAP *imagem, int i){
	if(i >= 15){
		return;
	}

	ALLEGRO_BITMAP *img[] = { imagem };
	int x = SLCT_X, y = SLCT_Y;

	while((i/3) >= 1){
		i -= 3;
		y += BTN_MARGIN;
	}

	x += (BTN_MARGIN * i);
	draw_cards(img, 1, x, y);
}

// Verifica se o clique do evento foi dentro das posições especificadas
int check_bounds(ALLEGRO_EVENT ev, int left, int top, int right, int bottom){
	int x = ev.mouse.x;
	int y = ev.mouse.y;
	
	if(x >= left && x <= right && y >= top && y <= bottom){
		return 1;
	} else {
		return 0;
	}
}

// Verifica se o clique foi nas cartas (prog, cond, action) e valida o evento
int check_cards_bounds(ALLEGRO_EVENT ev, int left, int top, int right, int bottom, clk_flag *flags, int level){
	int i = 0;
	int aux = top;
	int card = 0;

	for(i; i < 3; i++){
		top = (i * BTN_MARGIN) + aux;

		// Clique na primeira coluna
		if(check_bounds(ev, left, top, left+64, top+64) == 1){
			card = 2*i;
			return validate_card_click(flags, level, card);
		}
		// Clique na segunda coluna
		else if (check_bounds(ev, right-64, top, right, top+64) == 1){
			card = (2*i) + 1;
			return validate_card_click(flags, level, card);
		}
	}

	(*flags).ev_status = -1;
	return 0;
}

int validate_card_click(clk_flag *flags, int level, int card){
	// A carta tem que estar no nível disponível para o usuário
	if (card >= level){
		(*flags).ev_status = -1;
		(*flags).card_pos = -1;
		return 0;
	} else {		
		// Valida o evento
		(*flags).ev_status = 1;
		// Posição da carta na matriz de alocação de imagens
		(*flags).card_pos = card;
		// Número onde deve ser desenhada a carta na área de cartas selecionadas
		(*flags).card_num = (*flags).card_num + 1;
		return 1;
	}
}

/*
 Detecta a posição do clique do usuário, retorna 0 caso não seja válido,
 retorna 1 para prog, 2 para condition, 3 para action
 4 para memory, 5 para help, 6 para compile, 7 para reset.
*/
int detect_click_pos(ALLEGRO_EVENT ev, clk_flag *flags, challenger_rule cr){
	if(check_cards_bounds(ev, PROG_X, PROG_Y, PROG_X+PROG_W, PROG_Y+PROG_H, flags, cr.prog) == 1){
		return 1;
	} else if (check_cards_bounds(ev, COND_X, COND_Y, COND_X+COND_W, COND_Y+COND_H, flags, cr.cond) == 1){
		return 2;
	} else if (check_cards_bounds(ev, ACT_X, ACT_Y, ACT_X+ACT_W, ACT_Y+ACT_H, flags, cr.act) == 1){
		return 3;
	} else if (check_bounds(ev, MEM_X, MEM_Y, MEM_X+MEM_W, MEM_Y+MEM_H) == 1){
		return 4;
	} else if (check_bounds(ev, HELP_X, HELP_Y, HELP_X+HELP_W, HELP_Y+HELP_H) == 1){
		return 5;
	} else if (check_bounds(ev, COMP_X, COMP_Y, COMP_X+COMP_W, COMP_Y+COMP_H) == 1){
		return 6;
	} else if (check_bounds(ev, UND_X, UND_Y, UND_X+UND_W, UND_Y+UND_H) == 1){
		return 7;
	} else {
		return 0;
	}

	return 0;
}

/*
Verifica se os dois vetores de cartas são iguais. Se iguais, retorna -1.
Se diferentes, retorna o index onde começa a diferença entre eles.
*/
int validate_selection(int v1[], int v2[]) {
	// TODO realizar um break quando um dos dois for -1
	int i;
	int index = -1;

	for (i = 0; i < 15; i++) {
		if (v1[i] != v2[i]) {
			index = i;
			break;
		}
	}

	return i;
}


/* Inicializa a struct que controla os desafios. */
void cr_init(challenger_rule *cr) {
	int i;

	(*cr).prog = 0;
	(*cr).cond = 0;
	(*cr).act = 0;

	for(i = 0; i < 15; i++){
		(*cr).v[i] = -1;
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
Retorna -1 em erro ou o valor em sucesso (valor sempre positivo). 
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

/* Retorna quantas cartas tem na pilha. */
int card_stack_count(card_pile *cp) {
    return (*cp).f;
}