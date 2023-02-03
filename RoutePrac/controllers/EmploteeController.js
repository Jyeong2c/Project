const Employee = require('../models/Employee');

//Show the list of Employees 전체 고객 리스트
const index = (req, res, next) => {
    Employee.find()
    .then(response => {
        res.json({
            response
        });
    })
    .catch(error => {
        res.json({
            message : 'An error Occured!'
        });
    })
};

// Show single employee 검색 된 고객
const show = (req, res, next) => {
    let employeeID = req.body.employeeID;
    Employee.findById(employeeID)
    .then(response => {
        res.json({
            response
        });
    })
    .catch(error => {
        res.json({
            message : 'An error Occured!'
        });
    })
};

// Add new employee 새 고객을 저장
const store = (req, res, next) => {
    let employee = new Employee({
        name : req.body.name,
        designation : req.body.designation,
        email : req.body.email,
        phone : req.body.phone,
        age : req.body.age
    })
    employee.save()
    .then(response => {
        res.json({
            message : 'Employee Added Successfully!'
        });
    })
    .catch(error => {
        res.json({
            message : 'An error Occured!'
        });
    })
}
 
// Update an employee 고객 업데이트
const update = (req, res, next) => {
    //업데이트 해야되는 직원의 아이디
    let employeeID = req.body.employeeID

    let updatedData = {
        name : req.body.name,
        designation : req.body.designation,
        email : req.body.email,
        phone : req.body.phone,
        age : req.body.age
    }

    Employee.findByIdAndUpdate(employeeID, {$set : updatedData})
    .then(() => {
        res.json({
            message : 'Employee updated successfully!'
        });
    })
    .catch(error => {
        res.json({
            message : 'An error Occured!'
        });
    })
}

//delete an employee
const destroy = (req, res, next) => {
    let employeeID = req.body.employeeID;
    Employee.findByIdAndRemove(employeeID)
    .then(() => {
        res.json({
            message : 'Employee deleted successfully!'
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
//컨트롤러 사용 준비 완료