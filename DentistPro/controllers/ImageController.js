const Image = require('../models/Image'); // Image.js 모델 스키마 변수 할당
const fs = require('fs');                 // fs 패키지 사용(파일관리)

/*이미지 전체 리스트를 출력*/
const index = (req, res) => {
    Image.find()                //Mongoose 함수 find()로 
         /*찾으면 전체 저장된 이미지 리스트 출력*/
        .then(response => {    
            res.json({
                response
            });
        })
        /*못찾으면 에러 메세지 출력*/
        .catch(error => {       
            res.json({
                message: error
            });
        })
};

/*단일 이미지 정보를 출력*/
const show = (req, res) => {
    let ImageID = req.body.ImageID;     // 이미지 ID 변수 할당
    Image.findById(ImageID)             // 할당된 이미지 ID 변수를 통해 정보를 찾는 findById()
        /*찾으면 해당 단일 이미지의 정보 출력*/
        .then(response => {             
            res.json({
                response
            });
        })
        /*못 찾으면 에러 메세지 출력*/
        .catch(error => {               
            res.json({
                message: error
            });
        })
};

/*단일 이미지 정보를 입력후 저장*/
const store = (req, res) => {
    /*이미지 스키마를 빌려 해당 이미지 정보를 입력하는 변수 할당*/
    let image = new Image({         
        ImageName: req.body.ImageName,
        PatientName: req.body.PatientName,
        PixelLength: req.body.PixelLength,
        ImageKinds: req.body.ImageKinds,
        PhotoDate: req.body.PhotoDate,
    })
    /*이미지 파일을 저장할 시 해당 파일 경로를 서버에 저장*/
    if (req.file) {
        image.ImageFile = req.file.path;  
    }
    image.save()        // 입력한 이미지 정보 변수들을 저장하는 함수
        /*저장 완료시 성공 메세지 출력*/
        .then(response => {
            res.json({
                message: "Image Added Successfully!"
            });
        })
        /*반대의 경우 실패 메세지 출력*/
        .catch(error => {
            res.json({
                message: 'An error Occured!'
            })
        })
}

/*단일 이미지 정보를 수정*/
const update = (req, res) => {
    let ImageID = req.body.ImageID      // 수정할 이미지 아이디 변수 할당
    /*updateData 배열 함수 내부의 수정할 데이터를 할당*/
    let updateData = {
        ImageName: req.body.ImageName,
        PatientName: req.body.PatientName,
        PixelLength: req.body.PixelLength,
        ImageKinds: req.body.ImageKinds,
        PhotoDate: req.body.PhotoDate,
    }

    Image.findByIdAndUpdate(ImageID, { $set: updateData }) // 이미지 아이디 찾아 내부 데이터를 수정
        /*수정 완료시 성공 메세지 출력*/
        .then(() => {
            res.json({
                message: 'Image update successfully!'
            });
        })
        /*반대의 경우 실패 메세지 출력*/
        .catch(error => {
            res.json({
                message: 'An error Occured!'
            });
        })
};

/*단일 이미지 '정보'만 삭제*/
const destroy = (req, res) => {
    let ImageID = req.body.ImageID;     // 삭제할 이미지 아이디 변수 할당

    Image.findByIdAndRemove(ImageID)    // 이미지 아이디 변수로 해당 이미지 정보를 삭제
        /*이미지 정보 삭제시 성공 메세지 출력*/
        .then(() => {
            res.json({
                message: 'Image deleted successfully!',
            });
        })
        /*반대의 경우 실패 메세지 출력*/
        .catch(error => {
            res.json({
                message: 'An error Occured!'
            });
        })
}

/*서버내의 이미지 삭제*/
const removeSync = (req, res) => {
    var pngFile = req.query.pngFile;        // 삭제 이미지 변수 설정
    const directoryPath = "./uploads/";     // 삭제할 이미지 로컬 경로
    try {
        fs.unlinkSync(directoryPath + pngFile); // 해당경로의 이미지를 삭제
        /*이미지 삭제 성공 시 성공 메세지 출력*/
        res.status(200).send({
            message: "File is deleted.",
        });
    } catch (err) {
        /*반대의 경우 실패 메세지 출력*/
        res.status(500).send({
            message: "Could not delete the file. " + err,
        });
    }
};

/*위에서 설정한 컨트롤러들을 모듈에 추출, routes에 사용*/
module.exports = {
    index, show, store, update, destroy, removeSync
}
