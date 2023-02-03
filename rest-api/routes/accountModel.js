const mongoose = require("mongoose");

const accountSchema = new mongoose.Schema(
    {
        email: {
            type: String,
            required: true,
        },
        name: String,
        age: {
            type: Number,
            min: 18,
            max: 50
        },
        enrolled: {
            type: Date,
            default: Date.now
        }
    },
    {
        /*업데이트를 기록 */
        timestamps: true
    }
);

//mongoose.model()의 첫 번째 인자는 컬렉션 이름, 두 번째 인자는 내보낼 스키마의 이름
module.exports = mongoose.modelNames("Account", accountSchema);

//용어 정리
//-스키마 : 데이터의 틀(클래스)
//-모델 : 스키마를 통한 객체(객체)
//-디큐먼트 : MongoDB 상에 존재하는 각 데이터