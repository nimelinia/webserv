<?php
if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    $name = $_POST["name"];
    $address = $_POST["address"];
} else if ($_SERVER['REQUEST_METHOD'] == 'GET') {
    $name = $_GET["name"];
    $address = $_GET["address"];
}
?>

<html>
<title>Thank You from <?php echo $_SERVER['REQUEST_METHOD']; ?></title>
<h1>Thank you for filling out my form!</h1>
Thank you, <b><?php echo $name; ?></b>, for filling out my form!
I will mail information to <b><?php echo $address; ?></b> right away.
</html>