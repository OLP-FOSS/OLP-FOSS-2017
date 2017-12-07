const express = require('express');
const bodyParser = require('body-parser').urlencoded({extended: false});
const mongodb = require('mongodb');
const mongoose = require('mongoose');
mongoose.Promise = global.Promise;
// import Models
const Demo = require('./models/demoModels');

const app = express();


// set up app
app.set('views','./views');
app.set('view engine', 'ejs');
app.use(express.static('./public'));

// route app
// api get
// app.get('/', (req, res)=>{
//     res.render('login');
// })

app.get('/api/danh-sach/:pn', (req, res)=>{
    const { pn } = req.params;
    // console.log({pn});
    Demo.find(pn)
        .then(result => res.json(result))
        .catch(err => console.log(err.message));
        
    // Demo.find({id})
    // .then(result => {
    //    res.json(result);
    // })
    // .catch(err => console.log(err.message));
})
app.get('/api/danh-sach', (req, res)=>{
        
    Demo.find({})
    .then(result => {
       res.json(result);
    })
    .catch(err => console.log(err.message));
})
app.get('/danh-sach', (req, res)=>{
    Demo.find({})
        .then(result => {
            res.render('danhSach', {data: result});
        })
        .catch(err => console.log(err.message));
});

app.get('/dang-ky', (req, res)=>{
    res.render('login');
})
//test đăng ký 1 thành viên
app.post('/dang-ky', bodyParser, (req, res)=>{
    const { hoTen, sdt, soLuongNguoi, maThe, soPhong, matKhau } = req.body;
    // console.log({ hoTen, sdt, soLuongNguoi, maThe, soPhong, matKhau });
    const a = new Demo({hoTen, sdt, soLuongNguoi, maThe, soPhong,matKhau});
    a.save()
        .then(()=> res.redirect('/danh-sach'))
        .catch(err => console.log(err.message));
});
app.get('/yeu-cau', (req, res)=>{   
    res.render('yeuCau');
})


const uri = "mongodb://localhost/makerthon2017";
mongoose.connect(uri, {useMongoClient: true});
mongoose.connection.once('open', ()=>{
    app.listen(3000, ()=> console.log('Server start at port 3000'));
});