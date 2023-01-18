const multer = require('multer')
//const upload = multer({dest: 'images/'}) //dest : 저장 위치

router.post('/upload',upload.single('img'),(req,res) => {
    res.json(req.file)
    console.log(req.file)
})

var storage = multer.diskStorage({
    destination: function (req, file, cb) {
      cb(null,'./Files') // cb 콜백함수를 통해 전송된 파일 저장 디렉토리 설정
    },
    filename: function (req, file, cb) {
      cb(null, file.originalname) // cb 콜백함수를 통해 전송된 파일 이름 설정
    }
  })

const upload = multer({storage: storage})
