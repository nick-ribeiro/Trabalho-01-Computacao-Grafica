#ifndef PPM_H_INCLUDED
#define PPM_H_INCLUDED
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "PGM.hpp"
using namespace std;

struct PPM
{
    string tipo;
    int larg;
    int alt;
    int vmax;
    unsigned char *pixels; // unsigned char -> 0 a 255
    PPM()                  // construtor -> inicializar variaveis
    {
        pixels = nullptr;
        larg = 0;
        alt = 0;
        vmax = 255;
        tipo = "";
    }
};

struct RGB{
    unsigned char r, g, b;
    RGB()
    {
        r=0;
        g=0;
        b=0;
    }
    RGB(unsigned char _r, unsigned char _g,unsigned char _b)
    {
        r=_r;
        g=_g;
        b=_b;
    }
};

ostream& operator<<(ostream& os, const RGB rgb)
{
    return os << (int) rgb.r << ", " << (int) rgb.g << ", " << (int) rgb.b ;
}

void destruir(PPM *ppm)
{
    if (ppm->pixels)
        delete ppm->pixels;
    ppm->pixels = nullptr;
    ppm->larg = 0;
    ppm->alt = 0;
    ppm->tipo = "";
    ppm->vmax = 255;
}

void imprimir(PPM *ppm)
{
    cout << "\nPPM\n";
    cout << "Tipo: " << ppm->tipo << endl;
    cout << "Dimensao: " << ppm->larg << "x" << ppm->alt << endl;
    cout << "vMax: " << ppm->vmax << endl;
    if (ppm->pixels) // verifica se o vetor de pixels está alocado
        cout << "Vetor de pixels: " << &(ppm->pixels) << endl;
    else
        cout << "Vetor de pixels: NULL\n";
}

void cria(PPM *ppm, int largura, int altura, RGB corFundo)
{
    if (ppm->pixels)
        delete ppm->pixels;
    
    int tamanho = largura * altura * 3; //vezes 3, pois cada pixel possui RGB

    ppm->tipo = "P3";
    ppm->larg = largura;
    ppm->alt = altura;
    ppm->vmax = 255;
    ppm->pixels = new unsigned char[tamanho];

    // definir a cor preta para todos os pixels
    for (int i = 0; i < tamanho; i+= 3){
        ppm->pixels[i] = corFundo.r;
        ppm->pixels[i+1] = corFundo.g;
        ppm->pixels[i+2] = corFundo.b;
    }
}

bool grava(PPM *ppm, string caminho)
{
    if (!ppm->pixels)
        return false;

    ofstream arq;

    arq.open(caminho);
    if (!arq.is_open())
    {
        cout << "PPM: endereco do arquivo invalido\n";
        return false;
    }

    arq << ppm->tipo << endl;

    arq << ppm->larg << " " << ppm->alt << endl;

    arq << 255 << endl; // valor máximo de cor fixo em 255

    int tam = ppm->larg * ppm->alt * 3;  //vezes 3, pois cada pixel possui RGB
    for (int i = 0; i < tam; i++)
    {
        arq << (int)ppm->pixels[i] << endl;
        arq.flush();
    }

    arq.close();
    return true;
}


string lerLinhaArquivoPPM(std::ifstream &arq)
{
    string linha = "", dadoLido = "";
    while (!arq.eof())
    {
        linha = "";
        std::getline(arq, linha);
        if (linha.size() > 0 && linha[0] != '#')
            return linha;
    }
    return linha;
}

