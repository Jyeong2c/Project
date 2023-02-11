const express = require('express');
const mongoose = require('mongoose');       // mongoose 패키지 사용
const morgan = require('morgan');
require('dotenv').config({path:'variables.env'});  // mongodb와 연동할 url을 환경변수 파일에 불러오기
//const PATIENTDB_URL = 'mongodb+srv://patients:1234@patients.d52ldtz.mongodb.net/?retryWrites=true&w=majority'
const bodyParser = require('body-parser');

mongoose.connect(process.env.PATIENTDB_URL, {useNewUrlParser : true, useUnifiedTopology : true}); // mongodb 연결

const db = mongoose.connection;         // MongoDB 변수 할당 

/*DB 연동 실패인 경우*/
db.on('error', (err) => {
    console.log(err);
});

/*DB 연동 성공인 경우 */
db.once('open', () => {
    console.log('Database Connection Established!');
});

const app = express();

/*env 파일에 있는 Url설정을 해독 */
app.use(morgan('dev'));
app.use(bodyParser.urlencoded({extended : true}));
app.use(bodyParser.json());
/*Access server image file*/
app.use('/uploads', express.static('uploads'));   // http://192.168.0.12:40000/uploads/1675489038845.png
const PORT = process.env.PORT || 40000;   // 포트 번호 40000 설정

/*MongoDB 에서 설정한 Network Access와 node.js 설정한 포트로 네트워크 환경 구축*/
app.listen(PORT, () => { 
    console.log('Server is running on Port', PORT);
});


const PatientRoute = require('./routes/patient');  // routes of patient.js
const ImageRoute = require('./routes/image'); // routes of image.js
const LoginRoute = require('./routes/login'); // routes of login.js
 
app.use('/api/patient', PatientRoute); // use patientDB
app.use('/api/image', ImageRoute); // use imageDB
app.use('/api/login', LoginRoute); // use loginDB

