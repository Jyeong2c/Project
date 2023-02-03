const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const imageSchema = new Schema({
    ImageName : {
        type : String
    },
    PixelLength : {
        type : Number
    },
    ImageKinds : {
        type : String
    },
    ImagePathURL : {
        type : String
    }
},{timestamps : true});

const Image = mongoose.model('Image', imageSchema);
module.exports = Image;