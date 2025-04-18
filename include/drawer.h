#ifndef __DRAWER_H__
#define __DRAWER_H__
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "data.h"
#include "best_fit.h"

#define WINDOW_HEIGH 720
#define WINDOW_WIDTH 1080

#define SCALE 15


void draw(Data* data,  BoxStatus** items_status);


static double colors[][3] = {
    {1.0, 0.0, 0.0},   // Vermelho
    {0.0, 0.0, 1.0},   // Azul
    {1.0, 1.0, 0.0},   // Amarelo
    {0.0, 1.0, 0.0},   // Verde
    {1.0, 0.5, 0.0},   // Laranja
    {0.5, 0.0, 0.5},   // Roxo
    {1.0, 0.0, 0.5},   // Rosa
    {0.6, 0.3, 0.0},   // Marrom
    // {0.5, 0.5, 0.5},   // Cinza
    // {0.0, 0.0, 0.0},   // Preto
    {1.0, 1.0, 1.0},   // Branco
    {0.25, 0.88, 0.82}, // Turquesa
    {1.0, 0.0, 1.0},   // Magenta
    {0.7, 0.4, 1.0},   // Lilás
    {0.5, 0.0, 0.13},  // Vinho
    {0.96, 0.87, 0.7}, // Bege
    {1.0, 0.84, 0.0},  // Ouro
    {0.75, 0.75, 0.75}, // Prata
    {0.8, 0.5, 0.2},   // Bronze
    {0.53, 0.81, 0.92}, // Azul-claro
    {0.6, 1.0, 0.2},   // Verde-limão
    {1.0, 0.08, 0.58}, // Rosa-choque
    {0.0, 0.0, 0.5},   // Azul-marinho
    {0.53, 0.8, 0.92}, // Azul-celeste
    {1.0, 0.14, 0.0},  // Vermelho-escarlate
    {0.31, 0.78, 0.47}, // Verde-esmeralda
    {0.86, 0.7, 0.2},  // Amarelo-mostarda
    {0.9, 0.9, 0.98},  // Lavanda
    {1.0, 0.5, 0.31},  // Coral
    {0.42, 0.56, 0.14}, // Verde-oliva
    {0.0, 0.41, 0.41}, // Azul-petróleo
    {0.0, 1.0, 1.0},   // Ciano
    {0.57, 0.4, 0.7},  // Malva
    {0.5, 1.0, 0.0},   // Chartreuse
    {0.89, 0.45, 0.36}, // Terracota
    {0.82, 0.41, 0.12}, // Chocolate
    {0.98, 0.5, 0.45}, // Salmão
    {0.54, 0.6, 0.36}, // Verde-musgo
    {0.04, 0.73, 0.71}, // Azul-tiffany
    {0.88, 0.07, 0.37}, // Vermelho-rubi
    {0.76, 0.7, 0.5},  // Areia
    {0.94, 0.91, 0.84}, // Pérola
    {1.0, 0.75, 0.0},  // Âmbar
    {0.55, 0.0, 0.0},  // Vermelho-sangue
    {0.49, 0.98, 1.0}, // Azul-elétrico
    {0.33, 0.33, 0.33}, // Cinza-chumbo
    {0.94, 1.0, 1.0},  // Branco-gelo
    {0.92, 1.0, 0.2},  // Amarelo-limão
    {0.6, 1.0, 0.6},   // Verde-menta
    {0.8, 0.6, 0.6},   // Rosa-antigo
    {0.0, 0.78, 0.55}, // Verde-mar
    {0.35, 0.31, 0.55}, // Azul-ardósia
    {0.72, 0.5, 0.1},  // Dourado-claro
    {0.1, 0.1, 0.2},   // Azul-petróleo-profundo
    {1.0, 0.55, 0.0},  // Laranja-queimado
    {1.0, 0.27, 0.0},  // Vermelho-fogo
    {0.2, 0.8, 0.2},   // Verde-neon
    {0.3, 0.1, 0.7},   // Roxo-orquídea
    {0.93, 0.51, 0.93}, // Lilás-clarinho
    {0.73, 0.56, 0.37}  // Ocre-dourado
};




#endif