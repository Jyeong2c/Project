const express = require('express');
const server = express();

server.listen(3500, (err) => {
    if (err) {
        return console.log(err);
    } else {
        console.log('running port 3500');
    }
});

server.get("/test", (req, res) => {
    res.json(
        [
            {
                "Image": {
                    "ID": "8015",
                    "ImageName": "panorama8",
                    "PixelLength": 0.001,
                    "ImageKinds": "panorama",
                    "ImagePathURL": "http://192.168.0.12:4000/Files/panorama8.png"
                }
            }
        ]
    )
})