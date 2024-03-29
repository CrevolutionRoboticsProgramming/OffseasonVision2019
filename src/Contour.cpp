#include <iostream>
#include "Contour.hpp"

Contour::Contour()
{
}

Contour::Contour(std::vector<cv::Point> &pointsVector)
    : pointsVector{pointsVector}
{
}

bool Contour::isValid(double minArea, double maxArea, double minRotation, int error)
{
    // Approximates a closed polygon with error 3 around the contour and assigns it to newPoly
    cv::Mat newPoly;

    cv::approxPolyDP(cv::Mat(pointsVector), newPoly, error, true);

    // Saves the dimensions of the contour
    area = cv::contourArea(pointsVector);
    rotatedBoundingBox = cv::minAreaRect(newPoly);

    // If the area of the contour is less than the specified minimum area, delete it
    if (area < minArea || area > maxArea)
        return false;

    // Draws a rotated rectangle around the contour and assigns its points to points[]
    rotatedBoundingBox.points(rotatedBoundingBoxPoints);

    // Finds the indices of the highest and second-lowest points of the rotated rectangle
    int highestPoint{0}, secondLowestPoint{0};
    for (int i{0}; i < 4; ++i)
    {
        int pointsHigherThan{0}, pointsLowerThan{0};

        for (int o{0}; o < 4; ++o)
        {
            if (rotatedBoundingBoxPoints[i].y < rotatedBoundingBoxPoints[o].y)
                ++pointsHigherThan;
            if (rotatedBoundingBoxPoints[i].y > rotatedBoundingBoxPoints[o].y)
                ++pointsLowerThan;
        }

        if (pointsHigherThan == 3)
            highestPoint = i;
        else if (pointsLowerThan == 2)
            secondLowestPoint = i;
    }

    // Uses the highest and second-lowest points to calculate the rectangle's rotation
    // OpenCV provides a function for this, but it's not consistent with what edge is counted as the top and which indices count for which corners
    angle = std::atan((rotatedBoundingBoxPoints[secondLowestPoint].y - rotatedBoundingBoxPoints[highestPoint].y) / (rotatedBoundingBoxPoints[highestPoint].x - rotatedBoundingBoxPoints[secondLowestPoint].x)) * 180 / 3.1415926;

    // If the angle isn't steep enough, delete the contour
    if ((angle < 0 && angle > -minRotation) || (angle > 0 && angle < minRotation))
    {
	    //std::cout << "Bad angle: " << angle << "\n\n";
        return false;
    }

    // Saves a bounding box for the contour
    boundingBox = cv::boundingRect(newPoly);

    return true;
}