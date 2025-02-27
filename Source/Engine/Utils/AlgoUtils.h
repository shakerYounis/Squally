#pragma once
#include <tuple>
#include <vector>

#include "cocos/math/CCGeometry.h"

namespace cocos2d
{
	class DrawNode;
}

class AlgoUtils
{
public:
	struct Triangle
	{
		cocos2d::Vec2 coords[3];

		Triangle(cocos2d::Vec2 coordA, cocos2d::Vec2 coordB, cocos2d::Vec2 coordC)
		{
			coords[0] = coordA;
			coords[1] = coordB;
			coords[2] = coordC;
		}

		Triangle()
		{
			coords[0] = cocos2d::Vec2::ZERO;
			coords[1] = cocos2d::Vec2::ZERO;
			coords[2] = cocos2d::Vec2::ZERO;
		}
	};

	static cocos2d::Vec3 computeArcVelocity(cocos2d::Vec3 source, cocos2d::Vec3 destination, cocos2d::Vec3 acceleration, cocos2d::Vec3 time);
	static cocos2d::Vec2 pointOnCircle(cocos2d::Vec2 center, float radius, cocos2d::Vec2 closestPoint);
	static cocos2d::Vec2 pointOnEllipse(cocos2d::Vec2 center, float rx, float ry, cocos2d::Vec2 closestPoint);
	static std::vector<int> subsetSum(const std::vector<int>& numbers, int sum, int requiredLength);
	
	static std::vector<Triangle> trianglefyPolygon(const std::vector<cocos2d::Vec2>& polygonPoints, const std::vector<cocos2d::Vec2>& holePoints = { });
	static bool isPointInTriangle(const Triangle& triangle, cocos2d::Vec2 point);
	static bool isPointInPolygon(const std::vector<cocos2d::Vec2>& points, cocos2d::Vec2 point);
	static cocos2d::Vec2 getClosestPointOnLine(std::tuple<cocos2d::Vec2, cocos2d::Vec2> segment, cocos2d::Vec2 point);
	static float getDistanceFromSegment(std::tuple<cocos2d::Vec2, cocos2d::Vec2> segment, cocos2d::Vec2 point);
	static cocos2d::Vec2 getLineIntersectionPoint(std::tuple<cocos2d::Vec2, cocos2d::Vec2> segmentA, std::tuple<cocos2d::Vec2, cocos2d::Vec2> segmentB);
	static bool doSegmentsIntersect(std::tuple<cocos2d::Vec2, cocos2d::Vec2> segmentA, std::tuple<cocos2d::Vec2, cocos2d::Vec2> segmentB);
	static std::vector<std::tuple<cocos2d::Vec2, cocos2d::Vec2>>
			buildSegmentsFromPoints(const std::vector<cocos2d::Vec2>& points);
	static std::vector<std::tuple<cocos2d::Vec2, cocos2d::Vec2>>
			shrinkSegments(const std::vector<std::tuple<cocos2d::Vec2, cocos2d::Vec2>>& segments);
	static cocos2d::CRect getPolygonRect(const std::vector<cocos2d::Vec2>& points);
	static std::vector<cocos2d::Vec2> insetPolygon(const std::vector<Triangle>& triangles,
			const std::vector<std::tuple<cocos2d::Vec2, cocos2d::Vec2>>& segments, float insetDistance);
	static cocos2d::Vec2 getSegmentNormal(std::tuple<cocos2d::Vec2, cocos2d::Vec2> segment);
	static float getSegmentAngle(std::tuple<cocos2d::Vec2, cocos2d::Vec2> segment,
			const std::vector<Triangle>& triangles, cocos2d::DrawNode* debugDrawNode = nullptr);
	static float getSegmentNormalAngle(std::tuple<cocos2d::Vec2, cocos2d::Vec2> segment,
			const std::vector<Triangle>& triangles, cocos2d::DrawNode* debugDrawNode = nullptr);
	static cocos2d::Vec2 getOutwardNormal(std::tuple<cocos2d::Vec2, cocos2d::Vec2> segment,
			const std::vector<Triangle>& triangles, cocos2d::DrawNode* debugDrawNode = nullptr);
};
