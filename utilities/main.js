import http from 'node:http';

const server = http.createServer((req, res) => {
  res.write("Hello, World!");
  res.end();
});

server.listen(3000, () => {
  console.log('Listening on port 3000');
});

setTimeout(() => {
  server.close();
}, 10000);
