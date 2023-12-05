# #!/bin/python3
import os
import cgi

form = cgi.FieldStorage()
if "filename" not in form:
    message = "No file was uploaded"
else:
    object = form["filename"]

    file = object.file

    if not file:
        message = "No file was uploaded"
    else:
        dir_name = os.path.join(os.getcwd(), "files")
        file_name = os.path.basename(object.filename)
        if not os.path.exists(dir_name):
            os.mkdir(dir_name)
        file = open(file_name, "rb")
        with open(os.path.join(dir_name, file_name), "wb") as f:
            f.write(file.read())
        file.close()
        message = f"The file [{file_name}] was uploaded at [{dir_name}]"

print("<!DOCTYPE html>")
print("<html>")
print("<head>")
print("<title>File Upload</title>")
print("</head>")
print("<body>")
print("<h1>", message, "</h1>")
print("</body>")
print("</html>")
