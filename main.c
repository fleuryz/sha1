/*
Universidade de Brasília
Implementação do algoritmo do SHA1
Grupo 3
Bárbara Varanda Rangel
Gabriel Martins Miranda
Jadiel Teófilo Amorim de Oliveira
Marcos Cordeiro Fleury
Túlio de Caravlho Matia
*/

#include "meuSHA1.h"


uint64_t tamanho_arquivo = 0;

uint8_t* lerArquivo(uint8_t *arquivo_entrada)
{
	/*Mensagem do arquivo*/
	uint8_t *mensagem;
	FILE *entrada_fp = NULL;

	entrada_fp = fopen(arquivo_entrada, "rb");
	if (entrada_fp == NULL) {
		printf("arquivo nao encontrado.\n");
		exit(1);
	}

    /*Tamanho do arquivo*/
	fseek(entrada_fp, 0, SEEK_END);
	tamanho_arquivo = ftell(entrada_fp);
	fseek(entrada_fp,0,SEEK_SET);
	printf("tamanho_arquivo: %lu\n", tamanho_arquivo);

	mensagem = (uint8_t*) malloc(tamanho_arquivo);

	fread(mensagem, tamanho_arquivo, 1 , entrada_fp);
	fclose(entrada_fp);

	return mensagem;
}


int main(int argc, char* argv[])
{
    ContextoSHA sha;
    int i, j, err;
    uint8_t Message_Digest[20];
    float t_inicio = 0, t_fim = 0, t_gasto;
	char *arq_in   = NULL;
	char *arq_out     = NULL;
	uint8_t* mensagem		= NULL;

    t_inicio = (float)clock()/CLOCKS_PER_SEC;

    if(argc >= 3)
	{
		arq_in 	= argv[1];
		arq_out		= argv[2];

	}
	else if(argc == 2)
	{
		arq_in 	= argv[1];
		arq_out = (char*) malloc(11*sizeof(char));

		printf("(max:10) Arquivo de saida: \n->");
		scanf("%s", arq_out);
		getchar();
	}
	else
	{
		arq_in  	= (char*) malloc(11*sizeof(char));
		arq_out  	    = (char*) malloc(11*sizeof(char));

		printf("(max:10) Arquivo de entrada: \n->");
		scanf("%s", arq_in);
		getchar();

		printf("(max:10) Arquivo de saida: \n->");
		scanf("%s", arq_out);
		getchar();
	}

	mensagem = lerArquivo(arq_in);

	 InitSHA(&sha);


    InputSHA(&sha, (const unsigned char *) mensagem, strlen(mensagem));
    ResultadoSHA(&sha, Message_Digest);


        t_fim = (float)clock()/CLOCKS_PER_SEC;
        t_gasto = t_fim - t_inicio;
        printf("O tempo gasto foi %fs\n", t_gasto);

    FILE *saida_fp = fopen(arq_out, "w");

 	for(i = 0; i < 20 ; ++i)
            {
                printf("%02X ", Message_Digest[i]);
                fprintf(saida_fp, "%02X ", Message_Digest[i]);
            }

	free(mensagem);
}


