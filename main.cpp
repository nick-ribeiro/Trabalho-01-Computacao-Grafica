#include <iostream>
#include <stdio.h>
#include <math.h>
#include <dos.h>
#include "PGM.hpp"
#include "PPM.hpp"
using namespace std;

void DDAPGM (PGM *imgE, float x1, float x2, float y1, float y2)
{
    float x, y, dx, dy, step; // Dx e Dy serão os valores da subtração de x2 com x1 e y2 com y1
    // Já a variável step irá assumir dx ou dy caso dx seja maior ou igual a dy ou dy seja maior que dx
    int i = 1; // Contador

    dx = abs(x2 - x1); // Retorna o valor absoluto de dx
    dy = abs(y2 - y1); // Retorna o valor absoluto de dy

    if(dx >= dy)
        step = dx;
    else
        step = dy;
    
    dx = dx/step; // Divide tanto dx quanto dy pela variável step
    dy = dy/step;
 
    x = x1; // Assumi inicialmente o valor de x como x1
    y = y1; // Assumi inicialmente o valor de y como y1

    while(i <= step)
    {
        setPixel(imgE, x, y, 128);
        x = x + dx;
	    y = y + dy;
	    i = i + 1;
    }
}

void DDAPPM (PPM *imgE, float x1, float x2, float y1, float y2)
{
    float x, y, dx, dy, step; // Dx e Dy serão os valores da subtração de x2 com x1 e y2 com y1
    // Já a variável step irá assumir dx ou dy caso dx seja maior ou igual a dy ou dy seja maior que dx
    int i = 1; // Contador

    dx = abs(x2 - x1); // Retorna o valor absoluto de dx
    dy = abs(y2 - y1); // Retorna o valor absoluto de dx

    if(dx >= dy)
        step = dx;
    else
        step = dy;
    
    dx = dx/step; // Divide tanto dx quanto dy pela variável step 
    dy = dy/step;

    x = x1; // Assumi inicialmente o valor de x como x1
    y = y1; // Assumi inicialmente o valor de y como y1

    while(i <= step)
    {
        setPixel(imgE, x, y, RGB(128,0,128));
        x = x + dx;
	    y = y + dy;
	    i = i + 1;
    }
}

void Bresenham (PGM *imgE, int x1, int x2, int y1, int y2)
{
    int dx = x2 - x1, dy = y2 - y1; // Dx e Dy serão os valores da subtração de x2 com x1 e y2 com y1
    int d = 2 * dy - dx; // Valor inicial de d;
    int incE = 2 * dy, incNE = 2 * (dy - dx); // Variável incE é o incremento para mover E
    // Já a variável incNE é o incremento para mover NE
    int x = x1, y = y1; // Assumi inicialmente o valor de x como x1 e assumi inicialmente o valor de y como y1

    setPixel(imgE, x, y, 255);

    while (x < x2)
    {
        if(d <= 0) // Escolhe E (Abaixo do ponto médio)
        {
            d += incE;
            x++;
        }
        else // Escolhe NE (Acima do ponto médio)
        {
            d += incNE;
            x++;
            y++;
        }
        setPixel(imgE, x, y, 255);
    }
}

void BresenhamPPM (PPM *imgE, int x1, int x2, int y1, int y2)
{
    int dx = x2 - x1, dy = y2 - y1; // Dx e Dy serão os valores da subtração de x2 com x1 e y2 com y1
    int d = 2 * dy - dx; // Valor inicial de d;
    int incE = 2 * dy, incNE = 2 * (dy - dx); // Variável incE é o incremento para mover E
    // Já a variável incNE é o incremento para mover NE
    int x = x1, y = y1; // Assumi inicialmente o valor de x como x1 e assumi inicialmente o valor de y como y1

    setPixel(imgE, x, y, RGB(255, 255, 0));

    while (x < x2)
    {
        if(d <= 0) // Escolhe E (Abaixo do ponto médio)
        {
            d += incE;
            x++;
        }
        else // Escolhe NE (Acima do ponto médio)
        {
            d += incNE;
            x++;
            y++;
        }
        setPixel(imgE, x, y, RGB(255, 255, 0));
    }
}

// Diferença entre os algoritmos: DDA é um algoritmo de desenho de linha que incrementa as coordenadas x e y de um ponto
// em pequenos passos até atingir o ponto final. O algoritmo de Bresenham usa aritmética inteira para desenhar linhas. 
// Ele funciona determinando o pixel mais próximo ao caminho da linha em cada coordenada x e escolhendo o pixel mais próximo.

// O Algoritmo DDA usa aritmética de ponto flutuante para rasterização, enquanto o Algoritmo de Bresenham usa aritmética inteira, 
// tornando-a mais eficiente. 
// O Algoritmo de Bresenham gera desenhos de linha mais precisos do que o Algoritmo DDA.
// O Algoritmo de Bresenham é mais rápido e requer menos recursos computacionais do que o Algoritmo DDA.
// O Alogritmo de Bresenham permite otimização, enquanto o DDA não
// O DDA usa cálculos complexos em seu funcionamento, enquanto o Algoritmo de Bresenham usa cálculos simples.

int main (void)
{
    float x1 = 200, x2 = 300, y1 = 250, y2 = 350;
    PGM imgE;
    PPM imgE2;
    criar(&imgE, 500, 500);
    cria(&imgE2, 500, 500, RGB(0,0,0));

    DDAPGM(&imgE, x1, x2, y1, y2);
    cout << "\n Imagem gravada para PGM";
    gravar(&imgE, "dda.pgm");

    DDAPPM(&imgE2, x1, x2, y1, y2);
    cout << "\n Imagem gravada para PPM";
    grava(&imgE2, "dda.ppm");

    PGM imgE3;
    PPM imgE4;
    criar(&imgE3, 500, 500);
    cria(&imgE4, 500, 500, RGB(0,0,0));

    Bresenham(&imgE3, 200, 500, 100, 450);
    cout << "\n Imagem gravada para o Algoritmo Bresenham para PGM";
    gravar(&imgE3, "bresenham.pgm");
    BresenhamPPM(&imgE4, 200, 500, 100, 450);
    cout << "\n Imagem gravada para o Algoritmo Bresenham para PPM";
    grava(&imgE4, "bresenham.ppm");

    cout << "\n Pressione uma tecla para encerrar o programa.\n";
    getchar();
    return EXIT_SUCCESS;
}