bool le(PPM *ppm, string caminho)
{
    ifstream arq;
    string dado;
    stringstream str;

    arq.open(caminho);
    if (!arq.is_open())
    {
        cout << "PPM: endereco do arquivo invalido\n";
        return false;
    }

    // ler o tipo
    dado = lerLinhaArquivoPPM(arq);
    if (dado == "P3")
        ppm->tipo = dado;
    else
    {
        cout << "PPM: erro ao ler o tipo da imagem\n";
        return false;
    }

    // ler largura e altura
    dado = lerLinhaArquivoPPM(arq);
    str = stringstream(dado);
    if (!(str >> ppm->larg))
    {
        cout << "PPM: erro ao ler a largura\n";
        return false;
    }
    if (!(str >> ppm->alt))
    {
        cout << "PPM: erro ao ler a largura\n";
        return false;
    }

    // ler vmax
    dado = lerLinhaArquivoPPM(arq);
    str = stringstream(dado);
    if (!(str >> ppm->vmax))
    {
        cout << "PPM: erro ao ler vmax\n";
        return false;
    }

    if (ppm->pixels)
        delete ppm->pixels;

    int tam = ppm->larg * ppm->alt * 3; //vezes 3, pois cada pixel possui RGB

    ppm->pixels = new unsigned char[tam];

    int i = 0, p;
    while (!arq.eof())
    {

        if (arq >> p)
        {
            if (i < tam)
                ppm->pixels[i] = (unsigned char)p;
            i++;
        }
        else
            break;
    }

    if (i != tam)
    {
        cout << "PPM: erro ao ler os pixels\n";
        return false;
    }
    arq.close();
    return true;
}


RGB getPixel(PPM *ppm, int x, int y)
{
    RGB rgb;
    if (!ppm->pixels)
        return rgb;

    rgb.r = ppm->pixels[y * ppm->larg*3 + x*3];
    rgb.g = ppm->pixels[y * ppm->larg*3 + x*3 +1];
    rgb.b = ppm->pixels[y * ppm->larg*3 + x*3 +2];

    return rgb;
}

void setPixel(PPM *ppm, int x, int y, RGB rgb)
{
    if (!ppm->pixels)
        return;

    ppm->pixels[y * ppm->larg*3 + x*3] = rgb.r;
    ppm->pixels[y * ppm->larg*3 + x*3 +1] = rgb.g;
    ppm->pixels[y * ppm->larg*3 + x*3 +2] = rgb.b;
}

void floodFillPPM(PPM *imgE, int x, int y, RGB corA, RGB corN)
{
    if(x < 0 || x >= imgE->larg || y < 0 || y >= imgE->alt)
        return;

    RGB corAtual = getPixel(imgE, x, y);

    if(corAtual.r != corA.r || corAtual.g != corA.g || corAtual.r != corA.r)
        return;

    setPixel(imgE, x, y, corN);
    floodFillPPM(imgE, x+1, y, corA, corN); 
    floodFillPPM(imgE, x-1, y, corA, corN); 
    floodFillPPM(imgE, x, y+1, corA, corN); 
    floodFillPPM(imgE, x, y-1, corA, corN); 
}

void setLinha(PPM *ppm, int linha, RGB cor)
{
    if (!ppm->pixels)
        return;

    int larg = ppm->larg;
    for(int i = 0; i < larg; i++) {
        setPixel(ppm , i, linha, cor);
    }
}

bool cValida(PPM *ppm, int x, int y)
{
    if(x >= 0 && x < ppm->larg && y >= 0 && y < ppm->alt) {
        return true;
    }
    else 
    {
        return false;
    }
}

void setCoordenada(PPM *ppm, int x1, int y1, int x2, int y2, RGB cor)
{
    int larg = ppm->larg;
    int alt = ppm->alt;

    for(int j = x1; j <= x2; j++)
    {
        if(j >= 0 && j < larg)
        {
            setPixel(ppm, j, y1, cor);
            setPixel(ppm, j, y2, cor);
        }
    }

    for(int i = y1; i <= y2; i++) {
        if(i >= 0 && i < alt) {
            setPixel(ppm, x1, i, cor);
            setPixel(ppm, x2, i, cor);
        }
    }
}

void colorirALinha(PPM *ppm, int intervalo) {

    int larg = ppm->larg;
    int alt = ppm->alt;
    RGB cor;

    for(int i = intervalo; i < alt; i+= intervalo) {

        cor.r = rand () %256;
        cor.g = rand () %256;
        cor.b = rand () %256;

        for(int j = 0; j < larg; j++) {
            setPixel(ppm, j, i, cor);
        }
    }
}

int getLargura(PPM *ppm)
{
    return ppm->larg;
}

int getAltura(PPM *ppm)
{
    return ppm->alt;
}

