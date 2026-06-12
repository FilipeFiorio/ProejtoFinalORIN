#include <stdlib.h>
#include <stdio.h>

#include "raylib/raylib.h"

#include "Jogador.h"
#include "Tipos.h"
#include "Inimigo.h"
#include "Mapa.h"
#include "Animacao.h"
#include "ResourceManager.h"

static void resolverColisaoJogadorMapaX(GameWorld *gw);
static void resolverColisaoJogadorMapaY(GameWorld *gw, float delta);
static void verificarColisaoJogadorItem(GameWorld *gw);
static void verificarColisaoJogadorInimigo(GameWorld *gw);

static void desenharAnimacaoJogador(Jogador *j, QuadroAnimacao *quadro, Color tonalidade);
static Animacao *getAnimacaoAtualJogador(Jogador *j);
static QuadroAnimacao *getQuadroAnimacaoAtualJogador(Jogador *j);

static bool MOSTRAR_RETANGULO_COLISAO = false;

Jogador *criarJogador(float x, float y, float largura, float altura, Color cor) {

    Jogador *novoJogador = (Jogador*) malloc(sizeof(Jogador));

    novoJogador->ret = (Rectangle) {
        .x = x,
        .y = y,
        .width = largura,
        .height = altura,
    };
    novoJogador->vel = (Vector2) {0};
    novoJogador->cor = BLUE;

    novoJogador->vidas = 5;
    novoJogador->moedas = 0;

    novoJogador->velAndando = 225;
    novoJogador->velCorrendo = 325;
    novoJogador->velPulo = -425;
    novoJogador->velPuloCorrendo = -525;
    novoJogador->velMaxQueda = 600;
    novoJogador->velFreio = 1100;
    novoJogador->velDesacelarar = 800;
    novoJogador->velAcelerar = 600;
    novoJogador->velMax = 0;

    novoJogador->noChao = false;
    novoJogador->morto = false;
    novoJogador->paraDireita = true;
    novoJogador->freando = false;

    novoJogador->congelado = false;
    novoJogador->contadorTempoCongelado = 0;

    novoJogador->estado = JOGADOR_PARADO;

    int quantidadeAnimacoes = 0;

    novoJogador->animacaoParado.quantidadeQuadros = 1;
    novoJogador->animacaoParado.quadroAtual = 0;
    novoJogador->animacaoParado.contadorTempoQuadro = 0;
    novoJogador->animacaoParado.finalizada = false;
    novoJogador->animacaoParado.pararNoUltimoQuadro = false;
    novoJogador->animacaoParado.executarUmaVez = false;
    criarQuadroAnimacao(&novoJogador->animacaoParado, novoJogador->animacaoParado.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoJogador->animacaoParado.quadros,
        novoJogador->animacaoParado.quantidadeQuadros,
        1000,
        0,
        0,
        16,
        16,
        false,
        0
    );

    
    novoJogador->animacaoAndando.quantidadeQuadros = 3;
    novoJogador->animacaoAndando.quadroAtual = 0;
    novoJogador->animacaoAndando.contadorTempoQuadro = 0;
    novoJogador->animacaoAndando.finalizada = false;
    novoJogador->animacaoAndando.executarUmaVez = false;
    novoJogador->animacaoAndando.pararNoUltimoQuadro = false;
    criarQuadroAnimacao(&novoJogador->animacaoAndando, novoJogador->animacaoAndando.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoJogador->animacaoAndando.quadros,
        novoJogador->animacaoAndando.quantidadeQuadros,
        90,
        0,
        16,
        16,
        16,
        false,
        0
    );

    novoJogador->animacaoCorrendo.quantidadeQuadros = 3;
    novoJogador->animacaoCorrendo.quadroAtual = 0;
    novoJogador->animacaoCorrendo.contadorTempoQuadro = 0;
    novoJogador->animacaoCorrendo.finalizada = false;
    novoJogador->animacaoCorrendo.executarUmaVez = false;
    novoJogador->animacaoCorrendo.pararNoUltimoQuadro = false;
    criarQuadroAnimacao(&novoJogador->animacaoCorrendo, novoJogador->animacaoCorrendo.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoJogador->animacaoCorrendo.quadros,
        novoJogador->animacaoCorrendo.quantidadeQuadros,
        60,
        0,
        16,
        16,
        16,
        false,
        0
    );

    novoJogador->animacaoPulando.quantidadeQuadros = 1;
    novoJogador->animacaoPulando.quadroAtual = 0;
    novoJogador->animacaoPulando.contadorTempoQuadro = 0;
    novoJogador->animacaoPulando.finalizada = false;
    novoJogador->animacaoPulando.pararNoUltimoQuadro = false;
    novoJogador->animacaoPulando.executarUmaVez = false;
    criarQuadroAnimacao(&novoJogador->animacaoPulando, novoJogador->animacaoPulando.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoJogador->animacaoPulando.quadros,
        novoJogador->animacaoPulando.quantidadeQuadros,
        1000,
        0,
        32,
        16,
        16,
        false,
        0
    );

    novoJogador->animacaoMorrendo.quantidadeQuadros = 1;
    novoJogador->animacaoMorrendo.quadroAtual = 0;
    novoJogador->animacaoMorrendo.contadorTempoQuadro = 0;
    novoJogador->animacaoMorrendo.finalizada = false;
    novoJogador->animacaoMorrendo.executarUmaVez = true;
    novoJogador->animacaoMorrendo.pararNoUltimoQuadro = false;
    criarQuadroAnimacao(&novoJogador->animacaoMorrendo, novoJogador->animacaoMorrendo.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoJogador->animacaoMorrendo.quadros,
        novoJogador->animacaoMorrendo.quantidadeQuadros,
        500,
        0,
        48,
        16,
        16,
        false,
        0
    );

    novoJogador->animacaoFreando.quantidadeQuadros = 1;
    novoJogador->animacaoFreando.quadroAtual = 0;
    novoJogador->animacaoFreando.contadorTempoQuadro = 0;
    novoJogador->animacaoFreando.finalizada = false;
    novoJogador->animacaoFreando.executarUmaVez = true;
    novoJogador->animacaoFreando.pararNoUltimoQuadro = false;
    criarQuadroAnimacao(&novoJogador->animacaoFreando, novoJogador->animacaoFreando.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoJogador->animacaoFreando.quadros,
        novoJogador->animacaoFreando.quantidadeQuadros,
        500,
        0,
        64,
        16,
        16,
        false,
        1
    );

    novoJogador->animacoes[JOGADOR_PARADO] = &novoJogador->animacaoParado;
    quantidadeAnimacoes++;

    novoJogador->animacoes[JOGADOR_ANDANDO] = &novoJogador->animacaoAndando;
    quantidadeAnimacoes++;

    novoJogador->animacoes[JOGADOR_CORRENDO] = &novoJogador->animacaoCorrendo;
    quantidadeAnimacoes++;

    novoJogador->animacoes[JOGADOR_PULANDO] = &novoJogador->animacaoPulando;
    quantidadeAnimacoes++;

    novoJogador->animacoes[JOGADOR_MORRENDO] = &novoJogador->animacaoMorrendo;
    quantidadeAnimacoes++;

    novoJogador->animacoes[JOGADOR_FREANDO] = &novoJogador->animacaoFreando;
    quantidadeAnimacoes++;

    novoJogador->quantidadeAnimacoes = quantidadeAnimacoes;
    
    return novoJogador;
}

