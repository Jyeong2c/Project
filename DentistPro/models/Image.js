//MongoDB와 연동할 수 있는 mongoose 패키지 사용
const mongoose = require('mongoose');
const Schema = mongoose.Schema;     //MongoDB 스키마

//이미지 정보 스키마 
const imageSchema = new Schema({
    ImageName : {   //이미지 이름
        type : String
    },
    PatientName : { //환자 이름
        type : String
    },
    PixelLength : { //픽셀당 길이 
        type : Number
    },
    ImageKinds : {  //이미지 종류
        type : String
    },
    PhotoDate : {   //이미지 날짜
        type : String
    },
    ImageFile : {   //파일 정보
        type : String
    }
},{timestamps : true}); // 이미지 입력/수정 날짜 정보

const Image = mongoose.model('Image', imageSchema);
module.exports = Image;