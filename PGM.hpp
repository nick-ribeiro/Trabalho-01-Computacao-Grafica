#ifndef PGM_H_INCLUDED
#define PGM_H_INCLUDED
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

struct PGM
{
    string tipo;
    int larg;
    int alt;
    int vmax;
    unsigned char *pixels; // unsigned char -> 0 a 255
    PGM()                  // construtor -> inicializar variaveis
    {
        pixels = nullptr;
        larg = 0;
        alt = 0;
        vmax = 255;
        tipo = "";
    }
};

void destruir(PGM *pgm)
{
    if (pgm->pixels)
        delete pgm->pixels;
    pgm->pixels = nullptr;
    pgm->larg = 0;
    pgm->alt = 0;
    pgm->tipo = "";
    pgm->vmax = 255;
}

void imprimir(PGM *pgm)
{
    cout << "\nPGM\n";
    cout << "Tipo: " << pgm->tipo << endl;
    cout << "Dimensao: " << pgm->larg << "x" << pgm->alt << endl;
    cout << "vMax: " << pgm->vmax << endl;
    if (pgm->pixels) // verifica se o vetor de pixels está alocado
        cout << "Vetor de pixels: " << &(pgm->pixels) << endl;
    else
        cout << "Vetor de pixels: NULL\n";
}

void criar(PGM *pgm, int largura, int altura)
{
    if (pgm->pixels)
        delete pgm->pixels;
    
    int tamanho = largura * altura;

    pgm->tipo = "P2";
    pgm->larg = largura;
    pgm->alt = altura;
    pgm->vmax = 255;
    pgm->pixels = new unsigned char[tamanho];

    // definir a cor preta para todos os pixels
    for (int i = 0; i < tamanho; i++)
        pgm->pixels[i] = 0;
}

bool gravar(PGM *pgm, string caminho)
{
    if (!pgm->pixels)
        return false;

    ofstream arq;

    arq.open(caminho);
    if (!arq.is_open())
    {
        cout << "PGM: endereco do arquivo invalido\n";
        return false;
    }

    arq << pgm->tipo << endl;

    arq << pgm->larg << " " << pgm->alt << endl;

    arq << 255 << endl; // valor máximo de cor fixo em 255

    int tam = pgm->larg * pgm->alt;
    for (int i = 0; i < tam; i++)
    {
        arq << (int)pgm->pixels[i] << endl;
        arq.flush();
    }

    arq.close();
    return true;
}

//#Ex06#: validar uma coordenada
bool coordValida(PGM *pgm, int x, int y)
{
    if(x >= 0 && x < pgm->larg && y >= 0 && y < pgm->alt) {
        return true;
    }
    else 
    {
        return false;
    }
}

string lerLinhaArquivo(std::ifstream &arq)
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

bool ler(PGM *pgm, string caminho)
{
    ifstream arq;
    string dado;
    stringstream str;

    arq.open(caminho);
    if (!arq.is_open())
    {
        cout << "PGM: endereco do arquivo invalido\n";
        return false;
    }

    // ler o tipo
    dado = lerLinhaArquivo(arq);
    if (dado == "P2")
        pgm->tipo = dado;
    else
    {
        cout << "PGM: erro ao ler o tipo da imagem\n";
        return false;
    }

    // ler largura e altura
    dado = lerLinhaArquivo(arq);
    str = stringstream(dado);
    if (!(str >> pgm->larg))
    {
        cout << "PGM: erro ao ler a largura\n";
        return false;
    }
    if (!(str >> pgm->alt))
    {
        cout << "PGM: erro ao ler a largura\n";
        return false;
    }

    // ler vmax
    dado = lerLinhaArquivo(arq);
    str = stringstream(dado);
    if (!(str >> pgm->vmax))
    {
        cout << "PGM: erro ao ler vmax\n";
        return false;
    }

    if (pgm->pixels)
        delete pgm->pixels;

    int tam = pgm->larg * pgm->alt;

    pgm->pixels = new unsigned char[tam];

    int i = 0, p;
    while (!arq.eof())
    {

        if (arq >> p)
        {
            if (i < tam)
                pgm->pixels[i] = (unsigned char)p;
            i++;
        }
        else
            break;
    }

    if (i != tam)
    {
        cout << "PGM: erro ao ler os pixels\n";
        return false;
    }
    arq.close();
    return true;
}


unsigned char getPixel(PGM *pgm, int x, int y)
{
    if (!pgm->pixels)
        return 0;

    return pgm->pixels[y * pgm->larg + x];
}

void setPixel(PGM *pgm, int x, int y, unsigned char cor)
{
    if (!pgm->pixels)
        return;

    pgm->pixels[y * pgm->larg + x] = cor;
}

void setLinha(PGM *pgm, int linha, unsigned char cor)
{
    if (!pgm->pixels)
        return;

    int larg = pgm->larg;
    for(int i = 0; i < larg; i++) {
        setPixel(pgm , i, linha, cor);
    }
}

void setRegiao(PGM *pgm, int larg1, int alt1, int larg2, int alt2, unsigned char cor) {

    int larg = pgm->larg;
    int alt = pgm->alt;

    for(int i = alt1; i < alt2; i++) {
        for(int j = larg1; j < larg2; j++) {
            if(j >= 0 && j < larg && i >= 0 && i < alt) {
                setPixel(pgm, j, i, cor);
            }
        }
    }
}

void colorirALinha(PGM *pgm, int intervalo, unsigned char cor) {

    int larg = pgm->larg;
    int alt = pgm->alt;

    for(int i = intervalo; i < alt; i+= intervalo) {
        for(int j = 0; j < larg; j++) {
            setPixel(pgm, j, i, 255);
        }
    }
}

void flip(PGM *pgm) {
    
    int larg = pgm->larg;
    int alt = pgm->alt;

    for(int i = 0; i < alt/2; i++) {
        for(int j = 0; j < larg; j++) {
            unsigned char aux = getPixel(pgm, j, i);
            unsigned char aux2 = getPixel(pgm, j, alt - 1 - i);
            setPixel(pgm, j, i, aux2);
            setPixel(pgm, j, alt - 1 - i, aux);
        }
    }
}

int getLargura(PGM *pgm)
{
    return pgm->larg;
}

int getAltura(PGM *pgm)
{
    return pgm->alt;
}

void setCoordenada(PGM *pgm, int x1, int y1, int x2, int y2, PGM *imgS)
{
    int larg = pgm->larg;
    int alt = pgm->alt;
    unsigned char aux;

    for(int i = y1; i <= y2; i++)
    {
        for(int j = x1; j <= x2; j++)
        {
            if(j >= 0 && j < larg && i >= 0 && i < alt)
            {
                aux = getPixel(pgm, j, i);
                setPixel(imgS, j, i, aux);
            }
        }
    }
        
}

#endif