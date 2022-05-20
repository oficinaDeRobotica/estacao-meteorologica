<?php
/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

//declarando dados do banco de dados
$servername = "127.0.0.1";
$dbname = "esp_data";
$username = "esp_data";
$password = "eujogodemf";

//declarando chave da api, precisa ser o mesmo no codigo do esp
$api_key_value = "tPmAT5Ab3j7F9";

//criando variaveis
$api_key = $value1 = $value2 = $value3 = $value4 = $value5 = $value6 = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {

	$api_key = test_input($_POST["api_key"]);

	if($api_key == $api_key_value){
		$value1 = test_input($_POST["value1"]);
		$value2 = test_input($_POST["value2"]);
		$value3 = test_input($_POST["value3"]);
		$value4 = test_input($_POST["value4"]);
		$value5 = test_input($_POST["value5"]);
		$value6 = test_input($_POST["value6"]);

		//conectar
		$conn = new mysqli($servername, $username, $password, $dbname);
		//checar conexão
		if($conn->connect_error){
			die("Conexão falhou: " . $conn->connect_error);
		}

		$sql = "INSERT INTO sensor (value1, value2, value3, value4, value5, value6) VALUES ('" . $value1 . "', '" . $value2 . "', '" . $value3 . "', '" . $value4 . "', '" . $value5 . "', '" . $value6 . "')";

		if($conn->query($sql) === TRUE) {
			echo "Valores salvos com sucesso";
		} else {
			echo "Erro: " . $sql . "<br>" . $conn->error;
		}

		$conn->close();
	}
	else {
		echo "Chave API errada";
	}
}
else {
	echo "Nada veio junto com a requisição POST";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