void destruirJogador(Jogador *j) {
    if(j != NULL) {
        for(int i = 0; i < j->quantidadeAnimacoes; i++) {
            destruirQuadroAnimacao(j->animacoes[i]);
        }
        free(j);
    }
}


// Implementado para WASD, setinhas e controle(n sei se funciona)
void entradaJogador(Jogador *j) {
    
    if(j->estado != JOGADOR_MORRENDO) {

        EstadoJogador estadoAnterior = j->estado;

        bool correr = IsKeyDown(KEY_LEFT_SHIFT) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT));
        bool esquerda = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT));
        bool direita = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT));
            

        j->velMax = correr? j->velCorrendo : j->velAndando;

        if(esquerda) {
            j->paraDireita = false;

            if(j->vel.x > 0) {
                j->vel.x -= j->velFreio * GetFrameTime();
                j->freando = true;
                 if(j->vel.x < 0) {
                    j->vel.x = 0;
                    
                }
            } else {
                j->vel.x -= j->velAcelerar * GetFrameTime();
                if(j->vel.x < -j->velMax) {
                    j->vel.x = -j->velMax;
                }
            }
                
        } else if (direita) {
            j->paraDireita = true;
            
            if(j->vel.x < 0) {
                j->vel.x += j->velFreio * GetFrameTime();
                j->freando = true;
                if(j->vel.x > 0) {
                    j->vel.x = 0;
                   
                }
    
            } else {
                j->vel.x += j->velAcelerar * GetFrameTime();
                if(j->vel.x > j->velMax) {
                    j->vel.x = j->velMax;
                }
            }

        } else {
            if ( j->vel.x > 0 ) {
                j->vel.x -= j->velDesacelarar * GetFrameTime();
                    if ( j->vel.x < 0 ) {
                        j->vel.x = 0;
                    }
            } else if ( j->vel.x < 0 ) {
                j->vel.x += j->velDesacelarar * GetFrameTime();
                if ( j->vel.x > 0 ) {
                    j->vel.x = 0;
                }
            }
        }
            
        if(j->vel.x != 0 && j->noChao) {
            if(correr) {
                j->estado = JOGADOR_CORRENDO;
            } else {
                j->estado = JOGADOR_ANDANDO;
            }
        } 

        if(j->freando) {
            j->estado = JOGADOR_FREANDO;
        }
        
        bool pular = IsKeyPressed(KEY_SPACE) || (IsGamepadAvailable(0) && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN));
        
        if(pular && j->noChao) {
            j->vel.y = correr ? j->velPuloCorrendo : j->velPulo;
            j->noChao = false;
        }
        
        if(j->vel.y != 0) {
            j->estado = JOGADOR_PULANDO;
        }
        
        if(!direita && !esquerda && j->noChao) {
            j->estado = JOGADOR_PARADO;
        }

        if(estadoAnterior == JOGADOR_ANDANDO && j->estado == JOGADOR_CORRENDO) {
            sincronizarAnimacao(&j->animacaoCorrendo, &j->animacaoAndando);
        } else if(estadoAnterior == JOGADOR_CORRENDO && j->estado == JOGADOR_ANDANDO) {
            sincronizarAnimacao(&j->animacaoAndando, &j->animacaoCorrendo);
        }

    }

}

