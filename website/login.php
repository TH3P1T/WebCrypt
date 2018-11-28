<?php
include 'inc/base.php';

function captcha() {
  $url = 'https://www.google.com/recaptcha/api/siteverify';
  $privatekey = '6LdXGA8TAAAAAEItg6VkhIwYXswy3zoJYkSsa2bT';
  $response = file_get_contents($url."?secret=".$privatekey."&response=".strip_tags($_POST['g-recaptcha-response'])."&remoteip=".$_SERVER['REMOTE_ADDR']);
  $data = json_decode($response);
  return isset($data->success) AND $data->success == true;
}

// LOGIN
if (isset($_POST['login'], $_POST['username'], $_POST['password'])) {
    $query = $db->prepare('SELECT COUNT(*) AS nb, id, username FROM users WHERE username=? AND password=?');
    $query->execute([
      $_POST['username'],
      md5($_POST['password'])
    ]);
    $data = $query->fetch();
    if ($data->nb) {
        $_SESSION['user'] = $data->username;
        $_SESSION['user_id'] = $data->id;
        header("location: index.php");
        exit;
    } else {
      $error = "Password or username incorrect";
    } 
}

// SIGNUp
if (isset($_POST['signup'], $_POST['username'], $_POST['email'], $_POST['password'], $_POST['confirm'])) {
  if (preg_match("#^[a-z0-9_-]{3,30}$#i", $_POST['username'])) {
	  if (captcha()) {
    $query = $db->prepare("SELECT COUNT(*) AS nb FROM users WHERE username=?");
    $query->execute([
      $_POST['username']
    ]);
    $data = $query->fetch();

    if (!$data->nb) {
      if (filter_var($_POST['email'], FILTER_VALIDATE_EMAIL)) {
        $query = $db->prepare("SELECT COUNT(*) AS nb FROM users WHERE email=?");
        $query->execute([
          $_POST['email']
        ]);
        $data = $query->fetch();

        if (!$data->nb) {
          if ($_POST['password'] === $_POST['confirm']) {
            if (strlen($_POST['password']) >= 4) {

              $query = $db->prepare("INSERT INTO users(username, email, password) VALUES(:username, :email, :password)");
              $query->execute([
                "username" => $_POST['username'],
                "email" => $_POST['email'],
                "password" => md5($_POST['password'])
              ]);

              $_SESSION['user'] = $_POST['username'];
              $_SESSION['user_id'] = $db->lastInsertId();

              header("Location: index.php");
              exit;

            } else {
              $error = "The password must be at least 4 characters";
            }
          } else {
            $error = "The two passwords must match";
          }
        } else {
          $error = "This email is already taken";
        }
      } else {
        $error = "Incorrect email format";
      }
    } else {
      $error = "This username is already taken";
    }
  }
  else 
  {
	  $error = "Invalid captcha";  
  }
  }
  else
  {
	$error = "The username must be within 3 and 30 characters";
  }
}

include 'inc/header.php';
?>
    <?php
    if (isset($error)) echo "<div class=\"message_erreur\">$error</div>";
    ?>
		<section>
			<h1 class="title">Log in</h1>
			<form method="POST">
  			Username : <input type="text" name="username" required />
  			Password : <input type="password" name="password" required />
        <input type="submit" name="login" value="Se connecter" required />
	    </form>
		</section>

		<section>
  			<h2 class="title">Sign Up</h2>
		    <form method="POST">
            <label>Username :<br />
              <input type="text" name="username" <?php if(isset($_POST['username'])) echo 'value="'.htmlspecialchars($_POST['username']).'"'; ?> required />
            </label>
            <label>Email :<br />
              <input type="text" name="email" <?php if(isset($_POST['email'])) echo 'value="'.htmlspecialchars($_POST['email']).'"'; ?> required />
            </label>
            <label>Password :<br />
              <input type="password" name="password" required />
            </label>
            <label>Repeat password :<br />
              <input type="password" name="confirm" required />
            </label>
			<center><div class="g-recaptcha" data-sitekey="6LdXGA8TAAAAAKV0MvCvn5eT3--7v4eipyvKPD0s"></div></center><br/>
            <input type="submit" name="signup" value="Créer un compte" />
        </form>
		</section>
<?php
include 'inc/footer.php';
