/* clunk - cross-platform 3D audio API built on top SDL library
 * Copyright (C) 2007-2014 Netive Media Group
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include <clunk/types.h>
#include <clunk/object.h>
#include <clunk/context.h>
#include <clunk/locker.h>
#include <clunk/source.h>
#include <stdexcept>
#pragma warning( push )
#pragma warning( disable : 4101 ) // unreferenced local variable


using namespace clunk;

Object::Object(Context *context) : context(context), dead(false) {}

void Object::update(const v3f &pos, const v3f &vel) {
	AudioLocker l;
	_position = pos;
	_velocity = vel;
}

void Object::set_position(const v3f &pos) {
	AudioLocker l;
	_position = pos;
}

void Object::set_velocity(const v3f &vel) {
	AudioLocker l;
	_velocity = vel;
}

void Object::play(const std::string &name, Source *source) {
	AudioLocker l;
	named_sources.insert(NamedSources::value_type(name, source));
}

void Object::play(int index, Source *source) {
	AudioLocker l;
	indexed_sources.insert(IndexedSources::value_type(index, source));
}

bool Object::playing(const std::string &name) const {
	AudioLocker l;
	return named_sources.find(name) != named_sources.end();
}

bool Object::playing(int index) const {
	AudioLocker l;
	return indexed_sources.find(index) != indexed_sources.end();
}

void Object::fade_out(const std::string &name, float fadeout) {
	AudioLocker l;
	NamedSources::iterator b = named_sources.lower_bound(name);
	NamedSources::iterator e = named_sources.upper_bound(name);
	for(NamedSources::iterator i = b; i != e; ++i) {
		i->second->fade_out(fadeout);
	}
}

void Object::fade_out(int index, float fadeout) {
	AudioLocker l;
	IndexedSources::iterator b = indexed_sources.lower_bound(index);
	IndexedSources::iterator e = indexed_sources.upper_bound(index);
	for(IndexedSources::iterator i = b; i != e; ++i) {
		i->second->fade_out(fadeout);
	}
}

void Object::cancel(const std::string &name, float fadeout) {
	AudioLocker l;
	NamedSources::iterator b = named_sources.lower_bound(name);
	NamedSources::iterator e = named_sources.upper_bound(name);
	for(NamedSources::iterator i = b; i != e; ) {
		if (fadeout == 0) {
			//quickly destroy source
			delete i->second;
			named_sources.erase(i++);
			continue;
		} else if (i->second->loop)
			i->second->fade_out(fadeout);
		++i;
	}
}

void Object::cancel(int index, float fadeout) {
	AudioLocker l;
	IndexedSources::iterator b = indexed_sources.lower_bound(index);
	IndexedSources::iterator e = indexed_sources.upper_bound(index);
	for(IndexedSources::iterator i = b; i != e; ) {
		if (fadeout == 0) {
			//quickly destroy source
			delete i->second;
			indexed_sources.erase(i++);
			continue;
		} else if (i->second->loop)
			i->second->fade_out(fadeout);
		++i;
	}
}

bool Object::get_loop(const std::string &name) {
	AudioLocker l;
	NamedSources::iterator b = named_sources.lower_bound(name);
	NamedSources::iterator e = named_sources.upper_bound(name);
	for(NamedSources::iterator i = b; i != e; ++i) {
		if (i->second->loop)
			return true;
	}
	return false;
}

bool Object::get_loop(int index) {
	AudioLocker l;
	IndexedSources::iterator b = indexed_sources.lower_bound(index);
	IndexedSources::iterator e = indexed_sources.upper_bound(index);
	for(IndexedSources::iterator i = b; i != e; ++i) {
		if (i->second->loop)
			return true;
	}
	return false;
}


void Object::set_loop(const std::string &name, const bool loop) {
	AudioLocker l;
	NamedSources::iterator b = named_sources.lower_bound(name);
	NamedSources::iterator e = named_sources.upper_bound(name);
	for(NamedSources::iterator i = b; i != e; ++i) {
		i->second->loop = i == b? loop: false; //set loop only for the first. disable others. 
	}
}

void Object::set_loop(int index, const bool loop) {
	AudioLocker l;
	IndexedSources::iterator b = indexed_sources.lower_bound(index);
	IndexedSources::iterator e = indexed_sources.upper_bound(index);
	for(IndexedSources::iterator i = b; i != e; ++i) {
		i->second->loop = i == b? loop: false; //set loop only for the first. disable others. 
	}
}

template<class Sources>
void _cancel_all(Sources &sources, bool force, float fadeout) {
	for(typename Sources::iterator i = sources.begin(); i != sources.end(); ++i) {
		if (force) {
			delete i->second;
		} else {
			if (i->second->loop)
				i->second->fade_out(fadeout);
		}
	}
	if (force) {
		sources.clear();
	}
}

void Object::cancel_all(bool force, float fadeout) {
	AudioLocker l;
	_cancel_all(indexed_sources, force, fadeout);
	_cancel_all(named_sources, force, fadeout);
}

Object::~Object() {
	if (dead)
		return;
	AudioLocker l;
	cancel_all();
	context->delete_object(this);
}

bool Object::active() const {
	AudioLocker l;
	return !indexed_sources.empty() || !named_sources.empty();
}

void Object::autodelete() {
	AudioLocker l;
	cancel_all();
	dead = true;
}

ListenerObject::ListenerObject(Context *context):
	Object(context) {
	update_view(v3f(0, 1, 0), v3f(0, 0, 1));
}

void ListenerObject::update_view(v3f dir, v3f up)
{
	dir.normalize();
	up.normalize();

	AudioLocker l;
	v3f left = up.cross_product(dir);
	if (left.is0())
		throw std::runtime_error("colinear direction and \"up\" vector");
	left.normalize();

	_initialUp = up;
	_direction = dir;
	_up = dir.cross_product(left);
	_up.normalize();
}

void ListenerObject::set_direction(const v3f &dir) {
	AudioLocker l;
	update_view(dir, _initialUp);
}

void ListenerObject::set_up(const v3f &up) {
	AudioLocker l;
	update_view(_direction, up);
}

v3f ListenerObject::transform(v3f v) {
	return v - _position;
}
#pragma warning( pop )
