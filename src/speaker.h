/* 
 * Copyright (C) 2001-2005 Jacek Sieka, arnetheduck on gmail point com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef SPEAKER_H
#define SPEAKER_H

#include <list>
#include <algorithm>

template<typename Listener>
class Speaker {
	typedef std::list<Listener*> ListenerList;
	typedef typename ListenerList::iterator ListenerIter;

public:
	Speaker() throw() { };
	virtual ~Speaker() throw() { };

	template<typename T0>
	void fire(T0 type) {
		for (ListenerIter i = listeners.begin(); i != listeners.end(); ++i ) {
			(*i)->on(type);
		}
	}

	template<typename T0, class T1>
	void fire(T0 type, const T1& p1) {
		for (ListenerIter i = listeners.begin(); i != listeners.end(); ++i ) {
			(*i)->on(type, p1);
		}
	}

	template<typename T0, class T1, class T2>
	void fire(T0 type, const T1& p1, const T2& p2) {
		for (ListenerIter i = listeners.begin(); i != listeners.end(); ++i ) {
			(*i)->on(type, p1, p2);
		}
	}

	template<typename T0, class T1, class T2, class T3>
	void fire(T0 type, const T1& p1, const T2& p2, const T3& p3) {
		for (ListenerIter i = listeners.begin(); i != listeners.end(); ++i ) {
			(*i)->on(type, p1, p2, p3);
		}
	}

	template<typename T0, class T1, class T2, class T3, class T4>
	void fire(T0 type, const T1& p1, const T2& p2, const T3& p3, const T4& p4) {
		for (ListenerIter i = listeners.begin(); i != listeners.end(); ++i ) {
			(*i)->on(type, p1, p2, p3, p4);
		}
	}

	template<typename T0, class T1, class T2, class T3, class T4, class T5>
	void fire(T0 type, const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5) {
		for (ListenerIter i = listeners.begin(); i != listeners.end(); ++i ) {
			(*i)->on(type, p1, p2, p3, p4, p5);
		}
	}

	template<typename T0, class T1, class T2, class T3, class T4, class T5, class T6>
	void fire(T0 type, const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5, const T6& p6) {
		for (ListenerIter i = listeners.begin(); i != listeners.end(); ++i ) {
			(*i)->on(type, p1, p2, p3, p4, p5, p6);
		}
	}

	void addListener(Listener* aListener) {
		if (std::find(listeners.begin(), listeners.end(), aListener) == listeners.end())
			listeners.push_back(aListener);
	}

	void removeListener(Listener* aListener) {
		ListenerIter it = find(listeners.begin(), listeners.end(), aListener);
		if (it != listeners.end())
			listeners.erase(it);
	}

	void removeListeners() {
		listeners.clear();
	}
	
protected:
	ListenerList listeners;
};

#endif // SPEAKER_H
/**
 * @file
 * $Id: Speaker.h,v 1.8 2005/02/19 12:44:33 arnetheduck Exp $
 */
