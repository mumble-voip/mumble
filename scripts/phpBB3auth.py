#!/usr/bin/env python
# -*- coding: utf-8

#
#    phpBB3auth.py - Sample script to demonstrate authentication against
#                    an existing phpBB3 forum database.
#
#    Requirements:
#        python >=2.5
#        PIL >=1.1.6 (only if avatar import is enabled)
#        MySQLdb
#        daemon (when run as a daemon)

import sys
import Ice
import thread
import logging
import ConfigParser

from hashlib    import md5
from logging    import (debug,
                        info,
                        warning,
                        error,
                        critical,
                        getLogger)
from optparse   import OptionParser

def x2bool(s):
    """Helper function to convert strings from the config to bool"""
    if isinstance(s, bool):
        return s
    elif isinstance(s, basestring):
        return s.lower() in ['1', 'true']
    raise ValueError()

#
#--- Default configuration values
#
cfgfile = 'phpBB3auth.ini'
user_texture_resolution = (600,60)
default = {'database':(('lib', str, 'MySQLdb'),
                       ('name', str, 'phpbb3'),
                       ('user', str, 'phpbb3'),
                       ('password', str, 'secret'),
                       ('prefix', str, 'phpbb_'),
                       ('host', str, '127.0.0.1'),
                       ('port', int, 3306)),
                       
            'user':(('id_offset', int, 1000000000),
                    ('avatar_enable', x2bool, True),
                    ('avatar_path', str, 'http://forum.tld/phpBB3/download.php?avatar=')),
                    
            'ice':(('host', str, '127.0.0.1'),
                   ('port', int, 6502),
                   ('slice', str, 'Murmur.ice')),
                   
            'murmur':(('servers', lambda x:map(int, x.split(',')), []),),
            'glacier':(('enabled', x2bool, False),
                       ('user', str, 'phpBB3auth'),
                       ('password', str, 'secret'),
                       ('host', str, 'localhost'),
                       ('port', int, '4063')),
                       
            'log':(('level', int, logging.DEBUG),
                   ('file', str, 'phpBB3auth.log'))}
 
#
#--- Helper classes
#
class config(object):
    """
    Small abstraction for config loading
    """

    def __init__(self, filename = None, default = None):
        if not filename or not default: return
        cfg = ConfigParser.ConfigParser()
        cfg.read(filename)
        
        for h,v in default.iteritems():
            self.__dict__[h] = config()
            for name, conv, vdefault in v:
                try:
                    self.__dict__[h].__dict__[name] = conv(cfg.get(h, name))
                except (ValueError, ConfigParser.NoSectionError, ConfigParser.NoOptionError):
                    self.__dict__[h].__dict__[name] = vdefault
                    
class threadDbException(Exception): pass
class threadDB(object):
    """
    Small abstraction to handle database connections for multiple
    threads
    """
    
    db_connections = {}

    def connection(cls):
        tid = thread.get_ident()
        try:
            con = cls.db_connections[tid]
        except:
            info('Connecting to database server (%s %s:%d %s) for thread %d',
                 cfg.database.lib, cfg.database.host, cfg.database.port, cfg.database.name, tid)
            
            try:
                con = db.connect(host = cfg.database.host,
                                   port = cfg.database.port,
                                   user = cfg.database.user,
                                   passwd = cfg.database.password,
                                   db = cfg.database.name)
            except db.Error, e:
                error('Could not connect to database: %s', str(e))
                raise threadDbException()
            cls.db_connections[tid] = con
        return con
    connection = classmethod(connection)
    
    def cursor(cls):
        return cls.connection().cursor()
    cursor = classmethod(cursor)
    
    def execute(cls, *args, **kwargs):
        c = cls.cursor()
        try:
            c.execute(*args, **kwargs)
        except db.OperationalError, e:
            error('Database operational error %d: %s', e.args[0], e.args[1])
            c.close()
            cls.invalidate_connection()
            raise threadDbException()
        return c
    execute = classmethod(execute)
    
    def invalidate_connection(cls):
        tid = thread.get_ident()
        con = cls.db_connections.pop(tid, None)
        if con:
            debug('Invalidate connection to database for thread %d', tid)
            con.close()
            
    invalidate_connection = classmethod(invalidate_connection)
    
    def disconnect(cls):
        while cls.db_connections:
            tid, con = cls.db_connections.popitem()
            debug('Close database connection for thread %d', tid)
            con.close()
    disconnect = classmethod(disconnect)
            
