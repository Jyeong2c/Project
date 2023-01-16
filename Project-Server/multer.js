const express = require("express");
const app = express();
const multer = require("multer");
const fs = require("fs");

app.listen(3000, () => {
	const dir = "./uploads";
    if(!fs.existsSync(dir)) {
    	fs.mkdirSync(dir);
    }
    console.log("서버 실행");
});

const storage = multer.diskStorage({
  destination: function (req, file, cb) {
    cb(null, 'uploads')
  },
  filename: function (req, file, cb) {
    cb(null, file.fieldname + '-' + Date.now())
  }
});

const upload = multer({ storage: storage })

// 단일 파일 업로드
app.post('/', upload.single('img'), (req, res, next) => {
    res.status(200).send({
        message: "Ok",
        fileInfo: req.file
    })
});

// 다중 파일 업로드
app.post('/multipart', upload.array('img'), (req, res, next) => {

    // console check
    req.files.map((data) => {
        console.log(data);
    });
    
    res.status(200).send({
        message: "Ok",
        fileInfo: req.files
    })
});