<?php

// // 표준 입력 스트림 열기
// $handle = fopen("php://stdin", "r");
// $rawPostData = '';

// // 파일의 끝에 도달할 때까지 반복
// $lineNumber = 0;
// while ($lineNumber < 13) {
//     $line = fgets($handle);
//     $rawPostData .= $line;
//     $lineNumber++;
// }
// fclose($handle);

// // CONTENT_TYPE 환경 변수에서 경계 문자열 찾기
// $contentType = $_SERVER['CONTENT_TYPE'];
// $boundary = substr($contentType, strpos($contentType, "boundary=") + 9);

// // 경계 문자열로 데이터를 분할
// $parts = explode("--" . $boundary, $rawPostData);
// array_pop($parts); // 마지막 경계 문자열 제거

// // $_POST 초기화
// $_POST = [];

// foreach ($parts as $part) {
//     // Content-Disposition 헤더 찾기
//     if (strpos($part, "Content-Disposition: form-data;") !== false) {
//         // 변수 이름 추출
//         preg_match('/name="([^"]+)"/', $part, $name);

//         // 값 추출
//         $value = substr($part, strpos($part, "\r\n\r\n") + 4);
//         $value = substr($value, 0, -2);  // 마지막 CRLF 제거

//         // $_POST 배열에 추가
//         if (isset($name[1])) {
//             $_POST[$name[1]] = $value;
//         }
//     }
// }

// $num1 = isset($_POST['num1']) ? intval($_POST['num1']) : 0;
// $num2 = isset($_POST['num2']) ? intval($_POST['num2']) : 0;
// $operation = isset($_POST['operation']) ? $_POST['operation'] : 'add';

// 쿼리 문자열 가져오기
$queryString = $_SERVER['QUERY_STRING'];

// 쿼리 문자열 파싱
$params = [];
parse_str($queryString, $params);

// 파라미터 추출
$num1 = isset($params['num1']) ? intval($params['num1']) : 0;
$num2 = isset($params['num2']) ? intval($params['num2']) : 0;
$operation = isset($params['operation']) ? $params['operation'] : 'add';

$result = 0;
$operationSymbol = '';
switch ($operation) {
    case 'add':
        $operationSymbol = '+';
        $result = $num1 + $num2;
        break;
    case 'subtract':
        $operationSymbol = '-';
        $result = $num1 - $num2;
        break;
    case 'multiply':
        $operationSymbol = '×';
        $result = $num1 * $num2;
        break;
    case 'divide':
        if ($num2 != 0) {
            $operationSymbol = '÷';
            $result = $num1 / $num2;
        } else {
            $result = "Error: Cannot divide by zero";
        }
        break;
}

?>

<!DOCTYPE html>
<html lang="ko">
<head>
    <meta charset="UTF-8">
    <title>계산 결과</title>
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(to right, #6dd5ed, #2193b0);
            color: white;
            text-align: center;
            padding-top: 50px;
            margin: 0;
        }
        .result {
            display: inline-block;
            padding: 30px;
            border-radius: 15px;
            background-color: rgba(255, 255, 255, 0.9);
            box-shadow: 0 5px 20px rgba(0, 0, 0, 0.2);
            width: 300px;
            animation: appear 1s ease-in-out;
        }
        @keyframes appear {
            from {opacity: 0; transform: translateY(-20px);}
            to {opacity: 1; transform: translateY(0);}
        }
        h1 {
            margin-bottom: 20px;
            font-size: 24px;
            color: #30597f;
        }
        p {
            font-size: 1.2em;
            margin-bottom: 20px;
            color: #333;
        }
        .recalculate-btn {
            padding: 10px 20px;
            background-color: #4a77d4;
            color: white;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            text-decoration: none;
            font-weight: bold;
            transition: background-color 0.2s, transform 0.2s;
        }
        .recalculate-btn:hover {
            background-color: #3960a6;
            transform: scale(1.05);
        }
    </style>
</head>
<body>
    <div class="result">
        <h1>계산 결과</h1>
        <?php
        if (is_numeric($result)) {
            echo "<p>" . htmlspecialchars($num1) . " " . $operationSymbol . " " . htmlspecialchars($num2) . " = " . htmlspecialchars($result) . "</p>";
        } else {
            echo "<p>Error: " . htmlspecialchars($result) . "</p>";
        }
        ?>
        <a href="/calculate" class="recalculate-btn">한번 더 계산하기</a>
    </div>
</body>
</html>
