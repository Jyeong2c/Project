const express = require('express');
const formidable = require('formidable');
const app = express();
const http = require('http');
const url = require('url');
const fs = require('fs');

app.listen(5000, () => {
    console.log('Server listening on http://localhost:5000 ...');
});


app.get('/home', (req, res) => {
    res.send(`
    <form action="/upload" enctype="multipart/form-data" method="post">
      <div>File: <input type="file" name="files" /></div>
      <input type="submit" value="업로드" />
    </form>
    <form action="/download" enctype="multipart/form-data" method="get">
      <input type="submit" value="다운로드" />
    </form>
  `);
});

let lastFilepath = '';
app.post('/upload', (req, res, next) => {
    const form = formidable({
        multiples: true,
        uploadDir: './files',
    });

    form.parse(req, (err, fields, files) => {
        lastFilepath = files.files.filepath;  
    });

    res.redirect('./home');
});

app.get('/download', (req, res) => {
    console.log(lastFilepath);
    res.download(lastFilepath);
});