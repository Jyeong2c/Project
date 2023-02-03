const express = require('express');
const router = express.Router();

const ImageController = require('../controllers/ImageController');

router.get('/', ImageController.index);
router.post('/show', ImageController.show);
router.post('/store', ImageController.store);
router.post('/update', ImageController.update);
router.post('/delete', ImageController.destroy);

module.exports = router;