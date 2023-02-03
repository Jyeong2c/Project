const express = require('express');
const mongoose = require('mongoose');
const morgan = require('morgan');
const bodyParser = require('body-parser');

require('dotenv').config({ path: 'variables.env' });

mongoose.connect(process.env.MONGODB_URL, { useNewUrlParser: true, useUnifiedTopology : true });
const db = mongoose.connection;

db.on('error', (err) => {
    console.log(err);
});

db.once('open', () => {
    console.log('Database Connection Established!');
});

const app = express()

app.use(morgan('dev'))
app.use(bodyParser.urlencoded({extended : true}));
app.use(bodyParser.json());
const PORT = process.env.PORT || 20000

app.listen(PORT, () => {
    console.log('Server is running on Port', PORT);
});