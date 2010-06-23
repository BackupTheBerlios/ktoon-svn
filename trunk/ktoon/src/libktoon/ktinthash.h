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
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
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
        bool containsObject(int pos);
        
        int visualIndex(T val);
        int logicalIndex(T val);
        
        T visualValue(int pos);
        T takeVisual(int pos);
        
        void clear(bool alsoDelete = false)
        {
            if (alsoDelete)
                qDeleteAll(k->dataHash);

            k->dataHash.clear();
            k->counter = 0;
        }
        
        int count() const
        {
            return k->dataHash.count();
        }
        
        bool contains(int pos);
        
        void insert(int pos, T value);
        void add(T value);
        
        void expandValue(int index);
        
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
    QHash<int, T> dataHash;
    int counter;
};

template<typename T>
KTIntHash<T>::KTIntHash() : k(new Private)
{
}

template<typename T>
KTIntHash<T>::KTIntHash(const KTIntHash<T> &other) : k(new Private)
{
    k->dataHash = other.k->dataHash;
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
    if (containsObject(pos))
        k->dataHash.remove(pos);
}

template<typename T>
bool KTIntHash<T>::contains(int pos)
{
    return k->dataHash.contains(pos);
}

template<typename T>
void KTIntHash<T>::moveVisual(int from, int to)
{
    qDebug("******** Hello world!!!");
    qDebug("KTIntHash::moveVisual FROM: %d", from);
    qDebug("KTIntHash::moveVisual TO: %d", to);

    T value = k->dataHash.value(from);
    this->insert(to, value);

    qDebug("KTIntHash::moveVisual SIZE: %d", k->dataHash.size());
}

template<typename T>
bool KTIntHash<T>::containsObject(int pos)
{
    return pos >= 0 && pos < k->dataHash.size();
}

template<typename T>
KTIntHash<T> &KTIntHash<T>::operator=(const KTIntHash<T> &other)
{
    k->dataHash = other.k->dataHash;
    k->counter = other.k->counter;
    
    return *this;
}

template<typename T>
int KTIntHash<T>::logicalIndex(T val)
{
    return k->dataHash.key(val);
}

template<typename T>
int KTIntHash<T>::visualIndex(T val)
{
    return k->dataHash.key(val);
}

template<typename T>
T KTIntHash<T>::visualValue(int pos)
{
    if (containsObject(pos))
        return k->dataHash.value(pos);
    
    return k->dataHash.value(-1);
}

template<typename T>
T KTIntHash<T>::takeVisual(int pos)
{
    if (containsObject(pos))
        return k->dataHash.take(pos);
    
    return k->dataHash.take(-1);
}

template<typename T>
void KTIntHash<T>::insert(int pos, T value)
{
    if (k->dataHash.contains(pos))
        qDebug("######   OVERRIDING!! %d", pos);

    k->dataHash.insert(pos, value);
    k->counter++;
}

template<typename T>
void KTIntHash<T>::remove(int pos)
{
    if (containsObject(pos)) {
        if (pos == count()-1) {
            k->dataHash.remove(pos);
            k->counter--;
        } else {
            int total = count() - 1;
            for (int i=pos+1;i<=total;i++) {
                 T value = this->takeVisual(i); 
                 int index = i - 1;
                 k->dataHash.insert(index, value);
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
    T value = this->takeVisual(index);
    k->dataHash.insert(index + 1, value);
}

template<typename T>
T KTIntHash<T>::operator[](int index) const
{
    return k->dataHash.value(index);
}

template<typename T>
QList<T> KTIntHash<T>::visualValues() const
{
    QList<T> visualValues;
    
    foreach (int pos, k->dataHash.keys())
             visualValues << k->dataHash.value(pos);

    return visualValues;
}

template<typename T>
QList<T> KTIntHash<T>::values() const
{
    return k->dataHash.values();
}

template<typename T>
QList<int> KTIntHash<T>::visualIndices() const
{
    return k->dataHash.keys();
}

template<typename T>
bool KTIntHash<T>::isEmpty()
{
    return k->dataHash.isEmpty();
}

#endif
