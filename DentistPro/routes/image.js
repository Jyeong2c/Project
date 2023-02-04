const express = require('express');
const router = express.Router();

const ImageController = require('../controllers/ImageController');

const upload = require('../middleware/upload');

router.get('/', ImageController.index);
router.post('/show', ImageController.show);
router.post('/store', upload.single('ImageFile'), ImageController.store); //if use array array('ImageFile[]')
router.post('/update', ImageController.update);
router.post('/delete', ImageController.destroy);

module.exports = router;