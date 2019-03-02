import os
import re


def get_all_names_within(dir: str):
    return [f for f in os.listdir(dir) if os.path.isfile(os.path.join(dir, f))]


def main():
    resource_name = "./Resources.hpp"
    sprites_dir = "./Sprites256px"
    keyword_start = "//PYTHON_INSERT_MEMBERS{"
    keyword_end = "//PYTHON_INSERT_MEMBERS}"

    all_names = get_all_names_within(sprites_dir)

    file = open(resource_name, "r")
    contents: str = file.read()
    file.close()
    ind_start = contents.find(keyword_start)+len(keyword_start)
    ind_end = contents.find(keyword_end, ind_start)

    to_insert = ""

    field_names = [f.replace(".", "_").replace(" ", "_") for f in all_names]

    # getters
    to_insert += "\npublic: "
    for name in field_names:
        to_insert += f"""
        auto get_{name}()const->sf::Texture const&{{return {name};}}"""

    # fields
    to_insert += "\nprivate: "
    for name in field_names:
        to_insert += f"""
        sf::Texture {name};"""

    # constructor
    to_insert += "\nResources(){"
    for field,name in zip(field_names,all_names):
        to_insert += f"""
        {field}.loadFromFile("{sprites_dir+"/"+name}");"""
    to_insert += "}\n"

    contents = contents[:ind_start]+f"\n{to_insert}\n"+contents[ind_end:]

    file = open(resource_name, 'w')
    file.write(contents)
    file.close()


main()
