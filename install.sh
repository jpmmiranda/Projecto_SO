#!/bin/bash

if (( "$EUID" != 0)); then
	echo "Por favor corra como administrador."
	exit
fi

make clean
printf "A instalar..\n"
make
printf "A copiar executáveis para /usr/bin\n"
cp sobusrv /usr/bin
cp sobucli /usr/bin
printf "Instalação concluída\n"