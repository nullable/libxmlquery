import xmlquery

xml = xmlquery.parse_xml("../../test.xml")
c = xml.query("description")
#c[1].name = "banana"

for n in c:
    print n.__repr__()

