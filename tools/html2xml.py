#!/usr/bin/python

import sys
from BeautifulSoup import BeautifulSoup
import re


def translate(html, force=False):

    if force:
        html = re.sub(re.compile("<script(.*?)>(.*?)</script>", re.I|re.S), r"<script\1></script>", html)
    else:
        html = re.sub(re.compile("<script(.*?)>(.*?)</script>", re.I|re.S), r"<script\1><![CDATA[\2]]></script>", html)


    soup = BeautifulSoup(html)

    return soup.prettify()

if __name__ == "__main__":
    if not len(sys.argv) == 2:
        print "Usage: html2python 'file.html'"
    f = open(sys.argv[1], 'r')
    print translate(f.read())
    f.close()

