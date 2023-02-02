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
                "patient": {
                    "ID": "001",
                    "Name": "Jeong jae hyeon",
                    "Age": 27,
                    "DoctorID": "osstem1",
                    "PhotoDate": "2023-01-07",
                    "ImageListURL": "http://192.168.0.12:4000/patient/001/image"
                }
            },
            {
                "patient": {
                    "ID": "002",
                    "Name": "Jeong yu na",
                    "Age": 25,
                    "DoctorID": "osstem1",
                    "PhotoDate": "2023-01-07",
                    "ImageListURL": "http://192.168.0.12:4000/patient/002/image"
                }
            },
            {
                "patient": {
                    "ID": "003",
                    "Name": "Jo jae yeong",
                    "Age": 28,
                    "DoctorID": "osstem1",
                    "PhotoDate": "2023-01-07",
                    "ImageListURL": "http://192.168.0.12:4000/patient/003/image"
                }
            },
            {
                "patient": {
                    "ID": "004",
                    "Name": "Han en ji",
                    "Age": 29,
                    "DoctorID": "osstem1",
                    "PhotoDate": "2023-01-07",
                    "ImageListURL": "http://192.168.0.12:4000/patient/004/image"
                }
            }
        ]
    )
})