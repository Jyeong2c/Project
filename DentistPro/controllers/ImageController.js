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
        PixelLength: req.body.PixelLength,
        ImageKinds: req.body.ImageKinds,
        ImagePathURL: req.body.ImagePathURL
    })
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
        PixelLength: req.body.PixelLength,
        ImageKinds: req.body.ImageKinds,
        ImagePathURL: req.body.ImagePathURL
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


