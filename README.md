# Basic Multithreaded HTTP server
Multithreaded HTTP server supporting: GET, PUT, and DELETE, supports multiple connections at once and logs replies.<br />
Loosely follows the HTTP 1.1 standard: https://www.w3.org/Protocols/rfc2616/rfc2616.html.

Usage: ./httpserver [-t threads] [-l logfile] [port]<br />
defaults: threads = 4, logfile = logfile.txt

(GET)     curl http://localhost:8080/ -o response.txt<br />
(PUT)     curl -X PUT -T feed.txt http://localhost:8080/foo.txt -o response.txt<br />
(DELETE)  curl -X DELETE http://localhost:8080/foo.txt -o response.txt<br />

Log Format:
[Time] - [Oper] [URI] [Status-Code]\n

### Docker Build Instructions:
```
docker build -t httpserver-image .
docker run -p 8080:8080 httpserver-image
```

### Raw Build Instructions:
```
make
./httpserver [-t threads] [-l logfile] [port]
```

### How to monitor server live in different terminal instance:
```
docker ps - (grab the container ID) 
docker exec -it [containerID] sh
```

This opens the server container which can be interacted with normally.<br />
To read the contents of logfile.txt run:
```
cat logfile.txt
```