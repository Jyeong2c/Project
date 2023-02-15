const Patient = require('../models/Patient'); // Patient.js 모델 스키마 변수 할당

/*환자 전체 리스트를 출력*/
const index = (req, res) => {
    Patient.find()          // find 함수로 환자 전체 데이터 탐색
    /*찾으면 전체 환자리스트를 출력*/
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

/*단일 환자 정보 출력*/
const show = (req, res) => {
    let PatientID = req.body.PatientID;     // 환자 아이디 변수 할당
    Patient.findById(PatientID)            // 아이디를 찾고 해당 단일 정보를 출력하는 함수
    /*해당 정보를 찾으면 정보를 출력*/
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

/*환자 정보 입력 후 저장*/
const store = (req, res) => {
    /*환자 스키마를 빌려 해당 데이터를 입력하는 변수 할당*/
    let patient = new Patient({
        Name : req.body.Name,                       // 환자 성함 변수 할당
        Age : req.body.Age,                         // 환자 나이 변수 할당
        DoctorID : req.body.DoctorID                // 담당의사 로그인 아이디
    })
    patient.save()      // 입력을 다하면 저장하는 함수
    /*저장 성공시 성공 여부 메세지 출력*/
    .then(response => {
        res.json({
            message : "Patient Added Successfully!"
        });
    })
    /*반대의 경우 에러 메세지 출력*/
    .catch(error => {
        res.json({
            message : 'An error Occured!'
        })
    })
}

/*환자 정보를 수정*/
const update = (req, res) => {
    let patientID = req.body.PatientID      // 환자 아이디 변수 할당
    /*updateData 배열 함수 내부의 수정할 데이터를 할당*/
    let updateData = {
        Name : req.body.Name,
        Age : req.body.Age,
        DoctorID : req.body.DoctorID,
    }
    Patient.findByIdAndUpdate(patientID, {$set : updateData}) // 해당 환자 아이디를 찾고 수정하는 함수
    /*수정 완료시 성공 메세지 출력*/
    .then(() => {
        res.json({
            message : 'Patient update successfully!'
        });
    })
    /*반대의 경우 실패 메세지 출력*/
    .catch(error => {
        res.json({
            message : 'An error Occured!'
        });
    })
};

/*환자 정보 삭제*/
const destroy = (req, res) => {
    let PatientID = req.body.PatientID;     // 환자 아이디 변수 할당
    Patient.findByIdAndRemove(PatientID)    // 환자 아이디에 해당되는 데이터 삭제
    /*삭제 완료 시 성공 메세지 출력*/
    .then(() => {
        res.json({
            message : 'Patient deleted successfully!'
        });
    })
    /*반대의 경우 실패 메세지 출력*/
    .catch(error => {
        res.json({
            message : 'An error Occured!'
        });
    })
}

/*상단에 설정한 컨트롤러들을 모듈에 추출, routes에 사용*/
module.exports = {
    index, show, store, update, destroy
}
