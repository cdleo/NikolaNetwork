#!/bin/bash

if [ "$1" == "" ] || [ "$1" == "gene" ]; then

cd ../GeneFramework/Gene
make clean
make
cd ../../Nikola/

./copyLibs.sh

cd ../GeneFramework/GeneDB
make clean
make
cd ../

cd ./GeneSockets
make clean
make
cd ../

cd ./GeneThreads
make clean
make
cd ../../Nikola/

./copyLibs.sh

fi

if [ "$1" == "" ] || [ "$1" == "nikola" ]; then

cd ./Synapse
make clean
make
cd ../

./copyLibs.sh

cd ./NikolaSupport
make clean
make
cd ../

./copyLibs.sh

cd ./NetworkDiscovery
make clean
make
cd ../UserValidator
make clean
make
cd ../NikolaGateway
make clean
make
cd ../CapabilityModule
make clean
make
cd ../Terminal
make clean
make
cd ../GatewayTerminal
make clean
make
cd ../

fi

if [ "$1" == "" ] || [ "$1" == "gui" ]; then

cd ./GUINikolaSupport
make clean
make
cd ../

./copyLibs.sh

cd ./GUIGatewayTerminal
make clean
make

fi
