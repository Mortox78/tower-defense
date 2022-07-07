#ifndef _COLLISIONS_H_
#define _COLLISIONS_H_

#include "vector.h"


bool lineLine(t_line firstLine, t_line secondLine);

bool lineSegment(t_line line, t_segment segment);

bool segmentSegment(t_segment firstSegment, t_segment secondSegment);

bool segmentSegmentColinear(t_segment firstSegment, t_segment secondSegment);

bool segmentSegmentOnTheSide(t_segment firstSegment, t_segment secondSegment);

bool dotDot(t_point firstDot, t_point secondDot);

bool dotLine(t_point dot, t_line line);

bool dotSegment(t_point dot, t_segment);

bool dotCircle(t_point dot, t_circle circle);

bool dotRectangle(t_point dot, t_rectangle rectangle);

//bool circleLine(t_circle circle, t_line line);

bool segmentSegmentSuperimposed(t_segment firstSegment, t_segment secondSegment);

bool rectangleRectangle(t_rectangle firstRectangle, t_rectangle secondRectangle);



#endif
