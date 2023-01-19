var express = require('express')                            //express 패키지 사용
var fs = require('fs');                                     //fs 패키지 사용
// var http = require('http');
// var url = require('url');

// var mime = require('mime');
const app = express();                                      //app express 함수 사용

//Server 시작 및 메인 페이지
app.get('/', (req, res) => {                       //express 패키지 형태로 간단한 웹 페이지 구현
    res.send('This is Server Main Page');
});

var hostName = '192.168.0.48'
var portNum = 3000;

////////////////////////////////////////////////////////////////////////////////////////////////////////////

//이미지 파일 서버 업로드 부분
app.get('/Files/:id', (req, res) => {

    res.writeHead(200, {'Content-Type':'image/png'});
    fs.readFile(__dirname + '\\Files\\' + req.params.id, (err, data) => {
      if (err) {
        return console.error(err);
      }
      res.end(data, 'utf-8');
    });
  });

// var server = http.createServer(function(req, res){
//     var parsedUrl = url.parse(req.url);
//     var resource = parsedUrl.pathname;

//     if(resource.indexOf('/Files/') == 0){
//         var imgPath = resource.substring(1);
//         console.log('imgPath='+imgPath);
//         var imgMime = mime.getType(imgPath);
//         console.log('mime='+imgMime);

//         fs.readFile(imgPath, function(error, data){
//             if(error){
//                 res.writeHead(500, {'Content-Type' : 'application/json'});
//                 res.end('500 Internal Server ' + error);
//             }else{
//                 res.writeHead(200, {'Content-Type' : imgMime});
//                 res.end(data);
//             }
//         });
//     }else{
//         res.writeHead(404, {'Content-Type':'application/json'});
//         res.end('404 Page Not Found');
//     }
// });

// server.listen(3500, function(){
//     console.log('File Server running port http://127.0.0.1:3500/Files/');
// });

////////////////////////////////////////////////////////////////////////////////////////////////////////////

//환자 정보 리스트
var patients = [
    {ID : '001', Name : 'Jeong jae hyeon', Age : 27, DoctorID : 'osstem1', PhotoDate : '2023-01-07', ImageListURL : 'http://' + hostName + ':' + portNum + '/patient/001/image'},
    {ID : '002', Name : 'Jeong yu na', Age : 25, DoctorID : 'osstem1', PhotoDate : '2023-01-07', ImageListURL : 'http://' + hostName + ':' + portNum + '/patient/002/image'},
    {ID : '003', Name : 'Jo jae yeong', Age : 28, DoctorID : 'osstem1', PhotoDate : '2023-01-07', ImageListURL : 'http://' + hostName + ':' + portNum + '/patient/003/image'},
    {ID : '004', Name : 'Han en ji', Age : 29, DoctorID : 'osstem1', PhotoDate : '2023-01-07', ImageListURL : 'http://' + hostName + ':' + portNum + '/patient/004/image'},
    {ID : '005', Name : 'An da mi ro', Age : 29, DoctorID : 'osstem2', PhotoDate : '2023-01-08', ImageListURL : 'http://' + hostName + ':' + portNum + '/patient/005/image'},
    {ID : '006', Name : 'Bark byeong gyu', Age : 30, DoctorID : 'osstem2', PhotoDate : '2023-01-08', ImageListURL : 'http://' + hostName + ':' + portNum + '/patient/006/image'},
    {ID : '007', Name : 'Kim yu sun', Age : 28, DoctorID : 'osstem2', PhotoDate : '2023-01-08', ImageListURL : 'http://' + hostName + ':' + portNum + '/patient/007/image'},
    {ID : '008', Name : 'Lee jeong yeon', Age : 25, DoctorID : 'osstem2', PhotoDate : '2023-01-08', ImageListURL : 'http://' + hostName + ':' + portNum + '/patient/008/image'},
];
////////////////////////////////////////////////////////////////////////////////////////////////////////////


app.get('/patient', (req, res) => {
    res.json({patients});
    res.end();
});

//아이디 부분은 정수형을 쓰면 웹페이지에 출력하지 못하는 현상이 있기 때문에 주의해야 함.
app.get('/patient/:id', (req, res) => {
    var patient = patients.filter(function(e){
        return e.ID === req.params.id
    });
    res.json(patient);
    res.end();
});


