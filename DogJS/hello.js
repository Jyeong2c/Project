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

app.get('/patient', (req, res) => {
    fs.readFile('b.html', 'utf-8', (err, data) => { 
        if(err){res.send('No Such File of Directory')}
        res.send(data);
    })
});

app.get('/patient/osstem', (req, res) => {
    fs.readFile('./images/osstem.png', (err, data) => {
        if(err){res.send()}
        res.send(data);
    })
});


var http = require('http'),
    Stream = require('stream').Transform;

var url1 = 'http://127.0.0.1:3000/google';
var url2 = 'http://127.0.0.1:3000/patient/osstem';



//다운받는 이미지 경로 설정

http.request(url1, function(response){
    var data = new Stream();

    response.on('data', function(chunk){
        data.push(chunk);
    });

    response.on('end', function(){
        //fs.writeFileSync('./downloaded/image1.png', data.read());
        fs.writeFileSync('D:/GitDesktop/Project/Downloaded/image1.png', data.read());
    });
}).end();

http.request(url2, function(response){
    var data = new Stream();

    response.on('data', function(chunk){
        data.push(chunk);
    });

    response.on('end', function(){
        //fs.writeFileSync('./downloaded/image2.png', data.read());
        fs.writeFileSync('D:/GitDesktop/Project/Downloaded/image2.png', data.read());
    });
}).end();

app.listen(3000, () => {
    console.log('Server running port http://127.0.0.1:3000/');
});