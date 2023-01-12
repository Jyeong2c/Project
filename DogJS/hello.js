//서버에 원하는 이미지를 저장

var express = require('express')
var fs = require('fs');

const app = express();

app.get('/', (request, response) => {
    fs.readFile('a.html', 'utf-8', (err, data) => { 
        if(err){response.send('No Such File of Directory')}
        response.send(data);
    })
});

app.get('/google', (req, res) => {
    fs.readFile('./images/google.png', (err, data) => {
        if(err){res.send()}
        res.send(data);
    })
});

app.listen(3000);