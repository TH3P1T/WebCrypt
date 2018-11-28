<?php
require 'inc/base.php';

if (!isset($_SESSION['user_id'])) {
  header("Location: index.php"); exit;
}

include 'inc/header.php';
?>
		<section>
			<h1 class="title">Encrypt now</h1>
				<?php
        $query = $db->prepare("SELECT timestamp FROM users WHERE id=?");
        $query->execute([$_SESSION['user_id']]);
        $data = $query->fetch();

				if ($data->timestamp == 0) {
					echo "<div class=message_erreur>You can't access to this, please buy our crypter.</div>";
				} elseif (time() - $data->timestamp > 1296000) {
            // 15 jours
					echo "<div class=message_erreur>Your right to use Cesium Crypter has expired. Please buy again.</div>";
				} else {
          ?>
				  <div class="message_erreur">To keep it FUD, please don't check the file with virustotal.com. Use refud.me</div>
          <form action="upload.php" method="post" enctype="multipart/form-data">
          	Select file to upload:
          	<input type="hidden" name="MAX_FILE_SIZE" value="100485760" />
          	<input name="filexls" type="file">
          	<input type="submit" value="Upload" name="submit">
          </form>
          <?php
				}
				?>
		</section>
<?php
include 'inc/footer.php';
