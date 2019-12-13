/*
	IAGO SILVA OLIVEIRA 
	DANIEL PEREIRA MONTEIRO 
	GABRIEL NOGUEIRA 
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <string.h>

#ifdef __unix__
	#include <unistd.h>
	#include <stdlib.h>
#elif defined(_WIN32) || defined(WIN32)
	#define OS_Windows
	#include <windows.h>
#endif

typedef struct sJogador {
	char nome[51];
	float tempo;
} jogador;

typedef struct sListaRecordes {
	jogador jogadores[10];
	int ultimo;
	int linha; 
} lista;

typedef struct sCampo {
	int **campo;  
	int **oculto; 
	int lose;
	int win;
} campoMinado;

void limpaTela();
void color();

void limpaTela() {
	#ifdef OS_Windows  
		system("cls"); 
	#else			   
		system("clear"); 
	#endif			   
}

void color() {
#ifdef OS_Windows
	system("color f1");   
#endif
}


void geraBomba(campoMinado *campo, int tam, int totalBombas) {
	int i, l, c, count = 1, valorLinha, valorColuna;
	printf("\tTabuleiro = %dx%d Bombas = %d\n\tPressione ENTER para inicia laucher.",tam,tam,totalBombas);
	getchar();
	(*campo).campo = (int**) calloc(tam , sizeof(int*));
	(*campo).oculto = (int**) calloc(tam , sizeof(int*));
	for(i=0; i<tam; i++) {
		(*campo).campo[i]=(int*) calloc(tam , sizeof(int));
		(*campo).oculto[i]=(int*) calloc(tam , sizeof(int));
	}
	if((*campo).campo == NULL || (*campo).oculto == NULL) {
		printf("\tError de alocação\n");
	} else {
		for(l=0; l<tam; l++) {
			for(c=0; c<tam; c++) {
				(*campo).campo[l][c] = 0;
				(*campo).oculto[l][c] = 1;
			}
		}
		(*campo).lose = 0;
		(*campo).win = 0;
		srand(time(NULL));
		while (count <= totalBombas) {
			valorLinha = rand() % (tam-1);
			valorColuna = rand() % (tam-1);
			if ( (*campo).campo[valorLinha][valorColuna] > (tam-1) ) {
				srand(time(NULL) * count);
				continue;
			}
			(*campo).campo[valorLinha][valorColuna] = 9;
			if((valorLinha - 1) >= 0 && (valorColuna - 1) >= 0) {
				(*campo).campo[valorLinha - 1][valorColuna - 1]++;	
			}
			if((valorLinha - 1) >= 0 && (valorColuna) >= 0) {
				(*campo).campo[valorLinha - 1][valorColuna]++;	
			}
			if((valorLinha - 1) >= 0 && (valorColuna + 1) >= 0) {
				(*campo).campo[valorLinha - 1][valorColuna + 1]++;	
			}
			if((valorLinha) >= 0 && (valorColuna - 1) >= 0) {
				(*campo).campo[valorLinha][valorColuna - 1]++;	
			}
			if((valorLinha - 1) >= 0 && (valorColuna + 1) >= 0) {
				(*campo).campo[valorLinha][valorColuna + 1]++;	
			}
			if((valorLinha + 1) >= 0 && (valorColuna - 1) >= 0) {
				(*campo).campo[valorLinha + 1][valorColuna - 1]++;	
			}
			if((valorLinha + 1) >= 0 && (valorColuna) >= 0) {
				(*campo).campo[valorLinha + 1][valorColuna]++;	
			}
			if((valorLinha + 1) >= 0 && (valorColuna + 1) >= 0) {
				(*campo).campo[valorLinha + 1][valorColuna + 1]++;	
			}
			count++;
		}
	}
}


void imprimeMatriz(campoMinado campo, int tam) {
	int l, c, i;
	int **matriz; 
	matriz = (int**)calloc(tam, sizeof(int*));
	for(i=0; i < tam; i++) {
		matriz[i]=(int*)calloc(tam, sizeof(int));
	}
	for (l = 0; l < tam; l++) {
		for (c = 0; c < tam; c++) {
			matriz[l][c]=campo.campo[l][c];
		}
	}
	for (l = 0; l < tam; l++) {
		for (c = 0; c < tam; c++) {
			if(matriz[l][c] < 10) {
				if (matriz[l][c] == 9) {
					printf("B  ");
				}
				else if (matriz[l][c] == 0) {
					printf("~  ");
				}
				else {
					printf("%d  ", matriz[l][c]);
				}
			}
			else { 
				printf("B  ");
			}
		}
		printf("\n\t");
	}
	for(i=0; i<tam;i++) {
		free(matriz[i]);
	}
	free(matriz);
}

void desenha(campoMinado campo, int tamTabuleiro) {
	int l, c, i;
	for(i=0; i < tamTabuleiro; i++) {
		if(i==0){
			printf("\t\t%d  ",i);
		} else {
			printf("%d  ",i);		
		}
	}
	printf("\n\n");
	for (l = 0; l < tamTabuleiro; l++) {
		printf("\t%d\t", l);
		for (c = 0; c < tamTabuleiro; c++) { 
			if (campo.oculto[l][c] == 1) {
				printf("#  ");
				continue;
			}
			if (campo.oculto[l][c] == 2 || campo.oculto[l][c] == 3) {
				printf("!  ");
				continue;
			}
			if (campo.campo[l][c] > 8) {
				printf("B  ");
				continue;
			}
			if (campo.campo[l][c] == 0) {
				printf("~  ");
			}
			else {
				printf("%d  ", campo.campo[l][c]);
			}
		}
		printf("\n");
	}
}

int venceu(campoMinado tabuleiro, int tamTabuleiro, int totalBombas) {
	int l, c, cont = 0, posicoesOcultas = 0;
	for (l = 0; l < tamTabuleiro; l++) {
		for (c = 0; c < tamTabuleiro; c++) {
			if (tabuleiro.oculto[l][c] != 0) { 
					cont++;
			}
			if(tabuleiro.oculto[l][c] != 1){
				posicoesOcultas++;
			}
		}
	}
	if (cont <= totalBombas && posicoesOcultas == tamTabuleiro*tamTabuleiro) {
		return 1;
	} 	
	return 0;
}

void imprimeCampo(campoMinado campo, int tamTabuleiro, float tempo) { 
	/*printf("\n\t");
	imprimeMatriz( campo, tamTabuleiro);*/
	// Descomente para sabe onde estao as bombas #DICA
	printf("\n\n\n");
	desenha(campo, tamTabuleiro); 
	if (campo.lose == 1) {
		limpaTela();
		printf("\n\n\t\tYou Lose\n\tTempo Jogado = %f\n\n\t",tempo);
		imprimeMatriz(campo, tamTabuleiro); 
	}
	if (campo.win == 1) {
		limpaTela();
		printf("\n\n\t\tYou Win\n\tTempo Jogado = %f\n\n\t",tempo);
		imprimeMatriz(campo, tamTabuleiro);
	}
}

