//경로 파일 작업을 express로 실행
const express = require('express')
const router = express.Router();

const EmployeeController = require('../controllers/EmploteeController');

router.get('/', EmployeeController.index);
router.post('/show', EmployeeController.show);
router.post('/store', EmployeeController.store);
router.post('/update', EmployeeController.update);
router.post('/delete', EmployeeController.destroy);

module.exports = router; 