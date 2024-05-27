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

            <div id="chartContainer1" style="height: 370px; width: 100%;"></div>
            <div id="chartContainer2" style="height: 370px; width: 100%;"></div>
        </div>
        <script src="https://cdn.canvasjs.com/canvasjs.min.js"></script>
        <?php include "./views/partials/footer.php" ?>
    </body>

    </html>