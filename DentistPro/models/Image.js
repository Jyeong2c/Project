const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const imageSchema = new Schema({
    ImageName : {
        type : String
    },
    PatientName : {
        type : String
    },
    PixelLength : {
        type : Number
    },
    ImageKinds : {
        type : String
    },
    PhotoDate : {
        type : String
    },
    ImageFile : {
        type : String
    }
},{timestamps : true});

const Image = mongoose.model('Image', imageSchema);
module.exports = Image;