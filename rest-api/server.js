const express = require('express');
//mongoose database를 부르기 위한 패키지 설정
const mongoose = require('mongoose');
const server = express();

const accountRouter = require("./routes");
server.use("/routes", accountRouter);

//User 모델을 사용하기 위한 변수 설정
const User = require('./models/User');

//env파일을 사용하기위한 패키지 'dotenv'
require('dotenv').config({ path: 'variables.env' });

// server.get('/', (req, res) => {
//     const newUser = new User();
//     newUser.email = "danny@gmail.com";
//     newUser.name = "danny";
//     newUser.age = 25;

//     //위의 정보들은 저장한 후 json응답으로 create 했다는 정보를 표시
//     newUser
//         .save()
//         .then((user) => {   //성공시 콘솔에 user정보와 json형태로 저장을 성공했다는 문구를 출력
//             console.log(user);
//             res.json({
//                 message: 'User Create Successfully'
//             });
//         })
//         .catch((err) => {   //실패ㅣ
//             res.json({
//                 message: 'User was not successfully create'
//             });
//         })
// });

//console.log(process.env.MONGODB_URL);
server.listen(3000, (err) => {
    if (err) {
        return console.log(err);
    } else {   
        //mongoDB 사이트에서 만든 URL을 호출 (<password> = 1234(1234자리에 <password>라고 있었음))
        //const MONGODB_URL = 'mongodb+srv://root:1234@education.wkqeuzd.mongodb.net/?retryWrites=true&w=majority';

        //Url을 실제로 코드안에 작성하는 것은 매우 위험한 일임을 알아야함.
        //보안을 철저히 하기위해 variables.env 파일을 만든다.

        mongoose.connect(process.env.MONGODB_URL, { useNewUrlParser: true }, (err) => {
            if (err) {
                console.log(err);
            } else {
                console.log('Connected to database successfully');
            }
        });
    }
});

