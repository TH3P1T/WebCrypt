<?php
session_start();

$dbhost = "your_db";
$dbuser = "your_db_user";
$dbpass = "your_passwd";
$dbname = "your db_name";

$db = new PDO("mysql:dbname=$dbname;host=$dbhost", $dbuser, $dbpass, [
  PDO::MYSQL_ATTR_INIT_COMMAND => "SET NAMES utf8",
  PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
  PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_OBJ
]);
