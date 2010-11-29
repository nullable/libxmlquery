#!/usr/bin/python

import sys
from BeautifulSoup import BeautifulSoup

if not len(sys.argv) == 2:
    print "Usage: html2python 'file.html'"

f = open(sys.argv[1], 'r')

soup = BeautifulSoup(f.read())
print soup.prettify()

