const Login = require('../models/Login'); // Login.js 모델 스키마 변수 할당

/*로그인 전체 정보들을 출력*/
const index = (req, res) => {
    Login.find()                // Login 모델을 찾는 find 함수
    /*찾으면 전체 로그인 정보 출력*/
    .then(response => {
        res.json({
            response
        });
    })
    /*반대의 경우 에러 메세지 출력*/
    .catch(error => {
        res.json({
            message : error
        });
    })
};

/*단일 로그인 정보 출력*/
const show = (req, res) => {
    let LoginID = req.body.LoginID;  // 로그인 정보 아이디 변수 할당
    Login.findById(LoginID)   // 할당된 아이디 변수를 찾는 findById 함수 
    /*찾으면 해당 단일 정보 출력*/
    .then(response => {
        res.json({
            response
        });
    })
    /*반대의 경우 에러 메세지 출력*/
    .catch(error => {
        res.json({
            message : error
        });
    })
};

/*새로운 로그인 정보 입력*/
const store = (req, res) => {
    /*로그인 스키마를 빌려 해당 로그인 정보를 입력하는 변수 할당*/
    let login = new Login({
        DoctorID : req.body.DoctorID,               // 프로그램 로그인 아이디 변수 할당
        DoctorName : req.body.DoctorName,           // 의사 성함 변수 할당
        Password : req.body.Password                // 프로그램 로그인 비밀번호 변수 할당
    })
    login.save()        // 입력한 로그인 정보 변수들을 저장하는 함수
    /*저장 완료시 성공 메세지 출력*/
    .then(response => {
        res.json({
            message : "LoginInfo Added Successfully!"
        });
    })
    /*반대의 경우 실패 메세지 출력*/
    .catch(error => {
        res.json({
            message : 'An error Occured!'
        })
    })
}

/*단일 로그인 정보를 수정*/
const update = (req, res) => {
    let LoginID = req.body.LoginID      // 수정할 로그인 아이디 변수 할당
    /*updateData 배열 함수안에 수정할 정보들을 할당*/
    let updateData = {
        DoctorID : req.body.DoctorID,
        DoctorName : req.body.DoctorName,
        Password : req.body.Password,
    }

    Login.findByIdAndUpdate(LoginID, {$set : updateData}) // 해당 로그인 아이디를 찾아 내부 데이터를 수정
    /*수정 완료시 성공 메세지 출력*/
    .then(() => {
        res.json({
            message : 'LoginInfo update successfully!'
        });
    })
    /*반대의 경우 실패 메세지 출력*/
    .catch(error => {
        res.json({
            message : 'An error Occured!'
        });
    })
};

/*단일 로그인 정보를 삭제*/
const destroy = (req, res) => {
    let LoginID = req.body.LoginID;         // 로그인 아이디 변수 할당
    Login.findByIdAndRemove(LoginID)        // 로그인 아이디를 찾는 함수
    /*삭제 완료 시 성공 메세지 출력*/
    .then(() => {
        res.json({
            message : 'LoginInfo deleted successfully!'
        });
    })
    /*반대의 경우 실패 메세지 출력*/
    .catch(error => {
        res.json({
            message : 'An error Occured!'
        });
    })
}

/*위에서 설정한 컨트롤러들을 모듈로 추출 routes에 사용*/
module.exports = {
    index, show, store, update, destroy
}