void recursao(campoMinado *campo, int linha, int coluna, int tam) {
	if((*campo).oculto[linha][coluna]==0) { 
		(*campo).oculto[linha][coluna]=0;
	}
	if((*campo).oculto[linha][coluna]==2) { 
		return;
	}
	if((*campo).campo[linha][coluna]!=0) { 
		if((*campo).campo[linha][coluna]>8) {
			(*campo).oculto[linha][coluna]=0;
			(*campo).lose=1; 
		}
		(*campo).oculto[linha][coluna]=0; 
	} 
	if((*campo).campo[linha][coluna] == 0) { 
		if(linha-1 >= 0 && linha-1 < tam) {  
			if((*campo).campo[linha-1][coluna] > 0 && (*campo).campo[linha-1][coluna] < 9) { 
				(*campo).oculto[linha][coluna]=0; 
				(*campo).oculto[linha-1][coluna]=0; 
			}
			if((*campo).campo[linha-1][coluna] == 0 && (*campo).oculto[linha-1][coluna]==1) { 
				/*printf("IF CIMA");   //descomente para ver o loop recursivo
				getchar();*/
				(*campo).oculto[linha-1][coluna]=0; 
				(*campo).oculto[linha][coluna]=0; 
				recursao(campo, linha-1, coluna, tam); 
			}
		} 
		if(coluna+1 >= 0 && coluna+1 < tam) { //DIREITA
			if((*campo).campo[linha][coluna+1] > 0 && (*campo).campo[linha][coluna+1] < 9) {
				(*campo).oculto[linha][coluna]=0;
				(*campo).oculto[linha][coluna+1]=0;
			}
			if((*campo).campo[linha][coluna+1] == 0 && (*campo).oculto[linha][coluna+1]==1) {
				/*printf("IF DIREITA");  //descomente para ver o loop recursivo
				getchar();*/
				(*campo).oculto[linha][coluna]=0;
				(*campo).oculto[linha][coluna+1]=0;
				recursao(campo, linha, coluna+1, tam);
			}
		}
		if(linha+1 >= 0 && linha+1 < tam) {  //BAIXO
			if((*campo).campo[linha+1][coluna] > 0 && (*campo).campo[linha+1][coluna] < 9) {
				(*campo).oculto[linha][coluna]=0;
				(*campo).oculto[linha+1][coluna]=0;
			}
			if((*campo).campo[linha+1][coluna] == 0 && (*campo).oculto[linha+1][coluna]==1) {
				/*printf("IF BAIXO");  //descomente para ver o loop recursivo
				getchar();*/
				(*campo).oculto[linha][coluna]=0;
				(*campo).oculto[linha+1][coluna]=0;
				recursao(campo, linha+1, coluna, tam);
			}
		}
		if(coluna-1 >= 0 && coluna-1 < tam) {	//ESQUERDA
			if((*campo).campo[linha][coluna-1] > 0 && (*campo).campo[linha][coluna-1] < 9) {
				(*campo).oculto[linha][coluna]=0;
				(*campo).oculto[linha][coluna-1]=0;
			}
			if((*campo).campo[linha][coluna-1] == 0 && (*campo).oculto[linha][coluna-1]==1) {
				/*printf("IF ESQUERDA");   //descomente para ver o loop recursivo
				getchar();*/
				(*campo).oculto[linha][coluna]=0;
				(*campo).oculto[linha][coluna-1]=0;
				recursao(campo, linha, coluna-1, tam);
			}
		}
	}
}


