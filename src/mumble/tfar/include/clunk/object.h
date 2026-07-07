#ifndef CLUNK_OBJECT_H__
#define CLUNK_OBJECT_H__

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

#include <string>
#include <map>
#include <clunk/export_clunk.h>
#include <clunk/v3.h>

namespace clunk {
class Context;
class Source;

/*! 
	\brief Object containing sources.
	Objects - class containing several playing sources and controlling its behaviour
*/

class CLUNKAPI Object {
public: 
	friend struct DistanceOrder;
	struct DistanceOrder {
		v3f listener;
		DistanceOrder(const v3f &listener) : listener(listener) {}

		inline bool operator()(const Object *a, const Object * b) const {
			return listener.quick_distance(a->_position) < listener.quick_distance(b->_position); 
		}
	};
	///dtor, do not forget to delete object if you do not need it anymore
	~Object();

	/*! 
		\brief updates object's position, velocity and direction at once.
		\param[in] pos position
	*/
	void update(const v3f &pos, const v3f &vel);
	
	/*! 
		\brief updates object's position
		\param[in] pos position
	*/
	void set_position(const v3f &pos);

	/*! 
		\brief updates object's velocity
		\param[in] vel velocity
	*/
	void set_velocity(const v3f &vel);
	
	/*! 
		\brief plays given source
		\param[in] name any name you want, just for your confort :)
		\param[in] source source to be played, do not delete it, clunk::Object will delete it automatically.
	*/
	void play(const std::string &name, Source *source);

	/*! 
		\brief plays given source
		\param[in] index any int index
		\param[in] source source to be played, do not delete it, clunk::Object will delete it automatically.
	*/
	void play(int index, Source *source);

	/*!
		\brief returns status of the given source. 
		\param[in] index source name
	*/
	bool playing(const std::string &name) const;

	/*!
		\brief returns status of the given source. 
		\param[in] index source index
	*/
	bool playing(int index) const;

	/*! 
		\brief cancels given source
		\param[in] name source name
		\param[in] fadeout length of the fadeout effect to avoid clicks. 
	*/
	void cancel(const std::string &name, float fadeout = 0.1f);

	/*! 
		\brief cancels given source
		\param[in] index source index
		\param[in] fadeout length of the fadeout effect to avoid clicks. 
	*/
	void cancel(int index, float fadeout = 0.1f);

	/*! 
		\brief cancels all sources for this object.
		\param[in] force quick fadeout for the all sources.
		\param[in] fadeout length of the fadeout effect to avoid clicks. seconds.
	*/
	void cancel_all(bool force = false, float fadeout = 0.1f);

	/*! 
		\brief fades out given source
		\param[in] name source name
		\param[in] fadeout length of the fadeout effect. seconds.
	*/
	void fade_out(const std::string &name, float fadeout = 0.1f);

	/*! 
		\brief fades out given source
		\param[in] index source name
		\param[in] fadeout length of the fadeout effect. seconds.
	*/
	void fade_out(int index, float fadeout = 0.1f);

	/// returns if any sources are playing now.
	bool active() const;
	
	///marks objects for autodeletion.
	/*! 
		
		If you do not want to delete objects explicitly (this immediately cancels all sources), you could 
		call this method and forget pointer. clunk::Context automatically deletes your object after all 
		sources stop.
	*/
	void autodelete();

	/*! 
		\brief sets loop flag
		\param[in] name source name
		\param[in] loop repeat source's sound
	*/
	void set_loop(const std::string &name, const bool loop);

	/*! 
		\brief sets loop flag
		\param[in] index source index
		\param[in] loop repeat source's sound
	*/
	void set_loop(int index, const bool loop);

	/*!
		\brief returns loop status
		\param[in] name source name
	*/
	bool get_loop(const std::string &name);

	/*!
		\brief returns loop status
		\param[in] index source index
	*/
	bool get_loop(int index);

protected:
	friend class Context;

	Context *context;
	v3f _position, _velocity;

	Object(Context *context);

private:
	typedef std::multimap<const std::string, Source *> NamedSources;
	NamedSources named_sources;
	typedef std::multimap<const int, Source *> IndexedSources;
	IndexedSources indexed_sources;
	
	bool dead;
};

class CLUNKAPI ListenerObject : public Object {
	friend class Context;
	
	ListenerObject(Context *context);
private:
	v3f _direction, _initialUp, _up;

public:
	/*!
		\brief updates object's direction
		\param[in] dir direction
	*/
	void set_direction(const v3f &dir);

	/*!
		\brief updates object's up vector
		\param[in] up vector pointing "up"
	*/
	void set_up(const v3f &up);
	
	/*!
		\brief updates object's direction and up vector
		\param[in] dir direction
		\param[in] up vector pointing "up"
	*/
	void update_view(v3f dir, v3f up);

	/*!
		\brief transform vector in listener's coordinate system
		\param[in] v vector
		\return transformed vector
	*/
	v3f transform(v3f v);
};

}

#endif

