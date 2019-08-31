#include "PIG.h"

#define MAX_PREGOS 1000

typedef struct prego
{
    int X, Y;

}   Prego;

    Prego           Pregos[MAX_PREGOS];
    char IMAGEM[2000];
    int ALT_IMG, LARG_IMG;
    int QTD_ITERACAO, TAXA_LINHA, QTD_PREGOS;
    int janelaSemFoco = 1;
    int linhas[MAX_PREGOS][MAX_PREGOS];
    int Modelo, Quadro;
    int FontePreta, FonteBranca, FonteVermelha;
    int contador, proximoPonto, primeiroPonto, primeiroPontoDesenho, iniciar;
    int SpritePrego;

    SDL_Point* pontos[256];
    int quantidadePontos[256];


PIG_Cor getPixelColor(int objeto, int coluna, int linha, int largura, int altura)
{
    PIG_Cor cor;

    cor.a = 255;
    cor.r = CGerenciadorObjetos::objetos[objeto]->pixels[altura-1-linha][coluna].r;
    cor.g = CGerenciadorObjetos::objetos[objeto]->pixels[altura-1-linha][coluna].g;
    cor.b = CGerenciadorObjetos::objetos[objeto]->pixels[altura-1-linha][coluna].b;

    return cor;
}

void setPixelColor(int objeto, int coluna, int linha, int largura, int altura, PIG_Cor cor)
{
    CGerenciadorObjetos::objetos[objeto]->pixels[altura-1-linha][coluna].a = cor.a;
    CGerenciadorObjetos::objetos[objeto]->pixels[altura-1-linha][coluna].r = cor.r;
    CGerenciadorObjetos::objetos[objeto]->pixels[altura-1-linha][coluna].g = cor.g;
    CGerenciadorObjetos::objetos[objeto]->pixels[altura-1-linha][coluna].b = cor.b;
}

void somarColor(int objeto, int coluna, int linha, int largura, int altura, int coef)
{
    int r = CGerenciadorObjetos::objetos[objeto]->pixels[altura-1-linha][coluna].r;
    int g = CGerenciadorObjetos::objetos[objeto]->pixels[altura-1-linha][coluna].g;
    int b = CGerenciadorObjetos::objetos[objeto]->pixels[altura-1-linha][coluna].b;

    CGerenciadorObjetos::objetos[objeto]->pixels[altura-1-linha][coluna].a = 255;

    if(r + (coef*TAXA_LINHA) < 0)
    {
        CGerenciadorObjetos::objetos[objeto]->pixels[altura-1-linha][coluna].r = 0;
    }
    else
    {
        if(r + (coef*TAXA_LINHA) > 255)
        {
            CGerenciadorObjetos::objetos[objeto]->pixels[altura-1-linha][coluna].r = 255;
        }
        else
        {
            CGerenciadorObjetos::objetos[objeto]->pixels[altura-1-linha][coluna].r = r + (coef*TAXA_LINHA);
        }
    }


    if(g + (coef*TAXA_LINHA) < 0)
    {
        CGerenciadorObjetos::objetos[objeto]->pixels[altura-1-linha][coluna].g = 0;
    }
    else
    {
        if(g + (coef*TAXA_LINHA) > 255)
        {
            CGerenciadorObjetos::objetos[objeto]->pixels[altura-1-linha][coluna].g = 255;
        }
        else
        {
            CGerenciadorObjetos::objetos[objeto]->pixels[altura-1-linha][coluna].g = g + (coef*TAXA_LINHA);
        }
    }

    if(b + (coef*TAXA_LINHA) < 0)
    {
        CGerenciadorObjetos::objetos[objeto]->pixels[altura-1-linha][coluna].b = 0;
    }
    else
    {
        if(b + (coef*TAXA_LINHA) > 255)
        {
            CGerenciadorObjetos::objetos[objeto]->pixels[altura-1-linha][coluna].b = 255;
        }
        else
        {
            CGerenciadorObjetos::objetos[objeto]->pixels[altura-1-linha][coluna].b = b + (coef*TAXA_LINHA);
        }
    }
}

void DesenharPregos(double InicialX)
{
    double X,Y;

    for(int i=0; i<QTD_PREGOS; i++)
    {
        X = InicialX + Pregos[i].X;
        Y = Pregos[i].Y;

        X = ((X+CameraPosX)+((X+CameraPosX)-(LARG_TELA/2.0))*CameraZoom);
        Y = ((Y+CameraPosY)+((Y+CameraPosY)-(ALT_TELA/2.0))*CameraZoom);

        DesenharSprite(SpritePrego, X, Y, 10*(CameraZoom+1), 10*(CameraZoom+1), 0);
    }
}

