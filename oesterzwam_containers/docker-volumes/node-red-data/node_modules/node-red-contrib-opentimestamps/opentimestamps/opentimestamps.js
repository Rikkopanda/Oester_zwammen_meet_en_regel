/**
 * opentimestamps.js
 *
 *
 * Requires javascript-opentimestamps
 * Copyright 2017 Valerio Vaccaro - www.valeriovaccaro.it
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

//import the requirements
const OpenTimestamps = require('javascript-opentimestamps');

module.exports = function(RED) {
  // Node for stamp a Buffer content
  function opentimestamps_stamp(n) {
    RED.nodes.createNode(this, n);
    this.status({
      fill: "grey",
      shape: "dot",
      text: "Create new timestamp ..."
    });
    var msg = {};
    var node = this;
    this.on("input", function(msg) {
      // import the file content like Buffer
      const stampResultPromise = OpenTimestamps.stamp(msg.fileArray);
      stampResultPromise.then(stampResult => {
        // convert the stampResult in a Buffer
        var ots = new Buffer(stampResult);
        msg.otsArray = ots;
        this.status({
          fill: "green",
          shape: "dot",
          text: "Done"
        });
        node.send(msg);
      });
    });
  }

  function opentimestamps_info(n) {
    RED.nodes.createNode(this, n);
    this.status({
      fill: "grey",
      shape: "dot",
      text: "Get timestamp info ..."
    });
    var msg = {};
    var node = this;
    this.on("input", function(msg) {
      const infoResult = OpenTimestamps.info(msg.otsArray);
      msg.payload = infoResult;
      this.status({
        fill: "green",
        shape: "dot",
        text: "Done"
      });
      node.send(msg);
    });
  }

  function opentimestamps_upgrade(n) {
    RED.nodes.createNode(this, n);
    this.status({
      fill: "grey",
      shape: "dot",
      text: "Uprading timestamp ..."
    });
    var msg = {};
    var node = this;
    this.on("input", function(msg) {
      var ots = msg.otsArray;
      const upgradePromise = OpenTimestamps.upgrade(ots);
      upgradePromise.then(timestampBytes => {
        if (ots.equals(timestampBytes)) {
          msg.payload = 'Timestamp not upgraded';
          this.status({
            fill: "yellow",
            shape: "dot",
            text: "Timestamp note upgraded"
          });
          //node.send(msg);
        } else {
          var newOts = new Buffer(timestampBytes);
          msg.otsArray = newOts;
          msg.payload = 'Timestamp upgraded';
          this.status({
            fill: "green",
            shape: "dot",
            text: "Timestamp upgraded"
          });
          node.send(msg);
        }
      }).catch(err => {
        console.log(err);
      });
    });
  }

  function opentimestamps_verify(n) {
    RED.nodes.createNode(this, n);
    this.status({
      fill: "grey",
      shape: "dot",
      text: "Verify a timestamp ... "
    });
    var msg = {};
    var node = this;
    this.on("input", function(msg) {
      const verifyResultPromise = OpenTimestamps.verify(msg.otsArray,
        msg.fileArray);
      verifyResultPromise.then(verifyResult => {
        // return a timestamp if verified, undefined otherwise.
        msg.payload = verifyResult;
        this.status({
          fill: "green",
          shape: "dot",
          text: "Done"
        });
        node.send(msg);
      });
    });
  }

  // Register the node by name. This must be called before overriding any of the
  // Node functions.
  RED.nodes.registerType("OTS_Stamp", opentimestamps_stamp);
  RED.nodes.registerType("OTS_Info", opentimestamps_info);
  RED.nodes.registerType("OTS_Upgrade", opentimestamps_upgrade);
  RED.nodes.registerType("OTS_Verify", opentimestamps_verify);
}
