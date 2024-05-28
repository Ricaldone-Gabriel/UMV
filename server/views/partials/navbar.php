<nav class="navbar navbar-expand-lg bg-dark sticky-top" data-bs-theme="dark">
    <div class="container-fluid">
        <a class="navbar-brand" href="#">UMV</a>
        <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarNav" aria-controls="navbarNav" aria-expanded="false" aria-label="Toggle navigation">
            <span class="navbar-toggler-icon"></span>
        </button>
        <div class="collapse navbar-collapse" id="navbarNav">
            <ul class="navbar-nav me-auto mb-2 mb-lg-0">
                <li class="nav-item">
                    <a class="nav-link" aria-current="page" href="https://github.com/Ricaldone-Gabriel/UMV">GitHub</a>
                </li>
                <li class="nav-item">
                    <a class="nav-link" href="https://docs.google.com/document/d/1iNaTyI0aZtK-PkAiF6tzos07V4uVW1t3yA3OTu3w6TY/edit?usp=sharing">
                        <?php
                        switch ($_SESSION["language"]) {
                            case "ita":
                                echo "Documento tecnico";
                                break;
                            case "eng":
                                echo "Technical document";
                                break;
                        }
                        ?>
                    </a>
                </li>
            </ul>
            <ul class="navbar-nav">
                <li class="nav-item dropdown ">
                    <a class="nav-link dropdown-toggle active " href="#" role="button" data-bs-toggle="dropdown" aria-expanded="false">
                        <?php
                        switch ($_SESSION["language"]) {
                            case "ita":
                                echo "Lingue";
                                break;
                            case "eng":
                                echo "Languages";
                                break;
                        }
                        ?>
                    </a>
                    <ul class="dropdown-menu dropdown-menu-end">
                        <form action="<?php $_SERVER['PHP_SELF'] ?>" method="post">
                            <li><button class="dropdown-item" name="englishButton">English</button></li>
                            <li><button class="dropdown-item" name="italianButton">Italian</button></li>
                        </form>
                    </ul>
                </li>
            </ul>
        </div>
    </div>
</nav>