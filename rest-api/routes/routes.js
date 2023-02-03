const express = require('express');
const router = express.Router();

router.get('/', (req, res) => {
    res.send("GET Response");
});

router.post('/', (req, res) => {
    res.send("Post Response");
});

router.put("/:id", (req, res) => {
    res.send("Put Response");
});

router.delete("/:id", (req, res) => {
    res.send("Delete Response");
});

module.exports = router;