#include "raylib/raylib.h"

#include "Utils.h"

void drawTextAlinhado(const char *texto, int y, int tamanhoFonte, Color cor, int posicao) {
    
    int tamanhoTexto = MeasureText(texto, tamanhoFonte);
    int tamanhoTela = GetScreenWidth();
    int posicaoTexto = 0;
    
    switch (posicao) {
        case CENTRO:
        
            posicaoTexto = (tamanhoTela - tamanhoTexto) / 2;

            DrawText(texto, posicaoTexto, y, tamanhoFonte, cor);

            break;
        
        case DIREITA:

            posicaoTexto = (int) (tamanhoTela - tamanhoTexto - tamanhoTela * 0.03f);

            DrawText(texto, posicaoTexto, y, tamanhoFonte, cor);

            break;

        case ESQUERDA:

            posicaoTexto = (int) (tamanhoTela * 0.03f);

            DrawText(texto, posicaoTexto, y, tamanhoFonte, cor);

            break;
    
        default:
            TraceLog(LOG_INFO, "ERRO AO ESCREVER TEXTO ALINHADO");
            break;
    }


}