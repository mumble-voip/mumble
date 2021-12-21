# Docker Compose

## How to setup Mumble with Docker Compose?

1. Make a new directory and create a new file inside called docker-compose.yml

2. Copy the information from https://github.com/mumble-voip/mumble/scripts/docker-compose.yml into the new file.

3. Create the data folder and make 1000 it's owner.

4. Create a new file called murmur.ini and copy the information from https://github.com/mumble-voip/mumble/blob/master/scripts/murmur.ini

In this file ensure to change `database=` to `database=/var/lib/murmur/murmur.sqlite` to ensure that the database is created in the correct location. Edit the rest of the file to your liking.

5. Run docker-compose up -d to build and start the container.

## How do I view the logs?

To view the logs type `docker logs mumble-server`. You can add -f to make it follow the log automatically updating it.

## How do I use the release build instead of master?

To use stable version instead of the latest master change `#master` at the end of the build line in the docker compose to whichever branch you want to compile. For example, `build: github.com/mumble-voip/mumble#1.4.x` will build version 1.4.x instead of master.

## How do I add Letsencrypt Certificates with Docker?

To add Letsencrypt Certificates to docker you can move the certifcates to a location user 1000 can read and add an extra volume to the docker compose file.

Or you can add the run the docker container as root. To do this:

1. Add `user: "0:0"` to the docker compose file 

2. Add `- /etc/letsencrypt/:/certs:ro` to the volume section of the docker compose file.

3. Edit the murmur.ini changing `uname` to `uname=murmur` to make it switch to a non root user after reading the cert.

4. Change the `sslCert` and `sslKey` to the path of the certificate and key.
