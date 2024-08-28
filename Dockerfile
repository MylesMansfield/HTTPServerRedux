# First stage: Build the application
FROM alpine:latest AS builder
RUN apk add --no-cache build-base
WORKDIR /app
COPY . .
RUN make

# Second stage: Create the runtime image
FROM alpine:latest
WORKDIR /app
COPY --from=builder /app/httpserver /app/httpserver
RUN mkdir -p /app/serverdir
EXPOSE 8080
CMD ["./httpserver", "-t", "7", "-l", "logfile.txt", "8080"]