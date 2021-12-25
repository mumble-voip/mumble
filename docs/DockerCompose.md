# Docker Compose

## How do I set up Mumble with Docker Compose?

1. Make a new directory and create a new file inside called docker-compose.yml

2. Copy the information from https://github.com/mumble-voip/mumble/scripts/docker-compose.yml into the new file

3. Create the data folder and make 1000 its owner

4. Create a new file called murmur.ini and copy the information from https://github.com/mumble-voip/mumble/blob/master/scripts/murmur.ini

Be sure to change `database=` to `database=/var/lib/murmur/murmur.sqlite` in this file to ensure that the database is created in the correct location. Edit the rest of the file to your liking

5. Run docker-compose up -d to build and start the container

## How do I view the logs?

Type `docker logs mumble-server` to view the logs. You can add -f to make it follow the log.

## How do I use the release build instead of master?

To use stable version instead of the latest master change `#master` at the end of the build line in the docker compose to whichever branch you want to compile. For example, `build: github.com/mumble-voip/mumble#1.4.x` will build version 1.4.x instead of master.

## How do I add Let's Encrypt certificates with Docker Compose?

To add Let's Encrypt certificates to the container you can move the certificates to a location user 1000 can read and add an extra volume to the Docker Compose file.

Alternatively, you can run the Docker container as root by following these steps:

1. Add `user: "0:0"` to the Docker Compose file

2. Add `- /etc/letsencrypt/:/certs:ro` to the volume section of the Docker Compose file

3. Change `uname` to `uname=murmur` in murmur.ini to make Murmur switch to a non root user after reading the cert

4. Change `sslCert` and `sslKey` to the path of the certificate and key respectively
