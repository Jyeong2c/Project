const mongoose = require('mongoose');
const Schema = mongoose.Schema;

//로그인 정보 스키마 
const loginSchema = new Schema({
    DoctorID : {    //의사 아이디 정보
        type : String
    },
    DoctorName : {  //의사 이름 정보
        type : String
    },
    Password : {    //비밀번호 정보
        type : String
    }
},{timestamps : true}); //로그인 입력/수정 날짜 정보

const Login = mongoose.model('Login', loginSchema);
module.exports = Login;