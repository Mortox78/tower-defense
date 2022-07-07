
#include "../include/vector.h"
#include <math.h>


t_vector2 nullVector()
{
	t_vector2 vector = {0, 0};

	return vector;
}


t_vector2 unitaryVector(t_vector2 vector)
{
    t_vector2 unitaryVector;

    float length = calculateLengthOfVector(vector);
    
    if (length != 0)
    {
        unitaryVector.x = vector.x / length;
        unitaryVector.y = vector.y / length;
    }

    return unitaryVector;
}

float calculateLengthOfVector(t_vector2 vector)
{
    return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}

float distanceBetweenTwoDots(t_point firstPoint, t_point secondPoint)
{
    return sqrt(pow(secondPoint.x - firstPoint.x, 2) + pow(secondPoint.y - firstPoint.y, 2));
}

float calculateSquareLengthOfVector(t_vector2 vector)
{
	return pow(vector.x, 2) + pow(vector.y, 2);
}


bool isVectorEqual(t_vector2 firstVector, t_vector2 secondVector)
{
    return firstVector.x  < secondVector.x + epsilon
        && firstVector.x  > secondVector.x - epsilon
        && firstVector.y  < secondVector.y + epsilon
        && firstVector.y  > secondVector.y - epsilon;
}


t_vector2 add_two_vectors(t_vector2 firstVector, t_vector2 secondVector)
{
    t_vector2 vector = {firstVector.x + secondVector.x, firstVector.y + secondVector.y};

	return vector;
}


t_vector2 substractTwoVectors(t_vector2 firstVector, t_vector2 secondVector)
{
    t_vector2 vector = {firstVector.x - secondVector.x, firstVector.y - secondVector.y};

    return vector;
}


t_vector2 scaleVector(t_vector2 vector, float scale)
{
    t_vector2 newVector = {vector.x * scale, vector.y * scale};

    return newVector;
}


t_vector2 getOppositeVector(t_vector2 vector)
{
    t_vector2 oppositeVector = {vector.x * -1, vector.y * -1};

    return oppositeVector;
}


t_vector2 rotateVectorWithAngle(t_vector2 vector, float angle)
{
    t_vector2 rotatedVector;

    float cosAngle = cos(angle);
    float sinAngle = sin(angle);

    rotatedVector.x = vector.x * cosAngle - vector.y * sinAngle;
    rotatedVector.y = vector.x * sinAngle + vector.y * cosAngle;
    
    return rotatedVector;
}

t_vector2 rotateVectorWith90Angle(t_vector2 vector)
{
    t_vector2 rotatedVector = rotateVectorWithAngle(vector, PI/2);

    return rotatedVector;
}


t_point rotateDotAroundCenter(t_point dot, t_point center, float angle)
{
    t_point rotatedDot;

    // On soustrait les coordonnees de notre point avec celui du centre pour pouvoir se placer dans notre repere orthonorme et non pas autour du point (Afin d'effectuer la rotation)
    float substractedX = dot.x - center.x;
    float substractedY = dot.y - center.y;

    float cosAngle = cos(PI*angle/180);
    float sinAngle = sin(PI*angle/180);

    // calcul de rotation avec angle, sauf que l'on ajoute les coordonnes de notre centre à la fin afin de se replacer dans le repere avec le centre
    rotatedDot.x = substractedX * cosAngle - substractedY * sinAngle + center.x;
    rotatedDot.y = substractedX * sinAngle + substractedY * cosAngle + center.y;


    return rotatedDot;
}

float calculateDotProduct(t_vector2 firstVector, t_vector2 secondVector)
{
    return firstVector.x * secondVector.x + firstVector.y * secondVector.y;
}


float calculateAngleBetweenVectors(t_vector2 firstVector, t_vector2 secondVector)
{
    float dotProductBothVector = calculateDotProduct(firstVector, secondVector);
    float lengthFirstVector = calculateLengthOfVector(firstVector);
    float lengthSecondVector = calculateLengthOfVector(secondVector);

    if (lengthFirstVector == 0 || lengthSecondVector == 0)
        return 0;

    return acos(dotProductBothVector / (lengthFirstVector * lengthSecondVector));
}


t_vector2 getNormalVector(t_vector2 vector)
{
    t_vector2 normalVector = {vector.y, - vector.x};

    return normalVector;
}


t_vector2 worldToLocalVector(t_referential localRef, t_vector2 worldVec)
{
    t_vector2 newVector;

    newVector.x = calculateDotProduct(worldVec, localRef.vecRefX);
    newVector.y = calculateDotProduct(worldVec, localRef.vecRefY);

    return newVector;
}


