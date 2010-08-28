
void ::Murmur::ServerI::isRunning_async(const ::Murmur::AMD_Server_isRunningPtr &cb, const ::Ice::Current &current) {
	// qWarning() << "isRunning" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_isRunning_ALL
#ifdef ACCESS_Server_isRunning_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_isRunning_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_isRunning, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::start_async(const ::Murmur::AMD_Server_startPtr &cb, const ::Ice::Current &current) {
	// qWarning() << "start" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_start_ALL
#ifdef ACCESS_Server_start_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_start_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_start, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::stop_async(const ::Murmur::AMD_Server_stopPtr &cb, const ::Ice::Current &current) {
	// qWarning() << "stop" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_stop_ALL
#ifdef ACCESS_Server_stop_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_stop_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_stop, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::delete_async(const ::Murmur::AMD_Server_deletePtr &cb, const ::Ice::Current &current) {
	// qWarning() << "delete" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_delete_ALL
#ifdef ACCESS_Server_delete_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_delete_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_delete, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::id_async(const ::Murmur::AMD_Server_idPtr &cb, const ::Ice::Current &current) {
	// qWarning() << "id" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_id_ALL
#ifdef ACCESS_Server_id_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_id_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_id, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::addCallback_async(const ::Murmur::AMD_Server_addCallbackPtr &cb,  const ::Murmur::ServerCallbackPrx& p1, const ::Ice::Current &current) {
	// qWarning() << "addCallback" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_addCallback_ALL
#ifdef ACCESS_Server_addCallback_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_addCallback_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_addCallback, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::removeCallback_async(const ::Murmur::AMD_Server_removeCallbackPtr &cb,  const ::Murmur::ServerCallbackPrx& p1, const ::Ice::Current &current) {
	// qWarning() << "removeCallback" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_removeCallback_ALL
#ifdef ACCESS_Server_removeCallback_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_removeCallback_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_removeCallback, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::setAuthenticator_async(const ::Murmur::AMD_Server_setAuthenticatorPtr &cb,  const ::Murmur::ServerAuthenticatorPrx& p1, const ::Ice::Current &current) {
	// qWarning() << "setAuthenticator" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_setAuthenticator_ALL
#ifdef ACCESS_Server_setAuthenticator_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_setAuthenticator_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_setAuthenticator, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::getConf_async(const ::Murmur::AMD_Server_getConfPtr &cb,  const ::std::string& p1, const ::Ice::Current &current) {
	// qWarning() << "getConf" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_getConf_ALL
#ifdef ACCESS_Server_getConf_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_getConf_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getConf, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::getAllConf_async(const ::Murmur::AMD_Server_getAllConfPtr &cb, const ::Ice::Current &current) {
	// qWarning() << "getAllConf" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_getAllConf_ALL
#ifdef ACCESS_Server_getAllConf_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_getAllConf_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getAllConf, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::setConf_async(const ::Murmur::AMD_Server_setConfPtr &cb,  const ::std::string& p1,  const ::std::string& p2, const ::Ice::Current &current) {
	// qWarning() << "setConf" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_setConf_ALL
#ifdef ACCESS_Server_setConf_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_setConf_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_setConf, cb, QString::fromStdString(current.id.name).toInt(), p1, p2));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::setSuperuserPassword_async(const ::Murmur::AMD_Server_setSuperuserPasswordPtr &cb,  const ::std::string& p1, const ::Ice::Current &current) {
	// qWarning() << "setSuperuserPassword" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_setSuperuserPassword_ALL
#ifdef ACCESS_Server_setSuperuserPassword_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_setSuperuserPassword_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_setSuperuserPassword, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::getLog_async(const ::Murmur::AMD_Server_getLogPtr &cb,  ::Ice::Int p1,  ::Ice::Int p2, const ::Ice::Current &current) {
	// qWarning() << "getLog" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_getLog_ALL
#ifdef ACCESS_Server_getLog_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_getLog_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getLog, cb, QString::fromStdString(current.id.name).toInt(), p1, p2));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::getLogLen_async(const ::Murmur::AMD_Server_getLogLenPtr &cb, const ::Ice::Current &current) {
	// qWarning() << "getLogLen" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_getLogLen_ALL
#ifdef ACCESS_Server_getLogLen_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_getLogLen_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getLogLen, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::getUsers_async(const ::Murmur::AMD_Server_getUsersPtr &cb, const ::Ice::Current &current) {
	// qWarning() << "getUsers" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_getUsers_ALL
#ifdef ACCESS_Server_getUsers_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_getUsers_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getUsers, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::getChannels_async(const ::Murmur::AMD_Server_getChannelsPtr &cb, const ::Ice::Current &current) {
	// qWarning() << "getChannels" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_getChannels_ALL
#ifdef ACCESS_Server_getChannels_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_getChannels_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getChannels, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::getCertificateList_async(const ::Murmur::AMD_Server_getCertificateListPtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	// qWarning() << "getCertificateList" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_getCertificateList_ALL
#ifdef ACCESS_Server_getCertificateList_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_getCertificateList_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getCertificateList, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::getTree_async(const ::Murmur::AMD_Server_getTreePtr &cb, const ::Ice::Current &current) {
	// qWarning() << "getTree" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_getTree_ALL
#ifdef ACCESS_Server_getTree_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_getTree_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getTree, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::getBans_async(const ::Murmur::AMD_Server_getBansPtr &cb, const ::Ice::Current &current) {
	// qWarning() << "getBans" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_getBans_ALL
#ifdef ACCESS_Server_getBans_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_getBans_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getBans, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::setBans_async(const ::Murmur::AMD_Server_setBansPtr &cb,  const ::Murmur::BanList& p1, const ::Ice::Current &current) {
	// qWarning() << "setBans" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_setBans_ALL
#ifdef ACCESS_Server_setBans_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_setBans_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_setBans, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::kickUser_async(const ::Murmur::AMD_Server_kickUserPtr &cb,  ::Ice::Int p1,  const ::std::string& p2, const ::Ice::Current &current) {
	// qWarning() << "kickUser" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_kickUser_ALL
#ifdef ACCESS_Server_kickUser_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_kickUser_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_kickUser, cb, QString::fromStdString(current.id.name).toInt(), p1, p2));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::getState_async(const ::Murmur::AMD_Server_getStatePtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	// qWarning() << "getState" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_getState_ALL
#ifdef ACCESS_Server_getState_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_getState_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getState, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::setState_async(const ::Murmur::AMD_Server_setStatePtr &cb,  const ::Murmur::User& p1, const ::Ice::Current &current) {
	// qWarning() << "setState" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_setState_ALL
#ifdef ACCESS_Server_setState_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_setState_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_setState, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::sendMessage_async(const ::Murmur::AMD_Server_sendMessagePtr &cb,  ::Ice::Int p1,  const ::std::string& p2, const ::Ice::Current &current) {
	// qWarning() << "sendMessage" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_sendMessage_ALL
#ifdef ACCESS_Server_sendMessage_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_sendMessage_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_sendMessage, cb, QString::fromStdString(current.id.name).toInt(), p1, p2));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::hasPermission_async(const ::Murmur::AMD_Server_hasPermissionPtr &cb,  ::Ice::Int p1,  ::Ice::Int p2,  ::Ice::Int p3, const ::Ice::Current &current) {
	// qWarning() << "hasPermission" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_hasPermission_ALL
#ifdef ACCESS_Server_hasPermission_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_hasPermission_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_hasPermission, cb, QString::fromStdString(current.id.name).toInt(), p1, p2, p3));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::addContextCallback_async(const ::Murmur::AMD_Server_addContextCallbackPtr &cb,  ::Ice::Int p1,  const ::std::string& p2,  const ::std::string& p3,  const ::Murmur::ServerContextCallbackPrx& p4,  ::Ice::Int p5, const ::Ice::Current &current) {
	// qWarning() << "addContextCallback" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_addContextCallback_ALL
#ifdef ACCESS_Server_addContextCallback_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_addContextCallback_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_addContextCallback, cb, QString::fromStdString(current.id.name).toInt(), p1, p2, p3, p4, p5));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::removeContextCallback_async(const ::Murmur::AMD_Server_removeContextCallbackPtr &cb,  const ::Murmur::ServerContextCallbackPrx& p1, const ::Ice::Current &current) {
	// qWarning() << "removeContextCallback" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_removeContextCallback_ALL
#ifdef ACCESS_Server_removeContextCallback_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_removeContextCallback_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_removeContextCallback, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::getChannelState_async(const ::Murmur::AMD_Server_getChannelStatePtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	// qWarning() << "getChannelState" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_getChannelState_ALL
#ifdef ACCESS_Server_getChannelState_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_getChannelState_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getChannelState, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::setChannelState_async(const ::Murmur::AMD_Server_setChannelStatePtr &cb,  const ::Murmur::Channel& p1, const ::Ice::Current &current) {
	// qWarning() << "setChannelState" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_setChannelState_ALL
#ifdef ACCESS_Server_setChannelState_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_setChannelState_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_setChannelState, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::removeChannel_async(const ::Murmur::AMD_Server_removeChannelPtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	// qWarning() << "removeChannel" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_removeChannel_ALL
#ifdef ACCESS_Server_removeChannel_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_removeChannel_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_removeChannel, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::addChannel_async(const ::Murmur::AMD_Server_addChannelPtr &cb,  const ::std::string& p1,  ::Ice::Int p2, const ::Ice::Current &current) {
	// qWarning() << "addChannel" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_addChannel_ALL
#ifdef ACCESS_Server_addChannel_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_addChannel_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_addChannel, cb, QString::fromStdString(current.id.name).toInt(), p1, p2));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::sendMessageChannel_async(const ::Murmur::AMD_Server_sendMessageChannelPtr &cb,  ::Ice::Int p1,  bool p2,  const ::std::string& p3, const ::Ice::Current &current) {
	// qWarning() << "sendMessageChannel" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_sendMessageChannel_ALL
#ifdef ACCESS_Server_sendMessageChannel_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_sendMessageChannel_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_sendMessageChannel, cb, QString::fromStdString(current.id.name).toInt(), p1, p2, p3));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::getACL_async(const ::Murmur::AMD_Server_getACLPtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	// qWarning() << "getACL" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_getACL_ALL
#ifdef ACCESS_Server_getACL_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_getACL_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getACL, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::setACL_async(const ::Murmur::AMD_Server_setACLPtr &cb,  ::Ice::Int p1,  const ::Murmur::ACLList& p2,  const ::Murmur::GroupList& p3,  bool p4, const ::Ice::Current &current) {
	// qWarning() << "setACL" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_setACL_ALL
#ifdef ACCESS_Server_setACL_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_setACL_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_setACL, cb, QString::fromStdString(current.id.name).toInt(), p1, p2, p3, p4));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::addUserToGroup_async(const ::Murmur::AMD_Server_addUserToGroupPtr &cb,  ::Ice::Int p1,  ::Ice::Int p2,  const ::std::string& p3, const ::Ice::Current &current) {
	// qWarning() << "addUserToGroup" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_addUserToGroup_ALL
#ifdef ACCESS_Server_addUserToGroup_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_addUserToGroup_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_addUserToGroup, cb, QString::fromStdString(current.id.name).toInt(), p1, p2, p3));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::removeUserFromGroup_async(const ::Murmur::AMD_Server_removeUserFromGroupPtr &cb,  ::Ice::Int p1,  ::Ice::Int p2,  const ::std::string& p3, const ::Ice::Current &current) {
	// qWarning() << "removeUserFromGroup" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_removeUserFromGroup_ALL
#ifdef ACCESS_Server_removeUserFromGroup_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_removeUserFromGroup_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_removeUserFromGroup, cb, QString::fromStdString(current.id.name).toInt(), p1, p2, p3));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::redirectWhisperGroup_async(const ::Murmur::AMD_Server_redirectWhisperGroupPtr &cb,  ::Ice::Int p1,  const ::std::string& p2,  const ::std::string& p3, const ::Ice::Current &current) {
	// qWarning() << "redirectWhisperGroup" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_redirectWhisperGroup_ALL
#ifdef ACCESS_Server_redirectWhisperGroup_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_redirectWhisperGroup_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_redirectWhisperGroup, cb, QString::fromStdString(current.id.name).toInt(), p1, p2, p3));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::getUserNames_async(const ::Murmur::AMD_Server_getUserNamesPtr &cb,  const ::Murmur::IdList& p1, const ::Ice::Current &current) {
	// qWarning() << "getUserNames" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_getUserNames_ALL
#ifdef ACCESS_Server_getUserNames_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_getUserNames_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getUserNames, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::getUserIds_async(const ::Murmur::AMD_Server_getUserIdsPtr &cb,  const ::Murmur::NameList& p1, const ::Ice::Current &current) {
	// qWarning() << "getUserIds" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_getUserIds_ALL
#ifdef ACCESS_Server_getUserIds_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_getUserIds_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getUserIds, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::registerUser_async(const ::Murmur::AMD_Server_registerUserPtr &cb,  const ::Murmur::UserInfoMap& p1, const ::Ice::Current &current) {
	// qWarning() << "registerUser" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_registerUser_ALL
#ifdef ACCESS_Server_registerUser_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_registerUser_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_registerUser, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::unregisterUser_async(const ::Murmur::AMD_Server_unregisterUserPtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	// qWarning() << "unregisterUser" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_unregisterUser_ALL
#ifdef ACCESS_Server_unregisterUser_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_unregisterUser_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_unregisterUser, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::updateRegistration_async(const ::Murmur::AMD_Server_updateRegistrationPtr &cb,  ::Ice::Int p1,  const ::Murmur::UserInfoMap& p2, const ::Ice::Current &current) {
	// qWarning() << "updateRegistration" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_updateRegistration_ALL
#ifdef ACCESS_Server_updateRegistration_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_updateRegistration_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_updateRegistration, cb, QString::fromStdString(current.id.name).toInt(), p1, p2));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::getRegistration_async(const ::Murmur::AMD_Server_getRegistrationPtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	// qWarning() << "getRegistration" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_getRegistration_ALL
#ifdef ACCESS_Server_getRegistration_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_getRegistration_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getRegistration, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::getRegisteredUsers_async(const ::Murmur::AMD_Server_getRegisteredUsersPtr &cb,  const ::std::string& p1, const ::Ice::Current &current) {
	// qWarning() << "getRegisteredUsers" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_getRegisteredUsers_ALL
#ifdef ACCESS_Server_getRegisteredUsers_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_getRegisteredUsers_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getRegisteredUsers, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::verifyPassword_async(const ::Murmur::AMD_Server_verifyPasswordPtr &cb,  const ::std::string& p1,  const ::std::string& p2, const ::Ice::Current &current) {
	// qWarning() << "verifyPassword" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_verifyPassword_ALL
#ifdef ACCESS_Server_verifyPassword_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_verifyPassword_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_verifyPassword, cb, QString::fromStdString(current.id.name).toInt(), p1, p2));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::getTexture_async(const ::Murmur::AMD_Server_getTexturePtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	// qWarning() << "getTexture" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_getTexture_ALL
#ifdef ACCESS_Server_getTexture_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_getTexture_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getTexture, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::setTexture_async(const ::Murmur::AMD_Server_setTexturePtr &cb,  ::Ice::Int p1,  const ::Murmur::Texture& p2, const ::Ice::Current &current) {
	// qWarning() << "setTexture" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_setTexture_ALL
#ifdef ACCESS_Server_setTexture_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_setTexture_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_setTexture, cb, QString::fromStdString(current.id.name).toInt(), p1, p2));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::ServerI::getUptime_async(const ::Murmur::AMD_Server_getUptimePtr &cb, const ::Ice::Current &current) {
	// qWarning() << "getUptime" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Server_getUptime_ALL
#ifdef ACCESS_Server_getUptime_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Server_getUptime_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getUptime, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::MetaI::getServer_async(const ::Murmur::AMD_Meta_getServerPtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	// qWarning() << "getServer" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Meta_getServer_ALL
#ifdef ACCESS_Meta_getServer_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Meta_getServer_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Meta_getServer, cb, current.adapter, p1));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::MetaI::newServer_async(const ::Murmur::AMD_Meta_newServerPtr &cb, const ::Ice::Current &current) {
	// qWarning() << "newServer" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Meta_newServer_ALL
#ifdef ACCESS_Meta_newServer_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Meta_newServer_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Meta_newServer, cb, current.adapter));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::MetaI::getBootedServers_async(const ::Murmur::AMD_Meta_getBootedServersPtr &cb, const ::Ice::Current &current) {
	// qWarning() << "getBootedServers" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Meta_getBootedServers_ALL
#ifdef ACCESS_Meta_getBootedServers_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Meta_getBootedServers_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Meta_getBootedServers, cb, current.adapter));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::MetaI::getAllServers_async(const ::Murmur::AMD_Meta_getAllServersPtr &cb, const ::Ice::Current &current) {
	// qWarning() << "getAllServers" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Meta_getAllServers_ALL
#ifdef ACCESS_Meta_getAllServers_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Meta_getAllServers_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Meta_getAllServers, cb, current.adapter));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::MetaI::getDefaultConf_async(const ::Murmur::AMD_Meta_getDefaultConfPtr &cb, const ::Ice::Current &current) {
	// qWarning() << "getDefaultConf" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Meta_getDefaultConf_ALL
#ifdef ACCESS_Meta_getDefaultConf_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Meta_getDefaultConf_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Meta_getDefaultConf, cb, current.adapter));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::MetaI::getVersion_async(const ::Murmur::AMD_Meta_getVersionPtr &cb, const ::Ice::Current &current) {
	// qWarning() << "getVersion" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Meta_getVersion_ALL
#ifdef ACCESS_Meta_getVersion_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Meta_getVersion_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Meta_getVersion, cb, current.adapter));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::MetaI::addCallback_async(const ::Murmur::AMD_Meta_addCallbackPtr &cb,  const ::Murmur::MetaCallbackPrx& p1, const ::Ice::Current &current) {
	// qWarning() << "addCallback" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Meta_addCallback_ALL
#ifdef ACCESS_Meta_addCallback_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Meta_addCallback_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Meta_addCallback, cb, current.adapter, p1));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::MetaI::removeCallback_async(const ::Murmur::AMD_Meta_removeCallbackPtr &cb,  const ::Murmur::MetaCallbackPrx& p1, const ::Ice::Current &current) {
	// qWarning() << "removeCallback" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Meta_removeCallback_ALL
#ifdef ACCESS_Meta_removeCallback_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Meta_removeCallback_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Meta_removeCallback, cb, current.adapter, p1));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::MetaI::getUptime_async(const ::Murmur::AMD_Meta_getUptimePtr &cb, const ::Ice::Current &current) {
	// qWarning() << "getUptime" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Meta_getUptime_ALL
#ifdef ACCESS_Meta_getUptime_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Meta_getUptime_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Meta_getUptime, cb, current.adapter));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::MetaI::getSliceChecksums_async(const ::Murmur::AMD_Meta_getSliceChecksumsPtr &cb, const ::Ice::Current &current) {
	// qWarning() << "getSliceChecksums" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_Meta_getSliceChecksums_ALL
#ifdef ACCESS_Meta_getSliceChecksums_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_Meta_getSliceChecksums_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Meta_getSliceChecksums, cb, current.adapter));
	QCoreApplication::instance()->postEvent(mi, ie);
}

