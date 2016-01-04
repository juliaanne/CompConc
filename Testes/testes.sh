# !/bin/bash
# Reset
Color_Off='\e[0m'       # Text Reset

# Cores
Red='\e[0;31m'          # Sucesso
Green='\e[0;32m'        # Falha
Yellow='\e[0;33m'       # Funcao seno
Blue='\e[0;34m'         # Funcao elefante
Purple='\e[0;35m'       # Funcao parabola
Cyan='\e[0;36m'         # Cyan
White='\e[0;37m'        # White

SUCESSO = "${Green}Sucesso!"


echo "SIMULAÇÃO TESTES"
echo "Compilando funções elefante - sequencial e paralela"
gcc elefanteSequencial.c -o elefanteSequencial.exe -Wall -lm
gcc elefanteParalelo.c -o elefanteParalelo.exe -Wall -lm -lpthread
echo "--"

echo "Compilando funções exponencial - sequencial e paralela"
gcc exponencialSequencial.c -o exponencialSequencial.exe -Wall -lm
gcc exponencialParalelo.c -o exponencialParalelo.exe -Wall -lm -lpthread
echo "--"

echo "Compilando funções seno - sequencial e paralela"
gcc senoSequencial.c -o senoSequencial.exe -Wall -lm
gcc senoParalelo.c -o senoParalelo.exe -Wall -lm -lpthread
echo "--"
echo ""
echo ""


echo "REALIZANDO TESTES FUNÇÃO ELEFANTE"

echo "Simulação 1"
echo "Intervalo de 0 a 2"
echo ""

echo "Erro de 0.001"
echo "Threads variando de 4 a 6"
echo ""
./elefanteSequencial.exe 0 2 0.001
./elefanteParalelo.exe 0 2 0.001 4
./elefanteParalelo.exe 0 2 0.001 5
./elefanteParalelo.exe 0 2 0.001 6

echo "Erro de 0.0001"
echo "Threads variando de 4 a 6"
echo ""
./elefanteSequencial.exe 0 2 0.0001
./elefanteParalelo.exe 0 2 0.001 4
./elefanteParalelo.exe 0 2 0.001 5
./elefanteParalelo.exe 0 2 0.001 6

echo "Erro de 0.00001"
echo "Threads variando de 4 a 6"
echo ""
./elefanteSequencial.exe 0 2 0.00001
./elefanteParalelo.exe 0 2 0.001 4
./elefanteParalelo.exe 0 2 0.001 5
./elefanteParalelo.exe 0 2 0.001 6

echo "Erro de 0.00001"
echo "Threads variando de 4 a 6"
echo ""
./elefanteSequencial.exe 0 2 0.000001
./elefanteParalelo.exe 0 2 0.001 4
./elefanteParalelo.exe 0 2 0.001 5
./elefanteParalelo.exe 0 2 0.001 6

echo "Simulação 2"
echo "Intervalo de 2 a 5"
echo ""

echo "Erro de 0.001"
echo "Threads variando de 4 a 6"
echo ""
./elefanteSequencial.exe 2 5 0.001
./elefanteParalelo.exe 2 5 0.001 4
./elefanteParalelo.exe 2 5 0.001 5
./elefanteParalelo.exe 2 5 0.001 6

echo "Erro de 0.0001"
echo "Threads variando de 4 a 6"
echo ""
./elefanteSequencial.exe 2 5 0.0001
./elefanteParalelo.exe 2 5 0.001 4
./elefanteParalelo.exe 2 5 0.001 5
./elefanteParalelo.exe 2 5 0.001 6

echo "Erro de 0.00001"
echo "Threads variando de 4 a 6"
echo ""
./elefanteSequencial.exe 2 5 0.00001
./elefanteParalelo.exe 2 5 0.001 4
./elefanteParalelo.exe 2 5 0.001 5
./elefanteParalelo.exe 2 5 0.001 6

echo "Erro de 0.00001"
echo "Threads variando de 4 a 6"
echo ""
./elefanteSequencial.exe 2 5 0.000001
./elefanteParalelo.exe 2 5 0.001 4
./elefanteParalelo.exe 2 5 0.001 5
./elefanteParalelo.exe 2 5 0.001 6

echo "Simulação 3"
echo "Intervalo de 1 a 11"
echo ""

echo "Erro de 0.001"
echo "Threads variando de 4 a 6"
echo ""
./elefanteSequencial.exe 1 11 0.001
./elefanteParalelo.exe 1 11 0.001 4
./elefanteParalelo.exe 1 11 0.001 5
./elefanteParalelo.exe 1 11 0.001 6

