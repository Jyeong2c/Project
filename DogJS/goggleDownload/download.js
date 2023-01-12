//원하는 경로에 다운로드 할 수 있는 파일

var http = require('http'),
    Stream = require('stream').Transform,
    fs = require('fs');

var url = 'http://127.0.0.1:3000/google';

http.request(url, function(response){
    var data = new Stream();

    response.on('data', function(chunk){
        data.push(chunk);
    });

    response.on('end', function(){
        fs.writeFileSync('./downloaded/image.png', data.read());
    });
}).end();