void teste(campoMinado *campo, int tamTabuleiro) {
	int l, c;
	for (l = 0; l < tamTabuleiro; l++) {
		for (c = 0; c < tamTabuleiro; c++) {
			if ((*campo).campo[l][c] >= 9) {
				(*campo).oculto[l][c] = 2;
				continue;
			}
			(*campo).oculto[l][c] = 0;
			(*campo).win=1;
		}
	}
}

int bandeira(campoMinado *campo, int tamTabuleiro) {
	int l, c;
	do {
		printf("\n\tInforme a linha > ");
		scanf("%d", &l);
	} while (l < 0 || l > tamTabuleiro-1);
	do {
		printf("\tInforme a coluna > ");
		scanf("%d", &c);
	} while (c < 0 || c > tamTabuleiro-1);

	if ((*campo).oculto[l][c] == 0 || (*campo).oculto[l][c] == 2) {
		return 0;
	} else {
		if((*campo).campo[l][c]>8) {
			(*campo).oculto[l][c]=3;
		}else {
			(*campo).oculto[l][c]=2;	
		}
		return 1;
	}
}

lista inicializaRecordes() {
	lista recordes;
	recordes.ultimo = -1; 
	return recordes;
}

void insereFinal(lista *recordes, jogador j1){ 
	int i,j;
	if(j1.tempo == 0) {
		return;
	}
	if ((*recordes).ultimo == 9) {
		if (j1.tempo < (*recordes).jogadores[9].tempo) {
			(*recordes).jogadores[9] = j1;
			return;
		}
	}
	(*recordes).jogadores[(*recordes).ultimo + 1] = j1; 
	(*recordes).ultimo++; 
	return;
}

void organizaRecordes(lista *recordes) {
	jogador aux;
	int i, j, min;
	if((*recordes).ultimo == 0) {
		return;
	}	
	i=0;
	for(i = 0; i <= (*recordes).ultimo; i++){
		min = i;
		for (j = i + 1; j <= (*recordes).ultimo; j++)
		{
			if ((*recordes).jogadores[j].tempo < (*recordes).jogadores[min].tempo)
			{
				min = j;
			}
		}
		if (i != min) {
			aux = (*recordes).jogadores[i];
			(*recordes).jogadores[i] = (*recordes).jogadores[min];
			(*recordes).jogadores[min] = aux;
		}
	}
}

void cortaString(jogador *jogador){
	int i;
	for(i=0; i<50; i++) {
		if((*jogador).nome[i]=='\n'){
			(*jogador).nome[i]= '\0';
			break;
		}
	}
}

void verificaLista(lista* recordes, jogador j2, int tam) { 
	int i;
	for(i=0;i<10;i++) {
		if(recordes[i].linha == tam) { 
			insereFinal(&recordes[i], j2); 
			return;
		}
		if(recordes[i].ultimo == -1) { 
			recordes[i].linha= tam;
			insereFinal(&recordes[i], j2);
			return;
		}
	}
}

