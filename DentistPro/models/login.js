const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const loginSchema = new Schema({
    DoctorID : {
        type : String
    },
    DoctorName : {
        type : String
    },
    Password : {
        type : String
    }
},{timestamps : true});

const Login = mongoose.model('Login', loginSchema);
module.exports = Login;