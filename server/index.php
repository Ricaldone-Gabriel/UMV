    <?php

    include("./views/partials/databaseConfig.php");

    $dataPoints1 = array();
    $dataPoints2 = array();
    $dataPoints3 = array();

    $sql = "SELECT Temperature,Time FROM enviroment";

    if ($result = mysqli_query($con, $sql)) {
        foreach ($result as $row) {
            array_push($dataPoints1, array("x" => strtotime($row["Time"]) * 1000, "y" => $row["Temperature"]));
        }
    }

    $sql = "SELECT Humidity,Time FROM enviroment";

    if ($result = mysqli_query($con, $sql)) {
        foreach ($result as $row) {
            array_push($dataPoints2, array("x" => strtotime($row["Time"]) * 1000, "y" => $row["Humidity"]));
        }
    }

    $sql = "SELECT Pressure,Time FROM enviroment";

    if ($result = mysqli_query($con, $sql)) {
        foreach ($result as $row) {
            array_push($dataPoints3, array("x" => strtotime($row["Time"]) * 1000, "y" => $row["Pressure"]));
        }
    }
    mysqli_close($con);
    ?>

    <!DOCTYPE HTML>
    <html>

    <head>
        <?php include "./views/partials/styleConfig.php" ?>
    </head>


    <script>
        window.onload = function() {
            var chart1 = new CanvasJS.Chart("chartContainer1", {
                zoomEnabled: true,
                animationEnabled: true,
                exportEnabled: false,
                theme: "dark1", // "light1", "light2", "dark1", "dark2"
                title: {
                    text: "Valori ambientali acquisiti"
                },
                axisY: {
                    title: "Temperatura",
                    titleFontColor: "#C0504E",
                    lineColor: "#C0504E",
                    labelFontColor: "#C0504E",
                    tickColor: "#C0504E",
                },

                axisY2: {
                    title: "Umidità",
                    titleFontColor: "#4F81BC",
                    lineColor: "#4F81BC",
                    labelFontColor: "#4F81BC",
                    tickColor: "#4F81BC",
                },
                data: [{
                        type: "line", //change type to bar, line, area, pie, etc  
                        name: "Temperatura",
                        xValueType: "dateTime",
                        dataPoints: <?php echo json_encode($dataPoints1, JSON_NUMERIC_CHECK); ?>
                    },
                    {
                        type: "line", //change type to bar, line, area, pie, etc  
                        name: "Umidità",
                        axisYType: "secondary",
                        xValueType: "dateTime",
                        dataPoints: <?php echo json_encode($dataPoints2, JSON_NUMERIC_CHECK); ?>
                    }
                ]

            });
            var chart2 = new CanvasJS.Chart("chartContainer2", {
                zoomEnabled: true,
                animationEnabled: true,
                exportEnabled: false,
                theme: "dark1", // "light1", "light2", "dark1", "dark2"
                axisY: {
                    title: "Pressione",
                    titleFontColor: "#4F81BC",
                    lineColor: "#4F81BC",
                    labelFontColor: "#4F81BC",
                    tickColor: "#4F81BC",
                },
                data: [{
                    type: "line", //change type to bar, line, area, pie, etc  
                    name: "Pressione",
                    xValueType: "dateTime",
                    dataPoints: <?php echo json_encode($dataPoints3, JSON_NUMERIC_CHECK); ?>
                }]

            });
            chart1.render();
            chart2.render();
        }
    </script>

    <body class="d-flex flex-column min-vh-100">
        <?php include "./views/partials/navbar.php" ?>

        <div class="container-fluid">
            <!-- Sezione 1 -->
            <div class="row section no-gutters" id="section1">
                <div class="col-lg-6 order-lg-1 p-0 d-flex align-items-center">
                    <img src="https://via.placeholder.com/1000x1000" alt="Immagine 1">
                </div>
                <div class="col-lg-6 order-lg-2 d-flex align-items-center content">
                    <div>
                        <h2>Sezione 1</h2>
                        <p>Testo della prima sezione. Questa è una descrizione della sezione 1.</p>
                    </div>
                </div>
            </div>
            <!-- Sezione 2 -->
            <div class="row section no-gutters" id="section2">
                <div class="col-lg-6 order-lg-2 p-0 d-flex align-items-center">
                    <img src="https://via.placeholder.com/1000x1000" alt="Immagine 2">
                </div>
                <div class="col-lg-6 order-lg-1 d-flex align-items-center content">
                    <div>
                        <h2>Sezione 2</h2>
                        <p>Testo della seconda sezione. Questa è una descrizione della sezione 2.</p>
                    </div>
                </div>
            </div>
            <!-- Sezione 3 -->
            <div class="row section no-gutters" id="section3">
                <div class="col-lg-6 order-lg-1 p-0 d-flex align-items-center">
                    <img src="https://via.placeholder.com/1000x1000" alt="Immagine 3">
                </div>
                <div class="col-lg-6 order-lg-2 d-flex align-items-center content">
                    <div>
                        <h2>Sezione 3</h2>
                        <p>Testo della terza sezione. Questa è una descrizione della sezione 3.</p>
                    </div>
                </div>
            </div>
            <!-- Sezione 4 -->
            <div class="row section no-gutters" id="section4">
                <div class="col-lg-6 order-lg-2 p-0 d-flex align-items-center">
                    <img src="https://via.placeholder.com/1000x1000" alt="Immagine 4">
                </div>
                <div class="col-lg-6 order-lg-1 d-flex align-items-center content">
                    <div>
                        <h2>Sezione 4</h2>
                        <p>Testo della quarta sezione. Questa è una descrizione della sezione 4.</p>
                    </div>
                </div>
            </div>
            <div class="row" id="section5">

                <div id="chartContainer1" style="height: 370px; width: 100%;"></div>
                <div id="chartContainer2" style="height: 370px; width: 100%;"></div>
            </div>
        </div>
        <script src="https://cdn.canvasjs.com/canvasjs.min.js"></script>
        <script>
            let currentSection = 1;
            const totalSections = 5;

            function scrollToSection(sectionId) {
                document.getElementById(sectionId).scrollIntoView({
                    behavior: 'smooth'
                });
            }

            function handleScroll(event) {
                event.preventDefault();
                if (event.deltaY > 0) {
                    // Scroll down
                    if (currentSection < totalSections) {
                        currentSection++;
                    }
                } else {
                    // Scroll up
                    if (currentSection > 1) {
                        currentSection--;
                    }
                }
                scrollToSection('section' + currentSection);
            }

            window.addEventListener('wheel', handleScroll, {
                passive: false
            });
        </script>
        <?php include "./views/partials/footer.php" ?>
    </body>

    </html>