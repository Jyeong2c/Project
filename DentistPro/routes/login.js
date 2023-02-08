const express = require('express');
const router = express.Router();

const LoginController = require('../controllers/LoginController');

// index : show all patient list
router.get('/', LoginController.index);
// show : show selected patient
router.post('/show', LoginController.show);
// store : add new patient 
router.post('/store', LoginController.store);
// update : update selected patient
router.post('/update', LoginController.update);
// destroy : delete selected patient
router.post('/delete', LoginController.destroy);

module.exports = router;