const express = require('express');
const router = express.Router();

const PatientController = require('../controllers/PatientController');

// index : show all patient list
router.get('/', PatientController.index);
// show : show selected patient
router.post('/show', PatientController.show);
// store : add new patient 
router.post('/store', PatientController.store);
// update : update selected patient
router.post('/update', PatientController.update);
// destroy : delete selected patient
router.post('/delete', PatientController.destroy);

module.exports = router;