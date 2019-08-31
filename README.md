# AlgoritmoTecelao
Algoritmo que combina linhas para criar uma imagem

Vídeo demonstrativo: https://www.youtube.com/watch?v=YZtx4jNNbx8

O programa se encontra na pasta "src" com o nome de "Algoritmo Tecelao.exe"

Ao executar o algoritmo aparecerá uma tela pedindo as seguintes informações:

Nome do arquivo da imagem (ou o diretório completo, caso a imagem não esteja na mesma pasta que o executável):	
	
	Tente não usar imagens muitos grandes (1000x1000 é um ótimo tamanho)
	As imagens devem ser preferencialmente quadradas
	Os formatos suportados são .bmp e .png

Quantidade de pregos:

	Quanto mais pregos melhor, mais combinações o algoritmo pode fazer e obter um melhor resultado.

Quantidade de linhas:

	Quantas linhas você pretende traçar entre os pregos
	Aqui nós temos um dilema... 
		Poucas linhas = menos definição nas formas e mais contraste 
		Muitas linhas = mais definição nas formas e menos contraste (a imagem fica escura)
		Faça testes!! :)

Taxa de redução do pixel:
	
	Essa informação se refere a quantidade da cor preta que será tirado dos pixels da linha escolhida pelo algoritmo
	Essa taxa pode variar entre 0 até 255
	255 significa que os pixels da linha escolhida perderão 255 unidades no RGB, ou seja, ficarão completamente brancos.
	128 significa que os pixels da linha escolhida perderão 128 unidades no RGB, ou seja, ficarão cinza em vez de branco.
	0 significa que os pixels da linha escolhida não perderão nada, ou seja, o algoritmo vai ficar parado eternamente.

	Aqui nós temos outro problema

	Uma taxa de 25 gera uma imagem em tons de cinza e com um bom contraste, porém necessita de mais linhas para ter definição.
	Além disso, nós não temos garantia que o quadro final ficará da mesma forma que imagem calculada. Pois tudo vai depender
	da proporção entre o tamanho do aro e a espessura da linha!
	Escolher esse valor de forma alinhada com a realidade é a chave para ter um quadro bonito.

	Tente rodar o algoritmo pelo menos duas vezes, uma vez com essa taxa em 25 e outra com a taxa em 255... Você verá a diferença e entenderá o problema.

 
Controles:

	A/S/D/W movimentam a câmera para Esquerda/Baixo/Direita/Cima
	Q/E adiciona/retira o Zoom
	Enter inicia o algoritmo

	Se você segurar a tecla 'z' o desenho não será exibido e o algoritmo irá calcular as linhas mais rapidamente.


Lista de pregos:

	A lista de pregos será gerada na mesma pasta do executável do programa ao término do algoritmo.
	O arquivo com a lista se chama "listaDePregos.txt"

Obrigado e boa sorte! hahahah
