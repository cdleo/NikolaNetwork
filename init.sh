#!/bin/bash

if [ "$1" == "?" ]; then
	echo -e "\n Usage: ./start.sh <Nombre del binario a lanzar/relanzar> "	
	exit 0
fi

#./stop.sh $1

if [ "$1" == "" ]; then
	echo -e "\n ************ STARTING APPLICATIONS  ************ "
fi

cd ./bin/dist/Debug/GNU-Linux-x86/

if [ "$1" == "" ] || [ "$1" == "networkdiscovery" ]; then
#	cd ./NetworkDiscovery/dist/Debug/GNU-Linux-x86/
	./networkdiscovery ./networkdiscovery.conf > /dev/null 2>&1 &
#	cd ../../../../
fi

if [ "$1" == "" ] || [ "$1" == "uservalidator" ]; then
#	cd ./UserValidator/dist/Debug/GNU-Linux-x86/
	./uservalidator ./uservalidator.conf > /dev/null 2>&1 &
#	cd ../../../../
fi

if [ "$1" == "" ] || [ "$1" == "capabilitymodule" ]; then
#	cd ./CapabilityModule/dist/Debug/GNU-Linux-x86/
	./capabilitymodule ./capabilitymodule.conf > /dev/null 2>&1 &
#	cd ../../../../
fi

if [ "$1" == "" ] || [ "$1" == "nikolagateway" ]; then
#	cd ./NikolaGateway/dist/Debug/GNU-Linux-x86/
	./nikolagateway ./nikolagateway.conf > /dev/null 2>&1 &
#	cd ../../../../
fi

cd ../../../../

exec ./status.sh

