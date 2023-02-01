//mongoose database를 부르기 위한 패키지 설정
const mongoose = require('mongoose');

//env파일을 사용하기위한 패키지 'dotenv'
require('dotenv').config({path:'variables.env'});

//console.log(process.env.MONGODB_URL);

//mongoDB 사이트에서 만든 URL을 호출 (<password> = 1234(1234자리에 <password>라고 있었음))
//const MONGODB_URL = 'mongodb+srv://root:1234@education.wkqeuzd.mongodb.net/?retryWrites=true&w=majority';
 
//Url을 실제로 코드안에 작성하는 것은 매우 위험한 일임을 알아야함.
//보안을 철저히 하기위해 variables.env 파일을 만든다.

mongoose.connect(process.env.MONGODB_URL, { useNewUrlParser : true}, (err)=>{
    if(err){
        console.log(err);
    }else{
        console.log('Connected to database successfully');
    }
});