#ifndef _VECTOR_H_
#define _VECTOR_H_

#define PI 3.1415
#define epsilon 0.5

typedef struct s_vector2
{
	float x;
	float y;

}t_vector2;

// meme chose qu'un vecteur, mais on cree quand meme la struct pour plus de clarte
typedef struct s_point
{
	float x;
	float y;

}t_point;

// defini par un vecteur directeur et un point
typedef struct s_line
{
	t_vector2 directorVector;
	t_point   dot;

}t_line;

// segment definie par 2 points
typedef struct s_segment
{
	t_point firstPoint;
	t_point secondPoint;

}t_segment;


// cercle definie par un point centre et un rayon
typedef struct s_circle
{
	t_point   centerOfCircle;
	float 	  radCircle;

}t_circle;

typedef struct s_rectangle
{
    t_point centerOfRectangle;
    t_segment halfHeight;
    t_segment halfWidth;

}t_rectangle;

typedef struct s_orientedRectangle
{
    t_rectangle rectangle;
    float angle;

}t_orientedRectangle;

typedef struct s_referential
{
    t_point centerRef;
    t_vector2 vecRefX;
    t_vector2 vecRefY;

}t_referential;

typedef struct s_range
{
    float min;
    float max;

}t_range;

typedef struct s_convex_polygon
{
	t_point* dotArray;


}t_convex_polygon;



t_vector2 nullVector();

t_vector2 unitaryVector(t_vector2 vector);

float calculateLengthOfVector(t_vector2 vector);

bool isVectorEqual(t_vector2 firstVector, t_vector2 secondVector);

float distanceBetweenTwoDots(t_point firstPoint, t_point secondPoint);

float calculateSquareLengthOfVector(t_vector2 vector);

t_vector2 add_two_vectors(t_vector2 firstVector, t_vector2 secondVector);

t_vector2 substractTwoVectors(t_vector2 firstVector, t_vector2 secondVector);

t_vector2 scaleVector(t_vector2 vector, float scale);

t_vector2 getOppositeVector(t_vector2 vector);

t_vector2 rotateVectorWithAngle(t_vector2 vector, float angle);

t_vector2 rotateVectorWith90Angle(t_vector2 vector);

t_point rotateDotAroundCenter(t_point point, t_point center, float angle);

float calculateDotProduct(t_vector2 firstVector, t_vector2 secondVector);

float calculateAngleBetweenVectors(t_vector2 firstVector, t_vector2 secondVector);

t_vector2 getNormalVector(t_vector2 vector);

t_vector2 worldToLocalVector(t_referential localRef, t_vector2 worldVec);

t_vector2 localToWorldVector(t_referential localRef, t_vector2 localVec);

t_point worldToLocalPoint(t_referential localRef, t_point worldPoint);

t_point localToWorldPoint(t_referential localRef, t_point localPoint);

bool bothRangeCollide(t_range firstRange, t_range secondRange);

t_segment getSegmentFromDots(t_point firstPoint, t_point secondPoint);

t_vector2 getVectorFromSegment(t_segment segment);

t_vector2 getVectorFromDots(t_point firstPoint, t_point secondPoint);

t_range get_range(float A, float B);

t_range get_range_from_two_ranges(t_range firstRange, t_range secondRange);

t_rectangle define_rectangle(t_point centerRect, float posCenterX, float posCenterY, float halfHeight, float halfWidth);


#endif
