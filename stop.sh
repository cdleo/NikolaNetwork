#!/bin/bash

stopApp () {
    appName=$(echo $1 | tr -d ' ')
    count=$(ps -ef | grep "$appName" | grep -v "grep" | grep -v "start" | grep -v "stop" | wc -l)

    if [ "$count" -ge 1 ]; then
	ps -ef | grep "$appName" | grep -v "grep" | grep -v "start" | grep -v "stop" | awk '{print $2}' | xargs kill -2
        echo -e "$1:\t STOPPED"
    else
    	if [ "$count" -eq 0 ]; then
	    echo -e "$1:\t OFFLINE"
    	fi
    fi
}


if [ "$1" == "?" ]; then
	echo -e "\n Usage: ./stop.sh <Nombre del binario a detener> "	
	exit 0
fi

if [ "$1" == "" ]; then
	echo -e "\n ************ STOPPING APPLICATIONS  ************ "
fi

if [ "$1" == "" ] || [ "$1" == "networkdiscovery" ]; then
	stopApp "networkdiscovery" 
fi

if [ "$1" == "" ] || [ "$1" == "uservalidator" ]; then
	stopApp "uservalidator   " 
fi

if [ "$1" == "" ] || [ "$1" == "capabilitymodule" ]; then
	stopApp "capabilitymodule" 
fi

if [ "$1" == "" ] || [ "$1" == "nikolagateway" ]; then
	stopApp "nikolagateway   " 
fi

