

#include "cliphelper.h"

#include "qpathclipper_p.h"


QPainterPath ClipHelper::unite(const QPainterPath &path1, const QPainterPath &path2)
{
	if (path1.isEmpty() || path2.isEmpty())
		return path1.isEmpty() ? path2 : path1;
	
	QPathClipper clipper(path1, path2);
	return clipper.clip(QPathClipper::BoolOr);
}

QPainterPath ClipHelper::intersect(const QPainterPath &path1, const QPainterPath &path2)
{
	if (path1.isEmpty() || path2.isEmpty())
		return QPainterPath();
	QPathClipper clipper(path1, path2);
	return clipper.clip(QPathClipper::BoolAnd);
}

QPainterPath ClipHelper::subtract(const QPainterPath &path1, const QPainterPath &path2)
{
	if (path1.isEmpty() || path2.isEmpty())
		return path1;
	QPathClipper clipper(path1, path2);
	return clipper.clip(QPathClipper::BoolSub);
}

