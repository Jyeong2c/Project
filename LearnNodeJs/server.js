const express = require('express');

const server = express();

//javascript Array를 사용하여 사용자의 정보를 json형태로 저장
const users = [
    {
        id:"001",
        name:"jenny",
        email:"jenny@naver.com" 
    },
    {
        id:"002",
        name:"dennial",
        email:"denial@naver.com"
    }
];

server.get("/api/user", (req, res) =>{
    res.json(users);
});

server.listen(3000, () =>{
    console.log("The Server is running");
});