int main() {
	setlocale(LC_ALL, "portuguese");
	jogador j1; 
	jogador j2; 
	lista recordes[10]; 
	color();
	int l = 0, c, opcao, i=0, bombasMarcada = 0;
	int tamDoTabuleiro, totalDeBombas; 
	int escolha;
	int porcentagem;
	campoMinado campo;
	clock_t cronometro;
	clock_t cronometroPersonalizado;
	FILE* arquivo;
	recordes[0]=inicializaRecordes(); 
	recordes[1]=inicializaRecordes();
	recordes[2]=inicializaRecordes();
	recordes[3]=inicializaRecordes();
	recordes[4]=inicializaRecordes();
	recordes[5]=inicializaRecordes();
	recordes[6]=inicializaRecordes();
	recordes[7]=inicializaRecordes();
	recordes[8]=inicializaRecordes();
	recordes[9]=inicializaRecordes();
	arquivo = fopen("recordes.txt","w");
	if(arquivo == NULL) {
		printf("Error na abertura\n");
		getchar();
		exit(0);
	}
	while (i==0) {
		limpaTela();
		do {
			printf("\t\tMENU\n\t1-JOGAR\n\t2-NOVO TABULEIRO\n\t3-RECORDES\n\t4-SAIR\n\t ? ");
			scanf("%d", &opcao);
			limpaTela();
		} while (opcao < 1 || opcao > 4);
		if (opcao == 1) {
			bombasMarcada = 0;
			printf("\tBem-vindo ao tabuleiro padrãoo 9x9 10 Bombas\n\t\t\tBOM JOGO\n\n\tSeu nome: ");
			setbuf(stdin, NULL);
			fgets(j1.nome, sizeof(j1.nome), stdin);
			cortaString(&j1);
			cronometro = clock();
			tamDoTabuleiro = 9; 
			geraBomba(&campo, tamDoTabuleiro, 10); 
			while (campo.lose == 0 && campo.win == 0) {
				limpaTela();
				imprimeCampo(campo, 9, j1.tempo); 
				printf("\n\n");
				printf("\tBandeiras Marcadas %d\n",bombasMarcada);
				do
				{
					printf("\n\t1 - Realizar jogada\n\t-1 - Marcar bomba\n\t100-TESTE P/ GANHAR ? ");
					scanf("%d", &escolha);
				} while ((escolha != 1) && (escolha != -1) && (escolha != 100)); 
				if (escolha == 100) {
					teste(&campo, 9);
				}
				if (escolha == 1) {
					limpaTela();
					imprimeCampo(campo, 9, j1.tempo);
					do {
						printf("\n\tInforme a linha > ");
						scanf("%d", &l);
					} while(l < 0 || l > 9-1);
					do {
						printf("\n\tInforme a coluna > ");
						scanf("%d", &c);
					} while(c < 0 || c > 9-1);
					recursao(&campo, l, c, 9); 
				}
				else if (escolha == -1) {
					limpaTela();
					imprimeCampo(campo, 9,j1.tempo);
					bombasMarcada += bandeira(&campo,9);
				}
				campo.win = venceu(campo, 9, 10);
			}
			if (campo.win == 1) {
				cronometro = clock() - cronometro;
				j1.tempo = (float)cronometro / CLOCKS_PER_SEC; 
				recordes[0].linha=9;
				insereFinal(&recordes[0], j1); 
			}
			if(campo.lose == 1) {
				cronometro = clock() - cronometro;
				j1.tempo = (float)cronometro / CLOCKS_PER_SEC;
			}
			limpaTela();
			imprimeCampo(campo, 9, j1.tempo);
			printf("\n\n");
			setbuf(stdin, NULL);
			getchar();
		}
		else if (opcao == 2) {
			bombasMarcada = 0;
			printf("\tBem-vindo ao modo tabuleiro personalizado \n\n\t\t\tREGRAS\n\t.O tamanho minimo e de 5x5\n\t.Deve conter no minimo 4 bombas ï¿½ no maximo\n\t 75%c do tamanho do tabuleiro\n\n\tPersonalize seu tabuleiro\n\n",37);
			do {
				printf("\tTotal de linhas > ");
				scanf("%d",&tamDoTabuleiro);
			} while(tamDoTabuleiro <=0 || tamDoTabuleiro <5);
			do {
				printf("\tTotal de bombas > ");
				scanf("%d",&totalDeBombas);
				porcentagem = (tamDoTabuleiro * tamDoTabuleiro) * 0.75;
			} while(totalDeBombas <= 0 || totalDeBombas <4 ||totalDeBombas > porcentagem);
			printf("\n\tSeu nome: ");
			setbuf(stdin, NULL);
			fgets(j2.nome, sizeof(j2.nome), stdin);
			cortaString(&j2);
			setbuf(stdin, NULL);
			cronometroPersonalizado = clock();
			geraBomba(&campo, tamDoTabuleiro, totalDeBombas); 
			while (campo.lose == 0 && campo.win == 0) {
				limpaTela();
				imprimeCampo(campo, tamDoTabuleiro, j2.tempo);
				printf("\n\n");
				printf("\tBandeiras Marcadas %d\n",bombasMarcada);
				do {
					printf("\n\t1 - Realizar jogada\n\t-1 - Marcar bomba\n\t100-TESTE P/ GANHAR ? ");
					scanf("%d", &escolha);
				} while ((escolha != 1) && (escolha != -1) && (escolha != 100));
				if (escolha == 100) {
					teste(&campo, tamDoTabuleiro);
				}
				if (escolha == 1) {
					limpaTela();
					imprimeCampo(campo, tamDoTabuleiro, j2.tempo);
					do {
						printf("\n\tInforme a linha > ");
						scanf("%d", &l);
					} while(l < 0 || l > tamDoTabuleiro-1);
					do {
						printf("\n\tInforme a coluna > ");
						scanf("%d", &c);
					} while(c < 0 || c > tamDoTabuleiro-1);
					recursao(&campo, l, c, tamDoTabuleiro);
				}
				else if (escolha == -1) {
					limpaTela();
					imprimeCampo(campo, tamDoTabuleiro, j2.tempo);
					bombasMarcada += bandeira(&campo,tamDoTabuleiro);
				}
				campo.win = venceu(campo, tamDoTabuleiro, totalDeBombas);
			}
			if (campo.win == 1) {
				cronometroPersonalizado = clock() - cronometroPersonalizado;
				j2.tempo = (float)cronometroPersonalizado / CLOCKS_PER_SEC;
				verificaLista(recordes, j2, tamDoTabuleiro);
			}
			if(campo.lose==1) {
				cronometroPersonalizado = clock() - cronometroPersonalizado;
				j2.tempo = (float)cronometroPersonalizado / CLOCKS_PER_SEC;
			}
			limpaTela();
			imprimeCampo(campo, tamDoTabuleiro, j2.tempo);
			printf("\n\n");
			setbuf(stdin, NULL);
			getchar();
		}
		else if (opcao == 3) {
			int i,j,l,nome;
			float menor=recordes[0].jogadores[0].tempo;
			for(i=0; i<10; i++) {
				organizaRecordes(&recordes[i]);
			}
			for(i=0; i<10; i++) {
				if(recordes[i].jogadores[0].tempo !=0) {
					if(menor > recordes[i].jogadores[0].tempo) {
						menor = recordes[i].jogadores[0].tempo;
						nome=i;
					}
				}
			}
			limpaTela();
			printf("\t Melhor Tempo geral = %s: %f Tabuleiro %dx%d\n",recordes[nome].jogadores[0].nome, menor,recordes[nome].linha, recordes[nome].linha);
			arquivo=fopen("recordes.txt","w");
			for(l=0;l<10; l++) {
				if(recordes[l].ultimo!=-1) {
					printf("\n\t\t RESULTADOS TABULEIRO %dx%d\n\n",recordes[l].linha,recordes[l].linha);	
				}
				
				for(i = 0; i <= recordes[l].ultimo; i++) {
					if(recordes[l].jogadores[i].tempo==0) {
						continue;
					}
					printf("\t%d - %s: %f \n", i+1, recordes[l].jogadores[i].nome, recordes[l].jogadores[i].tempo);		
				}	
			}
			printf("\n\n");
			getchar();
			fprintf(arquivo,"\t Menor Tempo geral = %s: %f Tabuleiro %dx%d\n",recordes[nome].jogadores[0].nome, menor, recordes[nome].linha, recordes[nome].linha);
			for(l=0;l<10; l++) {
				if(recordes[l].ultimo!=-1) {
					fprintf(arquivo,"\n\t\t TOP 5 TABULEIRO %dx%d\n\n",recordes[l].linha,recordes[l].linha);	
				}	
				for(i = 0; i <= 4; i++){ 
					if(recordes[l].jogadores[i].tempo==0) {
						continue;
					}
					fprintf(arquivo,"\t%d - %s: %f \n", i+1, recordes[l].jogadores[i].nome, recordes[l].jogadores[i].tempo);		
				}
			}
			getchar();
		} 
		else {
			printf("\tJa vai :(\n\tPressione ENTER para finalizar o laucher.");
			setbuf(stdin, NULL);
			getchar();
			i = 1;
		}
	}
	for(i=0; i<tamDoTabuleiro;i++) {
		free(campo.campo[i]);
		free(campo.oculto[i]);
	}
	free(campo.campo);
	free(campo.oculto);
	return 0;
}
