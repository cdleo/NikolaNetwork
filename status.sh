#!/bin/bash

showInfo () {
    appName=$(echo $1 | tr -d ' ')
    count=$(ps -ef | grep "$appName" | grep -v "grep" | wc -l)

    if [ "$count" -eq 1 ]; then
	pid=$(ps -ef | grep "$appName" | grep -v "grep" | awk '{print $2}')
        echo -e "$1:\t ONLINE ($pid)"
    else
    	if [ "$count" -eq 0 ]; then
	    echo -e "$1:\t OFFLINE"
    	else
	    echo -e "$1:\t MULTIPLE OCCURRENCES"
    	fi
    fi
}

showInfo "networkdiscovery " 
showInfo "uservalidator    " 
showInfo "capabilitymodule " 
showInfo "nikolagateway    " 


