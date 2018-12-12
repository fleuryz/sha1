/************************************************************************
* Universidade de Brasilia
* Hash SHA1
*
* B�rbara Varanda Rangel
* Gabriel Martins de Miranda
* Jadiel Te�filo Amorim de Oliveira
* Marcos Cordeiro Fleury
* T�lio de Carvalho Matias
************************************************************************/

##SHA1

Desenvolvido em C na plataforma windows e compilado utilizando do comando <gcc -std=c99 meuSHA1.c main.c -lm -o sha1>. 


A execu��o pode ser das formas:
./sha1
./sha1 <arquivo_entrada>
./sha1 <arquivo_entrada> <arquivo_saida>

No primeiro caso, o programa ir� solicitar que o usu�rio entre com os arquivos de entrada e de sa�da.
No segundo caso, o programa ir� solicitar que o usu�rio entre com o arquivo de sa�da.
No terceiro caso, a execu��o ocorrer� normalmente.

Ser� mostrados na tela o tamanho em bytes do arquivo.

Sempre � printado o hash sha1 na sa�da padr�o e no arquivo de sa�da.


##Como funciona?

1. Tem-se uma mensagem de entrada composta de b bits em que queremos calcular seu hash (b>=0).
   Sempre representado em little endian.
2. Um padding � realizado, primeiro bit 1 e o resto em zero, de forma que o tamanho em bits da
   mensagem inicial seja congruente a 448 m�dulo 512, 64 bits afastado de ser m�ltiplo de 512.
   O padding � sempre realizado, mesmo que a mensagem j� seja congruente a 448 m�dulo 512.
3. O tamanho b bits � representado como 64 bits e concatenado � mensagem do passo anterior. Aqui 
   o tamanho da mensagem � exatamente m�ltipla de 512 bits, sendo tamb�m m�ltipla de 16 palavras
   de 32 bits.
4. O processamento � realizado utilizado 2 buffers compostos por 5 words de 32 bits, uma sequencia de    80 words de 32 bits e um buffer temporario. O primeiro buffer tem as words de labels:A,B,C,D,E. O    segundo buffer tem as words de labels: H0, H1, H2, H3, H4. E a sequencia de 80 words tem labels    indo de W(0) a W(79).
5. Primeiramente s�o inicializadas as palavras do buffer 2 com:
      H0 = 67452301
      H1 = EFCDAB89
      H2 = 98BADCFE
      H3 = 10325476
      H4 = C3D2E1F0.
 
6. Cada mensagem preenchida ser� divida em blocos de 16-words. Para cada bloco de 16 palavras, sendo M9i) o bloco atual s�o realizadas 80 opera��es de deslocamento circular a esquerda divididas em 4 passos.

   		As fun��es utilizadas s�o:
	X AND Y  =  bitwise logical "and" of  X and Y.
      	X OR Y   =  bitwise logical "inclusive-or" of X and Y.
     	X XOR Y  =  bitwise logical "exclusive-or" of X and Y.
      	NOT X    =  bitwise logical "complement" of X.
	X + Y  =  (x + y) mod 2^32.
    	S^n(X)  =  (X << n) OR (X >> 32-n).


   		/* passo 1 */
	Divida o bloco M(i) em 16 words - W(0) a W(15), onde W(0) � a palavra mais a esquerda.
	
   
		/* passo 2 */
	Para t = 16 a 79 fa�a
         W(t) = S^1(W(t-3) XOR W(t-8) XOR W(t-14) XOR W(t-16)).

		/* passo 3 */
	Fa�a A = H0, B = H1, C = H2, D = H3, E = H4.

		/* passo 4 */
	Para t = 0 to 79 fa�a
          TEMP = S^5(A) + f(t;B,C,D) + E + W(t) + K(t);
	  E = D;  D = C;  C = S^30(B);  B = A; A = TEMP;

		/* passo 5 */
	Fa�a
	  H0 = H0 + A, H1 = H1 + B, H2 = H2 + C, H3 = H3 + D, H4 = H4 + E.

6. Depois de processar as M(n) words, o digest � a string de 160 bits representada por H0 H1 H2 H3 H4.