# Docker Compose

## Configuring Murmur

1. Create a new directory and switch to it

2. Create the file `docker-compose.yml` and copy the [default docker-compose.yml](../scripts/docker-compose.yml) into it

3. Create the directory `data` and make `1000` its owner

4. Create the file `murmur.ini` and copy the [default murmur.ini](../scripts/murmur.ini) into it

5. Change `database` in `murmur.ini` to `/var/lib/murmur/murmur.sqlite`

6. Run `docker-compose up -d`

## Viewing the log

Run `docker logs mumble-server`.

## Using the release build instead of the master branch

Change `#master` at the end of the build line in `docker-compose.yml` to the branch you want to compile. For example, `build: github.com/mumble-voip/mumble#1.4.x` will build version `1.4.x`.

## How do I add Let's Encrypt certificates with Docker Compose?

To add Let's Encrypt certificates to the container you can move the certificates to a location user `1000` can read and add an extra volume to the Docker Compose file.

Alternatively, you can run the Docker container as root by following these steps:

1. Add `user: "0:0"` to the Docker Compose file

2. Add `- /etc/letsencrypt/:/certs:ro` to the volume section of the Docker Compose file

3. Change `uname` to `uname=murmur` in murmur.ini to make Murmur switch to a non root user after reading the cert

4. Change `sslCert` and `sslKey` to the path of the certificate and key respectively
