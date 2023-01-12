var http = require('http'),
    Stream = require('stream').Transform,
    fs = require('fs');

var url = 'http://127.0.0.1:4040/images/osstem.png';

http.request(url, function(response){
    var data = new Stream();

    response.on('data', function(chunk){
        data.push(chunk);
    });

    response.on('end', function(){
        fs.writeFileSync('./downloaded/image.png', data.read());
    });
}).end();