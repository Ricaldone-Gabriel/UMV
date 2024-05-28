<?php
session_start();

if (!isset($_SESSION['language'])) {
    $_SESSION['language'] = "ita";
}

if (isset($_POST["italianButton"])) {
    $_SESSION['language'] = "ita";
}
if (isset($_POST["englishButton"])) {
    $_SESSION['language'] = "eng";
}
