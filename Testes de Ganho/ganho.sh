# !/bin/bash

# Ve o numero de parametros 
if [ "$#" -ne 5 ]; then
    echo "Uso ./ganho.sh <valor de a> <valor de b> <valor do erro> <n threads> <funcao pra calcular 1-5>"
    exit
fi

# Pra calcular o ganho:
echo "Compilando funções sequencial e paralela"
gcc sequencial.c -o sequencial.exe -lm
gcc paralelo.c -o paralelo.exe -lm -lpthread
echo "Funcões compiladas"
echo "--"

#Le os argumentos da entrada tipo argv do C
a=$1
b=$2
erro=$3
threads=$4
funcao=$5

tempoSequencial=$(./sequencial.exe $a $b $erro $funcao)
tempoParalelo=$(./paralelo.exe $a $b $erro $threads $funcao)

echo "Tempo sequencial:" $tempoSequencial "s"
echo "Tempo paralelo:" $tempoParalelo "s"

ganho=$(echo "$tempoSequencial/$tempoParalelo" | bc -l)

echo "Ganho real:" "0"$ganho
