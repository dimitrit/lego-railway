# lego-railway

A simple [Lego Trains]("https://en.wikipedia.org/wiki/Lego_Trains#Power_Functions_trains") layout controlled by [Arduino Nano]("https://www.arduino.cc/en/Main/ArduinoBoardNano") clones, designed and built during the 2016 Christmas break.

Trains request access to the single track section by means of RFID tags stuck to the engine motors. The tags are read by [MFRC522]("https://www.nxp.com/documents/data_sheet/MFRC522.pdf") based RFID readers in the passing sections of the track. Should the single track section be clear, the points are set as required and the train is sent on its way. While the single track section is occupied, any further requests for access to the section are queued.

### Circuit diagrams

Circuit diagrams (created using [Eagle]("https://cadsoft.io/")) for the RFID reader and train and points controller can be found in the `eagle` directory. For convenience diagrams are also provided in PNG format. [More...](docs/circuits.md)

### Custom Lego parts

Custom 3D-printed Lego parts are required to switch the points. Additional custom parts include track sections to hold the RFID reader, and signals which double up as IR transmitters. [More...](docs/parts.md)

### License

This code is open source software, licensed under the [Apache 2.0 License]("http://www.apache.org/licenses/LICENSE-2.0.html").