void DesenharImagem(int Imagem, double InicialX)
{
    PIG_Cor cor;
    int Largura = PegarLargura(Imagem);
    int Altura = PegarAltura(Imagem);

    double X, Y;

    for(int i=0; i<256; i++)
    {
        quantidadePontos[i] = 0;
    }

    for(int i=0; i<Largura; i++)
    {
        for(int j=0; j<Altura; j++)
        {
            cor = getPixelColor(Imagem, i, j, Largura, Altura);

            int indice = cor.r;

            X = InicialX + i;
            Y = j;

            X = ((X+CameraPosX)+((X+CameraPosX)-(LARG_TELA/2.0))*CameraZoom);
            Y = ((Y+CameraPosY)+((Y+CameraPosY)-(ALT_TELA/2.0))*CameraZoom);

            pontos[indice][quantidadePontos[indice]].x = X;
            pontos[indice][quantidadePontos[indice]].y = ALT_TELA - Y;

            quantidadePontos[indice]++;
        }
    }

    for(int i=0; i<256; i++)
    {
        cor.r = i;
        cor.g = i;
        cor.b = i;
        if(i == 0)
        {
            cor.r = 0;
            cor.g = 0;
            cor.b = 0;
        }
        DesenharPontos(pontos[i], quantidadePontos[i], cor);
    }


}

void DesenharLinhaImagem(int Imagem, double X1, double Y1, double X2, double Y2)
{
    double dx = X2-X1;
    double dy = Y2-Y1;
    double Distancia = DistanciaEntrePontos(X1,Y1,X2,Y2);

    int Largura = PegarLargura(Imagem);
    int Altura = PegarAltura(Imagem);

    for(double i=0; i<=Distancia; i++)
    {
        double X = round(X1+(i*(dx/Distancia)));
        double Y = round((Y1+(i*(dy/Distancia))));
        setPixelColor(Imagem, X, Y, Largura, Altura, BRANCO);
    }
}

void MudarLinhaImagem(int Imagem, double X1, double Y1, double X2, double Y2, int coef)
{
    double dx = X2-X1;
    double dy = Y2-Y1;
    double Distancia = DistanciaEntrePontos(X1,Y1,X2,Y2);

    int Largura = PegarLargura(Imagem);
    int Altura = PegarAltura(Imagem);

    for(double i=0; i<=Distancia; i++)
    {
        double X = round(X1+(i*(dx/Distancia)));
        double Y = round((Y1+(i*(dy/Distancia))));
        somarColor(Imagem, X, Y, Largura, Altura, coef);
    }
}

void InicializarPregos()
{
    double Incremento = (2*ALT_IMG + 2*LARG_IMG)/(double)QTD_PREGOS;
    double Angulo = 0;

    int i;
    int dx = LARG_TELA/(QTD_PREGOS/4);
    int dy = 0;

//    Pregos[0].X = 0;
//    Pregos[0].Y = 0;
//
//    //printf("%d %d\n",dx,dy);
//
//    for(int i=1; i<QTD_PREGOS; i++)
//    {
//        Pregos[i].X = Pregos[i-1].X + dx;
//        Pregos[i].Y = Pregos[i-1].Y + dy;
//
//        printf("%d %d\n",Pregos[i].X, Pregos[i].Y);
//
//        if(i == QTD_PREGOS/4)
//        {
//            dy = dx;
//            dx = 0;
//        }
//        else
//            if(i == QTD_PREGOS/2)
//            {
//                dx = -dy;
//                dy = 0;
//            }
//            else
//                if(i == (3*QTD_PREGOS)/4)
//                {
//                    dy = dx;
//                    dx = 0;
//                }
//    }


    for(i=0; i<QTD_PREGOS; i++)
    {
//        Pregos[i].X = i*Incremento;
//        Pregos[i].Y = 0;
//        if(i*Incremento > LARG_IMG)
//        {
//            Pregos[i].X = Pregos[i].X - LARG_IMG;
//            Pregos[i].Y = Pregos[i].X;
//            Pregos[i].X = LARG_IMG;
//        }

        Pregos[i].X = LARG_IMG/2.0 +((LARG_IMG-2)/2.0*cos(Angulo*DEGTORAD));
        Pregos[i].Y = ALT_IMG/2.0 + ((ALT_IMG-2)/2.0*sin(Angulo*DEGTORAD));

        Angulo = Angulo + (360.0/(QTD_PREGOS));
    }
}