def do_main_program():
    #
    #--- Authenticator implementation
    #    All of this has to go in here so we can correctly daemonize the tool
    #    without loosing the file descriptors opened by the Ice module
    
    Ice.loadSlice(cfg.ice.slice)
    import Murmur
    
    class phpBBauthenticatorApp(Ice.Application):
        def run(self, args):
            self.shutdownOnInterrupt()
            ice = self.communicator()
            
            if cfg.glacier.enabled:
                #info('Connecting to Glacier2 server (%s:%d)', glacier_host, glacier_port)
                error('Glacier support not implemented yet')
                #TODO: Implement this
    
            info('Connecting to Ice server (%s:%d)', cfg.ice.host, cfg.ice.port)
            base = ice.stringToProxy('Meta:tcp -h %s -p %d' % (cfg.ice.host, cfg.ice.port))
            try:
                meta = Murmur.MetaPrx.checkedCast(base)
            except Ice.LocalException, e:
                error('Could not connect to Ice server, error %d: %s', e.error, str(e).replace('\n', ' '))
                return 1
        
            adapter = ice.createObjectAdapterWithEndpoints('Callback.Client', 'tcp -h %s' % cfg.ice.host)
            adapter.activate()
        
            for server in meta.getBootedServers():
                if not cfg.murmur.servers or server.id() in cfg.murmur.servers:
                    info('Setting authenticator for server %d', server.id())
                    authprx = adapter.addWithUUID(phpBBauthenticator(server, adapter))
                    auth = Murmur.ServerUpdatingAuthenticatorPrx.uncheckedCast(authprx)
                    server.setAuthenticator(auth)
            
            # Serve till we are stopped
            self.communicator().waitForShutdown()
            
            if self.interrupted():
                warning('Caught interrupt, shutting down')
            
            threadDB.disconnect()
            return 0
            
    class phpBBauthenticator(Murmur.ServerUpdatingAuthenticator):
        texture_cache = {}
        def __init__(self, server, adapter):
            Murmur.ServerUpdatingAuthenticator.__init__(self)
            self.server = server
            
            
        def authenticate(self, name, pw, certlist, certhash, strong, current = None):
            """
            This function is called to authenticate a user
            """
            
            # Search for the user in the database
            FALL_THROUGH = -2
            AUTH_REFUSED = -1
            
            if name == 'SuperUser':
                debug('Forced fall through for SuperUser')
                return (FALL_THROUGH, None, None)
            
            try:
                sql = 'SELECT user_id, user_password, user_type, username FROM %susers WHERE (user_type = 0 OR user_type = 3) AND LOWER(username) = LOWER(%%s)' % cfg.database.prefix
                cur = threadDB.execute(sql, name)
            except threadDbException:
                return (FALL_THROUGH, None, None)
            
            res = cur.fetchone()
            cur.close()
            if not res:
                info('Fall through for unknown user "%s"', name)
                return (FALL_THROUGH, None, None)
    
            uid, upw, utp, unm = res
            if phpbb_check_hash(pw, upw):
                # Authenticated, fetch group memberships
                try:
                    sql = 'SELECT group_name FROM %suser_group JOIN %sgroups USING (group_id) WHERE user_id = %%s' % (cfg.database.prefix, cfg.database.prefix)
                    cur = threadDB.execute(sql, uid)
                except threadDbException:
                    return (FALL_THROUGH, None, None)
                
                res = cur.fetchall()
                cur.close()
                if res:
                    res = [a[0] for a in res]
    
                info('User authenticated: "%s" (%d)', name, uid + cfg.user.id_offset)
                debug('Group memberships: %s', str(res))
                return (uid + cfg.user.id_offset, name, res)
            
            info('Failed authentication attempt for user: "%s" (%d)', name, uid + cfg.user.id_offset)
            return (AUTH_REFUSED, None, None)
            
        
        def getInfo(self, id, current = None):
            """
            Gets called to fetch user specific information
            """
            
            # We do not expose any additional information so always fall through
            debug('getInfo for %d -> denied', id)
            return (False, None)
        
        
        def nameToId(self, name, current = None):
            """
            Gets called to get the id for a given username
            """
            
            FALL_THROUGH = -2
            if name == 'SuperUser':
                debug('nameToId SuperUser -> forced fall through')
                return FALL_THROUGH
            
            try:
                sql = 'SELECT user_id FROM %susers WHERE (user_type = 0 OR user_type = 3) AND LOWER(username) = LOWER(%%s)' % cfg.database.prefix
                cur = threadDB.execute(sql, name)
            except threadDbException:
                return FALL_THROUGH
            
            res = cur.fetchone()
            cur.close()
            if not res:
                debug('nameToId %s -> ?', name)
                return FALL_THROUGH
            
            debug('nameToId %s -> %d', name, (res[0] + cfg.user.id_offset))
            return res[0] + cfg.user.id_offset
        
        
        def idToName(self, id, current = None):
            """
            Gets called to get the username for a given id
            """
            
            FALL_THROUGH = ""
            # Make sure the ID is in our range and transform it to the actual phpBB3 user id
            if id < cfg.user.id_offset:
                return FALL_THROUGH 
            bbid = id - cfg.user.id_offset
            
            # Fetch the user from the database
            try:
                sql = 'SELECT username FROM %susers WHERE (user_type = 0 OR user_type = 3) AND user_id = %%s' % cfg.database.prefix
                cur = threadDB.execute(sql, bbid)
            except threadDbException:
                return FALL_THROUGH
            
            res = cur.fetchone()
            cur.close()
            if res:
                if res[0] == 'SuperUser':
                    debug('idToName %d -> "SuperUser" catched')
                    return FALL_THROUGH
                
                debug('idToName %d -> "%s"', id, res[0])
                return res[0]
            
            debug('idToName %d -> ?', id)
            return FALL_THROUGH
            
            
        def idToTexture(self, id, current = None):
            """
            Gets called to get the corresponding texture for a user
            """

            FALL_THROUGH = ""
            
            debug('idToTexture for %d', id)
            if id < cfg.user.id_offset or not cfg.user.avatar_enable:
                debug('idToTexture %d -> fall through', id)
                return FALL_THROUGH
            
            # Otherwise get the users texture from phpBB3
            bbid = id - cfg.user.id_offset
            try:
                sql = 'SELECT user_avatar, user_avatar_type FROM %susers WHERE (user_type = 0 OR user_type = 3) AND user_id = %%s' % cfg.database.prefix
                cur = threadDB.execute(sql, bbid)
            except threadDbException:
                return FALL_THROUGH
            
            res = cur.fetchone()
            cur.close()
            if not res:
                debug('idToTexture %d -> user unknown, fall through', id)
                return FALL_THROUGH
            avatar_file, avatar_type = res
            if avatar_type != 1 and avatar_type != 2:
                debug('idToTexture %d -> no texture available for this user (%d), fall through', id, avatar_type)
                return FALL_THROUGH
            
            if avatar_file in self.texture_cache:
                return self.texture_cache[avatar_file]
            
            if avatar_type == 1:
                url = cfg.user.avatar_path + avatar_file
            else:
                url = avatar_file
                
            try:
                handle = urllib2.urlopen(url)
                file = StringIO.StringIO(handle.read())
                handle.close()
            except urllib2.URLError, e:
                warning('Image download for "%s" (%d) failed: %s', url, id, str(e))
                return FALL_THROUGH
            
            try:
                img = Image.open(file).convert("RGBA")
                img = img.transform(user_texture_resolution,
                                    Image.EXTENT,
                                    (0, 0, user_texture_resolution[0], user_texture_resolution[1]))
                r,g,b,a = img.split()
                raw = Image.merge('RGBA', (b, g, r, a)).tostring()
                comp = compress(raw)
                res = pack('>L', len(raw)) + comp
            except Exception, e:
                warning('Image manipulation for "%s" (%d) failed', url, id)
                debug(e)
                return FALL_THROUGH

            self.texture_cache[avatar_file] = res            
            return res
            
        
        def registerUser(self, name, current = None):
            """
            Gets called when the server is asked to register a user.
            """
            
            FALL_THROUGH = -2
            debug('registerUser "%s" -> fall through', name)
            return FALL_THROUGH
        
        
        def unregisterUser(self, id, current = None):
            """
            Gets called when the server is asked to unregister a user.
            """
            
            FALL_THROUGH = -1
            # Return -1 to fall through to internal server database, we will not modify the phpbb3 database
            # but we can make murmur delete all additional information it got this way.
            debug('unregisterUser %d -> fall through', id)
            return FALL_THROUGH
        
        
        def getRegisteredUsers(self, filter, current = None):
            """
            Returns a list of usernames in the phpBB3 database which contain
            filter as a substring.
            """
            
            if not filter:
                filter = '%'
            
            try:
                sql = 'SELECT user_id, username FROM %susers WHERE (user_type = 0 OR user_type = 3) AND username LIKE %%s' % cfg.database.prefix
                cur = threadDB.execute(sql, filter)
            except threadDbException:
                return {}
    
            res = cur.fetchall()
            cur.close()
            if not res:
                debug('getRegisteredUsers -> empty list for filter "%s"', filter)
                return {}
            debug ('getRegisteredUsers -> %d results for filter "%s"', len(res), filter)
            return dict([(a + cfg.user.id_offset, b) for a,b in res])
        
        
        def setInfo(self, id, info, current = None):
            """
            Gets called when the server is supposed to save additional information
            about a user to his database
            """
            
            FALL_THROUGH = -1
            # Return -1 to fall through to the internal server handler. We must not modify
            # the phpBB3 database so the additional information is stored in murmurs database
            debug('setInfo %d -> fall through', id)
            return FALL_THROUGH
        
        
        def setTexture(self, id, texture, current = None):
            """
            Gets called when the server is asked to update the user texture of a user
            """
            
            FAILED = 0
            FALL_THROUGH = -1
            
            if id < cfg.user.id_offset:
                debug('setTexture %d -> fall through', id)
                return FALL_THROUGH
            
            if cfg.user.avatar_enable:
                # Report a fail (0) as we will not update the avatar in the phpBB3 database.
                debug('setTexture %d -> failed', id)
                return FAILED
            
            # If we don't use textures from phpbb we let mumble save it
            debug('setTexture %d -> fall through', id)
            return FALL_THROUGH
            
        
    class CustomLogger(Ice.Logger):
        """
        Logger implementation to pipe Ice log messages into
        out own log
        """
        
        def __init__(self):
            Ice.Logger.__init__(self)
            self._log = getLogger("Ice")
            
        def _print(self, message):
            self._log.info(message)
            
        def trace(self, category, message):
            self._log.debug("Trace %s: %s", category, message)
            
        def warning(self, message):
            self._log.warning(message)
            
        def error(self, message):
            self._log.error(message)

    #
    #--- Start of authenticator
    #
    info('Starting phpBB3 mumble authenticator')
    initdata = Ice.InitializationData()
    initdata.logger = CustomLogger()
    
    app = phpBBauthenticatorApp()
    state = app.main(sys.argv[:1], initData = initdata)
    info('Shutdown complete')



