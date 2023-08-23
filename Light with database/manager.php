<?php
    include("config.php");

    if($_GET["func"]=="insert"){
        $mysqli = new mysqli($dbhost, $dbusername, $dbpassword, $dbname);

        if (mysqli_connect_errno()) {
            printf("Connect failed: %s\n", mysqli_connect_error());
            exit();
        }

        $value;
        $query = "SELECT hodnota FROM led_data ORDER BY id DESC LIMIT 1;";
        if ($stmt = $mysqli->prepare($query)) {
            $stmt->execute();
            $stmt->bind_result($hodnota);
            if($hodnota == 'zapnute'){
                $value='vypnute';
                echo $value;
            }
            else{
                $value='zapnute';
                echo $value;
            }
            echo $hodnota;
            $stmt->close();
        }

        $mysqli->close();

    }

    if($_GET["func"]=="stav"){
        $mysqli = new mysqli($dbhost, $dbusername, $dbpassword, $dbname);

        if (mysqli_connect_errno()) {
            printf("Connect failed: %s\n", mysqli_connect_error());
            exit();
        }

        $query = "SELECT hodnota FROM led_data ORDER BY id DESC LIMIT 1;";
        if ($stmt = $mysqli->prepare($query)) {
            $stmt->execute();
            $stmt->bind_result($hodnota);
            echo $hodnota;
            $stmt->close();
        }

    }
?>