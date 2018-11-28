<?php
include 'inc/base.php';

include 'inc/header.php';
?>
  		<section>
  			<form action="handle.php" method="POST">
  			ipn <input type="text" name="ipn_secret" />
  			status : <input type="text" name="status" />
        <input type="submit" name="login" value="submit" required />
	    </form>
  		</section>
<?php
include 'inc/footer.php';
?>
