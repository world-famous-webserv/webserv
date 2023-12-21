#!/usr/bin/env python

import cgi
import os
import json
import http.cookies as Cookie

UPLOAD_DIR = '../uploaded_files'
SESSION_FILE = '../session_data.json'

def load_session_data() -> dict:
    if not os.path.exists(SESSION_FILE):
        return {}
    with open(SESSION_FILE, 'r', encoding='utf-8') as f:
        return json.load(f)

def get_uploaded_files(session_id: str) -> list:
    session_data = load_session_data()
    return session_data.get(session_id, [])

def manage_session() -> str:
    cookie = Cookie.SimpleCookie(os.environ.get("HTTP_COOKIE"))
    session_id = cookie.get("session_id")
    if session_id:
        return session_id.value
    return ""

# 세션 관리 및 사용자가 업로드한 파일 목록 가져오기
session_id = manage_session()
uploaded_files = get_uploaded_files(session_id) if session_id else []

body = f"""
<!DOCTYPE html>
<html lang="ko">
<head>
    <meta charset="UTF-8">
    <title>Uploaded Files</title>
    <style>
        body {{
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background-color: #eaeaea;
            text-align: center;
            margin: 0;
            padding: 20px;
            color: #333;
        }}
        .container {{
            max-width: 700px;
            margin: 40px auto;
            padding: 30px;
            background-color: white;
            border-radius: 10px;
            box-shadow: 0 5px 15px rgba(0, 0, 0, 0.2);
        }}
        h1 {{
            margin-bottom: 20px;
            color: #4a69bd;
        }}
        ul {{
            list-style: none;
            padding: 0;
        }}
        li {{
            text-align: left;
            padding: 10px;
            margin: 5px 0;
            border-left: 4px solid #4a69bd;
            background-color: #f8f9fa;
        }}
        li:nth-child(odd) {{
            background-color: #e9ecef;
        }}
    </style>
</head>
<body>
    <div class="container">
        <h1>Uploaded Files</h1>
        <ul>
            {''.join(f'<li>{file}</li>' for file in uploaded_files)}
        </ul>
    </div>
</body>
</html>
"""

header = f"""Status: 200 Ok
Content-Type: text/html
Content-Length: {len(body)}
"""
print(body)
