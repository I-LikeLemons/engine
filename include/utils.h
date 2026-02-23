#ifndef UTILS_H
#define UTILS_H

typedef struct {
    float x;
    float y;
} Vector2;

typedef struct {
    float x;
    float y;
    float r;
}Circle;

typedef struct {
    float x;
    float y;
    float width;
    float height;
} Rectangle;

float radsToDegrees(float rad);
float DegreesToRads(float degrees);
float angleToPoint(float x1, float y1, float x2, float y2);
void normalize(float x, float y, float* dest_x, float* dest_y);
void rotatePoint(float x, float y,float rot, float *dest_x, float *dest_y);
float distf(float x1,float x2,float y1,float y2);
float clampf(float val, float min, float max);
float randf();
//collision detection code
int pointPoint(Vector2 p1, Vector2 p2);
int pointCircle(Vector2 p, Circle c);
int circleCircle(Circle c1, Circle c2);
int pointRectangle(Vector2 p, Rectangle r);
int rectangleRectangle(Rectangle r1, Rectangle r2); 
int circleRectangle(Circle c, Rectangle r);
//easings
float easeInSine(float t);
float easeOutSine(float t);
float easeInOutSine(float t);

float easeInCubic(float t);
float easeOutCubic(float t);
float easeInOutCubic(float t);

float easeInQuint(float t);
float easeOutQuint(float t);
float easeInOutQuint(float t);

float easeInCirc(float t);
float easeOutCirc(float t);
float easeInOutCirc(float t);

float easeInElastic(float t);
float easeOutElastic(float t);
float easeInOutElastic(float t);

float easeInQuad(float t);
float easeOutQuad(float t);
float easeInOutQuad(float t);

float easeInQuart(float t);
float easeOutQuart(float t);
float easeInOutQuart(float t);

float easeInExpo(float t);
float easeOutExpo(float t);
float easeInOutExpo(float t);

float easeInBack(float t);
float easeOutBack(float t);
float easeInOutBack(float t);

float easeInBounce(float t);
float easeOutBounce(float t);
float easeInOutBounce(float t);

#endif // !UTILS_H
