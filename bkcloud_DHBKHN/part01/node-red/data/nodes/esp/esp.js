module.exports = function (RED) {
    "use strict";

    function EspInNode(config) {
        RED.nodes.createNode(this, config);
        this.qos = 2;
        this.mqttBroker = config.broker;
        this.brokerConn = RED.nodes.getNode(this.mqttBroker);
        this.topic = config.topic;
        console.log(this.topic);
        var node = this;
        if (this.brokerConn) {
            this.status({
                fill: "red",
                shape: "ring",
                text: "disconnected"
            });
            node.brokerConn.register(this);
            this.brokerConn.subscribe(this.topic, this.qos, function (topic, payload, packet) {
                var sendMsg = {
                    'payload': JSON.parse(payload)
                };
                node.send(sendMsg);
            }, this.id);
            if (this.brokerConn.connected) {
                node.status({
                    fill: "green",
                    shape: "dot",
                    text: "node-red:common.status.connected"
                });
            }
            this.on('close', function (done) {
                if (node.brokerConn) {
                    node.brokerConn.unsubscribe(node.topic, node.id);
                    node.brokerConn.deregister(node, done);
                    node.log("Input node is closed!");
                }
            });
        } else {
            this.error(RED._("mqtt.errors.missing-config"));
        }
    }
    RED.nodes.registerType("esp-in", EspInNode);

    function EspOutNode(config) {
        RED.nodes.createNode(this, config);
        this.qos = 2;
        this.mqttBroker = config.broker;
        this.brokerConn = RED.nodes.getNode(this.mqttBroker);
        this.topic = config.topic;
        var node = this;
        if (this.brokerConn) {
            this.status({
                fill: "red",
                shape: "ring",
                text: "disconnected"
            });
            node.brokerConn.register(this);

            this.on("input", function (msg) {
                if (node.topic) {
                    msg.topic = node.topic;
                }
                if (msg.hasOwnProperty("payload")) {
                    if (msg.hasOwnProperty("topic") && (typeof msg.topic === "string") && (msg.topic !== "")) { // topic must exist
                        this.brokerConn.publish(msg); // send the message
                    } else {
                        node.warn(RED._("mqtt.errors.invalid-topic"));
                    }
                }
            });

            if (this.brokerConn.connected) {
                node.status({
                    fill: "green",
                    shape: "dot",
                    text: "node-red:common.status.connected"
                });
            }
            this.on('close', function (done) {
                if (node.brokerConn) {
                    node.brokerConn.unsubscribe(node.topic, node.id);
                    node.brokerConn.deregister(node, done);
                    node.log("Input node is closed!");
                }
            });
        } else {
            this.error(RED._("mqtt.errors.missing-config"));
        }
    }
    RED.nodes.registerType("esp-out", EspOutNode);
};