void atualizarJogador(Jogador *j, GameWorld *gw, float delta) {

    if(!j->morto) {

        Animacao *animacaoAtual = getAnimacaoAtualJogador(j);
        atualizarAnimacao(animacaoAtual, delta);

        if(j->estado == JOGADOR_MORRENDO && animacaoAtual->finalizada) {
            j->morto = true;
        }

        if(j->congelado) {
            j->velAcelerar = 100;
            j->velDesacelarar = 100;
            j->velFreio = 100;

            j->contadorTempoCongelado += delta * 1000;

            if(j->contadorTempoCongelado >= 2000) {
                j->congelado = false;
                j->contadorTempoCongelado = 0;
                j->velFreio = 1100;
                j->velDesacelarar = 800;
                j->velAcelerar = 600;
            }
        }
        

        if(j->freando && animacaoAtual->finalizada) {
            j->freando = false;
        }

        if(j->estado != JOGADOR_MORRENDO) {

            verificarColisaoJogadorItem(gw);
            
            j->ret.x += j->vel.x * delta;
            resolverColisaoJogadorMapaX(gw);
        
            j->vel.y += gw->gravidade * delta;
        
            if(j->vel.y > j->velMaxQueda) {
                j->vel.y = j->velMaxQueda;
            }
        
            j->ret.y += j->vel.y * delta;
            j->noChao = false;
            resolverColisaoJogadorMapaY(gw, delta);
        
            verificarColisaoJogadorInimigo(gw);
        
            if(j->moedas >= 100) {
                j->moedas -= 100;
                j->vidas++;
            }

        }

    }

}

void desenharJogador(Jogador *j) {
    
    QuadroAnimacao *quadro = getQuadroAnimacaoAtualJogador(j);
    desenharAnimacaoJogador(j, quadro, j->congelado ? BLUE : WHITE);

}

