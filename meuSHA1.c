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

#define ShiftCircular(bits,word) \
                (((word) << (bits)) | ((word) >> (32-(bits))))

/* Funções locais */

void Preenchimento(ContextoSHA *);
void Processamento(ContextoSHA *);

/*Função que inicializa os parametros para poder computar uma nova Hash*/
int InitSHA(ContextoSHA *contexto)
{
    if (!contexto)
    {
        return Vazio;
    }

    contexto->LO_comp             = 0;
    contexto->HI_comp            = 0;
    contexto->Index_msg    = 0;

    contexto->Hash_aux[0]   = 0x67452301;
    contexto->Hash_aux[1]   = 0xEFCDAB89;
    contexto->Hash_aux[2]   = 0x98BADCFE;
    contexto->Hash_aux[3]   = 0x10325476;
    contexto->Hash_aux[4]   = 0xC3D2E1F0;

    contexto->Computado   = 0;
    contexto->Corrompido  = 0;

    return Sucesso;
}

/*Função que computa o Digest da mensagem  */
int ResultadoSHA( ContextoSHA *contexto,
                uint8_t Message_Digest[TamanhoHash])
{
    int i;

    if (!contexto || !Message_Digest)
        return Vazio;

    if (contexto->Corrompido)
        return contexto->Corrompido;

    if (!contexto->Computado)
    {
        Preenchimento(contexto);
        for(i=0; i<64; ++i)
        {
            /* Limpar mensagem */
            contexto->Bloco_msg[i] = 0;
        }
        contexto->LO_comp = 0;    /* and clear length */
        contexto->HI_comp = 0;
        contexto->Computado = 1;
    }

    for(i = 0; i < TamanhoHash; ++i)
    {
        Message_Digest[i] = contexto->Hash_aux[i>>2]
                            >> 8 * ( 3 - ( i & 0x03 ) );
    }

    return Sucesso;
}

int InputSHA( ContextoSHA *contexto, const uint8_t  *array_msg, unsigned comp)
{
    if (!comp)
        return Sucesso;

    if (!contexto || !array_msg)
        return Vazio;

    if (contexto->Computado)
    {
        contexto->Corrompido = shaStateError;
        return shaStateError;
    }

    if (contexto->Corrompido)
         return contexto->Corrompido;


    while(comp-- && !contexto->Corrompido)
    {
    contexto->Bloco_msg[contexto->Index_msg++] =
                    (*array_msg & 0xFF);

    contexto->LO_comp += 8;
    if (contexto->LO_comp == 0)
    {
        contexto->HI_comp++;
        if (contexto->HI_comp == 0)
        {
            /* Mensagem muito longa */
            contexto->Corrompido = 1;
        }
    }

    if (contexto->Index_msg == 64)
        Processamento(contexto);

    array_msg++;
    }

    return Sucesso;
}


void Processamento(ContextoSHA *contexto)
{
	/* Constantes definidas pelo SHA-1   */
    const uint32_t K[] =    {0x5A827999,0x6ED9EBA1,0x8F1BBCDC,0xCA62C1D6};

    int           t;                 /* Contador                */
    uint32_t      temp;              /* Valor temporario da word        */
    uint32_t      W[80];             /* Sequencia de words               */
    uint32_t      A, B, C, D, E;     /* Buffers                */

    /* Initializa primeiras 16 words do array W  */
    for(t = 0; t < 16; t++)
    {
        W[t] = contexto->Bloco_msg[t * 4] << 24;
        W[t] |= contexto->Bloco_msg[t * 4 + 1] << 16;
        W[t] |= contexto->Bloco_msg[t * 4 + 2] << 8;
        W[t] |= contexto->Bloco_msg[t * 4 + 3];
    }

    for(t = 16; t < 80; t++)
    {
       W[t] = ShiftCircular(1,W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16]);
    }

    A = contexto->Hash_aux[0];
    B = contexto->Hash_aux[1];
    C = contexto->Hash_aux[2];
    D = contexto->Hash_aux[3];
    E = contexto->Hash_aux[4];

    for(t = 0; t < 20; t++)
    {
        temp =  ShiftCircular(5,A) +
                ((B & C) | ((~B) & D)) + E + W[t] + K[0];
        E = D;
        D = C;
        C = ShiftCircular(30,B);
        B = A;
        A = temp;
    }

    for(t = 20; t < 40; t++)
    {
        temp = ShiftCircular(5,A) + (B ^ C ^ D) + E + W[t] + K[1];
        E = D;
        D = C;
        C = ShiftCircular(30,B);
        B = A;
        A = temp;
    }

    for(t = 40; t < 60; t++)
    {
        temp = ShiftCircular(5,A) +
               ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
        E = D;
        D = C;
        C = ShiftCircular(30,B);
        B = A;
        A = temp;
    }

    for(t = 60; t < 80; t++)
    {
        temp = ShiftCircular(5,A) + (B ^ C ^ D) + E + W[t] + K[3];
        E = D;
        D = C;
        C = ShiftCircular(30,B);
        B = A;
        A = temp;
    }

    contexto->Hash_aux[0] += A;
    contexto->Hash_aux[1] += B;
    contexto->Hash_aux[2] += C;
    contexto->Hash_aux[3] += D;
    contexto->Hash_aux[4] += E;

    contexto->Index_msg = 0;
}

void Preenchimento(ContextoSHA *contexto)
{
    /* Checa para garantir que o bloco é de tamanho suficiente para ter todos os blocos. Se não for processa esse bloco e continua no próximo */
    if (contexto->Index_msg > 55)
    {
        contexto->Bloco_msg[contexto->Index_msg++] = 0x80;
        while(contexto->Index_msg < 64)
            contexto->Bloco_msg[contexto->Index_msg++] = 0;

        Processamento(contexto);

        while(contexto->Index_msg < 56)
            contexto->Bloco_msg[contexto->Index_msg++] = 0;
    }

    else
    {
        contexto->Bloco_msg[contexto->Index_msg++] = 0x80;
        while(contexto->Index_msg < 56)
            contexto->Bloco_msg[contexto->Index_msg++] = 0;
    }

    /* Salva tamanho da mensagem nos ultimos 64 bits */
    contexto->Bloco_msg[56] = contexto->HI_comp >> 24;
    contexto->Bloco_msg[57] = contexto->HI_comp >> 16;
    contexto->Bloco_msg[58] = contexto->HI_comp >> 8;
    contexto->Bloco_msg[59] = contexto->HI_comp;
    contexto->Bloco_msg[60] = contexto->LO_comp >> 24;
    contexto->Bloco_msg[61] = contexto->LO_comp >> 16;
    contexto->Bloco_msg[62] = contexto->LO_comp >> 8;
    contexto->Bloco_msg[63] = contexto->LO_comp;

    Processamento(contexto);
}

