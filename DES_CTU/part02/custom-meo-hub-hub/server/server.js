var mqtt = require('mqtt')
var express = require('express');
var bodyParser = require('body-parser');
var path = require('path');

const MQTT_URL = 'mqtt://localhost:1883';
const CLIENT_LIGHTS_OUT_TOPIC = 'client/lights/out';

var client  = mqtt.connect(MQTT_URL);

client.on('connect', function () {
    console.log("Connected MQTT Broker at " + MQTT_URL);
})

var app = express();

app.use(express.static(__dirname + "/public"));
app.use(bodyParser.json());

app.get('/', function(req, res){
    res.sendFile(path.join(__dirname + '/index.html'));
});

app.put('/lights', function(req, res) {
    var payload  = req.body;
    console.log("Payload: ", payload);
    client.publish(CLIENT_LIGHTS_OUT_TOPIC, payload.lights.join(";"));
    res.json(payload);
});

app.listen(8080);
console.log("Server is running on: http://localhost:8080");