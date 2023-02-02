const express = require('express');
const server = express();

server.listen(3500, (err) => {
    if(err){
        return console.log(err);
    } else {
        console.log('running port 3500');
    }
});

server.get("/test", (req, res) => {
    res.json(
        {
            name : "Burak Hamdi",
            surname : "TUFAN",
            age : 26,
            address : 
            {
                city : "Istanbul",
                country : "TURKEY"
            },
            phone :
            [
                "05555555555",
                "01111111111"
            ]
        }
    )
})