// Load the TCP Library
net = require('net');

// Keep track of the chat clients
const clients = [];

// Start a TCP Server
net.createServer((socket) => {
  // Identify this client
  socket.name = socket.remoteAddress + ":" + socket.remotePort

  // Put this new client in the list
  clients.push(socket)

  console.log('----------------------------------------------------------------')
  console.log('----------------------------------------------------------------')
  console.log('----------------------------------------------------------------')

  console.log('INIT CONN', socket.name)

  // Handle incoming messages from clients.
  socket.on('data', (data) => console.log('DATA', socket.name + "> " + data))

  // Remove the client from the list when it leaves
  socket.on('end', () => console.log('END', socket.name))

  //Error event
  socket.on('error', (error) => console.log("Error", error))
})
.listen(3000);

// Put a friendly message on the terminal of the server.
console.log("Chat server running at port 5000\n");