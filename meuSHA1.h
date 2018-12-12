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

#ifndef _SHA1_H_

	#define _SHA1_H_

	#include <stdint.h>
	#include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <time.h>
    #include <math.h>

	#ifndef _meuSHA1_enum_
		#define _meuSHA1_enum_
		enum
		{
			Sucesso = 0,
			Vazio,            /* Paramentro Ponteiro Nulo */
			Erro_input,    /* Dado de input muito longo */
			shaStateError       /* Input depois do Resultado */
		};
	#endif

	#define TamanhoHash 20

	/* Struct que define o contexto do SHA1 */
	typedef struct ContextoSHA
	{
		uint32_t Hash_aux[TamanhoHash/4]; /* Message Digest  */

		uint32_t LO_comp;            /* Comprimento da mensagem em bits - 32 bits menos significativos  */
		uint32_t HI_comp;           /* Comprimento da mensagem em bits - 32 bits mais significativos   */
		int_least16_t Index_msg;
		uint8_t Bloco_msg[64];      /* Blocos de mensagem de 512 bits */

		int Computado;               /* Digest foi computado?     */
		int Corrompido;             /* Digest está corrompido? */
	} ContextoSHA;


	/* Protótipos  */
	int InitSHA(  ContextoSHA *);
	int InputSHA(  ContextoSHA *,
					const uint8_t *,
					unsigned int);
	int ResultadoSHA( ContextoSHA *,
					uint8_t Message_Digest[TamanhoHash]);


#endif
