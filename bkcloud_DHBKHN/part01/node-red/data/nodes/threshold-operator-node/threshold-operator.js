module.exports = function (RED) {

    function ThesholdOperatorNode(config) {
        RED.nodes.createNode(this, config);
        this.temperatureLimit = config.temperatureLimit;
        var node = this;
        this.on("input", function (msg) {
            let deviceMacAddr = msg.payload.macAddr;
            //	C: [1, 100]; 	%: [0, 100],	lux: [1, 65535]
            let macAddr = msg.payload.macAddr;
            let sensorData = msg.payload.sensorsData;
            for (var i = 0; i < sensorData.length; i++) {
                let checkSensorData = sensorData[i];
                if (checkSensorData.unit == 'C') {
                    console.log(node.temperatureLimit);
                    let action = '';
                    let sensorValue = parseInt(checkSensorData.value);
                    if (sensorValue > node.temperatureLimit) {
                        action = 'ON';
                    } else {
                        action = 'OFF';
                    }
                    let sendMSg = {
                        payload: {
                            'type': 'ledAction',
                            'action': action
                        },
                        topic: 'esp/'+macAddr+'/action'
                    };
                    node.send(sendMSg);
                }
            }
        });
        this.on('close', function () {});
    }
    RED.nodes.registerType("threshold-operator", ThesholdOperatorNode);
};