#
#--- Python implementation of the phpBB3 check hash function (salted md5)
#
def _hash_encode64(sinput, count, itoa64):
    output = ''
    i = 0
    while True:
        value = ord(sinput[i])
        i += 1
        output += itoa64[value & 0x3f]
        
        if i < count:
            value |= (ord(sinput[i]) << 8)
        
        output += itoa64[(value >> 6) & 0x3f]
        
        if i >= count:
            break
        i += 1
        
        if i < count:
            value |= (ord(sinput[i]) << 16)
        
        output += itoa64[(value >> 12) & 0x3f]
        
        if i >= count:
            break
        
        i = i + 1
        output += itoa64[(value >> 18) & 0x3f]
        if i >= count:
            break
    return output

def _hash_crypt_private(password, settings, itoa64):
    output = '*'
    
    if settings[0:3] != '$H$':
        return output
    
    try:
        count_log2 = itoa64.index(settings[3])
    except ValueError:
        return output
    
    if (count_log2 < 7) or (count_log2 > 30):
        return output
    
    count = 1 << count_log2
    salt = settings[4:12]
    
    if len(salt) != 8:
        return output
    
    hash = md5(salt + password).digest()
    while True:
        hash = md5(hash + password).digest()
        count = count - 1
        if count <= 0:
            break
        
    output = settings[0:12]
    output += _hash_encode64(hash, 16, itoa64)
    
    return output

