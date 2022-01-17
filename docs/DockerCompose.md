# Docker Compose

## Configuring Murmur

1. Create a new directory and switch to it.

2. Create the file `docker-compose.yml` and copy the [default docker-compose.yml](../scripts/docker-compose.yml) into it.

3. Create the directory `data` and make `1000` its owner.

4. Create the file `murmur.ini` and copy the [default murmur.ini](../scripts/murmur.ini) into it.

5. Change `database` in `murmur.ini` to `/var/lib/murmur/murmur.sqlite`.

6. Run `docker-compose up -d`. Append the `--build` switch to force rebuilding, if needed (e.g. new commits since last build).

## Viewing the log

Run `docker logs mumble-server`.

## Using the release build instead of the master branch

Change `#master` at the end of the build line in `docker-compose.yml` to the branch you want to compile. For example, `build: github.com/mumble-voip/mumble#1.4.x` will build version `1.4.x`.

## Adding a certificate

First, give Murmur permission to read the certificate files. There are two options for this:

1. Move the certificate files to a location user `1000` can read
2. Have Murmur read the certificate files as `root` and then switch accounts. This is done by adding `user: "0:0"` to `docker-compose.yml` and changing `uname` to `uname=murmur` in `murmur.ini`

Then add a volume with the certificate files in the `volume` section of `docker-compose.yml`: `- /path/to/certificate-directory/:/certs:ro`.

Finally, change `sslCert` and `sslKey` in `murmur.ini` to their respective paths and run `docker-compose down` followed by `docker-compose up -d`.
