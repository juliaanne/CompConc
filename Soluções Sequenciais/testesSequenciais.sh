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


echo "SIMULAÇÃO TESTES SEQUENCIAIS"
echo "Compilando funções"
gcc elefanteSequencial.c -o elefanteSequencial.exe -Wall -lm
gcc exponencialSequencial.c -o exponencialSequencial.exe -Wall -lm
gcc senoSequencial.c -o senoSequencial.exe -Wall -lm
gcc linearSequencial.c -o linearSequencial.exe -Wall -lm
gcc parabolaSequencial.c -o parabolaSequencial.exe -Wall -lm

echo ""
echo "**"

echo "REALIZAÇÃO DE TESTES DA FUNÇÃO LINEAR"
./linearSequencial.exe 0 10 0.00001
echo "------- Valor esperado era: 60"
echo ""
./linearSequencial.exe 50 130 0.00001
echo "------- Valor esperado era: 7280"
echo ""
./linearSequencial.exe -30 70 0.00001
echo "------- Valor esperado era: 2100"
echo ""

echo "**"

echo "REALIZAÇÃO DE TESTES DA FUNÇÃO PARABOLA"
./parabolaSequencial.exe -1 1 0.00001
echo "------- Valor esperado era: 1.5708"
echo ""
./parabolaSequencial.exe 0 1 0.00001
echo "------- Valor esperado era: 0.78540"
echo ""

echo "**"

echo "REALIZAÇÃO DE TESTES DA FUNÇÃO EXPONENCIAL"
./exponencialSequencial.exe 0 20 0.00001
echo "------- Valor esperado era: 2667.88"
echo ""
./exponencialSequencial.exe -50 50 0.00001
echo "------- Valor esperado era: 83335.8"
echo ""
./exponencialSequencial.exe 30 100 0.00001
echo "------- Valor esperado era: 324333"
echo ""

echo "**"

echo "REALIZAÇÃO DE TESTES DA FUNÇÃO SENO"
./senoSequencial.exe 0 10 0.00001
echo "------- Valor esperado era: 0.583671"
echo ""
./senoSequencial.exe -30 0 0.00001
echo "------- Valor esperado era: 0.625544"
echo ""
./senoSequencial.exe -50 100 0.00001
echo "------- Valor esperado era: 0.625544"
echo ""

echo "**"

echo "REALIZAÇÃO DE TESTES DA FUNÇÃO ELEFANTE"
./elefanteSequencial.exe 0 20 0.00001
echo "------- Valor esperado era: 219.759"
echo ""
./elefanteSequencial.exe 10 100 0.00001
echo "------- Valor esperado era: 125077"
echo ""
./elefanteSequencial.exe -10 40 0.00001
echo "------- Valor esperado era: 3239.42"
echo ""