echo "Erro de 0.0001"
echo "Threads variando de 4 a 6"
echo ""
./elefanteSequencial.exe 1 11 0.0001
./elefanteParalelo.exe 1 11 0.001 4
./elefanteParalelo.exe 1 11 0.001 5
./elefanteParalelo.exe 1 11 0.001 6

echo "Erro de 0.00001"
echo "Threads variando de 4 a 6"
echo ""
./elefanteSequencial.exe 1 11 0.00001
./elefanteParalelo.exe 1 11 0.001 4
./elefanteParalelo.exe 1 11 0.001 5
./elefanteParalelo.exe 1 11 0.001 6

echo "Erro de 0.00001"
echo "Threads variando de 4 a 6"
echo ""
./elefanteSequencial.exe 1 11 0.000001
./elefanteParalelo.exe 1 11 0.001 4
./elefanteParalelo.exe 1 11 0.001 5
./elefanteParalelo.exe 1 11 0.001 6


echo "REALIZAÇÃO TESTES FUNÇÃO EXPONENCIAL"

echo "Simulação 1"
echo "Intervalo de 0 a 2"
echo ""

echo "Erro de 0.001"
echo "Threads variando de 4 a 6"
echo ""
./exponencialSequencial.exe 0 2 0.001
./exponencialParalelo.exe 0 2 0.001 4
./exponencialParalelo.exe 0 2 0.001 5
./exponencialParalelo.exe 0 2 0.001 6

echo "Erro de 0.0001"
echo "Threads variando de 4 a 6"
echo ""
./exponencialSequencial.exe 0 2 0.0001
./exponencialParalelo.exe 0 2 0.001 4
./exponencialParalelo.exe 0 2 0.001 5
./exponencialParalelo.exe 0 2 0.001 6

echo "Erro de 0.00001"
echo "Threads variando de 4 a 6"
echo ""
./exponencialSequencial.exe 0 2 0.00001
./exponencialParalelo.exe 0 2 0.001 4
./exponencialParalelo.exe 0 2 0.001 5
./exponencialParalelo.exe 0 2 0.001 6

echo "Erro de 0.00001"
echo "Threads variando de 4 a 6"
echo ""
./exponencialSequencial.exe 0 2 0.000001
./exponencialParalelo.exe 0 2 0.001 4
./exponencialParalelo.exe 0 2 0.001 5
./exponencialParalelo.exe 0 2 0.001 6

echo "Simulação 2"
echo "Intervalo de 2 a 5"
echo ""

echo "Erro de 0.001"
echo "Threads variando de 4 a 6"
echo ""
./exponencialSequencial.exe 2 5 0.001
./exponencialParalelo.exe 2 5 0.001 4
./exponencialParalelo.exe 2 5 0.001 5
./exponencialParalelo.exe 2 5 0.001 6

echo "Erro de 0.0001"
echo "Threads variando de 4 a 6"
echo ""
./exponencialSequencial.exe 2 5 0.0001
./exponencialParalelo.exe 2 5 0.001 4
./exponencialParalelo.exe 2 5 0.001 5
./exponencialParalelo.exe 2 5 0.001 6

echo "Erro de 0.00001"
echo "Threads variando de 4 a 6"
echo ""
./exponencialSequencial.exe 2 5 0.00001
./exponencialParalelo.exe 2 5 0.001 4
./exponencialParalelo.exe 2 5 0.001 5
./exponencialParalelo.exe 2 5 0.001 6

echo "Erro de 0.00001"
echo "Threads variando de 4 a 6"
echo ""
./exponencialSequencial.exe 2 5 0.000001
./exponencialParalelo.exe 2 5 0.001 4
./exponencialParalelo.exe 2 5 0.001 5
./exponencialParalelo.exe 2 5 0.001 6

echo "Simulação 3"
echo "Intervalo de 1 a 11"
echo ""

echo "Erro de 0.001"
echo "Threads variando de 4 a 6"
echo ""
./exponencialSequencial.exe 1 11 0.001
./exponencialParalelo.exe 1 11 0.001 4
./exponencialParalelo.exe 1 11 0.001 5
./exponencialParalelo.exe 1 11 0.001 6

echo "Erro de 0.0001"
echo "Threads variando de 4 a 6"
echo ""
./exponencialSequencial.exe 1 11 0.0001
./exponencialParalelo.exe 1 11 0.001 4
./exponencialParalelo.exe 1 11 0.001 5
./exponencialParalelo.exe 1 11 0.001 6