/**
 * Agora, alem de checar a colisao do retangulo do jogador com o retangulo do obstaculo, verifica 
 * a largura e altura de do retangulo de colisao, se a altura for maior atualiza em X, caso contrario 
 * atualizza em Y
 */
static void resolverColisaoJogadorMapaX(GameWorld *gw) {

    Jogador *j = gw->mapa->jogador;
    Mapa *mapa = gw->mapa;

    ElementoMapa *el = mapa->obstaculos;

    while (el != NULL) {

        Obstaculo *obs = (Obstaculo*) el->objeto;

        if (obs->tipo == OBSTACULO_NORMAL) {

            ObstaculoNormal *o = (ObstaculoNormal*) obs->objeto;

            if (CheckCollisionRecs(j->ret, o->ret)) {

                if (j->ret.x + j->ret.width / 2 < o->ret.x + o->ret.width / 2) {
                    j->ret.x = o->ret.x - j->ret.width;
                } else {
                    j->ret.x = o->ret.x + o->ret.width;
                }
                j->vel.x = 0;

            }
        

        }else if (obs->tipo == OBSTACULO_MOVEL) {

            ObstaculoMovel *o = (ObstaculoMovel*) obs->objeto;

            if (CheckCollisionRecs(j->ret, o->ret)) {

                Rectangle retSobre = GetCollisionRec(j->ret, o->ret);

                if (retSobre.width <= retSobre.height - 2) {
                    if (j->ret.x + j->ret.width / 2 < o->ret.x + o->ret.width / 2) {
                        j->ret.x = o->ret.x - j->ret.width;
                    } else {
                        j->ret.x = o->ret.x + o->ret.width;
                    }
                    j->vel.x = 0;
                }
            }

        } else if (obs->tipo == OBSTACULO_CHEGADA) {

            ObstaculoChegada *o = (ObstaculoChegada*) obs->objeto;

            if (CheckCollisionRecs(j->ret, o->ret)) {

                gw->mapa->faseCompleta = true;

            }

        } else if (obs->tipo == OBSTACULO_GELO) {

            ObstaculoGelo *o = (ObstaculoGelo*) obs->objeto;

            if (CheckCollisionRecs(j->ret, o->ret)) {

                if (j->ret.x + j->ret.width / 2 < o->ret.x + o->ret.width / 2) {
                    j->ret.x = o->ret.x - j->ret.width;
                } else {
                    j->ret.x = o->ret.x + o->ret.width;
                }
                j->vel.x = 0;

            }
        } 
            
        el = el->proximo;
    }
}

static void resolverColisaoJogadorMapaY(GameWorld *gw, float delta) {

    Jogador *j = gw->mapa->jogador;
    Mapa *mapa = gw->mapa;
    ElementoMapa *el = mapa->obstaculos;

    while (el != NULL) {

        Obstaculo *obs = (Obstaculo*) el->objeto;

        if (obs->tipo == OBSTACULO_NORMAL) {

            ObstaculoNormal *o = (ObstaculoNormal*) obs->objeto;

            if (CheckCollisionRecs(j->ret, o->ret)) {

                if (j->ret.y + j->ret.height / 2 < o->ret.y + o->ret.height / 2) {
                    j->ret.y = o->ret.y - j->ret.height;
                    j->noChao = true;
                } else {
                    j->ret.y = o->ret.y + o->ret.height;
                }
                j->vel.y = 0;
                
            }
            
        } else if (obs->tipo == OBSTACULO_CHEGADA) {

            ObstaculoChegada *o = (ObstaculoChegada*) obs->objeto;

            if (CheckCollisionRecs(j->ret, o->ret)) {

                gw->mapa->faseCompleta = true;

            }

        } else if (obs->tipo == OBSTACULO_GELO) {

            ObstaculoGelo *o = (ObstaculoGelo*) obs->objeto;

            if (CheckCollisionRecs(j->ret, o->ret)) {

                if (j->ret.y + j->ret.height / 2 < o->ret.y + o->ret.height / 2) {
                    j->ret.y = o->ret.y - j->ret.height;
                    j->noChao = true;

                    if(!j->congelado) {
                        j->congelado = true;
                    }
                        
                } else {
                    j->ret.y = o->ret.y + o->ret.height;
                }
                j->vel.y = 0;
            
            }
            
        } else if (obs->tipo == OBSTACULO_MOVEL) {
            
            ObstaculoMovel *o = (ObstaculoMovel*) obs->objeto;
            
            if (CheckCollisionRecs(j->ret, o->ret)) {

                Rectangle retSobre = GetCollisionRec(j->ret, o->ret);
                
                // Ficar de olho pra ver se funciona, correção meio meia boca
                if (retSobre.height < retSobre.width + 2) {
                    if (j->ret.y + j->ret.height / 2 < o->ret.y + o->ret.height / 2) {
                        j->ret.y = o->ret.y - j->ret.height + (o->vel.y * delta);
                        j->noChao = true;
                        
                        if (o->retornando) {
                            j->ret.x -= o->vel.x * delta;
                        } else {
                            j->ret.x += o->vel.x * delta;
                            
                        }
                    } else {
                        j->ret.y = o->ret.y + o->ret.height;
                    }
                    j->vel.y = 0;
                }
            }
        } 
        
        el = el->proximo;
    }
}

