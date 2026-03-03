const express = require('express');
const app = express();

app.use('/static',express.static('static'));

app.get('/', (req,res) => {
    res.sendFile(__dirname + "/static/board.html");
    }   
);

app.get('/',(req,res) =>{
    res.sendFile(__dirname + "/static/titleScreen.html");
})
app.listen(3000, () => console.log('Listening on http://localhost:3000'));
