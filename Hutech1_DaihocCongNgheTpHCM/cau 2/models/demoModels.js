const mongoose = require('mongoose');
mongoose.Promise = global.Promise;

const Schema = mongoose.Schema;

const DemoSchema = new Schema({
    hoTen: String,
    sdt: String,
    soLuongNguoi: String,
    maThe: String,
    soPhong: String,
    matKhau: String
});

const Demo = mongoose.model('Demo', DemoSchema);
module.exports  = Demo; 