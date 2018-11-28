<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
		    <link rel="stylesheet" href="stylesheets/style.css" />
        <title>- Cesium Crypter -</title>
		<script src='https://www.google.com/recaptcha/api.js'></script>
    </head>

    <body>
      <header>
    		<a href="index.php"><img src="img/logo.png" style="margin-top:10px;margin-bottom:5px;margin-left:30px;"/></a>
    	</header>
      <aside>
  			<center>Menu</center>
  			<!-- Menu -->
  			<table>
  				<tbody>
  		<?php
  		if (isset($_SESSION["user"])):
      ?>
					<tr>
						<td><img src="img/user.png" style="margin-top:5px;margin-right:10px;"/><a href="encrypt.php">Encrypt !</a></td>
					</tr>
					<tr>
						<td><img src="img/inscription.png" style="margin-top:5px;margin-right:10px;"/><a href="purchase.php">Products</a></td>
					</tr>
					<tr>
						<td><img src="img/logout.png" style="margin-top:5px;margin-right:10px;"/><a href="logout.php">Log out</a></td>
					</tr>
      <?php else: ?>
        <tr>
          <td><img src="img/home.png" style="margin-top:5px;margin-right:10px;"/><a href="index.php">Home</a></td>
        </tr>
        <tr>
          <td><img src="img/user.png" style="margin-top:5px;margin-right:10px;"/><a href="login.php">Signup / Login</a></td>
        </tr>
        <tr>
          <td><img src="img/inscription.png" style="margin-top:5px;margin-right:10px;"/><a href="purchase.php">Products</a></td>
        </tr>
      <?php
      endif;
  		?>
  				</tbody>
  			</table>
  		</aside>
      <main>
