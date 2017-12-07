module.exports = function (RED) {

    function MeoEspOutNode(config) {
        RED.nodes.createNode(this, config);
        this.temperatureLimit = config.temperatureLimit;
        this.humidityLimit = config.humidityLimit;
        this.luxLimit = config.luxLimit;
        var node = this;
        this.findSensor = function (inputSensors, unit) {
            for (var i = 0; i < inputSensors.length; i++) {
                if (inputSensors[i].unit == unit) {
                    return inputSensors[i];
                }
            }
        }
        this.on("input", function (msg) {
            //	C: [1, 100]; 	%: [0, 100],	lux: [1, 65535]
            let macAddr = msg.payload.macAddr;
            let sensorsData = msg.payload.sensorsData;
            let motionSensor = node.findSensor(sensorsData, "");
            let temperatureSensor = node.findSensor(sensorsData, "C");
            let humiditySensor = node.findSensor(sensorsData, "%");
            let lightSensor = node.findSensor(sensorsData, "Lux");
            let controlMsg = {
                'type': 'ledAction',
                'greenLed': 'OFF',
                'redLed': 'OFF',
                'yellowLed': 'OFF',
            };
            if (motionSensor.value == true) {
                if (temperatureSensor.value > node.temperatureLimit) {
                    controlMsg.greenLed='ON';
                }
                if (humiditySensor.value > node.humidityLimit) {
                    controlMsg.redLed='ON';
                }
                if (lightSensor.value > node.luxLimit) {
                    controlMsg.yellowLed='ON';
                }
            }
            let sendMSg = {
                payload: controlMsg,
                topic: 'icse/' + macAddr + '/action',
            };
            node.send(sendMSg);
        });
        this.on('close', function () {});
    }
    RED.nodes.registerType("meo-esp-out", MeoEspOutNode);
};