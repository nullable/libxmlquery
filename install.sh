#!/bin/bash

if [ "$(id -u)" != "0" ]
then
    echo "You must run install as super user"
    exit 1
fi

function install {
    if [ -d include/ -a -f libxmlquery.so ]
    then
	if [ -d /etc/ld.so.conf.d/ ]
	then
	    echo "/usr/lib" > /etc/ld.so.conf.d/libxmlquery.conf
	fi

	cp libxmlquery.so /usr/lib/
    cp xmlquery /usr/bin/
	chmod -x /usr/lib/libxmlquery.so

	if [ ! -e /usr/include/xmlquery ]
	then
	    mkdir /usr/include/xmlquery
	fi
	cp -r include/* /usr/include/xmlquery
    else
	echo "Unable to find libxmlquery header files or libxmlquery.so. Header files should be in ./include dir and libxmlquery.so in ./ ."
    fi
}

function uninstall {
    if [ -d /etc/ld.so.conf.d/ ]
    then
	rm -f /etc/ld.so.conf.d/libxmlquery.conf
    fi

    rm -f /usr/lib/libxmlquery.so
    rm -rf /usr/include/xmlquery/
}

if [ $# -eq 0 ]
then
    install
else
    if [ "$1" == "-u" ]
    then
	uninstall
    else
	echo "Unrecognized options: $*"
    fi
fi