def phpbb_check_hash(password, hash):
    """
    Python implementation of the phpBB3 check hash function
    """
    
    itoa64 = './0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'
    if len(hash) == 34:
        return _hash_crypt_private(password, hash, itoa64) == hash

    return md5(password).hexdigest() == hash

#
#--- Start of program
#
if __name__ == '__main__':
    # Parse commandline options
    parser = OptionParser()
    parser.add_option('-i', '--ini',
                      help = 'load configuration from INI', default = cfgfile)
    parser.add_option('-v', '--verbose', action='store_true', dest = 'verbose',
                      help = 'verbose output [default]', default = True)
    parser.add_option('-q', '--quiet', action='store_false', dest = 'verbose',
                      help = 'only error output')
    parser.add_option('-d', '--daemon', action='store_true', dest = 'force_daemon',
                      help = 'run as daemon', default = False)
    parser.add_option('-a', '--app', action='store_true', dest = 'force_app',
                      help = 'do not run as daemon', default = False)
    (option, args) = parser.parse_args()
    
    if option.force_daemon and option.force_app:
        parser.print_help()
        sys.exit(1)
        
    # Load configuration
    try:
        cfg = config(option.ini, default)
    except Exception, e:
        print>>sys.stderr, 'Fatal error, could not load config file from "%s"' % cfgfile
        sys.exit(1)

    # Do conditional imports
    if cfg.user.avatar_enable:
        # If we use avatars we need PIL to manipulate it and some other stuff for working with them
        try:
            import Image
        except ImportError, e:
            print>>sys.stderr, 'Error, could not import PIL library, '\
            'please install the missing dependency and restart the authenticator'
            sys.exit(1)

        import urllib2
        import StringIO
        
        from zlib   import compress
        from struct import pack
        
    try:
        db = __import__(cfg.database.lib)
    except ImportError, e:
        print>>sys.stderr, 'Fatal error, could not import database library "%s", '\
        'please install the missing dependency and restart the authenticator' % cfg.database.lib
        sys.exit(1)
    
    
    # Initialize logger
    if cfg.log.file:
        try:
            logfile = open(cfg.log.file, 'a')
        except IOError, e:
            print>>sys.stderr, 'Fatal error, could not open logfile "%s"' % cfg.log.file
            sys.exit(1)
            
    if option.verbose:
        level = cfg.log.level
    else:
        level = logging.ERROR
    
    logging.basicConfig(level = level,
                        format='%(asctime)s %(levelname)s %(message)s',
                        stream = logfile if cfg.log.file else logging.sys.stderr)
        
    # As the default try to run as daemon. Silently degrade to running as a normal application if this fails
    # unless the user explicitly defined what he expected with the -a / -d parameter. 
    try:
        if option.force_app:
            raise ImportError # Pretend that we couldn't import the daemon lib
        import daemon
    except ImportError:
        if option.force_daemon:
            print>>sys.stderr, 'Fatal error, could not daemonize process due to missing "daemon" library, ' \
            'please install the missing dependency and restart the authenticator'
            sys.exit(1)
        do_main_program()
    else:
        context = daemon.DaemonContext(working_directory = sys.path[0],
                                       stderr = logfile if cfg.log.file else sys.stderr)
        context.__enter__()
        try:
            do_main_program()
        finally:
            context.__exit__(None, None, None)
