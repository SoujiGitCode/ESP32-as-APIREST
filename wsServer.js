const { WebSocketServer } = require('ws');

const port = 8080;
const server = new WebSocketServer({ port });

server.on('listening', () => {
    console.log(`WebSocket server listening on port ${port}`);
});

server.on('connection', (socket) => {
    console.log('Client connected');

    const sendJsonData = () => {
        const data = {
            sections: {
                a: {
                    a1: getRandomInt(),
                    a2: getRandomInt(),
                    a3: getRandomInt(),
                    a4: getRandomInt()
                },
                b: {
                    b1: getRandomInt(),
                    b2: getRandomInt(),
                    b3: getRandomInt(),
                    b4: getRandomInt()
                },
                c: {
                    c1: getRandomInt(),
                    c2: getRandomInt(),
                    c3: getRandomInt(),
                    c4: getRandomInt()
                }
            }
        };

        const jsonData = JSON.stringify(data);

        socket.send(jsonData, (error) => {
            if (error) {
                console.error(`Error sending data: ${error.message}`);
            }
        });
    };

    sendJsonData();

    // Send the JSON data every 1 second
    const intervalId = setInterval(sendJsonData, 1000);

    socket.on('close', () => {
        console.log('Client disconnected');
        clearInterval(intervalId);
    });

    socket.on('error', (error) => {
        console.error(`Socket error: ${error.message}`);
    });
});

function getRandomInt() {
    return Math.floor(Math.random() * 51) + 50;
}
