
#include "../include/collisions.h"
#include "../include/vector.h"


bool lineLine(t_line firstLine, t_line secondLine)
{
	float firstVectorX = firstLine.directorVector.x;
	float firstVectorY = firstLine.directorVector.y;
	float secondVectorX = secondLine.directorVector.x;
	float secondVectorY = secondLine.directorVector.y;


    t_vector2 vectorfirstPointLineTosecondPointLine;
    vectorfirstPointLineTosecondPointLine.x = secondLine.dot.x - firstLine.dot.x;
    vectorfirstPointLineTosecondPointLine.y = secondLine.dot.y - firstLine.dot.y;
    float vectorfirstPointLineTosecondPointLineX = vectorfirstPointLineTosecondPointLine.x;
    float vectorfirstPointLineTosecondPointLineY = vectorfirstPointLineTosecondPointLine.y;

    // Si ils ne sont pas colineaire, alors il y a collision
	if (firstVectorX * secondVectorY - secondVectorX * firstVectorY == 0 
	&& vectorfirstPointLineTosecondPointLineX * secondVectorY - secondVectorX * vectorfirstPointLineTosecondPointLineY != 0)
		return false;
	else
		return true;

}


bool lineSegment(t_line line, t_segment segment)
{
    t_vector2 normalVectorOfLine = rotateVectorWith90Angle(line.directorVector);

    // definition de nos 2 vecteurs dans le plan grace aux points du segment ainsi que le point de la ligne
	float dotLineX = line.dot.x;
	float dotLineY = line.dot.y;

	float firstPointSegmentX = segment.firstPoint.x;
	float firstPointSegmentY = segment.firstPoint.y;

	float secondPointSegmentX = segment.secondPoint.x;
	float secondPointSegmentY = segment.secondPoint.y;

    t_vector2 firstVector;
    firstVector.x = firstPointSegmentX - dotLineX;
    firstVector.y = firstPointSegmentY - dotLineY;

    t_vector2 secondVector;
    secondVector.x = secondPointSegmentX - dotLineX;
    secondVector.y = secondPointSegmentY - dotLineY;


    // calcul des deux produits scalaire
    float firstPointProduct = calculateDotProduct(normalVectorOfLine, firstVector);
    float secondPointProduct = calculateDotProduct(normalVectorOfLine, secondVector);

//    float epsilon = -0.5;

    // Si le produit de nos deux produits scalaire est superieur a 0 (epsilon) alors il n'y a pas de collisions
    // (signifie que nos deux cos sont inferieur a 90 et donc que notre resultat est positif
    // le segment est que d'un cote de la ligne)
    if (firstPointProduct * secondPointProduct >= 0)
        return false;
    else
        return true;

}


bool segmentSegment(t_segment firstSegment, t_segment secondSegment)
{

    // If both vectors (segments) are colinear and not superimposed, there is no collision
    if (segmentSegmentColinear(firstSegment, secondSegment) && segmentSegmentSuperimposed(firstSegment, secondSegment) == false)
        return false;
    // If a segment is on the other side of the other segment (the two points of the segment both in one side of the other segment),
    // there is no collision
    if (segmentSegmentOnTheSide(firstSegment, secondSegment))
        return false;
    else 
        return true;
}


bool segmentSegmentColinear(t_segment firstSegment, t_segment secondSegment)
{
	float firstPointFirstSegmentX = firstSegment.firstPoint.x;
	float firstPointFirstSegmentY = firstSegment.firstPoint.y;

	float secondPointFirstSegmentX = secondSegment.secondPoint.x;
	float secondPointFirstSegmentY = secondSegment.secondPoint.y;

	float firstPointSecondSegmentX = firstSegment.firstPoint.x;
	float firstPointSecondSegmentY = firstSegment.firstPoint.y;

	float secondPointSecondSegmentX = secondSegment.secondPoint.x;
	float secondPointSecondSegmentY = secondSegment.secondPoint.y;



    t_vector2 firstVector;
    firstVector.x = firstPointFirstSegmentX - secondPointFirstSegmentX;
    firstVector.y = firstPointFirstSegmentY - secondPointFirstSegmentY;

    t_vector2 secondVector;
    secondVector.x = firstPointSecondSegmentX - secondPointSecondSegmentX;
    secondVector.y = firstPointSecondSegmentY - secondPointSecondSegmentY;


	if (firstVector.x * secondVector.y - secondVector.x * firstVector.y != 0)
        return true;
    else
        return false;

}


