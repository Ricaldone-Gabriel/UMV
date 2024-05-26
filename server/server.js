const express = require("express");
const app = express();
var plotly = require("plotly.express");
const path = require("path");
const bodyParser = require("body-parser");
const http = require("http");
const ejs = require("ejs");
const port = 80;

app.set("view engine", "ejs");
app.use(express.static("public"));
app.use("/", express.static(__dirname + "/views"));
app.set("views", path.join(__dirname, "views"));
app.use(bodyParser.urlencoded({ extended: true }));

app.get("/", (req, res) => {
  res.render("index.ejs");
});

app.listen(port, () => console.log(`Example app listening on port ${port}!`));
