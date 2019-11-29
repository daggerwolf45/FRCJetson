#!/bin/bash

YELLOW='\033[1;33m'     #Colors
NC='\033[0m'

if [[ $(dpkg-query -W -f='${Status}' libopencv-dev 2>/dev/null | grep -c "ok installed") -eq 0 ]] || [[ $(dpkg-query -W -f='${Status}' libsqlite3-dev 2>/dev/null | grep -c "ok installed") -eq 0 ]]; 
then
    echo -n "Required dependancies not found: "
    if [[ $(dpkg-query -W -f='${Status}' libopencv-dev 2>/dev/null | grep -c "ok installed") -eq 0 ]]; then
        echo -n "libopencv-dev "
    fi
    if [[ $(dpkg-query -W -f='${Status}' libsqlite3-dev 2>/dev/null | grep -c "ok installed") -eq 0 ]]; then
        echo -n "libsqlite3-dev "
    fi
    echo "Would you like to install them? (y/n)"
    read
    if [[ $REPLY = 'y' ]]; then
        sudo apt install --yes -y libopencv-dev libsqlite3-dev
    fi
fi
    
if [[ ! -d "build" ]]; then
	mkdir build
fi

cd build/	#Enters Build dir
if [ -d "build/CMakeFiles" ]; then	#Checks for pre-existing build
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
