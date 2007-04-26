/***************************************************************************
 *   Copyright (C) 2007 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef KTINTHASH_H
#define KTINTHASH_H

#include <QHash>

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
template<typename T>
class KTIntHash
{
	public:
		KTIntHash();
		KTIntHash(const KTIntHash<T> &other);
		~KTIntHash();
		
		void removeVisual(int pos);
		void remove(int pos);
		void moveVisual(int from, int to);
		bool containsVisual(int pos);
		
		int visualIndex(T val);
		int logicalIndex(T val);
		
		T visualValue(int pos);
		T takeVisual(int pos);
		
		void clear(bool alsoDelete = false)
		{
			if( alsoDelete )
				qDeleteAll(d->logicalIndices);
			d->logicalIndices.clear();
			d->visualIndices.clear();
			
			d->counter = 0;
		}
		
		int count() const
		{
			return d->logicalIndices.count();
		}
		
		bool contains(int pos);
		
		void insert(int pos, T value);
		void add(T value);
		
		void expandValue(int index);
		
		T value(int index) const;
		T operator[](int index) const;
		QList<T> visualValues() const;
		QList<T> values() const;
		
		QList<int> visualIndices() const;
		
		bool isEmpty();
		
	public:
		KTIntHash &operator=(const KTIntHash &other);
		
	private:
		struct Private;
		Private *const d;
};

template<typename T>
struct KTIntHash<T>::Private
{
	Private() : counter(0) {}
	
	QList<int> visualIndices;
	QHash<int, T> logicalIndices;
	
	int counter;
};

template<typename T>
KTIntHash<T>::KTIntHash() : d(new Private)
{
}

template<typename T>
KTIntHash<T>::KTIntHash(const KTIntHash<T> &other) : d(new Private)
{
	d->visualIndices = other.d->visualIndices;
	d->logicalIndices = other.d->logicalIndices;
	d->counter = other.d->counter;
}

template<typename T>
KTIntHash<T>::~KTIntHash()
{
	delete d;
}

template<typename T>
void KTIntHash<T>::removeVisual(int pos)
{
	if( containsVisual(pos) )
	{
		int logicalIndex = d->visualIndices.takeAt(pos);
		d->logicalIndices.remove(logicalIndex);
	}
}

template<typename T>
bool KTIntHash<T>::contains(int pos)
{
	return d->logicalIndices.contains(pos);
}

template<typename T>
void KTIntHash<T>::remove(int pos)
{
	d->logicalIndices.remove(pos);
	d->visualIndices.removeAll(pos);
}

template<typename T>
void KTIntHash<T>::moveVisual(int from, int to)
{
	if( containsVisual(from) && containsVisual(to) )
	{
		int val = d->visualIndices.takeAt(from);
		d->visualIndices.insert(to, val);
	}
}

template<typename T>
bool KTIntHash<T>::containsVisual(int pos)
{
	return pos >= 0 && pos < d->visualIndices.count();
}

template<typename T>
KTIntHash<T> &KTIntHash<T>::operator=(const KTIntHash<T> &other)
{
	d->visualIndices = other.d->visualIndices;
	d->logicalIndices = other.d->logicalIndices;
	d->counter = other.d->counter;
	
	return *this;
}

template<typename T>
int KTIntHash<T>::logicalIndex(T val)
{
	return d->logicalIndices.key(val);
}

template<typename T>
int KTIntHash<T>::visualIndex(T val)
{
	return d->visualIndices.indexOf( d->logicalIndices.key(val) );
}

template<typename T>
T KTIntHash<T>::visualValue(int pos)
{
	if( containsVisual(pos) )
	{
		return d->logicalIndices.value(d->visualIndices[pos]);
	}
	
	return d->logicalIndices.value(-1);
}

template<typename T>
T KTIntHash<T>::takeVisual(int pos)
{
	if( containsVisual(pos) )
	{
		int logicalIndex = d->visualIndices.takeAt(pos);
		return d->logicalIndices.take(logicalIndex);
	}
	
	return d->logicalIndices.take(-1);
}

template<typename T>
void KTIntHash<T>::insert(int pos, T value)
{
	if ( d->logicalIndices.contains(pos) )
		qDebug("###########################     OVERRIDING!! %d", pos);
	d->logicalIndices.insert(d->counter, value);
	d->visualIndices.insert(pos, d->counter);
	
	d->counter++;
}

template<typename T>
void KTIntHash<T>::add(T value)
{
	this->insert(d->counter, value);
}

template<typename T>
void KTIntHash<T>::expandValue(int index)
{
	int value = d->visualIndices[index];
	d->visualIndices.insert(index+1, value);
}


template<typename T>
T KTIntHash<T>::value(int index) const
{
	return d->visualIndices.value(index);
}

template<typename T>
T KTIntHash<T>::operator[](int index) const
{
	return d->logicalIndices.value(index);
}

template<typename T>
QList<T> KTIntHash<T>::visualValues() const
{
	QList<T> visualValues;
	
	foreach(int pos, d->visualIndices)
	{
		visualValues << d->logicalIndices.value(pos);
	}
	
	
	return visualValues;
}

template<typename T>
QList<T> KTIntHash<T>::values() const
{
	return d->logicalIndices.values();
}


template<typename T>
QList<int> KTIntHash<T>::visualIndices() const
{
	return d->visualIndices;
}

template<typename T>
bool KTIntHash<T>::isEmpty()
{
	return d->logicalIndices.isEmpty();
}

#endif

