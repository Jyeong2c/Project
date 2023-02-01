//json형태의 데이터를 주고받기 위함과 서버를 활성화하기 위한 패키지
const express = require('express');
//현재 있는 데이터들을 json 포멧으로 읽을 수 있게 도와주는 패키지
//만일 body-parser를 쓰지 않으면 postman에서 post를 할 때 데이터가 undefined으로 표시
const bodyParser = require('body-parser');
const server = express();

//bodyParser를 씀으로써 post로 들어오는 모든 데이터를 서버에서 읽을 수 있게 됨
server.use(bodyParser.json());

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

//get method로 요청이 왔을 경우 users 배열에 있는 데이터를 응답
server.get("/api/user", (req, res) =>{
    res.json(users);
});

//파라미터를 사용하는 방법(여기서 주의사항은 반드시 해당 URL밑에 파라미터를 만들어야 함)
//localhost:3000/api/user/001 라고 URL에 불러올 수 있음.
server.get('/api/user/:id', (req, res)=>{
    //console.log(req.params.id); 응답은 없지만 콘솔에 해당하는 아이디를 출력
    /*해당 아이디를 찾는 변수를 설정*/
    const user = users.find((u) =>{
        return u.id === req.params.id;
        /*만일 매칭이 안되는 경우 반환값은 NULL이 되어버림 */
    });

    if(user){
        //해당 user을 찾은 경우 user에 대한 json 데이터를 출력
        res.json(user);
    }else{
        //없는 경우 404 에러 메세지를 출력
        res.status(404).json({errrorMessage : "User was not found"});
    }
});

//post method로 요청이 왔을 경우 user 배열에 있는 데이터를 응답
//post method는 실제 데이터를 보낼 때 해당 데이터를 감추어서 보내는 메소드이다.
server.post("/api/user", (req, res) => {
    //console.log(req.body); 들어오는 데이터를 console로 받음
    /* 만일 users에 데이터를 넣고 싶다면*/
    users.push(req.body);
    res.json(users);
});

//localhost:3000 으로 서버를 활성화 
server.listen(3000, () =>{
    console.log("The Server is running");
});