echo "Erro de 0.00001"
echo "Threads variando de 4 a 6"
echo ""
./exponencialSequencial.exe 1 11 0.00001
./exponencialParalelo.exe 1 11 0.001 4
./exponencialParalelo.exe 1 11 0.001 5
./exponencialParalelo.exe 1 11 0.001 6

echo "Erro de 0.00001"
echo "Threads variando de 4 a 6"
echo ""
./exponencialSequencial.exe 1 11 0.000001
./exponencialParalelo.exe 1 11 0.001 4
./exponencialParalelo.exe 1 11 0.001 5
./exponencialParalelo.exe 1 11 0.001 6


echo "REALIZAÇÃO TESTES FUNÇÃO SENO"

echo "Simulação 1"
echo "Intervalo de 0 a 2"
echo ""

echo "Erro de 0.001"
echo "Threads variando de 4 a 6"
echo ""
./senoSequencial.exe 0 2 0.001
./senoParalelo.exe 0 2 0.001 4
./senoParalelo.exe 0 2 0.001 5
./senoParalelo.exe 0 2 0.001 6

echo "Erro de 0.0001"
echo "Threads variando de 4 a 6"
echo ""
./senoSequencial.exe 0 2 0.0001
./senoParalelo.exe 0 2 0.001 4
./senoParalelo.exe 0 2 0.001 5
./senoParalelo.exe 0 2 0.001 6

echo "Erro de 0.00001"
echo "Threads variando de 4 a 6"
echo ""
./senoSequencial.exe 0 2 0.00001
./senoParalelo.exe 0 2 0.001 4
./senoParalelo.exe 0 2 0.001 5
./senoParalelo.exe 0 2 0.001 6

echo "Erro de 0.00001"
echo "Threads variando de 4 a 6"
echo ""
./senoSequencial.exe 0 2 0.000001
./senoParalelo.exe 0 2 0.001 4
./senoParalelo.exe 0 2 0.001 5
./senoParalelo.exe 0 2 0.001 6

echo "Simulação 2"
echo "Intervalo de 2 a 5"
echo ""

echo "Erro de 0.001"
echo "Threads variando de 4 a 6"
echo ""
./senoSequencial.exe 2 5 0.001
./senoParalelo.exe 2 5 0.001 4
./senoParalelo.exe 2 5 0.001 5
./senoParalelo.exe 2 5 0.001 6

echo "Erro de 0.0001"
echo "Threads variando de 4 a 6"
echo ""
./senoSequencial.exe 2 5 0.0001
./senoParalelo.exe 2 5 0.001 4
./senoParalelo.exe 2 5 0.001 5
./senoParalelo.exe 2 5 0.001 6

echo "Erro de 0.00001"
echo "Threads variando de 4 a 6"
echo ""
./senoSequencial.exe 2 5 0.00001
./senoParalelo.exe 2 5 0.001 4
./senoParalelo.exe 2 5 0.001 5
./senoParalelo.exe 2 5 0.001 6

echo "Erro de 0.00001"
echo "Threads variando de 4 a 6"
echo ""
./senoSequencial.exe 2 5 0.000001
./senoParalelo.exe 2 5 0.001 4
./senoParalelo.exe 2 5 0.001 5
./senoParalelo.exe 2 5 0.001 6

echo "Simulação 3"
echo "Intervalo de 1 a 11"
echo ""

echo "Erro de 0.001"
echo "Threads variando de 4 a 6"
echo ""
./senoSequencial.exe 1 11 0.001
./senoParalelo.exe 1 11 0.001 4
./senoParalelo.exe 1 11 0.001 5
./senoParalelo.exe 1 11 0.001 6

echo "Erro de 0.0001"
echo "Threads variando de 4 a 6"
echo ""
./senoSequencial.exe 1 11 0.0001
./senoParalelo.exe 1 11 0.001 4
./senoParalelo.exe 1 11 0.001 5
./senoParalelo.exe 1 11 0.001 6

echo "Erro de 0.00001"
echo "Threads variando de 4 a 6"
echo ""
./senoSequencial.exe 1 11 0.00001
./senoParalelo.exe 1 11 0.001 4
./senoParalelo.exe 1 11 0.001 5
./senoParalelo.exe 1 11 0.001 6

echo "Erro de 0.00001"
echo "Threads variando de 4 a 6"
echo ""
./senoSequencial.exe 1 11 0.000001
./senoParalelo.exe 1 11 0.001 4
./senoParalelo.exe 1 11 0.001 5
./senoParalelo.exe 1 11 0.001 6
