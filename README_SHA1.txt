/************************************************************************
* Universidade de Brasilia
* Hash SHA1
*
* Bárbara Varanda Rangel
* Gabriel Martins de Miranda
* Jadiel Teófilo Amorim de Oliveira
* Marcos Cordeiro Fleury
* Túlio de Carvalho Matias
************************************************************************/

##SHA1

Desenvolvido em C na plataforma windows e compilado utilizando do comando <gcc -std=c99 meuSHA1.c main.c -lm -o sha1>. 


A execução pode ser das formas:
./sha1
./sha1 <arquivo_entrada>
./sha1 <arquivo_entrada> <arquivo_saida>

No primeiro caso, o programa irá solicitar que o usuário entre com os arquivos de entrada e de saída.
No segundo caso, o programa irá solicitar que o usuário entre com o arquivo de saída.
No terceiro caso, a execução ocorrerá normalmente.

Será mostrados na tela o tamanho em bytes do arquivo.

Sempre é printado o hash sha1 na saída padrão e no arquivo de saída.


##Como funciona?

1. Tem-se uma mensagem de entrada composta de b bits em que queremos calcular seu hash (b>=0).
   Sempre representado em little endian.
2. Um padding é realizado, primeiro bit 1 e o resto em zero, de forma que o tamanho em bits da
   mensagem inicial seja congruente a 448 módulo 512, 64 bits afastado de ser múltiplo de 512.
   O padding é sempre realizado, mesmo que a mensagem já seja congruente a 448 módulo 512.
3. O tamanho b bits é representado como 64 bits e concatenado à mensagem do passo anterior. Aqui 
   o tamanho da mensagem é exatamente múltipla de 512 bits, sendo também múltipla de 16 palavras
   de 32 bits.
4. O processamento é realizado utilizado 2 buffers compostos por 5 words de 32 bits, uma sequencia de    80 words de 32 bits e um buffer temporario. O primeiro buffer tem as words de labels:A,B,C,D,E. O    segundo buffer tem as words de labels: H0, H1, H2, H3, H4. E a sequencia de 80 words tem labels    indo de W(0) a W(79).
5. Primeiramente são inicializadas as palavras do buffer 2 com:
      H0 = 67452301
      H1 = EFCDAB89
      H2 = 98BADCFE
      H3 = 10325476
      H4 = C3D2E1F0.
 
6. Cada mensagem preenchida será divida em blocos de 16-words. Para cada bloco de 16 palavras, sendo M9i) o bloco atual são realizadas 80 operações de deslocamento circular a esquerda divididas em 4 passos.

   		As funções utilizadas são:
	X AND Y  =  bitwise logical "and" of  X and Y.
      	X OR Y   =  bitwise logical "inclusive-or" of X and Y.
     	X XOR Y  =  bitwise logical "exclusive-or" of X and Y.
      	NOT X    =  bitwise logical "complement" of X.
	X + Y  =  (x + y) mod 2^32.
    	S^n(X)  =  (X << n) OR (X >> 32-n).


   		/* passo 1 */
	Divida o bloco M(i) em 16 words - W(0) a W(15), onde W(0) é a palavra mais a esquerda.
	
   
		/* passo 2 */
	Para t = 16 a 79 faça
         W(t) = S^1(W(t-3) XOR W(t-8) XOR W(t-14) XOR W(t-16)).

		/* passo 3 */
	Faça A = H0, B = H1, C = H2, D = H3, E = H4.

		/* passo 4 */
	Para t = 0 to 79 faça
          TEMP = S^5(A) + f(t;B,C,D) + E + W(t) + K(t);
	  E = D;  D = C;  C = S^30(B);  B = A; A = TEMP;

		/* passo 5 */
	Faça
	  H0 = H0 + A, H1 = H1 + B, H2 = H2 + C, H3 = H3 + D, H4 = H4 + E.

6. Depois de processar as M(n) words, o digest é a string de 160 bits representada por H0 H1 H2 H3 H4.