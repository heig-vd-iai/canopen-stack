from canopen import Node, ObjectDictionary
import jinja2


EDS_FILENAME = "example.eds"
TEMPLATE_FILENAME = "od.hpp.template"


env = jinja2.Environment()
template = env.get_template("template.hpp")


od: ObjectDictionary = Node(1, EDS_FILENAME).object_dictionary
for obj in od.values():
    if obj.index >= 0x1000: print(obj.name)

