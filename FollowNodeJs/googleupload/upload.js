var formidable = require('formidable');
var http = require('http');
var fs = require('fs');

//html 양식을 생성
http.createServer(function(req, res){
    if(req.url == '/fileupload'){
        var form = new formidable.IncomingForm();
        form.parse(req, function(err, fields, files){
            var oldpath = files.filetoupload.filepath;
            var newpath = 'D:/GitDesktop/Project/FollowNodeJs/googleupload/files/' 
                            + files.filetoupload.originalFilename;
            fs.rename(oldpath, newpath, function(err){
                if(err) throw err;
                res.write('File uploaded and moved!');
                res.end();
            });
        });
    }else{
        res.writeHead(200, {'Content-Type': 'text/html'});
        res.write('<form action="fileupload" method="post" enctype="multipart/form-data">');
        res.write('<input type="file" name="fileoupload"><br>');
        res.write('<input type="submit">');
        res.write('</form>');
        return res.end();
    }
}).listen(4080);

