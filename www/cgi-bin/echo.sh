#!/bin/bash

# HTTP 상태 코드 및 Content type 헤더 출력
echo "Status: 200 OK"
echo "Content-type: text/html; charset=UTF-8"
echo ""

# HTML 시작
echo "<!DOCTYPE html>"
echo "<html lang=\"ko\">"
echo "<head>"
echo "    <meta charset=\"UTF-8\">"
echo "    <title>쿼리 문자열 출력</title>"
echo "    <style>"
echo "        body {"
echo "            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;"
echo "            background-color: #f4f4f4;"
echo "            text-align: center;"
echo "            padding-top: 50px;"
echo "        }"
echo "        .container {"
echo "            width: 80%;"
echo "            margin: auto;"
echo "            background: white;"
echo "            padding: 20px;"
echo "            border-radius: 10px;"
echo "            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);"
echo "        }"
echo "        .param {"
echo "            margin: 10px 0;"
echo "            padding: 10px;"
echo "            border: 1px solid #ddd;"
echo "            border-radius: 5px;"
echo "            background-color: #f9f9f9;"
echo "        }"
echo "    </style>"
echo "</head>"
echo "<body>"
echo "    <div class=\"container\">"
echo "        <h1>쿼리 문자열 파라미터</h1>"

# 쿼리 문자열 처리 및 출력
IFS='&'
for param in $QUERY_STRING; do
    decoded_param=$(echo $param | perl -MURI::Escape -e 'print uri_unescape(<STDIN>);')
    echo "        <div class=\"param\">$decoded_param</div>"
done

# HTML 종료
echo "    </div>"
echo "</body>"
echo "</html>"