void ::Murmur::MetaI::getSlice_async(const ::Murmur::AMD_Meta_getSlicePtr& cb, const Ice::Current&) {
	cb->ice_response(std::string("#include <Ice/SliceChecksumDict.ice>\nmodule Murmur\n{\n[\"python:seq:tuple\"] sequence<byte> NetAddress;\nstruct User {\nint session;\nint userid;\nbool mute;\nbool deaf;\nbool suppress;\nbool prioritySpeaker;\nbool selfMute;\nbool selfDeaf;\nbool recording;\nint channel;\nstring name;\nint onlinesecs;\nint bytespersec;\nint version;\nstring release;\nstring os;\nstring osversion;\nstring identity;\nstring context;\nstring comment;\nNetAddress address;\nbool tcponly;\nint idlesecs;\n};\nsequence<int> IntList;\nstruct Channel {\nint id;\nstring name;\nint parent;\nIntList links;\nstring description;\nbool temporary;\nint position;\n};\nstruct Group {\nstring name;\nbool inherited;\nbool inherit;\nbool inheritable;\nIntList add;\nIntList remove;\nIntList members;\n};\nconst int PermissionWrite = 0x01;\nconst int PermissionTraverse = 0x02;\nconst int PermissionEnter = 0x04;\nconst int PermissionSpeak = 0x08;\nconst int PermissionWhisper = 0x100;\nconst int PermissionMuteDeafen = 0x10;\nconst int PermissionMove = 0x20;\nconst int PermissionMakeChannel = 0x40;\nconst int PermissionMakeTempChannel = 0x400;\nconst int PermissionLinkChannel = 0x80;\nconst int PermissionTextMessage = 0x200;\nconst int PermissionKick = 0x10000;\nconst int PermissionBan = 0x20000;\nconst int PermissionRegister = 0x40000;\nconst int PermissionRegisterSelf = 0x80000;\nstruct ACL {\nbool applyHere;\nbool applySubs;\nbool inherited;\nint userid;\nstring group;\nint allow;\nint deny;\n};\nstruct Ban {\nNetAddress address;\nint bits;\nstring name;\nstring hash;\nstring reason;\nint start;\nint duration;\n};\nstruct LogEntry {\nint timestamp;\nstring txt;\n};\nclass Tree;\nsequence<Tree> TreeList;\nenum ChannelInfo { ChannelDescription, ChannelPosition };\nenum UserInfo { UserName, UserEmail, UserComment, UserHash, UserPassword, UserLastActive };\ndictionary<int, User> UserMap;\ndictionary<int, Channel> ChannelMap;\nsequence<Channel> ChannelList;\nsequence<User> UserList;\nsequence<Group> GroupList;\nsequence<ACL> ACLList;\nsequence<LogEntry> LogList;\nsequence<Ban> BanList;\nsequence<int> IdList;\nsequence<string> NameList;\ndictionary<int, string> NameMap;\ndictionary<string, int> IdMap;\nsequence<byte> Texture;\ndictionary<string, string> ConfigMap;\nsequence<string> GroupNameList;\nsequence<byte> CertificateDer;\nsequence<CertificateDer> CertificateList;\ndictionary<UserInfo, string> UserInfoMap;\nclass Tree {\nChannel c;\nTreeList children;\nUserList users;\n};\nexception MurmurException {};\nexception InvalidSessionException extends MurmurException {};\nexception InvalidChannelException extends MurmurException {};\nexception InvalidServerException extends MurmurException {};\nexception ServerBootedException extends MurmurException {};\nexception ServerFailureException extends MurmurException {};\nexception InvalidUserException extends MurmurException {};\nexception InvalidTextureException extends MurmurException {};\nexception InvalidCallbackException extends MurmurException {};\nexception InvalidSecretException extends MurmurException {};\ninterface ServerCallback {\nidempotent void userConnected(User state);\nidempotent void userDisconnected(User state);\nidempotent void userStateChanged(User state);\nidempotent void channelCreated(Channel state);\nidempotent void channelRemoved(Channel state);\nidempotent void channelStateChanged(Channel state);\n};\nconst int ContextServer = 0x01;\nconst int ContextChannel = 0x02;\nconst int ContextUser = 0x04;\ninterface ServerContextCallback {\nidempotent void contextAction(string action, User usr, int session, int channelid);\n};\ninterface ServerAuthenticator {\nidempotent int authenticate(string name, string pw, CertificateList certificates, string certhash, bool certstrong, out string newname, out GroupNameList groups);\nidempotent bool getInfo(int id, out UserInfoMap info);\nidempotent int nameToId(string name);\nidempotent string idToName(int id);\nidempotent Texture idToTexture(int id);\n};\ninterface ServerUpdatingAuthenticator extends ServerAuthenticator {\nint registerUser(UserInfoMap info);\nint unregisterUser(int id);\nidempotent NameMap getRegisteredUsers(string filter);\nidempotent int setInfo(int id, UserInfoMap info);\nidempotent int setTexture(int id, Texture tex);\n};\n[\"amd\"] interface Server {\nidempotent bool isRunning() throws InvalidSecretException;\nvoid start() throws ServerBootedException, ServerFailureException, InvalidSecretException;\nvoid stop() throws ServerBootedException, InvalidSecretException;\nvoid delete() throws ServerBootedException, InvalidSecretException;\nidempotent int id() throws InvalidSecretException;\nvoid addCallback(ServerCallback *cb) throws ServerBootedException, InvalidCallbackException, InvalidSecretException;\nvoid removeCallback(ServerCallback *cb) throws ServerBootedException, InvalidCallbackException, InvalidSecretException;\nvoid setAuthenticator(ServerAuthenticator *auth) throws ServerBootedException, InvalidCallbackException, InvalidSecretException;\nidempotent string getConf(string key) throws InvalidSecretException;\nidempotent ConfigMap getAllConf() throws InvalidSecretException;\nidempotent void setConf(string key, string value) throws InvalidSecretException;\nidempotent void setSuperuserPassword(string pw) throws InvalidSecretException;\nidempotent LogList getLog(int first, int last) throws InvalidSecretException;\nidempotent int getLogLen() throws InvalidSecretException;\nidempotent UserMap getUsers() throws ServerBootedException, InvalidSecretException;\nidempotent ChannelMap getChannels() throws ServerBootedException, InvalidSecretException;\nidempotent CertificateList getCertificateList(int session) throws ServerBootedException, InvalidSessionException, InvalidSecretException;\nidempotent Tree getTree() throws ServerBootedException, InvalidSecretException;\nidempotent BanList getBans() throws ServerBootedException, InvalidSecretException;\nidempotent void setBans(BanList bans) throws ServerBootedException, InvalidSecretException;\nvoid kickUser(int session, string reason) throws ServerBootedException, InvalidSessionException, InvalidSecretException;\nidempotent User getState(int session) throws ServerBootedException, InvalidSessionException, InvalidSecretException;\nidempotent void setState(User state) throws ServerBootedException, InvalidSessionException, InvalidChannelException, InvalidSecretException;\nvoid sendMessage(int session, string text) throws ServerBootedException, InvalidSessionException, InvalidSecretException;\nbool hasPermission(int session, int channelid, int perm) throws ServerBootedException, InvalidSessionException, InvalidChannelException, InvalidSecretException;\nvoid addContextCallback(int session, string action, string text, ServerContextCallback *cb, int ctx) throws ServerBootedException, InvalidCallbackException, InvalidSecretException;\nvoid removeContextCallback(ServerContextCallback *cb) throws ServerBootedException, InvalidCallbackException, InvalidSecretException;\nidempotent Channel getChannelState(int channelid) throws ServerBootedException, InvalidChannelException, InvalidSecretException;\nidempotent void setChannelState(Channel state) throws ServerBootedException, InvalidChannelException, InvalidSecretException;\nvoid removeChannel(int channelid) throws ServerBootedException, InvalidChannelException, InvalidSecretException;\nint addChannel(string name, int parent) throws ServerBootedException, InvalidChannelException, InvalidSecretException;\nvoid sendMessageChannel(int channelid, bool tree, string text) throws ServerBootedException, InvalidChannelException, InvalidSecretException;\nidempotent void getACL(int channelid, out ACLList acls, out GroupList groups, out bool inherit) throws ServerBootedException, InvalidChannelException, InvalidSecretException;\nidempotent void setACL(int channelid, ACLList acls, GroupList groups, bool inherit) throws ServerBootedException, InvalidChannelException, InvalidSecretException;\nidempotent void addUserToGroup(int channelid, int session, string group) throws ServerBootedException, InvalidChannelException, InvalidSessionException, InvalidSecretException;\nidempotent void removeUserFromGroup(int channelid, int session, string group) throws ServerBootedException, InvalidChannelException, InvalidSessionException, InvalidSecretException;\nidempotent void redirectWhisperGroup(int session, string source, string target) throws ServerBootedException, InvalidSessionException, InvalidSecretException;\nidempotent NameMap getUserNames(IdList ids) throws ServerBootedException, InvalidSecretException;\nidempotent IdMap getUserIds(NameList names) throws ServerBootedException, InvalidSecretException;\nint registerUser(UserInfoMap info) throws ServerBootedException, InvalidUserException, InvalidSecretException;\nvoid unregisterUser(int userid) throws ServerBootedException, InvalidUserException, InvalidSecretException;\nidempotent void updateRegistration(int userid, UserInfoMap info) throws ServerBootedException, InvalidUserException, InvalidSecretException;\nidempotent UserInfoMap getRegistration(int userid) throws ServerBootedException, InvalidUserException, InvalidSecretException;\nidempotent NameMap getRegisteredUsers(string filter) throws ServerBootedException, InvalidSecretException;\nidempotent int verifyPassword(string name, string pw) throws ServerBootedException, InvalidSecretException;\nidempotent Texture getTexture(int userid) throws ServerBootedException, InvalidUserException, InvalidSecretException;\nidempotent void setTexture(int userid, Texture tex) throws ServerBootedException, InvalidUserException, InvalidTextureException, InvalidSecretException;\nidempotent int getUptime() throws ServerBootedException, InvalidSecretException;\n};\ninterface MetaCallback {\nvoid started(Server *srv);\nvoid stopped(Server *srv);\n};\nsequence<Server *> ServerList;\n[\"amd\"] interface Meta {\nidempotent Server *getServer(int id) throws InvalidSecretException;\nServer *newServer() throws InvalidSecretException;\nidempotent ServerList getBootedServers() throws InvalidSecretException;\nidempotent ServerList getAllServers() throws InvalidSecretException;\nidempotent ConfigMap getDefaultConf() throws InvalidSecretException;\nidempotent void getVersion(out int major, out int minor, out int patch, out string text);\nvoid addCallback(MetaCallback *cb) throws InvalidCallbackException, InvalidSecretException;\nvoid removeCallback(MetaCallback *cb) throws InvalidCallbackException, InvalidSecretException;\nidempotent int getUptime();\nidempotent string getSlice();\nidempotent Ice::SliceChecksumDict getSliceChecksums();\n};\n};\n"));
}
