#!/bin/bash

if [ "$1" == "" ]; then
	echo -e "\n Usage: ./setConfig.sh <casa|movi>"	
	exit 0
fi

if [ "$1" != "casa" ] && [ "$1" != "movi" ]; then
	echo -e "\n Usage: ./setConfig.sh <casa|movi>"	
	exit 0
fi

rm ./NetworkDiscovery/dist/Debug/GNU-Linux-x86/networkdiscovery.conf
cp ./NetworkDiscovery/conf/networkdiscovery.conf."$1" ./NetworkDiscovery/dist/Debug/GNU-Linux-x86/networkdiscovery.conf

rm ./UserValidator/dist/Debug/GNU-Linux-x86/uservalidator.conf
cp ./UserValidator/conf/uservalidator.conf."$1" ./UserValidator/dist/Debug/GNU-Linux-x86/uservalidator.conf

rm ./CapabilityModule/dist/Debug/GNU-Linux-x86/capabilitymodule.conf
cp ./CapabilityModule/conf/capabilitymodule.conf."$1" ./CapabilityModule/dist/Debug/GNU-Linux-x86/capabilitymodule.conf

rm ./NikolaGateway/dist/Debug/GNU-Linux-x86/nikolagateway.conf
cp ./NikolaGateway/conf/nikolagateway.conf."$1" ./NikolaGateway/dist/Debug/GNU-Linux-x86/nikolagateway.conf

rm ./NikolaVoiceAccessRetrievalSystem/dist/Debug/GNU-Linux-x86/nvars.conf
cp ./NikolaVoiceAccessRetrievalSystem/conf/nvars.conf."$1" ./NikolaVoiceAccessRetrievalSystem/dist/Debug/GNU-Linux-x86/nvars.conf

rm ./Terminal/dist/Debug/GNU-Linux-x86/terminal.conf
cp ./Terminal/conf/terminal.conf."$1" ./Terminal/dist/Debug/GNU-Linux-x86/terminal.conf

rm ./GatewayTerminal/dist/Debug/GNU-Linux-x86/gatewayterminal.conf
cp ./GatewayTerminal/conf/gatewayterminal.conf."$1" ./GatewayTerminal/dist/Debug/GNU-Linux-x86/gatewayterminal.conf

rm ./GUIGatewayTerminal/dist/Debug/GNU-Linux-x86/GUIGatewayTerminal.conf
cp ./GUIGatewayTerminal/conf/GUIGatewayTerminal.conf."$1" ./GUIGatewayTerminal/dist/Debug/GNU-Linux-x86/GUIGatewayTerminal.conf


