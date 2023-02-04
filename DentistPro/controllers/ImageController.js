const Image = require('../models/Image');

//Show the list of Patients
const index = (req, res) => {
    Image.find()
        .then(response => {
            res.json({
                response
            });
        })
        .catch(error => {
            res.json({
                message: error
            });
        })
};

//Show single Patient
const show = (req, res) => {
    let ImageID = req.body.ImageID;
    Image.findById(ImageID)
        .then(response => {
            res.json({
                response
            });
        })
        .catch(error => {
            res.json({
                message: error
            });
        })
};

// Add new patient
const store = (req, res) => {
    let image = new Image({
        ImageName: req.body.ImageName,
        PatientName : req.body.PatientName,
        PixelLength: req.body.PixelLength,
        ImageKinds: req.body.ImageKinds,
        PhotoDate : req.body.PhotoDate,
    })
    // if requset store image file
    if (req.file) {
        image.ImageFile = req.file.path;
    }
    // if request store image files
    // if(req.files){
    //     let path = ''
    //     req.files.forEach(function(files, index, arr){
    //         path = path + files.path + ',';
    //     })
    //     path = path.substring(0, path.lastIndexOf(","));
    //     image.ImageFile = path;
    // }
    image.save()
        .then(response => {
            res.json({
                message: "Image Added Successfully!"
            });
        })
        .catch(error => {
            res.json({
                message: 'An error Occured!'
            })
        })
}

// Update an Patient
const update = (req, res) => {
    let ImageID = req.body.ImageID

    let updateData = {
        ImageName: req.body.ImageName,
        PatientName : req.body.PatientName,
        PixelLength: req.body.PixelLength,
        ImageKinds: req.body.ImageKinds,
        PhotoDate : req.body.PhotoDate,
    }

    Image.findByIdAndUpdate(ImageID, { $set: updateData })
        .then(() => {
            res.json({
                message: 'Image update successfully!'
            });
        })
        .catch(error => {
            res.json({
                message: 'An error Occured!'
            });
        })
};

//delete an patient
const destroy = (req, res) => {
    let ImageID = req.body.ImageID;
    Image.findByIdAndRemove(ImageID)
        .then(() => {
            res.json({
                message: 'Image deleted successfully!'
            });
        })
        .catch(error => {
            res.json({
                message: 'An error Occured!'
            });
        })
}

module.exports = {
    index, show, store, update, destroy
}