double somarLinha(int Imagem, int Inicio, int Final)
{
    double dx = Pregos[Final].X - Pregos[Inicio].X;
    double dy = Pregos[Final].Y - Pregos[Inicio].Y;
    double Distancia = DistanciaEntrePontos(Pregos[Inicio].X,
                                            Pregos[Inicio].Y,
                                            Pregos[Final].X,
                                            Pregos[Final].Y);

    int Largura = PegarLargura(Imagem);
    int Altura = PegarAltura(Imagem);

    double escuridao = 0;

    for(double i=0; i<=Distancia; i++)
    {
        int X = round(Pregos[Inicio].X+(i*(dx/Distancia)));
        int Y = round((Pregos[Inicio].Y+(i*(dy/Distancia))));
        PIG_Cor cor = getPixelColor(Imagem, X, Y, Largura, Altura);

        escuridao = escuridao + (255-cor.r) + (255-cor.g) + (255-cor.b);
    }

    return (escuridao/(Distancia*255.0*3.0));
    //return (escuridao);
}

int procurarPonto(int pontoInicio)
{
    double maisEscura = 0;
    int pontoMaisEscuro = 0;

    for(int i=0; i<QTD_PREGOS; i++)
    {
        if(i != pontoInicio)
        {
            double valorEscuridao = somarLinha(Modelo, pontoInicio, i);
            if(valorEscuridao > maisEscura)
            {
                maisEscura = valorEscuridao;
                pontoMaisEscuro = i;
            }
        }
    }
    return pontoMaisEscuro;
}

void movimentarCamera()
{
    double A = 5.0/(CameraZoom+1);
    if(PIG_meuTeclado[TECLA_w] == 1)
    {
        CameraPosY = CameraPosY - A;
    }
    if(PIG_meuTeclado[TECLA_s] == 1)
    {
        CameraPosY = CameraPosY + A;
    }
    if(PIG_meuTeclado[TECLA_a] == 1)
    {
        CameraPosX = CameraPosX + A;
    }
    if(PIG_meuTeclado[TECLA_d] == 1)
    {
        CameraPosX = CameraPosX - A;
    }

    double B = 0.05;
    double C = (B/(3.0/(CameraZoom+1)));
    if(PIG_meuTeclado[TECLA_q] == 1)
    {
        if(CameraZoom >= 0)
        {
            CameraZoom = CameraZoom + B;
        }
        else
        {
            CameraZoom = CameraZoom + C;
        }

        if(CameraZoom >= 50)
        {
            CameraZoom = 50;
        }
    }
    if(PIG_meuTeclado[TECLA_e] == 1)
    {
        if(CameraZoom >= 0)
        {
            CameraZoom = CameraZoom - B;
        }
        else
        {
            CameraZoom = CameraZoom - C;
        }

        if(CameraZoom <= -0.9999)
        {
            CameraZoom = -0.9999;
        }
    }
}


void converterXY(double* X, double* Y)
{
    *X = ((*X+CameraPosX)+((*X+CameraPosX)-(LARG_TELA/2.0))*CameraZoom);
    *Y = ((*Y+CameraPosY)+((*Y+CameraPosY)-(ALT_TELA/2.0))*CameraZoom);
}

void desenhar()
{
    double X,Y;
    double X1,Y1;
    char String[1000];

    IniciarDesenho();

    if(PIG_meuTeclado[TECLA_z] == 0)
    {
        DesenharImagem(Modelo, 0);
        DesenharImagem(Quadro, LARG_IMG+10);
        DesenharPregos(0);
        DesenharPregos(LARG_IMG+10);
    }

    if(contador<QTD_ITERACAO)
    {
        sprintf(String,"Fios traçados: %d de %d", contador, QTD_ITERACAO);
    }
    else
    {
        sprintf(String,"Lista de pregos geradas com sucesso!", contador, QTD_ITERACAO);
    }


    X = 700;
    Y = 800;
    converterXY(&X,&Y);
    EscreverCentralizada(String, X, Y, FonteBranca);

    /// ----

    X = Pregos[primeiroPontoDesenho].X;
    Y = Pregos[primeiroPontoDesenho].Y;
    converterXY(&X,&Y);
    X1 = Pregos[proximoPonto].X;
    Y1 = Pregos[proximoPonto].Y;
    converterXY(&X1,&Y1);

    DesenharLinhaSimples(X, Y, X1, Y1, VERMELHO);

    /// ----

    X = Pregos[primeiroPontoDesenho].X + LARG_IMG+10;
    Y = Pregos[primeiroPontoDesenho].Y;
    converterXY(&X,&Y);
    X1 = Pregos[proximoPonto].X + LARG_IMG+10;
    Y1 = Pregos[proximoPonto].Y;
    converterXY(&X1,&Y1);

    DesenharLinhaSimples(X, Y, X1, Y1, VERMELHO);


    if(janelaSemFoco == 1)
    {
        sprintf(String,"Clique na janela para continuar!");
        EscreverCentralizada(String, 600, 300, FonteVermelha);
    }
    EncerrarDesenho();
}

