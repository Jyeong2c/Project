const http = require('http');
const url = require('url');
var mime = require('mime');

const hostname = '127.0.0.1';
const port = 3000;

const server = http.createServer((req, res) => {
  const requestUrl = url.parse(req.url);
  const path = requestUrl.pathname;

  const parts = path.split('/').slice(1);

  // This is really brittle, but assuming you know it's going to be 2 parts remaining after the above..
  var parsedUrl = url.parse(request.url);
    var resource = parsedUrl.pathname;

    //2. 요청한 자원의 주소가 '/images/'문자열로 시작하면
    if(resource.indexOf('/Panorama/') == 0){
        //3. 첫글자 인 '/'를 제외하고 경로를 imgPath 변수에 저장
        var imgPath = resource.substring(1);
        console.log('imgPath='+imgPath);
        //4. 서비스 하려는 파일의 mime type
        var imgMime = mime.getType(imgPath);    //lookup->getType으로 변경됨
        console.log('mime='+imgMime);

        //5.해당 파일을 읽어 오는데 두번째 인자인 인코딩(utf-8)값 업음
        fs.readFile(imgPath, function(error, data){
            if(error){
                response.writeHead(500, {'Content-Type':'text/html'});
                response.end('500 Internal Server' + error);
            }else{
                //6.Content-Type 에 4번에서 추출한 mime type을 입력
                response.writeHead(200, {'Content-Type':imgMime});
                response.end(data);
            }
        });
    }else{
        response.writeHead(404, {'Content-Type': 'text/html'});
        response.end('404 Page Not Found');
    }

  res.statusCode = 200;
  res.setHeader('Content-Type', 'text/plain');
  res.end(parts[1]);
});

server.listen(port, hostname, () => {
  console.log(`Server running at http://${hostname}:${port}/`);
});