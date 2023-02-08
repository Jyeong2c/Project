const mongoose = require('mongoose');
const Schema = mongoose.Schema;

//환자 정보 스키마
const patientSchema = new Schema({
    Name : {    //환자 성함
        type : String
    },
    Age : {     //환자 나이
        type : Number
    },
    DoctorID : {//의사 아이디
        type : String
    }
},{timestamps : true}); //환자 입력/수정 날짜 표시

const Patient = mongoose.model('Patient', patientSchema);
module.exports = Patient;