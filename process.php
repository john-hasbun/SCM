<!DOCTYPE html>
<html lang="en">
<head>
    <title>SCM</title>
    <link rel="stylesheet" href="scm.css">
    <meta charset="utf-8">
    <meta name="description" content="This is an SEO Content Manager. Upload your homepage and the webapp will create as many pages as necessary.">
    <meta name="author" content="Jonathan Hasbun">
</head>
<body>
    <!-- CONTAINER -->
    <div id="container">

        <!-- HEADER -->
        <header>
            <p id="banner">SEO Content Manager</p>
        </header>

        <!-- CONTENT -->
        <div id="body">
            <section id="content">
                <?php
                    $city=$_POST['city'];
                    $title=$_POST['title'];
                    $file_result = "";

                    echo "<div name=\"output\">";


                    # This checks for errors.
                    if ($_FILES["file"]["error"] > 0) {
                        $file_result .= "No File Uploaded or Invalid File ";
                        $file_result .= "Error Code:" . $FILES["file"]["error"] . "<br>";
                    } else {
                        $file_result .=
                        "Upload: " . $_FILES["file"]["name"] . "<br>" .
                        "Type: " . $_FILES["file"]["type"] . "<br>" .
                        "Size: " . ($_FILES["file"]["size"] / 1024) . " KB<br>" .
                        "Temp file: " . $_FILES["file"]["tmp_name"] . "<br><br>" .
                        "File Upload Successful!";    
                    }

                    echo "<p>$file_result</p><br><hr><br>";


                    # This stores the system command used to convert the page into multiple pages.
                    $create="sudo /var/www/parse \"" . $city . "\" " . $title . " " . $_FILES["file"]["tmp_name"];
                    # This stores the system command used to compress the landing pages folder.
                    $zip="sudo tar -zcf pages/" . $title . ".tar.gz " . "pages/" . $title;

                    exec("$create");
                    exec("$zip");

                    $folder="pages/" . $title . ".tar.gz";
                    echo "<a href=\"$folder\" download=\"$title\">Download</a>";
                    echo "</div>";
                ?>
            </section>
        </div>

        <!-- FOOTER -->
        <footer>
            <p></p>
        </footer>
    </div>
</body>
</html>
