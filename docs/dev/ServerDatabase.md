# Server database

The Mumble server uses a database to persist data across restarts. This document is about the (high-level) implementation details.

The database implementation consists of two separate parts. First, the general DB wrappers that allow managing the database convenient from within C++
(mostly) without having to worry about the exact backend that is used. Second, the server-specific implementation which encodes the exact identity of
tables and ways to access them. The former implementation lives under `src/database` and is completely decoupled from the server and even mostly
decoupled from Mumble as a whole. The latter lives under `src/murmur/database`.

Overall, we are using [SOCI](https://github.com/SOCI/soci/) for abstracting and unifying the low-level communication with different database backends.


## Transactions

A core design philosophy of our database implementation is that **every** database operation (READ _and_ WRITE) is encapsulated by a transaction
ensuring that all operations happen atomically (from a logical point of view).

Oftentimes, a given function doesn't know whether the parent function has already created a transaction. To avoid creating deeply nested transactions,
the `Database` and `Table` base classes have a member function `ensureTransaction` that will create a transaction, if none is already in progress. The
returned `TransactionHolder` object can be treated the same in both cases as it has the necessary logic embedded to deal with cases in which no new
transaction was created.


## Exceptions

Errors during any database operation are communicated by means of exceptions. In order to ensure that the exceptions will have the expected type, make
sure to always wrap SOCI operations in a `try`-`catch` block that rethrows exceptions wrapped in one of Mumble's own database exception classes. Use
`std::throw_with_nested` function of the C++ standard.


## Database migrations

If a server is started on a database that has an older scheme version than the server wants to use, an automatic migration procedure is started that
will update the database to conform to the new scheme. This encompasses a complete re-creation of all tables followed by data migration from the old
tables into the new ones. Once everything has successfully completed, the old tables will be dropped.


## Changing the database scheme

The database scheme is a monotonically increasing number that acts as a version number for the database layout. The layout encompasses the existence
of specific tables, columns (along with their data types) within tables and constraints, keys, etc. within tables.

Any changes to the database layout needs to be accompanied by the following steps:

1. Increase the scheme version by one. The scheme version is defined by the constant `ServerDatabase::DB_SCHEME_VERSION` (see
   [here](https://github.com/mumble-voip/mumble/blob/4ac51e86ff7b2243774d86a4d9fdac548127389a/src/murmur/database/ServerDatabase.h#L40).
2. Adapt the `migrate` function of affected tables to data from the old table (which has the same name but with an added suffix of
   `Database::OLD_TABLE_SUFFIX`) into the new table. If all data is simply copied from a column in the old table into a column of the same name in the
   new table, without any required type transformations, it is sufficient to call the base implementation `Table::migrate` which will take care of
   that. When implementing these function explicitly, it is important that while columns names in the new table should use the constants defined in
   the `column` namespace, the names of the columns in the old table should always be written out **explicitly**. This is to ensure that even if the
   names change in newer versions of the table (i.e. the ones in the `column` namespace change), the migration code still works as expected. For an
   example see [here](https://github.com/mumble-voip/mumble/blob/4ac51e86ff7b2243774d86a4d9fdac548127389a/src/murmur/database/BanTable.cpp#L372).
3. Add test cases for the migration path as well as the new DB functionality you added.


### Migration test cases

Test cases for server migration are steered by the data found in `src/tests/TestDatabase/server/table_data`. This directory contains JSON files
describing the tables and their contents at different scheme versions along with the expected outcome after a successful migration has been performed
on that data.

By looking through the existing data, you should be able to get a feeling for the format. It should be noted that it is possible to inherit data from
previous scheme versions and then only perform modifications based off that. Again, search the existing examples for how to achieve this. Worst case,
you can also inspect `src/tests/TestDatabase/server/JSONAssembler.cpp` which is responsible for assembling the final JSON used to initialize the test
tables.
