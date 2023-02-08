const express = require('express');
const router = express.Router();
//컨트롤러 변수 할당
const ImageController = require('../controllers/ImageController');
//해당 경로에 이미지 업로드
const upload = require('../middleware/upload');

router.get('/', ImageController.index);
router.post('/show', ImageController.show);
//이미지 정보와 1개의 이미지 파일을 저장하기 위한 post문
router.post('/store', upload.single('ImageFile'), ImageController.store); //if use array array('ImageFile[]')
router.post('/update', ImageController.update);
router.post('/delete', ImageController.destroy);
router.get('/removeSync', ImageController.removeSync);

module.exports = router;