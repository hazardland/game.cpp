
// server.js
const WebSocket = require('ws');

const wss = new WebSocket.Server({ port: 9000 });

console.log('[Node Server] Listening on ws://localhost:9000');

wss.on('connection', function connection(ws) {
    console.log('[Node Server] Client connected');

    ws.on('message', function incoming(message) {
        console.log('[Node Server] Received:', message.toString());

        // Echo it back
        ws.send(message);
    });

    ws.on('close', () => {
        console.log('[Node Server] Client disconnected');
    });

    ws.on('error', (err) => {
        console.error('[Node Server] Error:', err);
    });
});
