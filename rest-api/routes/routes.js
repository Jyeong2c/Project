const express = require('express');
const router = express.Router();
const Account = require("./accountModel");

router.get('/', async (req, res) => {
    try{
        const accounts = await Account.find();
        res.json(accounts);
    } catch(err){
        res.status(500).json({message : err.message});
    }
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