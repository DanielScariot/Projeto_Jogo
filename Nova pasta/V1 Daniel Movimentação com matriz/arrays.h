

char letras[27] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' '};

/*
Explicaçao dos numeros na matriz:
Numero 0 representa um caminho onde os monstros nao mudarao sua rota, continuarao com o movimento anterior;
Numero 1 representa uma mudança na direçao, o monstro irá começar a ir para baixo;
Numero 2 representa uma mudança na direçao, o monstro irá começar a ir para cima;
Numero 3 representa uma mudança na direçao, o monstro irá começar a ir para esquerda;
Numero 4 representa uma mudança na direçao, o monstro irá começar a ir para direita;
Numero 5 representa a parede que irá beirar o caminho dos inimigos, nao será possivel criar torres nestes locais;
Numero 6 representa o local onde os monstros serao criados (o local de spawn);
Numero 7 representa o local onde torres poderao ser construídas;
Numero 8 representa que existe uma torre naquele local;
Numero 9 representa a torre do tipo 1;
Numero 10 indica que existe uma torre do tipo 1;
*/
int mapa[A][B] = {
   //                                                    A A A A A A -- no código: char a
   //A B C D E F G H I J K L M N O P Q R S T U V W X Y Z A B C D E F -- no código: char b
   //0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //0
    {5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //1
    {6,0,0,1,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //2
    {5,5,5,0,5,5,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //3
    {5,1,0,3,5,5,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //4
    {5,0,5,5,5,5,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //5
    {5,4,0,0,0,0,2,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //6
    {5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //7
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //8
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //9
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //10
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //11
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //12
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //13
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //14
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //15
    {0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //16
    {5,5,5,5,5,5,5,0,5,4,4,4,4,4,4,1,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //17
    {6,4,4,4,4,1,5,0,5,2,5,5,5,5,5,1,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //18
    {5,5,5,5,5,1,5,0,5,2,5,0,0,0,5,1,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //19
    {0,0,0,0,5,1,5,0,5,2,5,0,0,0,5,1,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //20
    {0,0,0,0,5,1,5,0,5,2,5,0,0,0,5,1,5,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0}, //21
    };
