const Login = require('../models/Login');

//Show the list of LoginInfo
const index = (req, res) => {
    Login.find()
    .then(response => {
        res.json({
            response
        });
    })
    .catch(error => {
        res.json({
            message : error
        });
    })
};

//Show single LoginInfo
const show = (req, res) => {
    let LoginID = req.body.LoginID;
    Login.findById(LoginID)
    .then(response => {
        res.json({
            response
        });
    })
    .catch(error => {
        res.json({
            message : error
        });
    })
};

// Add new LoginInfo
const store = (req, res) => {
    let login = new Login({
        DoctorID : req.body.DoctorID,
        DoctorName : req.body.DoctorName,
        Password : req.body.Password
    })
    login.save()
    .then(response => {
        res.json({
            message : "LoginInfo Added Successfully!"
        });
    })
    .catch(error => {
        res.json({
            message : 'An error Occured!'
        })
    })
}

// Update an LoginInfo
const update = (req, res) => {
    let LoginID = req.body.LoginID

    let updateData = {
        DoctorID : req.body.DoctorID,
        DoctorName : req.body.DoctorName,
        Password : req.body.Password,
    }

    Login.findByIdAndUpdate(LoginID, {$set : updateData})
    .then(() => {
        res.json({
            message : 'LoginInfo update successfully!'
        });
    })
    .catch(error => {
        res.json({
            message : 'An error Occured!'
        });
    })
};

//delete an LoginInfo
const destroy = (req, res) => {
    let LoginID = req.body.LoginID;
    Login.findByIdAndRemove(LoginID)
    .then(() => {
        res.json({
            message : 'LoginInfo deleted successfully!'
        });
    })
    .catch(error => {
        res.json({
            message : 'An error Occured!'
        });
    })
}

module.exports = {
    index, show, store, update, destroy
}