static void verificarColisaoJogadorItem(GameWorld *gw) {

    Jogador *j = gw->mapa->jogador;
    ElementoMapa *el = gw->mapa->itens;

    while(el != NULL) {

        Item *item = (Item*) el->objeto;

        if(item->tipo == ITEM_MOEDA) {

            ItemMoeda *i = (ItemMoeda*) item->objeto;

            if(!i->ativo || i->estado == ITEM_COLETADO) {
                el = el->proximo;
                continue;
            }

            if(CheckCollisionRecs(j->ret, i->ret) && i->estado == ITEM_GIRANDO) {
                i->estado = ITEM_COLETADO;
                j->moedas += i->valor;
            }

        } else if(item->tipo == ITEM_MOEDA_ESPECIAL) {

            ItemMoedaEspecial *i = (ItemMoedaEspecial*) item->objeto;

            if(!i->ativo || i->estado == ITEM_COLETADO) {
                el = el->proximo;
                continue;
            }

            if(CheckCollisionRecs(j->ret, i->ret) && i->estado == ITEM_GIRANDO) {
                i->estado = ITEM_COLETADO;
                j->moedas += i->valor;
            }
            
        } else if(item->tipo == ITEM_VIDA) {

            ItemVida *i = (ItemVida*) item->objeto;

            if(!i->ativo || i->estado == ITEM_COLETADO) {
                el = el->proximo;
                continue;
            }

            if(CheckCollisionRecs(j->ret, i->ret) && i->estado == ITEM_GIRANDO) {
                i->estado = ITEM_COLETADO;
                j->vidas++;
            }

        }


        el = el->proximo;
    }

}

