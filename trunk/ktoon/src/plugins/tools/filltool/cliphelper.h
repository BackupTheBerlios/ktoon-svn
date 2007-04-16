

#ifndef CLIPHELPER_H
#define CLIPHELPER_H

#include <QPainterPath>

class ClipHelper
{
	public:
		static QPainterPath unite(const QPainterPath &path1, const QPainterPath &path2);
		static QPainterPath intersect(const QPainterPath &path1, const QPainterPath &path2);
		static QPainterPath subtract(const QPainterPath &path1, const QPainterPath &path2);
};



#endif




