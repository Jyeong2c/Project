const express = require('express');
const router = express.Router();
const PatientController = require('../controllers/PatientController');  // PatientController를 얻기 위한 변수

router.get('/', PatientController.index);               // 전체 환자 정보를 출력하기 위한 get 함수
router.post('/show', PatientController.show);           // 단일 환자 정보를 출력하기 위한 post 함수
router.post('/store', PatientController.store);         // 단일 환자 정보를 입력후 저장하기 위한 post 함수 
router.post('/update', PatientController.update);       // 단일 환자 정보를 수정하기 위한 post 함수
router.post('/delete', PatientController.destroy);      // 단일 환자 정보를 삭제하기 위한 post 함수

module.exports = router;