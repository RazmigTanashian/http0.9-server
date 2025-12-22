# http0.9-server
An educational project to implement an http0.9 server in C

Test Steps:
    - Open two terminals
    - Run server program on one terminal
    - On the second terminal run 'curl --http0.9 http://127.0.0.1:8080/index.html'
    - On the window the curl is being ran from, verify that contents of index.html is received