////////////////////////////////////////////////////////////////////////////////////////////////////////////

//환자 1 이미지 정보 리스트 (환자 사진 var 값이 중복될 경우 웹 서버에 불러오지 못하는 일이 발생 할 수 있으니 주의 하기 바람)
var p1Images = [
    {ID : '1001', ImageName : 'P1_14FMX1.png', PixelLength : 0.005, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX_0_1.png'},
    {ID : '1002', ImageName : 'P1_14FMX2.png', PixelLength : 0.005, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX_0_2.png'},
    {ID : '1003', ImageName : 'P1_14FMX3.png', PixelLength : 0.005, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX_0_3.png'},
    {ID : '1004', ImageName : 'P1_14FMX4.png', PixelLength : 0.005, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX_0_4.png'},
    {ID : '1005', ImageName : 'P1_14FMX5.png', PixelLength : 0.005, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX_0_5.png'},
    {ID : '1006', ImageName : 'P1_14FMX6.png', PixelLength : 0.005, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX_0_6.png'},
    {ID : '1007', ImageName : 'P1_14FMX7.png', PixelLength : 0.005, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX_0_7.png'},
    {ID : '1008', ImageName : 'P1_14FMX8.png', PixelLength : 0.005, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX_0_8.png'},
    {ID : '1009', ImageName : 'P1_14FMX9.png', PixelLength : 0.005, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX_0_9.png'},
    {ID : '1010', ImageName : 'P1_14FMX10.png', PixelLength : 0.005, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX_1_0.png'},
    {ID : '1011', ImageName : 'P1_14FMX11.png', PixelLength : 0.005, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX_1_1.png'},
    {ID : '1012', ImageName : 'P1_14FMX12.png', PixelLength : 0.005, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX_1_2.png'},
    {ID : '1013', ImageName : 'P1_14FMX13.png', PixelLength : 0.005, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX_1_3.png'},
    {ID : '1014', ImageName : 'P1_14FMX14.png', PixelLength : 0.005, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX_1_4.png'},
    {ID : '1015', ImageName : 'panorama1.png', PixelLength : 0.005, ImageKinds : 'panorama', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/panorama1.png'}
]

app.get('/patient/001/image', (req, res) => {
    res.json(p1Images);
    res.end();
});

app.get('/patient/001/image/:id', (req, res) => {
    var Image = p1Images.filter(function(e){
        return e.ID === req.params.id
    });
    res.json(Image);
    res.end();
});

////////////////////////////////////////////////////////////////////////////////////////////////////////////

//환자 2 이미지 정보 리스트
var p2Images = [
    {ID : '2015', ImageName : 'panorama2.png', PixelLength : 0.005, ImageKinds : 'panorama', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/panorama2.png'}
]

app.get('/patient/002/image', (req, res) => {
    res.json(p2Images);
    res.end();
});

app.get('/patient/002/image/:id', (req, res) => {
    var Image = p2Images.filter(function(e){
        return e.ID === req.params.id
    });
    res.json(Image);
    res.end();
});

////////////////////////////////////////////////////////////////////////////////////////////////////////////

//환자 3 이미지 정보 리스트
var p3Images = [
    {ID : '3015', ImageName : 'panorama3.png', PixelLength : 0.005, ImageKinds : 'panorama', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/panorama3.png'}
]

app.get('/patient/003/image', (req, res) => {
    res.json(p3Images);
    res.end();
});

app.get('/patient/003/image/:id', (req, res) => {
    var Image = p3Images.filter(function(e){
        return e.ID === req.params.id
    });
    res.json(Image);
    res.end();
});

////////////////////////////////////////////////////////////////////////////////////////////////////////////

//환자 4 이미지 정보 리스트
var p4Images = [
    {ID : '4015', ImageName : 'panorama4.png', PixelLength : 0.005, ImageKinds : 'panorama', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/panorama4.png'}
]

app.get('/patient/004/image', (req, res) => {
    res.json(p4Images);
    res.end();
});

app.get('/patient/004/image/:id', (req, res) => {
    var Image = p4Images.filter(function(e){
        return e.ID === req.params.id
    });
    res.json(Image);
    res.end();
})


/* 위에서 지정된 port 와 IPAddress를 연결후 서버 활성화 */
app.listen(portNum, hostName, function() {
    console.log('Listening to port:  ' + portNum);
});