t_vector2 localToWorldVector(t_referential local, t_vector2 localVec)
{
    t_vector2 newVector;

    newVector.x = local.vecRefX.x * localVec.x + local.vecRefY.y * localVec.y;
    newVector.y = local.vecRefX.x * localVec.x + local.vecRefY.y * localVec.y;

    return newVector;
}

t_point worldToLocalPoint(t_referential local, t_point pointToChange)
{    
    t_vector2 vecWorldLocal = {local.centerRef.x, local.centerRef.y};

    t_vector2 vecWorldToPoint = {pointToChange.x, pointToChange.y};
    
    t_vector2 vecLocalToPoint;
    vecLocalToPoint.x = vecWorldToPoint.x - vecWorldLocal.x;
    vecLocalToPoint.y = vecWorldToPoint.y - vecWorldLocal.y;
    
    t_point newDot;
    newDot.x = calculateDotProduct(vecLocalToPoint, local.vecRefX);
    newDot.y = calculateDotProduct(vecLocalToPoint, local.vecRefY);
    
    return newDot;
}



t_point localToWorldPoint(t_referential localRef, t_point localPoint)
{
	t_vector2 newRefLocalX = {localRef.vecRefX.x * localPoint.x, localRef.vecRefX.y * localPoint.x};
	t_vector2 newRefLocalY = {localRef.vecRefY.x * localPoint.y, localRef.vecRefY.y * localPoint.y};

	t_vector2 vecLocalToLocalPoint = add_two_vectors(newRefLocalX, newRefLocalY);

	t_vector2 vecWorldToLocal = {localRef.centerRef.x, localRef.centerRef.y};

	t_vector2 centerRefToDotInWorld = add_two_vectors(vecLocalToLocalPoint, vecWorldToLocal);

	t_point dotInWorld = {centerRefToDotInWorld.x, centerRefToDotInWorld.y};

	return dotInWorld;
}


bool bothRangeCollide(t_range firstRange, t_range secondRange)
{
	if (firstRange.min > secondRange.max || secondRange.min > firstRange.max)
		return false;
	else
		return true;
}


t_segment getSegmentFromDots(t_point firstPoint, t_point secondPoint)
{
	t_segment segment;

	segment.firstPoint = firstPoint;
	segment.secondPoint = secondPoint;

	return segment;
}
t_vector2 getVectorFromSegment(t_segment segment)
{
	t_vector2 vector;

	vector.x = segment.secondPoint.x - segment.firstPoint.x;
	vector.y = segment.secondPoint.y - segment.firstPoint.y;

	return vector;
}


t_vector2 getVectorFromDots(t_point firstPoint, t_point secondPoint)
{
	t_segment segment = getSegmentFromDots(firstPoint, secondPoint);

	t_vector2 vector = getVectorFromSegment(segment);

	return vector;

}

t_range get_range(float A, float B)
{
	t_range range;

	if (A < B)
	{
		range.min = A;
		range.max = B;
	}
	else if (A == B)
	{
		range.min = A;
		range.max = A;
	}
	else
	{
		range.min = B;
		range.max = A;
	}

	return range;
}

t_range get_range_from_two_ranges(t_range firstRange, t_range secondRange)
{
	t_range newRange;

	newRange = firstRange;

	if (secondRange.min > firstRange.max)
	{
		newRange.max = secondRange.max;
		return newRange;
	}
	if (secondRange.max < firstRange.min)
	{
		newRange.min = secondRange.min;
		return firstRange;
	}
	if (secondRange.max > firstRange.max)
		newRange.max = secondRange.max;
	if (secondRange.min < firstRange.min)
		newRange.min = secondRange.min;


	return newRange;
}

t_rectangle define_rectangle(t_point centerRect, float posCenterX, float posCenterY, float halfHeight, float halfWidth)
{
	t_rectangle rect;

	rect.centerOfRectangle.x = centerRect.x + posCenterX;
	rect.centerOfRectangle.y = centerRect.y + posCenterY;
	
	rect.halfHeight.firstPoint 	= centerRect;

	rect.halfHeight.secondPoint.x = centerRect.x;
        rect.halfHeight.secondPoint.y = centerRect.y + halfHeight;

	rect.halfWidth.firstPoint 	= centerRect;
	
	rect.halfWidth.secondPoint.x  = centerRect.x + halfWidth;
	rect.halfWidth.secondPoint.y  = centerRect.y;

	return rect;
}





