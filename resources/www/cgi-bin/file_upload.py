import cgi
import os
import json
import uuid
import traceback
import http.cookies as Cookie
from datetime import datetime, timedelta

UPLOAD_DIR = '../uploaded_files'
SESSION_FILE = '../session_data.json'

def load_session_data() -> None:
    if not os.path.exists(SESSION_FILE):
        return {}
    with open(SESSION_FILE, 'r', encoding='utf-8') as f:
        return json.load(f)

def save_session_data(session_data: str) -> None:
    with open(SESSION_FILE, 'w') as f:
        json.dump(session_data, f, ensure_ascii=False, indent=4)

def manage_session() -> tuple:
    cookie = Cookie.SimpleCookie(os.environ.get("HTTP_COOKIE"))
    # 세션 쿠키를 확인하고, 없으면 새로 생성합니다.
    if 'session_id' not in cookie:
        session_id = str(uuid.uuid4())
        cookie['session_id'] = session_id
        # 쿠키 속성 설정 (예: 경로, 만료 시간 등)
        cookie['session_id']['path'] = '/'
        cookie['session_id']['httponly'] = True
        cookie['session_id']['secure'] = True
#        cookie['session_id']['samesite'] = 'Lax'
        expiration = datetime.now() + timedelta(days=7)
        cookie['session_id']['expires'] = expiration.strftime("%a, %d-%b-%Y %H:%M:%S GMT")
        print(cookie.output())  # 'Set-Cookie' 헤더를 출력합니다.
    else:
        session_id = cookie['session_id'].value
    
    return session_id, cookie

def upload(session_id: str):
    if not os.path.exists(UPLOAD_DIR):
        os.makedirs(UPLOAD_DIR)

    # 폼 데이터 처리
    form = cgi.FieldStorage()

    if "file" not in form:
        return 500, "No file was uploaded"

    fileitem = form['file']
    if not fileitem.filename:
        return 500, "No file was uploaded"

    # 파일 이름 정리 (디렉토리 트래버설 공격 방지)
    filename = os.path.basename(fileitem.filename)
    file_path = os.path.join(UPLOAD_DIR, filename)

    # 이미 존재하는 파일 체크
    if os.path.exists(file_path):
        return 409, f"Error: The file '{filename}' already exists"

    with open(file_path, 'wb') as f:
        f.write(fileitem.file.read())

    # 파일 목록 업데이트
    session_data = load_session_data()
    session_data.setdefault(session_id, []).append(filename)
    save_session_data(session_data)

    return 200, f"The file '{filename}' was uploaded successfully"

# 세션 관리 실행
try:
    session_id, cookie = manage_session()
    code, message = upload(session_id)
except Exception as e:
    traceback.print_exc()
    code, message = 500, "An error occurred during file upload"

# HTML 출력
body = f"""
<!DOCTYPE html>
<html lang="ko">
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
        <a href="/file_upload.html" class="button">Upload Another File</a> <!-- 파일 업로드 폼으로 돌아가는 링크 -->
        <a href="/cgi-bin/myfiles.py" class="button">Check Uploaded Files</a>
    </div>
</body>
</html>
"""

status_messages = {500: "Internal Server Error", 409: "Conflict"}
status_message = status_messages.get(code, "Ok")

header = f"""Status: {code} {status_message}
Content-Type: text/html
Content-Length: {len(body)}
"""

print(header)
print(body)