// secondSegment is our line
bool segmentSegmentOnTheSide(t_segment firstSegment, t_segment secondSegment)
{
    // Passage de notre secondSegment en vecteur
    t_vector2 secondSegmentVector;
    secondSegmentVector.x = secondSegment.firstPoint.x - secondSegment.secondPoint.x;
    secondSegmentVector.y = secondSegment.firstPoint.y - secondSegment.secondPoint.y;


    // definition de notre droite 
    t_line line;
    line.directorVector = secondSegmentVector;
    line.dot = secondSegment.firstPoint;


    // obtenir le vecteur normal de notre droite;
    t_vector2 normalVectorOfLine = rotateVectorWith90Angle(line.directorVector);


    // definition de nos 2 vecteurs dans le plan grace aux points du segment ainsi que le point de la ligne
    float dotLineX = line.dot.x;
    float dotLineY = line.dot.y;

    float firstPointSegmentX = firstSegment.firstPoint.x;
    float firstPointSegmentY = firstSegment.firstPoint.y;

    float secondPointSegmentX = firstSegment.secondPoint.x;
    float secondPointSegmentY = firstSegment.secondPoint.y;

    t_vector2 firstVector;
    firstVector.x = firstPointSegmentX - dotLineX;
    firstVector.y = firstPointSegmentY - dotLineY;

    t_vector2 secondVector;
    secondVector.x = secondPointSegmentX - dotLineX;
    secondVector.y = secondPointSegmentY - dotLineY;


    // calcul des deux produits scalaire
    float firstPointProduct = calculateDotProduct(normalVectorOfLine, firstVector);
    float secondPointProduct = calculateDotProduct(normalVectorOfLine, secondVector);

//    float epsilon = -0.5;

    // Si le produit de nos deux produits scalaire est superieur a 0 (epsilon) alors il n'y a pas de collisions
    // (signifie que nos deux cos sont inferieur a 90 et donc que notre resultat est positif,
    // le segment est que d'un cote de la ligne)
    if (firstPointProduct * secondPointProduct >= 0)
        return true;
    else
        return false;




}


bool dotDot(t_point firstPoint, t_point secondPoint)
{
    if (firstPoint.x > secondPoint.x - epsilon
        && firstPoint.x < secondPoint.x + epsilon
        && firstPoint.y > secondPoint.y - epsilon
        && firstPoint.y < secondPoint.y + epsilon)
            return true;
    else
            return false;


}

bool dotLine(t_point dot, t_line line)
{
    t_vector2 vectorDotLineToDot;
    vectorDotLineToDot.x = dot.x - line.dot.x;
    vectorDotLineToDot.y = dot.y - line.dot.y;

    float vectorDotLineToDotX = vectorDotLineToDot.x;
    float vectorDotLineToDotY = vectorDotLineToDot.y;
    float directorVectorX     = line.directorVector.x;
    float directorVectorY     = line.directorVector.y;

    // Si ils sont colineaires, cela signifie que le point est sur la droite, donc il y a collision
    if (vectorDotLineToDotX * directorVectorY - directorVectorX * vectorDotLineToDotY == 0)
        return true;
    else 
        return false;


}



