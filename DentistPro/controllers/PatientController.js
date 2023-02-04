const Patient = require('../models/Patient');

//Show the list of Patients
const index = (req, res) => {
    Patient.find()
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

//Show single Patient
const show = (req, res) => {
    let PatientID = req.body.PatientID;
    Patient.findById(PatientID)
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

// Add new patient
const store = (req, res) => {
    let patient = new Patient({
        Name : req.body.Name,
        Age : req.body.Age,
        DoctorID : req.body.DoctorID
    })
    patient.save()
    .then(response => {
        res.json({
            message : "Patient Added Successfully!"
        });
    })
    .catch(error => {
        res.json({
            message : 'An error Occured!'
        })
    })
}

// Update an Patient
const update = (req, res) => {
    let patientID = req.body.PatientID

    let updateData = {
        Name : req.body.Name,
        Age : req.body.Age,
        DoctorID : req.body.DoctorID,
    }

    Patient.findByIdAndUpdate(patientID, {$set : updateData})
    .then(() => {
        res.json({
            message : 'Patient update successfully!'
        });
    })
    .catch(error => {
        res.json({
            message : 'An error Occured!'
        });
    })
};

//delete an patient
const destroy = (req, res) => {
    let PatientID = req.body.PatientID;
    Patient.findByIdAndRemove(PatientID)
    .then(() => {
        res.json({
            message : 'Patient deleted successfully!'
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


