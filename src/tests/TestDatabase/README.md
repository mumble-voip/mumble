# Database tests

This directory contains test cases for Mumble's database (backend) implementation. The tests can (and generally should) be performed for multiple
database flavors: SQLite, MySQL and PostgreSQL. However for the latter two, it is necessary to perform some initial setup in order for the test
program to obtain access to the respective databases.

In order to avoid failing tests due to incorrect (or missing) setup, the tests for MySQL and PostgreSQL are disabled by default, but they can be
enabled via the `database-mysql-test` and `database-postgresql-test` cmake options (just set these to `ON` when invoking cmake).


## Required setup

The setup is very similar for MySQL and PostgreSQL. Remember that you need to execute the following instructions via an existing account with the
necessary privileges. On standard Ubuntu installations, this is most easily accomplished by connecting to the DB via `sudo mysql` and
`sudo -u postgres psql` respectively.

### MySQL

1. Create a database with the name `mumble_test_db`:
```sql
CREATE DATABASE mumble_test_db;
```

2. Create a new user with the name `mumble_test_user` and password `MumbleTestPassword`
```sql
CREATE USER 'mumble_test_user'@'localhost' IDENTIFIED BY 'MumbleTestPassword';
```

3. Grant this new user all privileges on the test database:
```sql
GRANT ALL PRIVILEGES ON mumble_test_db.* TO 'mumble_test_user'@'localhost';
```

4. Enable `log-bin-trust-function-creators` as otherwise creating triggers and stored functions would require global `SUPER` privilege for the Mumble
   user. To enable this mode, edit `/etc/mysql/my.cnf` and add
```
[mysqld]
log-bin-trust-function-creators = 1
```
  to that file. Afterwards, restart the MySQL daemon via `systemctl restart mysql`.

  Note that Mumble's database backend will take care of using a binlog format that should not cause issues with this setting.


### PostgreSQL

1. Create a database with the name `mumble_test_db`:
```sql
CREATE DATABASE mumble_test_db;
```

2. Create a new user with the name `mumble_test_user` and password `MumbleTestPassword`
```sql
CREATE USER mumble_test_user ENCRYPTED PASSWORD 'MumbleTestPassword';
```

3. Grant this new user all privileges on the test database:
```sql
GRANT ALL PRIVILEGES ON DATABASE mumble_test_db TO mumble_test_user;
```

