/*서버에 이미지 업로드 및 다운로드 & 환자정보 JSON 데이터 포멧*/

var express = require('express')                            //express 패키지 사용
var fs = require('fs');                                     //fs 패키지 사용

const app = express();                                      //app express 함수 사용

app.get('/', (request, response) => {                       //express 패키지 형태로 간단한 웹 페이지 구현
    //response.send('This is main page');
    fs.readFile('a.html', 'utf-8', (err, data) => {         //a.html 파일을 읽고 못찾는 경우 에러메세지 호출
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

/////////////////////////////////////////////////////////////////////////////////////////////////
// 환자1의 파노라마 사진과 정보를 해당 url에 저장

app.get('/patient/1', (req, res) => {
    fs.readFile('c.html', 'utf-8', (err, data) => { 
        if(err){res.send('No Such File of Directory')}
        res.send(data);
    });
});

app.get('/patient/1/panorama1', (req, res) => {
    fs.readFile('./images/panorama1.png', (err, data) => {
        if(err){res.send()}
        res.send(data);
    })
});

app.get('/patient/1/info', (req, res) => {      //환자의 정보는 json 형태로 웹서버에 업로드
    res.json(
        {
            id : 1,                         
            name : 'jae hyeon',
            age : 27,
            local : 'D:/GitDesktop/Project/Downloaded/panorama1.png'  //경로를 지정한 이유 : Qt클라이언트 내에서 이미지를 업로드할 경로가 필요  
        }
    )
});

/////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////
// 환자2의 파노라마 사진과 정보를 해당 url에 저장

app.get('/patient/2', (req, res) => {
    fs.readFile('d.html', 'utf-8', (err, data) => { 
        if(err){res.send('No Such File of Directory')}
        res.send(data);
    });
});

app.get('/patient/2/panorama2', (req, res) => {
    fs.readFile('./images/panorama2.png', (err, data) => {
        if(err){res.send()}
        res.send(data);
    })
});

app.get('/patient/2/info', (req, res) => {
    res.json(
        {
            id : 2,
            name : 'yu na',
            age : 25,
            local : 'D:/GitDesktop/Project/Downloaded/panorama2.png'    
        }
    )
});

/////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////
// 환자3의 파노라마 사진과 정보를 해당 url에 저장

app.get('/patient/3', (req, res) => {
    fs.readFile('e.html', 'utf-8', (err, data) => { 
        if(err){res.send('No Such File of Directory')}
        res.send(data);
    });
});

app.get('/patient/3/panorama3', (req, res) => {
    fs.readFile('./images/panorama3.png', (err, data) => {
        if(err){res.send()}
        res.send(data);
    })
});

app.get('/patient/3/info', (req, res) => {
    res.json(
        {
            id : 3,
            name : 'jae yeong',
            age : 28,
            local : 'D:/GitDesktop/Project/Downloaded/panorama3.png'    
        }
    )
});

/////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////
// 환자4의 파노라마 사진과 정보를 해당 url에 저장

app.get('/patient/4', (req, res) => {
    fs.readFile('f.html', 'utf-8', (err, data) => { 
        if(err){res.send('No Such File of Directory')}
        res.send(data);
    });
});

app.get('/patient/4/panorama4', (req, res) => {
    fs.readFile('./images/panorama4.png', (err, data) => {
        if(err){res.send()}
        res.send(data);
    })
});

app.get('/patient/4/info', (req, res) => {
    res.json(
        {
            id : 4,
            name : 'en ji',
            age : 29,
            local : 'D:/GitDesktop/Project/Downloaded/panorama4.png'    
        }
    )
});

/////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////

//다운로드 할 url 모음 및 json에 저장한 

var http = require('http'),
    Stream = require('stream').Transform;

var url1 = 'http://127.0.0.1:3000/google';
var url2 = 'http://127.0.0.1:3000/patient/osstem';
var url3 = 'http://127.0.0.1:3000/patient/1/panorama1';
var url4 = 'http://127.0.0.1:3000/patient/2/panorama2';
var url5 = 'http://127.0.0.1:3000/patient/3/panorama3';
var url6 = 'http://127.0.0.1:3000/patient/4/panorama4';

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

//환자 1의 이미지를 다운로드
http.request(url3, function(response){
    var data = new Stream();

    response.on('data', function(chunk){
        data.push(chunk);
    });

    response.on('end', function(){
        //fs.writeFileSync('./downloaded/image2.png', data.read());
        fs.writeFileSync('D:/GitDesktop/Project/Downloaded/panorama1.png', data.read());
    });
}).end();

//환자 2의 이미지를 다운로드
http.request(url4, function(response){
    var data = new Stream();

    response.on('data', function(chunk){
        data.push(chunk);
    });

    response.on('end', function(){
        //fs.writeFileSync('./downloaded/image2.png', data.read());
        fs.writeFileSync('D:/GitDesktop/Project/Downloaded/panorama2.png', data.read());
    });
}).end();

//환자 3의 이미지를 다운로드
http.request(url5, function(response){
    var data = new Stream();

    response.on('data', function(chunk){
        data.push(chunk);
    });

    response.on('end', function(){
        //fs.writeFileSync('./downloaded/image2.png', data.read());
        fs.writeFileSync('D:/GitDesktop/Project/Downloaded/panorama3.png', data.read());
    });
}).end();

//환자 4의 이미지를 다운로드
http.request(url6, function(response){
    var data = new Stream();

    response.on('data', function(chunk){
        data.push(chunk);
    });

    response.on('end', function(){
        //fs.writeFileSync('./downloaded/image2.png', data.read());
        fs.writeFileSync('D:/GitDesktop/Project/Downloaded/panorama4.png', data.read());
    });
}).end();

app.listen(3000, () => {
    console.log('Server running port http://127.0.0.1:3000/');
});