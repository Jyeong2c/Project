const express = require('express');
const router = express.Router();
//LoginController를 얻기 위한 변수
const LoginController = require('../controllers/LoginController');

// index : show all logininfo list
router.get('/', LoginController.index);
// show : show selected logininfo
router.post('/show', LoginController.show);
// store : add new logininfo 
router.post('/store', LoginController.store);
// update : update selected logininfo
router.post('/update', LoginController.update);
// destroy : delete selected logininfo
router.post('/delete', LoginController.destroy);

module.exports = router;