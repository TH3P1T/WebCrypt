<?php 

function getPostVariable($sKey)
{
    return (isset($_POST[$sKey]) ? $_POST[$sKey] : false);
}

$dbhost = "mysql-senkai6259.alwaysdata.net";
$dbuser = "103840";
$dbpass = "nicolas62";
$dbname = "senkai6259_ddb";

mysql_connect($dbhost,$dbuser,$dbpass);
mysql_select_db($dbname);

$ipnSecret = getPostVariable('ipn_secret');
$status = getPostVariable('status');
$aboubakar = $_POST['custom_form_fields']['cesium_crypter_username'];

   // Open the text file
        $f = fopen("test1.txt", "w");
 
        // Write text
        fwrite($f, $aboubakar);
        fwrite($f, $ipnSecret);
        fwrite($f, $status);
 
        // Close the text file
        fclose($f);

if ($ipnSecret == "82ba417d4571711bb33e581879cced9f" && $status == "complete")
{
	mysql_query("UPDATE users SET timestamp=" . time() . " WHERE pseudo ='" . $aboubakar . "'");
}
?>