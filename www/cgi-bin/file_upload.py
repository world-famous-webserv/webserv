import cgi
import os

def upload():
    UPLOAD_DIR = '../uploaded_files'
    if not os.path.exists(UPLOAD_DIR):
        return "The upload directory does not exist"

    # 폼 데이터 처리
    form = cgi.FieldStorage()

    if "file" not in form:
        return "No file was uploaded"

    fileitem = form['file']
    if not fileitem.filename:
        return "No file was uploaded"

    # 파일 이름 정리 (디렉토리 트래버설 공격 방지)
    filename = os.path.basename(fileitem.filename)
    file_path = os.path.join(UPLOAD_DIR, filename)

    with open(file_path, 'wb') as f:
        f.write(fileitem.file.read())
    
    return f"The file '{filename}' was uploaded successfully"

message = upload()
# HTML 출력
print(f"""
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>File Upload Result</title>
    <style>
        body {{
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background-color: #f0f0f0;
            text-align: center;
            padding-top: 50px;
            margin: 0;
        }}
        .container {{
            background-color: white;
            width: 40%;
            margin: auto;
            padding: 40px;
            border-radius: 15px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
            transition: all 0.3s ease-in-out;
        }}
        .container:hover {{
            box-shadow: 0 8px 16px rgba(0, 0, 0, 0.3);
        }}
        h1 {{
            color: #5a5a5a;
            margin-bottom: 20px;
        }}
        p {{
            color: #333;
            font-size: 1.2em;
            line-height: 1.4;
        }}
        .button {{
            display: inline-block;
            margin-top: 20px;
            padding: 10px 20px;
            background-color: #007bff;
            color: white;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            text-decoration: none;
            transition: background-color 0.2s;
        }}
        .button:hover {{
            background-color: #0056b3;
        }}
    </style>
</head>
<body>
    <div class="container">
        <h1>File Upload Result</h1>
        <p>{message}</p>
        <a href="/" class="button">Back to Home</a>
        <a href="/file_upload" class="button">Upload Another File</a> <!-- 파일 업로드 폼으로 돌아가는 링크 -->
    </div>
</body>
</html>
""")
