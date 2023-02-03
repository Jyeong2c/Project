const express = require('express');
const app = express();
const mongoose = require('mongoose');

mongoose.connect("mongodb+srv://user:1234@cluster0.2hgkt6j.mongodb.net/NodeConnect?retryWrites=true&w=majority",{
    //아래 두 옵션을 추가하지 않으면 경고를 출력
    useUndefiedTopology : true,
    useNewUrlParser : true,
});

const db = mongoose.connection;
db.on("error", (err) => console.error(err));
db.once("open", () => console.log("데이터베이스 연결 성공"));


const accountRouter = require("./routes");
app.use('/account', accountRouter);

app.listen(2500, () => {
    console.log("server started at 10000");
});