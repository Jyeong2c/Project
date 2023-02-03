const express = require('express');
const mongoose = require('mongoose');
const morgan = require('morgan');
require('dotenv').config({path:'variables.env'});
//const PATIENTDB_URL = 'mongodb+srv://patients:1234@patients.d52ldtz.mongodb.net/?retryWrites=true&w=majority'
const bodyParser = require('body-parser');

mongoose.connect(process.env.PATIENTDB_URL, {useNewUrlParser : true, useUnifiedTopology : true});

const db = mongoose.connection;

db.on('error', (err) => {
    console.log(err);
});

db.once('open', () => {
    console.log('Database Connection Established!');
});

const app = express();

app.use(morgan('dev'));
app.use(bodyParser.urlencoded({extended : true}));
app.use(bodyParser.json());
const PORT = process.env.PORT || 40000;

app.listen(PORT, () => {
    console.log('Server is running on Port', PORT);
});

//routes of patient.js
const PatientRoute = require('./routes/patient');
//routes of image.js
const ImageRoute = require('./routes/image');

//use patientDB
app.use('/api/patient', PatientRoute);
//use imageDB
app.use('/api/image', ImageRoute);

