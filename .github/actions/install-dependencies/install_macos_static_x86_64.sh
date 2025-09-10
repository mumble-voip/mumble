#!/usr/bin/env bash

set -e
set -x

source "$( dirname "$0" )/common.sh"

verify_required_env_variables_set

brew install coreutils aria2 gnu-tar xz

make_build_env_available "tar.xz"


# Setup PostgreSQL database for the Mumble tests
# Note: we don't configure MySQL as that's not installed on the Azure runners for macOS
# by default and installing it via homebrew takes forever.

brew install postgresql
brew services start postgresql

# Give the database some time to start
sleep 5

configure_database_tables "postgresql"
