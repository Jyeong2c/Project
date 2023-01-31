var express = require('express')                            //express 패키지 사용
var fs = require('fs');                                     //fs 패키지 사용
const app = express();                                      //app express 함수 사용

/*Server 시작 및 메인 페이지*/
app.get('/', (req, res) => {                       //express 패키지 형태로 간단한 웹 페이지 구현
    res.send('This is Server Main Page');
});

/*Server의 IP와 host를 지정하는 변수*/
var hostName = '192.168.0.12'
var portNum = 3000;

////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*이미지 파일 서버 업로드 부분*/
app.get('/Files/:id', (req, res) => {
    /*파일 디렉토리 이미지 Content-Type : image/png로 설정(멘토님 께선 application/json로 설정 권장하였음.) */ 
    res.writeHead(200, {'Content-Type':'image/png'});
    fs.readFile(__dirname + '\\Files\\' + req.params.id, (err, data) => {
      if (err) {
        return console.error(err);
      }
      res.end(data, 'utf-8');
    });
  });

////////////////////////////////////////////////////////////////////////////////////////////////////////////

//환자 정보 리스트 (8명의 환자 정보 입력된 상태 / 의사의 아이디에 따라 환자 정보를 구분지음)
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
    {ID : '1001', ImageName : 'P1_14FMX0-1', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX0-1.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '1002', ImageName : 'P1_14FMX0-2', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX0-2.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '1003', ImageName : 'P1_14FMX0-3', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX0-3.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '1004', ImageName : 'P1_14FMX0-4', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX0-4.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '1005', ImageName : 'P1_14FMX0-5', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX0-5.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '1006', ImageName : 'P1_14FMX0-6', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX0-6.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '1007', ImageName : 'P1_14FMX0-7', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX0-7.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '1008', ImageName : 'P1_14FMX0-8', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX0-8.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '1009', ImageName : 'P1_14FMX0-9', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX0-9.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '1010', ImageName : 'P1_14FMX1-0', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX1-0.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '1011', ImageName : 'P1_14FMX1-1', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX1-1.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '1012', ImageName : 'P1_14FMX1-2', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX1-2.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '1013', ImageName : 'P1_14FMX1-3', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX1-3.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '1014', ImageName : 'P1_14FMX1-4', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P1_14FMX1-4.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '1015', ImageName : 'panorama1', PixelLength : 0.003, ImageKinds : 'panorama', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/panorama1.png', ImageWidth : 400, ImageHeight : 250}
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
    {ID : '2001', ImageName : 'P2_14FMX0-1', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P2_14FMX0-1.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '2002', ImageName : 'P2_14FMX0-2', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P2_14FMX0-2.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '2003', ImageName : 'P2_14FMX0-3', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P2_14FMX0-3.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '2004', ImageName : 'P2_14FMX0-4', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P2_14FMX0-4.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '2005', ImageName : 'P2_14FMX0-5', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P2_14FMX0-5.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '2006', ImageName : 'P2_14FMX0-6', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P2_14FMX0-6.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '2007', ImageName : 'P2_14FMX0-7', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P2_14FMX0-7.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '2008', ImageName : 'P2_14FMX0-8', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P2_14FMX0-8.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '2009', ImageName : 'P2_14FMX0-9', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P2_14FMX0-9.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '2010', ImageName : 'P2_14FMX1-0', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P2_14FMX1-0.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '2011', ImageName : 'P2_14FMX1-1', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P2_14FMX1-1.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '2012', ImageName : 'P2_14FMX1-2', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P2_14FMX1-2.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '2013', ImageName : 'P2_14FMX1-3', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P2_14FMX1-3.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '2014', ImageName : 'P2_14FMX1-4', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P2_14FMX1-4.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '2015', ImageName : 'panorama2', PixelLength : 0.004, ImageKinds : 'panorama', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/panorama2.png', ImageWidth : 400, ImageHeight : 250}
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
    {ID : '3001', ImageName : 'P3_14FMX0-1', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P3_14FMX0-1.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '3002', ImageName : 'P3_14FMX0-2', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P3_14FMX0-2.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '3003', ImageName : 'P3_14FMX0-3', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P3_14FMX0-3.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '3004', ImageName : 'P3_14FMX0-4', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P3_14FMX0-4.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '3005', ImageName : 'P3_14FMX0-5', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P3_14FMX0-5.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '3006', ImageName : 'P3_14FMX0-6', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P3_14FMX0-6.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '3007', ImageName : 'P3_14FMX0-7', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P3_14FMX0-7.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '3008', ImageName : 'P3_14FMX0-8', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P3_14FMX0-8.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '3009', ImageName : 'P3_14FMX0-9', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P3_14FMX0-9.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '3010', ImageName : 'P3_14FMX1-0', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P3_14FMX1-0.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '3011', ImageName : 'P3_14FMX1-1', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P3_14FMX1-1.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '3012', ImageName : 'P3_14FMX1-2', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P3_14FMX1-2.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '3013', ImageName : 'P3_14FMX1-3', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P3_14FMX1-3.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '3014', ImageName : 'P3_14FMX1-4', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P3_14FMX1-4.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '3015', ImageName : 'panorama3', PixelLength : 0.001, ImageKinds : 'panorama', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/panorama3.png', ImageWidth : 400, ImageHeight : 250}
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
    {ID : '4001', ImageName : 'P4_14FMX0-1', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P4_14FMX0-1.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '4002', ImageName : 'P4_14FMX0-2', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P4_14FMX0-2.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '4003', ImageName : 'P4_14FMX0-3', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P4_14FMX0-3.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '4004', ImageName : 'P4_14FMX0-4', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P4_14FMX0-4.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '4005', ImageName : 'P4_14FMX0-5', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P4_14FMX0-5.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '4006', ImageName : 'P4_14FMX0-6', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P4_14FMX0-6.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '4007', ImageName : 'P4_14FMX0-7', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P4_14FMX0-7.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '4008', ImageName : 'P4_14FMX0-8', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P4_14FMX0-8.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '4009', ImageName : 'P4_14FMX0-9', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P4_14FMX0-9.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '4010', ImageName : 'P4_14FMX1-0', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P4_14FMX1-0.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '4011', ImageName : 'P4_14FMX1-1', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P4_14FMX1-1.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '4012', ImageName : 'P4_14FMX1-2', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P4_14FMX1-2.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '4013', ImageName : 'P4_14FMX1-3', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P4_14FMX1-3.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '4014', ImageName : 'P4_14FMX1-4', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P4_14FMX1-4.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '4015', ImageName : 'panorama4', PixelLength : 0.004, ImageKinds : 'panorama', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/panorama4.png', ImageWidth : 400, ImageHeight : 250}
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
});

////////////////////////////////////////////////////////////////////////////////////////////////////////////

//환자 5 이미지 정보 리스트
var p5Images = [
    {ID : '5001', ImageName : 'P5_14FMX0-1', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P5_14FMX0-1.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '5002', ImageName : 'P5_14FMX0-2', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P5_14FMX0-2.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '5003', ImageName : 'P5_14FMX0-3', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P5_14FMX0-3.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '5004', ImageName : 'P5_14FMX0-4', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P5_14FMX0-4.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '5005', ImageName : 'P5_14FMX0-5', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P5_14FMX0-5.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '5006', ImageName : 'P5_14FMX0-6', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P5_14FMX0-6.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '5007', ImageName : 'P5_14FMX0-7', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P5_14FMX0-7.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '5008', ImageName : 'P5_14FMX0-8', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P5_14FMX0-8.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '5009', ImageName : 'P5_14FMX0-9', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P5_14FMX0-9.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '5010', ImageName : 'P5_14FMX1-0', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P5_14FMX1-0.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '5011', ImageName : 'P5_14FMX1-1', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P5_14FMX1-1.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '5012', ImageName : 'P5_14FMX1-2', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P5_14FMX1-2.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '5013', ImageName : 'P5_14FMX1-3', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P5_14FMX1-3.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '5014', ImageName : 'P5_14FMX1-4', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P5_14FMX1-4.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '5015', ImageName : 'panorama5', PixelLength : 0.004, ImageKinds : 'panorama', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/panorama5.png', ImageWidth : 400, ImageHeight : 250}
]

app.get('/patient/005/image', (req, res) => {
    res.json(p5Images);
    res.end();
});

app.get('/patient/005/image/:id', (req, res) => {
    var Image = p5Images.filter(function(e){
        return e.ID === req.params.id
    });
    res.json(Image);
    res.end();
});

////////////////////////////////////////////////////////////////////////////////////////////////////////////

//환자 6 이미지 정보 리스트
var p6Images = [
    {ID : '6001', ImageName : 'P6_14FMX0-1', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P6_14FMX0-1.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '6002', ImageName : 'P6_14FMX0-2', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P6_14FMX0-2.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '6003', ImageName : 'P6_14FMX0-3', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P6_14FMX0-3.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '6004', ImageName : 'P6_14FMX0-4', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P6_14FMX0-4.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '6005', ImageName : 'P6_14FMX0-5', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P6_14FMX0-5.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '6006', ImageName : 'P6_14FMX0-6', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P6_14FMX0-6.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '6007', ImageName : 'P6_14FMX0-7', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P6_14FMX0-7.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '6008', ImageName : 'P6_14FMX0-8', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P6_14FMX0-8.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '6009', ImageName : 'P6_14FMX0-9', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P6_14FMX0-9.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '6010', ImageName : 'P6_14FMX1-0', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P6_14FMX1-0.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '6011', ImageName : 'P6_14FMX1-1', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P6_14FMX1-1.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '6012', ImageName : 'P6_14FMX1-2', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P6_14FMX1-2.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '6013', ImageName : 'P6_14FMX1-3', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P6_14FMX1-3.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '6014', ImageName : 'P6_14FMX1-4', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P6_14FMX1-4.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '6015', ImageName : 'panorama6', PixelLength : 0.002, ImageKinds : 'panorama', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/panorama6.png', ImageWidth : 400, ImageHeight : 250}
]

app.get('/patient/006/image', (req, res) => {
    res.json(p6Images);
    res.end();
});

app.get('/patient/006/image/:id', (req, res) => {
    var Image = p6Images.filter(function(e){
        return e.ID === req.params.id
    });
    res.json(Image);
    res.end();
});

////////////////////////////////////////////////////////////////////////////////////////////////////////////

//환자 7 이미지 정보 리스트
var p7Images = [
    {ID : '7001', ImageName : 'P7_14FMX0-1', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P7_14FMX0-1.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '7002', ImageName : 'P7_14FMX0-2', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P7_14FMX0-2.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '7003', ImageName : 'P7_14FMX0-3', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P7_14FMX0-3.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '7004', ImageName : 'P7_14FMX0-4', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P7_14FMX0-4.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '7005', ImageName : 'P7_14FMX0-5', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P7_14FMX0-5.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '7006', ImageName : 'P7_14FMX0-6', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P7_14FMX0-6.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '7007', ImageName : 'P7_14FMX0-7', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P7_14FMX0-7.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '7008', ImageName : 'P7_14FMX0-8', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P7_14FMX0-8.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '7009', ImageName : 'P7_14FMX0-9', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P7_14FMX0-9.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '7010', ImageName : 'P7_14FMX1-0', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P7_14FMX1-0.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '7011', ImageName : 'P7_14FMX1-1', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P7_14FMX1-1.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '7012', ImageName : 'P7_14FMX1-2', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P7_14FMX1-2.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '7013', ImageName : 'P7_14FMX1-3', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P7_14FMX1-3.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '7014', ImageName : 'P7_14FMX1-4', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P7_14FMX1-4.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '7015', ImageName : 'panorama7', PixelLength : 0.003, ImageKinds : 'panorama', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/panorama7.png', ImageWidth : 400, ImageHeight : 250}
]

app.get('/patient/007/image', (req, res) => {
    res.json(p7Images);
    res.end();
});

app.get('/patient/007/image/:id', (req, res) => {
    var Image = p7Images.filter(function(e){
        return e.ID === req.params.id
    });
    res.json(Image);
    res.end();
});

////////////////////////////////////////////////////////////////////////////////////////////////////////////

//환자 8 이미지 정보 리스트
var p8Images = [
    {ID : '8001', ImageName : 'P8_14FMX0-1', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P8_14FMX0-1.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '8002', ImageName : 'P8_14FMX0-2', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P8_14FMX0-2.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '8003', ImageName : 'P8_14FMX0-3', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P8_14FMX0-3.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '8004', ImageName : 'P8_14FMX0-4', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P8_14FMX0-4.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '8005', ImageName : 'P8_14FMX0-5', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P8_14FMX0-5.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '8006', ImageName : 'P8_14FMX0-6', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P8_14FMX0-6.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '8007', ImageName : 'P8_14FMX0-7', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P8_14FMX0-7.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '8008', ImageName : 'P8_14FMX0-8', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P8_14FMX0-8.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '8009', ImageName : 'P8_14FMX0-9', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P8_14FMX0-9.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '8010', ImageName : 'P8_14FMX1-0', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P8_14FMX1-0.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '8011', ImageName : 'P8_14FMX1-1', PixelLength : 0.001, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P8_14FMX1-1.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '8012', ImageName : 'P8_14FMX1-2', PixelLength : 0.002, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P8_14FMX1-2.png', ImageWidth : 90, ImageHeight : 120},
    {ID : '8013', ImageName : 'P8_14FMX1-3', PixelLength : 0.003, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P8_14FMX1-3.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '8014', ImageName : 'P8_14FMX1-4', PixelLength : 0.004, ImageKinds : 'IO', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/P8_14FMX1-4.png', ImageWidth : 200, ImageHeight : 120},
    {ID : '8015', ImageName : 'panorama8', PixelLength : 0.001, ImageKinds : 'panorama', ImagePathURL : 'http://' + hostName + ':' + portNum + '/Files/panorama8.png', ImageWidth : 400, ImageHeight : 250}
]

app.get('/patient/008/image', (req, res) => {
    res.json(p8Images);
    res.end();
});

app.get('/patient/008/image/:id', (req, res) => {
    var Image = p8Images.filter(function(e){
        return e.ID === req.params.id
    });
    res.json(Image);
    res.end();
})

/* 위에서 지정된 port 와 IPAddress를 연결후 서버 활성화 */
app.listen(portNum, hostName, function() {
    console.log('Listening to port:  ' + portNum);
});