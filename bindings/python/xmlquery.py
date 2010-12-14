import libxmlquery as lxq
import collections

def parse_xml(filename):
    if not isinstance(filename, str): raise TypeError
    return NodeWrapper(lxq.internal_parse_xml_file(filename))


class NodeList:
    def __init__(self, listptr):
        self._list = listptr

    def __len__(self):
        return lxq.list_size(self._list)

    def __getitem__(self, key):
        if isinstance(key, int):
            if 0 <= key < len(self): return NodeWrapper(lxq.get_dom_node_at(self._list, key))
            elif -len(self) <= key < 0: return NodeWrapper(lxq.get_dom_node_at(self._list, len(self) + key))
            else: raise IndexError
        elif isinstance(key, slice):
            if key.start == None: start = 0
            if key.stop == None: stop = len(self)
            if key.step == None: step = 1;
            if key.step == 0: raise ValueError("Step value cannot be 0")
            return lxq.slice_list(self._list, start, stop, step)
        else: raise TypeError

    def __contains__(self, item):
        if not isinstance(item, Node): return False
        return not lxq.get_element_pos(self._list, item._node) == -1


class Node:
    def __init__(self, name, namespace=None):
        self._node = lxq.new_element_node(name, namespace)
        self._python_owner = True

    def query(self, q):
        if not isinstance(q, str): raise TypeError
        if not q or q[0] != "@": q = "@" + q
        return NodeList(lxq.query(q, self._node))

    def __getattr__(self, name):
        if name in self.__dict__: return self.__dict__[name]
        elif name == 'name': return lxq.get_name(self._node)
        elif name == 'namespace': return lxq.get_namespace(self._node)
        elif name == 'children': return NodeList(lxq.get_children(self._node))
        elif name == 'attributes': return AttributeMap(self._node)
        elif name == 'text': return str(reduce(lambda x, y: str(x)+ '\n' +str(y), self.__getattr__("children")))
        else: raise AttributeError

    def __setattr__(self, name, value):
        if name == 'name': lxq.set_name(self._node, value)
        elif name == 'namespace': lxq.set_namespace(self._node, value)
        elif name == 'children': raise AttributeError("Attribute 'children' is read only")
        elif name == 'attributes': raise AttributeError("Attribute 'attributes' is read only")
        else: self.__dict__[name] = value

    def __str__(self):
        return lxq.node_to_xml_string(self._node)[:-1]

    def __repr__(self):
        name = self.name
        if self.namespace: name = self.namespace + self.name
        return "<" + name + " .../>"

#    def __del__(self):
#        if self._python_owner:
#            lxq.destroy_dom_node(self._node)
#            self._node = None


class NodeWrapper(Node):
    def __init__(self, node_ptr):
        self._node = node_ptr
        self.python_owner = False


class AttributeMap:
    def __init__(self, nodeptr):
        self._node = nodeptr

    def __contains__(self, item):
        if not isinstance(item, str): return False
        return not lxq.get_attribute_by_name(nodeptr, item) == None

    def __get_item__(self, name):
        if not isinstance(name, str): raise TypeError
        r = lxq.get_attribute_by_name(nodeptr, name)
        if r: return r
        else: raise KeyError

    def __set_item__(self, name, value):
        if not (isinstance(name, str) and isinstance(value, str)):
            raise TypeError
        attr = lxq.new_attribute(name, value)
        lxq.add_attribute(self._node, attr)

#class Attribute:
#    def __init__(self, name, value):
#        self._node = lxq.new_attribute(name, value)
#        self._python_owner = True
#
#    def __getattr__(self, name):
#        if name == 'name': return lxq.get_name(self._node)
#        elif name == 'value': return lxq.get_value(self._node)
#        else: raise AttributeError
#
#    def __setattr__(self, name, value)
#        if name == 'name': lxq.set_name(self._node, value)
#        elif name == 'value': lxq.set_value(self._node, value)
#        else: raise AttributeError
#
#    def __del__(self):
#        if self._python_owner:
#            lxq.destroy_dom_node(self._node)
#            self._node = None

