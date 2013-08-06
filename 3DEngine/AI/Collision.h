#ifndef __Collision__
#define __Collision__

#include "../Config.h"
#include "../CurvesData.h"
#include "../Node.h"

class Line2D
{
public:
	NEWDEL_DECL
	PVRTVECTOR2 p0;
	PVRTVECTOR2 p1;
};

class Line3D
{
public:
	NEWDEL_DECL
	PVRTVECTOR3 p0;
	PVRTVECTOR3 p1;
};

class Collision
{
private:
	Collision(void) {}

public:
	NEWDEL_DECL;

	inline static bool CircleTriangleEdgeIntersection(PVRTVec2 p0, PVRTVec2 p1, PVRTVec2 pos, float radius)
	{

		float A = pos.x - p0.x;
		float B = pos.y - p0.y;
		float C = p1.x - p0.x;
		float D = p1.y - p0.y;

		float dot = A * C + B * D;
		float len_sq = C * C + D * D;
		float param = dot / len_sq;

		PVRTVec2 inter;

		if(param < 0)
		{
			inter.x = p0.x;
			inter.y = p0.y;
		}
		else if(param > 1)
		{
			inter.x = p1.x;
			inter.y = p1.y;
		}
		else
		{
			inter.x = p0.x + param * C;
			inter.y = p0.y + param * D;
		}
		PVRTVec2 distVec = pos - inter;
		if ( MM(radius) > distVec.lenSqr())
		{
			return true;
		}

		return false;
	}


	inline static float Vec2DistanceFromLine2D(const Line2D & line, PVRTVec2 wordlPos)
	{
		PVRTVec2 pos;

		pos.x = wordlPos.x;
		pos.y = wordlPos.y;


		float A = pos.x - line.p0.x;
		float B = pos.y - line.p0.y;
		float C = line.p1.x - line.p0.x;
		float D = line.p1.y - line.p0.y;

		float dot = A * C + B * D;
		float len_sq = C * C + D * D;
		float param = dot / len_sq;

		PVRTVec2 inter;

		if(param < 0)
		{
			inter.x = line.p0.x;
			inter.y = line.p0.y;
		}
		else if(param > 1)
		{
			inter.x = line.p1.x;
			inter.y = line.p1.y;
		}
		else
		{
			inter.x = line.p0.x + param * C;
			inter.y = line.p0.y + param * D;
		}
		PVRTVec2 distVec = pos - inter;
		return distVec.lenSqr();
	}

	inline static PVRTVec2 Vec2CollisionLine2D(const Line2D & line, PVRTVec2 wordlPos)
	{
		PVRTVec2 pos;

		pos.x = wordlPos.x;
		pos.y = wordlPos.y;


		float A = pos.x - line.p0.x;
		float B = pos.y - line.p0.y;
		float C = line.p1.x - line.p0.x;
		float D = line.p1.y - line.p0.y;

		float dot = A * C + B * D;
		float len_sq = C * C + D * D;
		float param = dot / len_sq;

		PVRTVec2 inter;

		if(param < 0)
		{
			inter.x = line.p0.x;
			inter.y = line.p0.y;
		}
		else if(param > 1)
		{
			inter.x = line.p1.x;
			inter.y = line.p1.y;
		}
		else
		{
			inter.x = line.p0.x + param * C;
			inter.y = line.p0.y + param * D;
		}
		return inter;
	}


	inline static bool LineSegmentIntersection( Line2D line1,
												Line2D line2,
												float & intersX,
												float & intersY )
	{
		float  distAB, theCos, theSin, newX, ABpos ;

		//  Fail if either line segment is zero-length.
		/*
		if (line1.p0.x==line1.p1.x && line1.p0.y==line1.p1.y || line2.p0.x==line2.p1.x && line2.p0.y==line2.p1.y)
			return false;

		//  Fail if the segments share an end-point.
		if (line1.p0.x==line2.p0.x && line1.p0.y==line2.p0.y ||
			line1.p1.x==line2.p0.x && line1.p1.y==line2.p0.y ||
			line1.p0.x==line2.p1.x && line1.p0.y==line2.p1.y ||
			line1.p1.x==line2.p1.x && line1.p1.y==line2.p1.y) 
		{
				return false; 
		}
		*/

		//  (1) Translate the system so that point A is on the origin.
		line1.p1.x-=line1.p0.x;
		line1.p1.y-=line1.p0.y;
		line2.p0.x-=line1.p0.x;
		line2.p0.y-=line1.p0.y;
		line2.p1.x-=line1.p0.x;
		line2.p1.y-=line1.p0.y;

		//  Discover the length of segment A-B.
		distAB=sqrt(line1.p1.x*line1.p1.x+line1.p1.y*line1.p1.y);

		//  (2) Rotate the system so that point B is on the positive X axis.
		theCos = line1.p1.x/distAB;
		theSin = line1.p1.y/distAB;
		newX = line2.p0.x*theCos+line2.p0.y*theSin;
		line2.p0.y = line2.p0.y*theCos-line2.p0.x*theSin;
		line2.p0.x = newX;
		newX = line2.p1.x*theCos+line2.p1.y*theSin;
		line2.p1.y = line2.p1.y*theCos-line2.p1.x*theSin;
		line2.p1.x = newX;

		//  Fail if segment C-D doesn't cross line A-B.
		if (line2.p0.y < 0.0f && line2.p1.y<0.0f || line2.p0.y>=0.0f && line2.p1.y>=0.0f ) 
			return false;

		//  (3) Discover the position of the intersection point along line A-B.
		ABpos = line2.p1.x+(line2.p0.x-line2.p1.x)*line2.p1.y/(line2.p1.y-line2.p0.y);

		//  Fail if segment C-D crosses line A-B outside of segment A-B.
		if (ABpos<0. || ABpos>distAB)
			return false;

		//  (4) Apply the discovered position to line A-B in the original coordinate system.
		intersX = line1.p0.x+ABpos*theCos;
		intersY = line1.p0.y+ABpos*theSin;

		//  Success.
		return true;
	}

};


#endif //__Collision__