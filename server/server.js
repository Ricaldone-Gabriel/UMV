const express = require("express");
const app = express();
const path = require("path");
const bodyParser = require("body-parser");
const http = require("http");
const ejs = require("ejs");
const port = 3000;

app.set("view engine", "ejs");
app.use(express.static("public"));
app.use("/", express.static(__dirname + "/views"));
app.set("views", path.join(__dirname, "views"));
app.use(bodyParser.urlencoded({ extended: true }));

app.get("/", (req, res) => res.send("Hello World!"));

app.listen(port, () => console.log(`Example app listening on port ${port}!`));
