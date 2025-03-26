const WebSocket = require('ws');

const PORT = 9000;
const server = new WebSocket.Server({ port: PORT });

const clients = new Set();

server.on('connection', (ws, req) => {
  const ip = req.socket.remoteAddress;
  console.log(`[+] New client connected from ${ip}`);
  clients.add(ws);
  console.log(`Total clients: ${clients.size}`);

  ws.on('message', (message, isBinary) => {
    // console.log(`[>] Received ${isBinary ? 'binary' : 'text'} message of length ${message.length} from ${ip}`);

    if (!isBinary) return; // Skip non-binary messages

    // Relay to other clients
    for (const client of clients) {
      if (client !== ws && client.readyState === WebSocket.OPEN) {
        client.send(message, { binary: true });
      }
    }
  });

  ws.on('close', () => {
    clients.delete(ws);
    console.log(`[-] Client from ${ip} disconnected. Remaining: ${clients.size}`);
  });

  ws.on('error', (err) => {
    console.error(`[!] Error from ${ip}:`, err);
  });
});

console.log(`✅ WebSocket server running on ws://localhost:${PORT}`);
