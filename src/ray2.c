#include <assert.h>
#include <stdio.h>
#include "kazmath/ray2.h"

void kmRay2Fill(kmRay2* ray, kmScalar px, kmScalar py, kmScalar vx, kmScalar vy) {
    ray->start.x = px;
    ray->start.y = py;    
    ray->dir.x = vx;
    ray->dir.y = vy;
}

kmBool kmRay2IntersectLineSegment(const kmRay2* ray, const kmVec2 p1, const kmVec2 p2, kmVec2* intersection) {
    kmScalar A1, B1, C1;
    kmScalar A2, B2, C2;
    
    A1 = ray->dir.y;
    B1 = ray->dir.x;
    C1 = A1 * ray->start.x + B1 * ray->start.y;
    
    A2 = p2.y - p1.y;
    B2 = p2.x - p1.x;
    C2 = A2 * p1.x + B2 * p1.y;
    
    double det = (A1 * B2) - (A2 * B1);
    if(det == 0) {
        printf("Parallel\n");
        return KM_FALSE;
    }
    
    double x = (B2*C1 - B1*C2) / det;
    double y = (A1*C2 - A2*C1) / det;
    
    if(x < min(p1.x, p2.x) - kmEpsilon || x > max(p1.x, p2.x) + kmEpsilon ||
       y < min(p1.y, p2.y) - kmEpsilon || y > max(p1.y, p2.y) + kmEpsilon) {
        //Outside of line
        printf("Outside of line, %f %f (%f %f)(%f, %f)\n", x, y, p1.x, p1.y, p2.x, p2.y);
        return KM_FALSE;
    }
    
    kmScalar x1 = ray->start.x, x2 = ray->start.x + ray->dir.x;
    kmScalar y1 = ray->start.y, y2 = ray->start.y + ray->dir.y;
    
    if(x < min(x1, x2) - kmEpsilon || x > max(x1, x2) + kmEpsilon ||
       y < min(y1, y2) - kmEpsilon || y > max(y1, y2) + kmEpsilon) {
        printf("Outside of ray, %f %f (%f %f)(%f, %f)\n", x, y, x1, y1, x2, y2);
        return KM_FALSE;
    }
    
    intersection->x = x;
    intersection->y = y;
    
    return KM_TRUE;
}

void calculate_line_normal(kmVec2 p1, kmVec2 p2, kmVec2* normal_out) {
    kmVec2 tmp;
    kmVec2Subtract(&tmp, &p2, &p1); //Get direction vector
    
    normal_out->x = -tmp.y;
    normal_out->y = tmp.x;
    kmVec2Normalize(normal_out, normal_out);
    
    //TODO: should check that the normal is pointing out of the triangle
}

kmBool kmRay2IntersectTriangle(const kmRay2* ray, const kmVec2 p1, const kmVec2 p2, const kmVec2 p3, kmVec2* intersection, kmVec2* normal_out) {
    kmVec2 intersect;
    kmVec2 normal;
    
    kmBool intersected = KM_FALSE;
 
    if(kmRay2IntersectLineSegment(ray, p1, p2, &intersect)) {
        intersection->x = intersect.x;
        intersection->y = intersect.y;
        intersected = KM_TRUE;                        
        calculate_line_normal(p1, p2, &normal);               
    }
    
    if(kmRay2IntersectLineSegment(ray, p2, p3, &intersect)) {
        intersection->x = intersect.x;
        intersection->y = intersect.y;
        intersected = KM_TRUE;                    
        calculate_line_normal(p2, p3, &normal);        
    }
    
    if(kmRay2IntersectLineSegment(ray, p3, p1, &intersect)) {
        intersection->x = intersect.x;
        intersection->y = intersect.y;
        intersected = KM_TRUE;                    
        calculate_line_normal(p3, p1, &normal);        
    }
    
    if(intersected) {
        intersection->x = intersect.x;
        intersection->y = intersect.y;
        if(normal_out) {
            normal_out->x = normal.x;
            normal_out->y = normal.y;
        }
    }
    
    return intersected;
}

kmBool kmRay2IntersectCircle(const kmRay2* ray, const kmVec2 centre, const kmScalar radius, kmVec2* intersection) {
    assert(0 && "Not implemented");
}