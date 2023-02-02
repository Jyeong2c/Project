const express = require('express');
const app = express();

app.listen(4500, (err) => {
    if (err) {
        return console.log(err);
    } else {
        console.log("runnig port 4500");
    }
});

var patients = [
  {patient:{ ID:'001', name:'홍길동', birth:'2000/10/01', doctor:'Dentist1' }}, 
  {patient:{ ID:'002', name:'안병곤', birth:'2001/12/30', doctor:'Dentist1' }},
  {patient:{ ID:'003', name:'김우준', birth:'2002/09/12', doctor:'Dentist2' }},
  {patient:{ ID:'004', name:'배경훈', birth:'2003/06/20', doctor:'Dentist2' }},
];

app.get('/patientlist', (req, res) => {

  var doctor = req.query.by;
  var patientlist;

  if(doctor != undefined)
  {
    patientlist = patients.filter(function(e){
      return e.patient.doctor === doctor;
    })
  }
  else
  {
    patientlist = patients;
  }

  res.json(patientlist);
  res.end();

});

app.get('/patient/:id', (req, res) => {

  var patientID = req.params.id;

  var patient = patients.filter(function(e){
    return e.patient.ID === patientID;
  })

  var imglist;

  imglist = images.filter(function(e){
    return e.image.patientID === patientID;
  })

  res.json(patient);
  res.end();
});

// 확인 방법
// http://localhost:3000/patientlist?by=Dentist1