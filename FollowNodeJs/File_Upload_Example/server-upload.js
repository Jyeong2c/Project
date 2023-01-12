const express = require('express');
const multer = require('multer');
const uuid = require('uuid').v4;

const storage = multer.diskStorage({
    destination: (req, file, cb) =>{
        cb(null, 'uploads');
    },
    filename: (req, file, cb) => {
        const {originalname} = file;
        cb(null, `${uuid()}-${originalname}`);      //uuid 패키지로 이미지의 id를 재정의
    }
})

const upload = multer({ storage });      //업로드 성공시 현재 폴더에서 upload 디렉토리 생성

const app = express();
app.use(express.static('public'));

app.post('/upload', upload.array('avatar'), (req, res) => {
    return res.json({status : 'OK', uploaded : req.files.length});
})

app.listen(3001, () => console.log('App is listening...'));
