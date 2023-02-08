const Image = require('../models/Image');
//const multer = require('multer');
const fs = require('fs');

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

//Show single ImageInfo
const show = (req, res) => {
    let ImageID = req.body.ImageID;
    //console.log(multer.toString(req.file.ImageFile));
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

// Add new ImageInfo
const store = (req, res) => {
    let image = new Image({
        ImageName: req.body.ImageName,
        PatientName: req.body.PatientName,
        PixelLength: req.body.PixelLength,
        ImageKinds: req.body.ImageKinds,
        PhotoDate: req.body.PhotoDate,
    })
    // if requset store image file
    if (req.file) {
        image.ImageFile = req.file.path;
        //var filename = path.basename(req.file.path);  
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

// Update an ImageInfo
const update = (req, res) => {
    let ImageID = req.body.ImageID

    let updateData = {
        ImageName: req.body.ImageName,
        PatientName: req.body.PatientName,
        PixelLength: req.body.PixelLength,
        ImageKinds: req.body.ImageKinds,
        PhotoDate: req.body.PhotoDate,
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

//delete an ImageInfo
const destroy = (req, res) => {
    let ImageID = req.body.ImageID;
    // if (req.file) {
    //     image.ImageFile = req.file.path;
    // }

    // try{
    //     fs.unlinkSync('./uploads/1675661012939.png');
    //     console.log('Delete complete');
    // }catch(err){
    //     console.log(err);
    // }

    Image.findByIdAndRemove(ImageID)
        .then(() => {
            res.json({
                message: 'Image deleted successfully!',
            });
        })
        .catch(error => {
            res.json({
                message: 'An error Occured!'
            });
        })
}

//서버내의 이미지 삭제
const removeSync = (req, res) => {
    var pngFile = req.query.pngFile;

    res.json(pngFile);
    res.end();
    const directoryPath = "./uploads/";

    try {
        fs.unlinkSync(directoryPath + pngFile);

        res.status(200).send({
            message: "File is deleted.",
        });
    } catch (err) {
        res.status(500).send({
            message: "Could not delete the file. " + err,
        });
    }
};


module.exports = {
    index, show, store, update, destroy, removeSync
}


