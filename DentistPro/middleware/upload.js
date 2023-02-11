/*클라이언트의 이미지를 서버로 업로드 하기위해 필요한 패키지*/
const path = require('path');
const multer = require('multer');

/*저장할 경로와 파일이름을 업로드*/
var stroate = multer.diskStorage({
    destination : function(req, file, cb) {
        cb(null, 'uploads/');       // 업로드 경로 설정
    },
    filename: function(req, file, cb) {
        let ext = path.extname(file.originalname);
        cb(null, Date.now() + ext);      // 파일 업로드 날짜는 현재날짜로 지정
    }
});

var upload = multer({
    storage : stroate,
    fileFilter : function(req, file, callback){
        if(
            /*파일 마임 타입을 jpg, png 인 경우*/
            file.mimetype == "image/png" ||
            file.mimetype == "image/jpg" 
        ){
            callback(null, true);   // 이미지 저장
        }else{
            console.log('only jpg & png file supported!');
            callback(null, false);  // 이미지 저장 불가
        }
    },
    limits: {
        fileSize : 1024 * 1024 * 2   // 최대 2,097,152 바이트 이미지 제한
    }
})

module.exports = upload;   // 업로드 모듈 추출