bool dotSegment(t_point dot, t_segment segment)
{
    t_vector2 vectorfirstPointSegmentToDot;
    vectorfirstPointSegmentToDot.x = dot.x - segment.firstPoint.x;
    vectorfirstPointSegmentToDot.y = dot.y - segment.firstPoint.y;

    t_vector2 vectorsecondPointSegmentToDot;
    vectorsecondPointSegmentToDot.x = dot.x - segment.secondPoint.x;
    vectorsecondPointSegmentToDot.y = dot.y - segment.secondPoint.y;

    t_vector2 vectorSegment;
    vectorSegment.x = segment.secondPoint.x - segment.firstPoint.x;
    vectorSegment.y = segment.secondPoint.y - segment.firstPoint.y;

    float vectorfirstPointSegmentToDotX = vectorfirstPointSegmentToDot.x;
    float vectorfirstPointSegmentToDotY = vectorfirstPointSegmentToDot.y;
    float vectorSegmentX              = vectorSegment.x;
    float vectorSegmentY              = vectorSegment.y;



    float lengthOfVectorfirstPointSegmentToDot = calculateLengthOfVector(vectorfirstPointSegmentToDot);
    float lengthOfVectorsecondPointSegmentToDot= calculateLengthOfVector(vectorsecondPointSegmentToDot);
    float lengthOfVectorSegment              = calculateLengthOfVector(vectorSegment);



    // Si nos deux vecteurs sont colineaires, cela veut dire que notre point est forcement sur la droite sous-tendant de notre segment,
    // si la longueur entre le premier point du segment et notre point est inferieur a la longueur
    // de notre segment, ALORS cela veut dire que notre point est sur notre segment (collision)
    if (vectorfirstPointSegmentToDotX * vectorSegmentY - vectorSegmentX * vectorfirstPointSegmentToDotY == 0
        && lengthOfVectorfirstPointSegmentToDot < lengthOfVectorSegment
        && lengthOfVectorsecondPointSegmentToDot < lengthOfVectorSegment)
        return true;
    else
        return false;


}


bool dotCircle(t_point dot, t_circle circle)
{
    t_vector2 vectorCenterCircleToDot;
    vectorCenterCircleToDot.x = dot.x - circle.centerOfCircle.x;
    vectorCenterCircleToDot.y = dot.y - circle.centerOfCircle.y;

    float lengthOfVectorCenterCircleToDot = calculateLengthOfVector(vectorCenterCircleToDot);

    if (lengthOfVectorCenterCircleToDot < circle.radCircle)
        return true;
    else
        return false;
}



bool dotRectangle(t_point dot, t_rectangle rectangle)
{


    // calculate the length of our 2 half segments of the rectangle
    float lengthOfSegmentHalfHeight = distanceBetweenTwoDots(rectangle.halfHeight.firstPoint, rectangle.halfHeight.secondPoint);
    float lengthOfSegmentHalfWidth  = distanceBetweenTwoDots(rectangle.halfWidth.firstPoint, rectangle.halfWidth.secondPoint);

    float limitMaxX = rectangle.halfWidth.secondPoint.x;
    float limitMinX = limitMaxX - (lengthOfSegmentHalfWidth * 2);
    float limitMaxY = rectangle.halfHeight.secondPoint.y;
    float limitMinY = limitMaxY - (lengthOfSegmentHalfHeight * 2);

    if (dot.x < limitMaxX
     && dot.x > limitMinX
     && dot.y < limitMaxY
     && dot.y > limitMinY)
        return true;
    else
        return false;


}


/*bool circleLine(t_circle circle, t_line line)
{
    t_vector2 normalVectorOfLine = rotateVectorWith90Angle(line.directorVector);


    t_segment radiusOfCircle = circle.radiusOfCircle;
    // definition de nos 2 vecteurs dans le plan grace aux points du segment ainsi que le point de la ligne
	float dotLineX = line.dot.x;
	float dotLineY = line.dot.y;

	float firstPointSegmentX = radiusOfCircle.firstPoint.x;
	float firstPointSegmentY = radiusOfCircle.firstPoint.y;

	float secondPointSegmentX = radiusOfCircle.secondPoint.x;
	float secondPointSegmentY = radiusOfCircle.secondPoint.y;

    t_vector2 firstVector;
    firstVector.x = firstPointSegmentX - dotLineX;
    firstVector.y = firstPointSegmentY - dotLineY;

    t_vector2 secondVector;
    secondVector.x = secondPointSegmentX - dotLineX;
    secondVector.y = secondPointSegmentY - dotLineY;


    // calcul des deux produits scalaire
    float firstPointProduct = calculateDotProduct(normalVectorOfLine, firstVector);
    float secondPointProduct = calculateDotProduct(normalVectorOfLine, secondVector);

//    float epsilon = -0.5;

    // Si le produit de nos deux produits scalaire est superieur a 0 (epsilon) alors il n'y a pas de collisions (signifie que nos deux cos
    // sont inferieur a 90 et donc que notre resultat est positif
    // le segment est que d'un cote de la ligne)
    if (firstPointProduct * secondPointProduct >= 0)
        return false;
    else
        return true;

}
*/

