const express = require('express');
const router = express.Router();
const ImageController = require('../controllers/ImageController');   // 컨트롤러 변수 할당
const upload = require('../middleware/upload');                  // 해당 경로에 이미지 업로드

router.get('/', ImageController.index);         // 전체 이미지 정보를 출력하기 위한 get 함수
router.post('/show', ImageController.show);     // 1개의 이미지 정보를 출력하기 위한 post 함수
router.post('/store', upload.single('ImageFile'), ImageController.store); // 단일 이미지 정보를 입력 후 저장하는 post 함수
router.post('/update', ImageController.update);             // 단일 이미지 정보르 수정하는 post 함수
router.post('/delete', ImageController.destroy);            // 단일 이미지 정보를 삭제하는 post 함수
router.get('/removeSync', ImageController.removeSync);      // 이미지 서버 내부의 이미지를 삭제하는 get 함수

module.exports = router;                //환자 라우터 추출