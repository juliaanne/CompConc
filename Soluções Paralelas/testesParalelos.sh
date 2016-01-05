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

#SUCESSO = "${Green}Sucesso!"


echo "SIMULAÇÃO TESTES PARALELOS"
echo "Compilando funções"
gcc elefanteParalelo.c -o elefanteParalelo.exe -Wall -lpthread -lm 
gcc exponencialParalelo.c -o exponencialParalelo.exe -Wall -lpthread -lm
gcc senoParalelo.c -o senoParalelo.exe -Wall -lpthread -lm
gcc linearParalelo.c -o linearParalelo.exe -Wall -lpthread -lm
gcc parabolaParalelo.c -o parabolaParalelo.exe -Wall -lpthread -lm

echo ""
echo "**"

echo "REALIZAÇÃO DE TESTES DA FUNÇÃO LINEAR"
./linearParalelo.exe 0 10 0.00001 4
echo "------- Valor esperado era: 60"
echo ""
./linearParalelo.exe 50 130 0.00001 4
echo "------- Valor esperado era: 7280"
echo ""
./linearParalelo.exe -30 70 0.00001 4
echo "------- Valor esperado era: 2100"
echo ""

echo "**"

echo "REALIZAÇÃO DE TESTES DA FUNÇÃO PARABOLA"
./parabolaParalelo.exe -1 1 0.00001 4
echo "------- Valor esperado era: 1.5708"
echo ""
./parabolaParalelo.exe 0 1 0.00001 4
echo "------- Valor esperado era: 0.78540"
echo ""

echo "**"

echo "REALIZAÇÃO DE TESTES DA FUNÇÃO EXPONENCIAL"
./exponencialParalelo.exe 0 20 0.00001 4
echo "------- Valor esperado era: 2667.88"
echo ""
./exponencialParalelo.exe -50 50 0.00001 4
echo "------- Valor esperado era: 83335.8"
echo ""
./exponencialParalelo.exe 30 100 0.00001 4
echo "------- Valor esperado era: 324333"
echo ""

echo "**"

echo "REALIZAÇÃO DE TESTES DA FUNÇÃO SENO"
./senoParalelo.exe 0 10 0.00001 4
echo "------- Valor esperado era: 0.583671"
echo ""
./senoParalelo.exe -30 0 0.00001 4
echo "------- Valor esperado era: 0.625544"
echo ""
./senoParalelo.exe -50 100 0.00001 4
echo "------- Valor esperado era: 1.25048"
echo ""

echo "**"

echo "REALIZAÇÃO DE TESTES DA FUNÇÃO ELEFANTE"
./elefanteParalelo.exe 0 20 0.00001 4
echo "------- Valor esperado era: 219.759"
echo ""
./elefanteParalelo.exe 10 100 0.00001 4
echo "------- Valor esperado era: 125077"
echo ""
./elefanteParalelo.exe -10 40 0.00001 4
echo "------- Valor esperado era: 3239.42"
echo ""