bool circleCircle(t_circle firstCircle, t_circle secondCircle)
{  
    float distanceBetweenBothCenterOfCircle   = distanceBetweenTwoDots(firstCircle.centerOfCircle, secondCircle.centerOfCircle);
    
    
    if (distanceBetweenBothCenterOfCircle < firstCircle.radCircle + secondCircle.radCircle)
        return true;
    else
        return false;
    
}


bool rectangleRectangle(t_rectangle firstRectangle, t_rectangle secondRectangle)
{
    // Dots of firstRectangle  : A = top, B = right, C = bottom, D = left
    // Dots of secondRectangle : E = top, F = right
    // FR = firstRectaingle
    // SR = secondRectangle

    t_point centerFR = firstRectangle.centerOfRectangle;
    t_point centerSR = secondRectangle.centerOfRectangle;
    t_point A 	   = firstRectangle.halfHeight.secondPoint;
    t_point B  	   = firstRectangle.halfWidth.secondPoint;
    t_point E 	   = secondRectangle.halfHeight.secondPoint;
    t_point F 	   = secondRectangle.halfWidth.secondPoint;

    // calculate the length of both half segments of the firstRectangle
    float lengthHalfHeightFR = distanceBetweenTwoDots(centerFR, A);
    float lengthHalfWidthFR  = distanceBetweenTwoDots(centerFR, B);

    t_point D  = centerFR;
    D.x -= lengthHalfWidthFR;

    t_point C  = centerFR;
    C.y -= lengthHalfHeightFR;

    // calculate the length of both half segments of the secondRectangle
    float lengthHalfHeightSR = distanceBetweenTwoDots(centerSR, E);
    float lengthHalfWidthSR  = distanceBetweenTwoDots(centerSR, F);

    // set the limits for the collisions between both rectangles
    float maxSecRecX = F.x;
    float minSecRecX = maxSecRecX - (lengthHalfWidthSR * 2);
    float maxSecRecY = E.y;
    float minSecRecY = maxSecRecY - (lengthHalfHeightSR * 2);
    
    // If the top dot hit
    if ((A.y <= maxSecRecY && A.y >= minSecRecY && D.x <= maxSecRecX && D.x >= minSecRecX)
     || (C.y <= maxSecRecY && C.y >= minSecRecY && D.x <= maxSecRecX && D.x >= minSecRecX)
     || (C.y <= maxSecRecY && C.y >= minSecRecY && B.x <= maxSecRecX && B.x >= minSecRecX)
     || (B.x <= maxSecRecX && B.x >= minSecRecX && A.y <= maxSecRecY && A.y >= minSecRecY))
	     	return true;
    else
        return false;
}


bool segmentSegmentSuperimposed(t_segment firstSegment, t_segment secondSegment)
{
    // here two segments AB and CD (firstSegment == AB and secondSegment = CD)
    float lengthOfAB = distanceBetweenTwoDots(firstSegment.firstPoint, firstSegment.secondPoint);
    float lengthOfAC = distanceBetweenTwoDots(firstSegment.firstPoint, secondSegment.firstPoint);
    float lengthOfBC = distanceBetweenTwoDots(firstSegment.secondPoint, secondSegment.firstPoint);
    float lengthOfAD = distanceBetweenTwoDots(firstSegment.firstPoint, secondSegment.secondPoint);
    float lengthOfBD = distanceBetweenTwoDots(firstSegment.secondPoint, secondSegment.secondPoint);


    if ((lengthOfAB >= lengthOfAC && lengthOfAB >= lengthOfBC && lengthOfAC + lengthOfBC == lengthOfAB)
     || (lengthOfAB >= lengthOfAD && lengthOfAB >= lengthOfBD && lengthOfAD + lengthOfBD == lengthOfAB))
       return true;
    else
       return false; 

}