#!/bin/bash

YELLOW='\033[1;33m'	#Colors
NC='\033[0m'

cd ~/FRCJetson/2019GRiP/build/	#Enters Build dir
if [ -d "/home/ubuntu/FRCJetson/2019GRiP/build/CMakeFiles" ]; then	#Checks for pre-existing build
	echo -e "${YELLOW}Found Pre-existing Build${NC}"
	echo -e "${YELLOW}Deleting Previous Build${NC}"
	rm -rf *
	echo -e "${YELLOW}Building 1/2...${NC}"
	cmake ..
	echo -e "${YELLOW}Building 2/2...${NC}"
	make
	exit
else	
	echo -e "${YELLOW}Building 1/2...${NC}"
	cmake ..
	echo -e "${YELLOW}Building 2/2...${NC}"
	make	
	exit
fi