int main(int argc, char* args[])
{
    printf("Antes de digitar o nome da imagem certifique-se que ela esta na mesma pasta deste executavel...\n");
    printf("Digite o nome da sua imagem (exemplo: teste.bmp): ");
    scanf("%s",IMAGEM);

    FILE* ftest = fopen(IMAGEM,"rb");
    if(ftest == NULL)
    {
        printf("Imagem nao encontrada (nome errado ou nao se encontra na pasta)\n");
        system("pause");
        exit(1);
    }
    else
    {
        fclose(ftest);
    }


    printf("Digite a quantidade de pregos (maximo 1000): ");
    scanf("%d",&QTD_PREGOS);
    printf("Digite a quantidade de linhas: ");
    scanf("%d",&QTD_ITERACAO);
    printf("Digite a taxa de reducao do pixel (255 retira o pixel todo, 0 nao retira nada): ");
    scanf("%d",&TAXA_LINHA);



    CriarJanela("Algoritmo Tecelao", 0);

    int Timer = CriarTimer();

    FontePreta = CriarFonteNormal("..\\fontes\\arial.ttf", 15, PRETO, 0, PRETO);
    FonteBranca = CriarFonteNormal("..\\fontes\\arial.ttf", 15, BRANCO, 0, BRANCO);
    FonteVermelha = CriarFonteNormal("..\\fontes\\arial.ttf", 35, VERMELHO, 0, BRANCO);
    SpritePrego = CriarSprite("prego.png");

    Modelo = criarObjetoTamanhoOriginal(IMAGEM, 0, 0, 0);

    ALT_IMG = PegarAltura(Modelo);
    LARG_IMG = PegarLargura(Modelo);

    Quadro = criarObjeto("branco.bmp", 0, 0, ALT_IMG, LARG_IMG, 0);

    FILE* f = fopen("listaDePregos.txt","w");

    InicializarPregos();

    for(int j=0; j<QTD_PREGOS; j++)
    {
        for(int k=0; k<QTD_PREGOS; k++)
        {
            linhas[j][k] = -1;
        }
    }

    for(int i=0; i<256; i++)
    {
        pontos[i] = (SDL_Point*)malloc(sizeof(SDL_Point)*LARG_IMG*ALT_IMG);
        if(pontos[i] == NULL)
        {
            printf("\nImagem muito grande, nao houve espaco suficiente para alocar!\nReduza a imagem e tente novamente.\n");
            system("pause");
            exit(1);
        }
    }

    primeiroPonto = 0;
    contador = 0;
    iniciar = 0;

    while(PIG_JogoRodando == 1)
    {
        AtualizarJanela();

        if(PIG_Tecla == TECLA_ENTER)
        {
            iniciar = 1;
        }

        if(iniciar == 1)
        {
            if(contador < QTD_ITERACAO)
            {
                proximoPonto = procurarPonto(primeiroPonto);

                linhas[primeiroPonto][proximoPonto] = 1;
                fprintf(f,"%d -> %d\n", primeiroPonto, proximoPonto);

                MudarLinhaImagem(Modelo,
                                 Pregos[primeiroPonto].X, Pregos[primeiroPonto].Y,
                                 Pregos[proximoPonto].X, Pregos[proximoPonto].Y,
                                 1);

                MudarLinhaImagem(Quadro,
                                 Pregos[primeiroPonto].X, Pregos[primeiroPonto].Y,
                                 Pregos[proximoPonto].X, Pregos[proximoPonto].Y,
                                 -1);

                primeiroPontoDesenho = primeiroPonto;
                primeiroPonto = (proximoPonto)%QTD_PREGOS;

                contador++;
            }
            else
            {
                fclose(f);
            }
        }

        if(PIG_Botao == 1)
        {
            janelaSemFoco = 0;
        }

        if(TempoDecorrido(Timer) >= 0.01)
        {
            movimentarCamera();

            desenhar();

            ReiniciarTimer(Timer);
        }
    }

    FinalizarJanela();

    return 0;
}
