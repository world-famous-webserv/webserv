#!/bin/python3
import os
import cgi

form = cgi.FieldStorage()
object = form["file"]

file = object.file
dir_name = os.path.join(os.getcwd(), "files")
file_name = os.path.basename(object.filename)

if not file:
    message = "No file was uploaded"
else:
    if not os.path.exists(dir_name):
        os.mkdir(dir_name)
    with open(os.path.join(dir_name, file_name), "wb") as f:
        f.write(file.read())
    message = f"The file [{file_name}] was uploaded at [{dir_name}]"

print("Content-type: text/html;charset=utf-8")
print()
print("<H1>", message, "</H1>")
