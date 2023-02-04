const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const patientSchema = new Schema({
    Name : {
        type : String
    },
    Age : {
        type : Number
    },
    DoctorID : {
        type : String
    }
},{timestamps : true});

const Patient = mongoose.model('Patient', patientSchema);
module.exports = Patient;