var express = require('express');
var fs = require('fs');
var app = express();

app.use(express.bodyParser());
app.listen(4000, "localhost", function(){
    console.log('Server start on port 4000');
});

app.get('/', function(req, res){
    fs.readFile('index')
})