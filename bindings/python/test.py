import xmlquery

xml = xmlquery.parse_xml("<a><b /></a>")
c = xml.query("*")
#c[1].name = "banana"

for n in c:
    print n.name

