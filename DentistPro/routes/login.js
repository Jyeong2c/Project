const express = require('express');
const router = express.Router();
const LoginController = require('../controllers/LoginController');  //LoginController를 얻기 위한 변수


router.get('/', LoginController.index);         // 전체 로그인 정보를 출력하기 위한 get 함수
router.post('/show', LoginController.show);     // 1개의 로그인 정보를 출력하기 위한 post 함수
router.post('/store', LoginController.store);   // 단일 로그인 정보를 입력후 저장하는 post 함수
router.post('/update', LoginController.update);     // 단일 로그인 정보를 수정하는 post 함수
router.post('/delete', LoginController.destroy);  // 단일 로그인 정보를 삭제하는 post 함수

module.exports = router;        // 로그인 라우터 추출