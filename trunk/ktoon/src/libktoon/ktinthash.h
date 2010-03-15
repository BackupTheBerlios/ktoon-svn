/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9a                              *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 * 
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *   2003:                                                                 *
 *    Fernado Roldan                                                       * 
 *    Simena Dinas                                                         *
 *                                                                         *
 *   License:                                                              *
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
            if (alsoDelete)
                qDeleteAll(k->logicalIndices);

            k->logicalIndices.clear();
            k->visualIndices.clear();
            
            k->counter = 0;
        }
        
        int count() const
        {
            return k->logicalIndices.count();
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
        Private *const k;
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
KTIntHash<T>::KTIntHash() : k(new Private)
{
}

template<typename T>
KTIntHash<T>::KTIntHash(const KTIntHash<T> &other) : k(new Private)
{
    k->visualIndices = other.k->visualIndices;
    k->logicalIndices = other.k->logicalIndices;
    k->counter = other.k->counter;
}

template<typename T>
KTIntHash<T>::~KTIntHash()
{
    delete k;
}

template<typename T>
void KTIntHash<T>::removeVisual(int pos)
{
    if (containsVisual(pos)) {
        int logicalIndex = k->visualIndices.takeAt(pos);
        k->logicalIndices.remove(logicalIndex);
    }
}

template<typename T>
bool KTIntHash<T>::contains(int pos)
{
    return k->logicalIndices.contains(pos);
}

template<typename T>
void KTIntHash<T>::moveVisual(int from, int to)
{
    if (containsVisual(from) && containsVisual(to)) {
        int val = k->visualIndices.takeAt(from);
        k->visualIndices.insert(to, val);
    }
}

template<typename T>
bool KTIntHash<T>::containsVisual(int pos)
{
    return pos >= 0 && pos < k->visualIndices.count();
}

template<typename T>
KTIntHash<T> &KTIntHash<T>::operator=(const KTIntHash<T> &other)
{
    k->visualIndices = other.k->visualIndices;
    k->logicalIndices = other.k->logicalIndices;
    k->counter = other.k->counter;
    
    return *this;
}

template<typename T>
int KTIntHash<T>::logicalIndex(T val)
{
    return k->logicalIndices.key(val);
}

template<typename T>
int KTIntHash<T>::visualIndex(T val)
{
    return k->visualIndices.indexOf(k->logicalIndices.key(val));
}

template<typename T>
T KTIntHash<T>::visualValue(int pos)
{
    if (containsVisual(pos))
        return k->logicalIndices.value(k->visualIndices[pos]);
    
    return k->logicalIndices.value(-1);
}

template<typename T>
T KTIntHash<T>::takeVisual(int pos)
{
    if (containsVisual(pos)) {
        int logicalIndex = k->visualIndices.takeAt(pos);
        return k->logicalIndices.take(logicalIndex);
    }
    
    return k->logicalIndices.take(-1);
}

template<typename T>
void KTIntHash<T>::insert(int pos, T value)
{
    if (k->logicalIndices.contains(pos))
        qDebug("######   OVERRIDING!! %d", pos);

    k->logicalIndices.insert(pos, value);
    k->visualIndices.insert(pos, pos);
    
    k->counter++;
}

template<typename T>
void KTIntHash<T>::remove(int pos)
{
    if (containsVisual(pos)) {
        if (pos == count()-1) {
            int logicalIndex = k->visualIndices.takeAt(pos);
            k->logicalIndices.remove(logicalIndex);
            k->counter--;
        } else {
            //int logicalPos = k->visualIndices.takeAt(pos);
            //k->logicalIndices.remove(logicalPos);
            //k->removeVisual(pos);
            int total = count() - 1;
            for (int i=pos+1;i<=total;i++) {
                 T value = this->takeVisual(i); 
                 int index = i - 1;
                 k->logicalIndices.insert(index, value);
                 k->visualIndices.insert(index, index);
            }
        }
    } 
}

template<typename T>
void KTIntHash<T>::add(T value)
{
    this->insert(k->counter, value);
}

template<typename T>
void KTIntHash<T>::expandValue(int index)
{
    int value = k->visualIndices[index];
    k->visualIndices.insert(index+1, value);
}

template<typename T>
T KTIntHash<T>::value(int index) const
{
    return k->visualIndices.value(index);
}

template<typename T>
T KTIntHash<T>::operator[](int index) const
{
    return k->logicalIndices.value(index);
}

template<typename T>
QList<T> KTIntHash<T>::visualValues() const
{
    QList<T> visualValues;
    
    foreach (int pos, k->visualIndices)
             visualValues << k->logicalIndices.value(pos);
    
    return visualValues;
}

template<typename T>
QList<T> KTIntHash<T>::values() const
{
    return k->logicalIndices.values();
}

template<typename T>
QList<int> KTIntHash<T>::visualIndices() const
{
    return k->visualIndices;
}

template<typename T>
bool KTIntHash<T>::isEmpty()
{
    return k->logicalIndices.isEmpty();
}

#endif