void setCoord(PPM *ppm, int x1, int y1, int x2, int y2, PPM *imgS) 
{
    int larg = ppm->larg;
    int alt = ppm->alt;
    RGB aux;

    for(int i = y1; i <= y2; i++)
    {
        for(int j = x1; j <= x2; j++)
        {
            if(j >= 0 && j < larg && i >= 0 && i < alt)
            {
                aux = getPixel(ppm, j, i);
                setPixel(imgS, j, i, aux);
            }
        }
    }
}

void setPretoBranco(PPM *ppm, int x1, int y1, int x2, int y2, PGM *imgS)
{
    RGB cor;
	unsigned char cinza;

    int larg = ppm->larg;
    int alt = ppm->alt;

    if(!ppm->pixels)
    {
        cout << "Leia a imagem primeiro!!\n";
        return;
    }

    for(int y = y1; y <= y2; y++)
    {
        for(int x = x1; x <= x2; x++)
        {
            if(x >= 0 && x < larg && y >= 0 && y < alt)
            {
                cor = getPixel(ppm, x , y);
                cinza = ((0.299 * cor.r) + (0.587 * cor.g) + (0.114 * cor.b));
                setPixel(imgS, x, y, cinza); 
            }
        }
    }
}

void flip(PPM *ppm) {
    
    int larg = ppm->larg;
    int alt = ppm->alt;
    RGB cor;

    for(int i = 0; i < alt; i++) {
        for(int j = 0; j < larg/2; j++) {
            RGB aux = getPixel(ppm, j, i);
            RGB aux2 = getPixel(ppm, larg - 1 - j, i);
            setPixel(ppm, j, i, aux2);
            setPixel(ppm, larg - 1 - j, i, aux);
        }
    }
}

void setCoordenadaEBorda(PPM *ppm, int x1, int y1, int x2, int y2, RGB cor)
{
    int larg = ppm->larg;
    int alt = ppm->alt;

    for(int j = x1; j <= x2; j++)
    {
        if(j >= 0 && j < larg)
        {
            setPixel(ppm, j, y1, cor);
            setPixel(ppm, j, y1 - 1, cor);
            setPixel(ppm, j, y1 - 2, cor);
            setPixel(ppm, j, y1 - 3, cor);
            setPixel(ppm, j, y1 - 4, cor);
            setPixel(ppm, j, y1 - 5, cor);
            setPixel(ppm, j, y1 - 6, cor);
            setPixel(ppm, j, y1 - 7, cor);
            setPixel(ppm, j, y1 - 8, cor);
            setPixel(ppm, j, y1 - 9, cor);
            setPixel(ppm, j, y2, cor);
            setPixel(ppm, j, y2 + 1, cor);
            setPixel(ppm, j, y2 + 2, cor);
            setPixel(ppm, j, y2 + 3, cor);
            setPixel(ppm, j, y2 + 4, cor);
            setPixel(ppm, j, y2 + 5, cor);
            setPixel(ppm, j, y2 + 6, cor);
            setPixel(ppm, j, y2 + 7, cor);
            setPixel(ppm, j, y2 + 8, cor);
            setPixel(ppm, j, y2 + 9, cor);
        }
    }

    for(int i = y1; i <= y2; i++) {
        if(i >= 0 && i < alt) {
            setPixel(ppm, x1, i, cor);
            setPixel(ppm, x1 - 1, i, cor);
            setPixel(ppm, x1 - 2, i, cor);
            setPixel(ppm, x1 - 3, i, cor);
            setPixel(ppm, x1 - 4, i, cor);
            setPixel(ppm, x1 - 5, i, cor);
            setPixel(ppm, x1 - 6, i, cor);
            setPixel(ppm, x1 - 7, i, cor);
            setPixel(ppm, x1 - 8, i, cor);
            setPixel(ppm, x1 - 9, i, cor);
            setPixel(ppm, x2, i, cor);
            setPixel(ppm, x2 + 1, i, cor);
            setPixel(ppm, x2 + 2, i, cor);
            setPixel(ppm, x2 + 3, i, cor);
            setPixel(ppm, x2 + 4, i, cor);
            setPixel(ppm, x2 + 5, i, cor);
            setPixel(ppm, x2 + 6, i, cor);
            setPixel(ppm, x2 + 7, i, cor);
            setPixel(ppm, x2 + 8, i, cor);
            setPixel(ppm, x2 + 9, i, cor);
        }
    }
}

#endif