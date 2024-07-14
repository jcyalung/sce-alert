import secret from "./secret.json";

const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
const client = require('prom-client');

// required 
const app = express();
app.use(cors());
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended: false}));
const BAD_REQUEST = 500;
const PORT = 8000;
/*
 curl -X POST \
 -H "Content-Type: application/json" \
 -d '{"content": "bruh", "tts": "false"}'\
 -s https://discord.com/api/webhooks/1260748111307870288/KEW3XT4qEJumZf8dqssLBEsolNZd_KjEcRw1UXeHWaMufXSntjEwJGk6YyxTDY_lYTFm

 curl -X POST \
 -H "Content-Type: application/json" \
 -d '{"content": "gyat", "tts": "false"}'\
 -s http://localhost:8000/send



 */

app.post('/send', (request, response) => {
    const { content } = request.body;
    console.log("request received for " + content);
    if(!content) { return response.status(BAD_REQUEST).send({"message" : "send valid message"}); }
    else {
        try {
            fetch(secret["discord_webhook"], {
                method: 'POST',
                headers: {
                  'Content-Type': 'application/json'
                },
                // body: '{"content": "bruh", "tts": "false"}',
                body: JSON.stringify({
                  'content': content,
                  'tts': 'false'
                })
              }).then((result) => {console.log(result);});
            return response.send({"message":"sent"});
        } catch(e) {
            console.log(e);
            return response.send({"error" : e})
        }
    }
})
app.get('/', (request, response) => {
    console.log("root received");
    response.status(200).send({"message":"hello gang"});
})

app.listen(PORT, () => {
    console.log(`Started server, Listening on port ${PORT}.`);
})