static void verificarColisaoJogadorInimigo(GameWorld *gw) {

    Jogador *j = gw->mapa->jogador;
    ElementoMapa *el = gw->mapa->inimigos;

    while (el != NULL) {

        Inimigo *inimigo = (Inimigo*) el->objeto;

        if (inimigo->tipo == INIMIGO_NORMAL) {

            InimigoNormal *i = (InimigoNormal*) inimigo->objeto;

            if(!i->estaVivo || i->estado == INIMIGO_NORMAL_MORRENDO) {
                el = el->proximo;
                continue;
            }

            if (CheckCollisionRecs(j->ret, i->ret)) {

                if (j->vel.y > 0 && j->ret.y + j->ret.height / 2 < i->ret.y + i->ret.height / 2) {
                        i->vel = (Vector2) {0};
                        i->estado = INIMIGO_NORMAL_MORRENDO;
                        j->vel.y = -j->vel.y * 0.75f;
                } else {
                    j->estado = JOGADOR_MORRENDO;
                }
                
                return;
            }

        } else if (inimigo->tipo == INIMIGO_DASH) {

            InimigoDash *i = (InimigoDash*) inimigo->objeto;

            if(!i->estaVivo || i->estado == INIMIGO_DASH_MORRENDO) {
                el = el->proximo;
                continue;
            }

            if (CheckCollisionRecs(j->ret, i->ret)) {

                if (j->vel.y > 0 && j->ret.y + j->ret.height / 2 < i->ret.y + i->ret.height / 2) {
                    i->velXInicial = 0;
                    i->estado = INIMIGO_DASH_MORRENDO;
                    j->vel.y = -j->vel.y * 0.75f;
                } else {
                    j->estado = JOGADOR_MORRENDO;
                }

                return;
            }

        } else if (inimigo->tipo == INIMIGO_VOADOR) {

            InimigoVoador *i = (InimigoVoador*) inimigo->objeto;

            if(!i->estaVivo || i->estado == INIMIGO_VOADOR_MORRENDO) {
                el = el->proximo;
                continue;
            }

            if (CheckCollisionRecs(j->ret, i->ret)) {


                if (j->vel.y > 0 && j->ret.y + j->ret.height / 2 < i->ret.y + i->ret.height / 2) {
                    i->vel = (Vector2) {0};
                    i->estado = INIMIGO_VOADOR_MORRENDO;
                    j->vel.y = -j->vel.y * 0.75f;
                } else {
                    j->estado = JOGADOR_MORRENDO;
                }
               
                return;
            }


        // A partir daqui não é possivel matar inimigos entao verificamos apenas se ha colisao
        // nao importando o estado do inimigo
        } else if (inimigo->tipo == INIMIGO_ESPINHO) {

            InimigoEspinho *i = (InimigoEspinho*) inimigo->objeto;

            if (CheckCollisionRecs(j->ret, i->ret)) {

                j->estado = JOGADOR_MORRENDO;

                return;
            }

        } else if (inimigo->tipo == INIMIGO_FANTASMA) {

            InimigoFantasma *i = (InimigoFantasma*) inimigo->objeto;

            if (CheckCollisionRecs(j->ret, i->ret)) {

                j->estado = JOGADOR_MORRENDO;
                
                return;
            }

        } else if (inimigo->tipo == INIMIGO_PEDRA) {

            InimigoPedra *i = (InimigoPedra*) inimigo->objeto;

            if (CheckCollisionRecs(j->ret, i->ret)) {

                j->estado = JOGADOR_MORRENDO;

                return;
            }


        } else if (inimigo->tipo == INIMIGO_PLANTA) {

            InimigoPlanta *i = (InimigoPlanta*) inimigo->objeto;

            if (CheckCollisionRecs(j->ret, i->ret)) {
                
                j->estado = JOGADOR_MORRENDO;

                return;
            }

            if(i->tiro != NULL && i->tiro->ativo && i->tiro->estado == TIRO_VIAJANDO) {
                if(CheckCollisionRecs(j->ret, i->tiro->ret)) {
                    j->estado = JOGADOR_MORRENDO;
                }
            }

        } else if (inimigo->tipo == INIMIGO_PLANTA_GELO) {

            InimigoPlantaGelo *i = (InimigoPlantaGelo*) inimigo->objeto;

            if (CheckCollisionRecs(j->ret, i->ret)) {

                j->estado = JOGADOR_MORRENDO;

                return;
            }

            if(i->tiro != NULL && i->tiro->ativo && i->tiro->estado == TIRO_VIAJANDO) {
                if(CheckCollisionRecs(j->ret, i->tiro->ret)) {
                    j->congelado = true;
                }
            }

        }

        el = el->proximo;
    }

}

static void desenharAnimacaoJogador(Jogador *j, QuadroAnimacao *quadro, Color tonalidade) {

    if(quadro != NULL) {

        DrawTexturePro(
            rm.texturaJogador,
            (Rectangle) {
                quadro->fonte.x,
                quadro->fonte.y,
                j->paraDireita ? quadro->fonte.width : -quadro->fonte.width,
                quadro->fonte.height
            },
            j->ret,
            (Vector2) {0},
            0.0f,
            tonalidade
        );

        if ( MOSTRAR_RETANGULO_COLISAO) {
            DrawRectangleRec(j->ret, Fade(GREEN, 0.5f));
        }

    }

}

static Animacao *getAnimacaoAtualJogador(Jogador *j) {

    return j->animacoes[j->estado];

}

static QuadroAnimacao *getQuadroAnimacaoAtualJogador(Jogador *j) {

    return getQuadroAtualAnimacao(getAnimacaoAtualJogador(j));

}