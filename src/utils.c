#include "utils.h"
#include <math.h>
#include <stdlib.h>
//general funcs

float DegreesToRads(float degrees) {
    return degrees * M_PI/180;
}
float radsToDegrees(float rad) {
    return rad * 180/M_PI;
}

float angleToPoint(float x1,float y1,float x2,float y2) {
    float angle = atan2f(x2 - x1, y2-y1);
    return radsToDegrees(angle);
}

float clampf(float val, float min, float max) {
    return fmaxf(min,fminf(val, max));
}

void normalize(float x, float y, float *dest_x, float *dest_y) {
    float magnitude = sqrtf(x * x + y * y);
    if (magnitude != 0) {
	*dest_x = x/magnitude;
	*dest_y = y/magnitude;
    } else {
	*dest_x = 0;
	*dest_y = 0;
    }
}

//rotate a point around 0
void rotatePoint(float x, float y, float rot, float *dest_x, float *dest_y) {
    *dest_x = x * cosf(DegreesToRads(rot)) - y * sinf(DegreesToRads(rot));
    *dest_y = y * cosf(DegreesToRads(rot)) + x * sinf(DegreesToRads(rot));
}

//distance between two points
float distf(float x1, float x2, float y1, float y2) {
    return sqrtf(powf(x2 - x1,2) + (powf(y2-y1,2)));
}

//returns a random float between 0 and 1
float randf() {
    return rand()/(float)RAND_MAX;
}
//collision detection 
int pointPoint(Vector2 p1, Vector2 p2) {
    if (p1.x == p2.x && p1.y == p2.y) {
	return 1;
    } return 0;
}

int pointCircle(Vector2 p, Circle c) {
    if (distf(c.x, p.x, c.y, p.y) <= c.r) {
	return 1;
    } return 0;
}

int circleCircle(Circle c1, Circle c2) {
    if (distf(c2.x, c1.x, c2.y, c1.y) <= c1.r+c2.r) {
	return 1;
    } return 0;
}

int pointRectangle(Vector2 p, Rectangle r) {
    if (p.x > r.x &&
	p.x < r.x + r.width &&
	p.y > r.y &&
	p.y < r.y + r.height) {
	return 1;
    } return 0;
}

int rectangleRectangle(Rectangle r1, Rectangle r2) {
    if (r1.x + r1.width > r2.x &&
	r1.x < r2.x +r2.width &&
	r1.y + r1.height > r2.y &&
	r1.y < r2.y + r2.height) {
	return 1;
    } return 0;
}

int circleRectangle(Circle c, Rectangle r) {
    float test_x = c.x;
    float test_y = c.y;

    if (c.x < r.x) {
	test_x = r.x;
    } else if (c.x > r.x + r.width) {
	test_x = r.x + r.width;
    }

    if (c.y < r.y) {
	test_y = r.y;
    } else if (c.y > r.y + r.height) {
	test_y = r.y + r.height;
    }

    if (distf(test_x, c.x, test_y, c.y) <= c.r) {
	return 1;
    } return 0;
}

// easing code
float easeInSine(float t) {
    return 1 - cosf((t * M_PI)/2);
}
float easeOutSine(float t) {
    return sinf((t * M_PI)/2);
}
float easeInOutSine(float t) {
    return -(cosf(M_PI * t) - 1)/2;
}

float easeInCubic(float t) {
    return t * t * t;
}

float easeOutCubic(float t) {
    return 1 - powf(1-t, 3);
}

float easeInOutCubic(float t) {
    if (t < 0.5) {
	return 4 * t * t * t;
    }
    else {
	return 1 - powf(-2 * t + 2, 3)/2;
    }
}

float easeInQuint(float t) {
    return t * t * t * t * t;
}

float easeOutQuint(float t) {
    return 1 - powf(1-t,5);
}

float easeInOutQuint(float t) {
    if (t < 0.5) {
	return 16 * t * t * t * t * t;
    } else {
	return 1 - powf(-2 * t + 2, 5) /2;
    }
}

float easeInCirc(float t) {
    return 1 - sqrtf(1 - powf(t,2));
}

float easeOutCirc(float t) {
    return sqrtf(1 - powf(t - 1, 2));
}

float easeInOutCirc(float t) {
    if (t < 0.5) {
	return (1 - sqrtf(1 - powf(2 * t, 2)))/2;
    } else {
	return (sqrtf(1 - powf(-2 * t + 2, 2)) + 1) /2;
    }
}

float easeInElastic(float t) {
    const float c4 = (2 * M_PI) / 3;
    if (t == 0) {
	return 0;
    } else if (t == 1) {
	return 1;
    }
    else {
	return -powf(2, 10 * t - 10) * sinf((t * 10 - 10.75) * c4);
    }
}

float easeOutElastic(float t) {
    const float c4 = (2 * M_PI)/2;
    if (t == 0) {
	return 0;
    } else if (t == 1){
	return 1;
    } else {
	return powf(2, -10 * t) * sinf((t * 10 - 0.75) * c4) + 1;
    }
}

float easeInOutElastic(float t) {
    const float c5 = (2*M_PI) /4.5;

    if (t ==  0) {
	return 0;
    } else if (t == 1) {
	return 1;
    } else if (t < 0.5) {
	return -(powf(2, 20 * t - -10) * sinf((20*t -11.125) * c5)) / 2;
    } else {
	return (powf(2, -20 * t + 10) * sinf((20*t-11.125) * c5)) /2+1;
    }
}

float easeInQuad(float t) {
    return t * t;
}

float easeOutQuad(float t) {
    return 1 - (1 - t) * (1 - t);
}

float easeInOutQuad(float t) {
    if (t < 0.5) {
	return 2 * t * t;
    } else {
	return 1 - powf(-2 * t + 2,2) /2;
    }
}

float easeInQuart(float t) {
    return t * t * t * t;
}

float easeOutQuart(float t) {
    return 1 - powf(1-t, 4);
}

float easeInOutQuart(float t) {
    if (t < 0.5) {
	return 8 * t * t * t * t;
    } else {
	return 1 - powf(-2 * t + 2,4) /2;
    }
}

float easeInExpo(float t) {
    if (t == 0) {
	return 0;
    } else {
	return powf(2, 10 * t - 10);
    }
}

float easeOutExpo(float t) {
    if (t == 1) {
	return 1;
    } else {
	return 1 - powf(2, -10 * t);
    }
}

float easeInOutExpo(float t) {
    if (t == 0) {
	return 0;
    } else if (t == 1) {
	return 1;
    } else if (t < 0.5){
	return powf(2, 20 * t - 10) /2;
    } else {
	return (2 - powf(2, -20 * t + 10)) / 2;
    }
}

float easeInBack(float t) {
    const float c1 = 1.70158;
    const float c3 = c1 + 1;

    return c3 * t * t * t - c1 * t * t;
}

float easeOutBack(float t) {
    const float c1 = 1.70158;
    const float c3 = c1 + 1;
    
    return 1 + c3 * powf(t - 1, 3) + c1 * powf(t - 1, 2);
}

float easeInOutBack(float t) {
    const float c1 = 1.70158;
    const float c2 = c1 * 1.525;

    if (t < 0.5) {
	return (powf(2 * t, 2) * ((c2 + 1) * 2 * t - c2)) /2;
    } else {
	return (powf(2 * t - 2, 2) *  ((c2 + 1) * (t * 2 - 2) +c2 ) + 2) /2;
    }
}

float easeOutBounce(float t) {
    const float n1 = 7.5625;
    const float d1 = 2.75;

    if (t < 1/d1) {
	return n1 * t * t;
    } else if (t < 2/d1) {
	float x = t - (1.5/d1);
	return n1 * x * x + 0.75;
    } else if (t < 2.5/d1) {
	float x = t - (2.25/d1);
	return n1 * x * x + 0.9375;
    } else {
	float x = t - (2.625/d1);
	return n1 * x * x + 0.984375;
    }
}

float easeInBounce(float t) {
    return 1 - easeOutBounce(1 - t);
}

float easeInOutBounce(float t) {
    if (t < 0.5) {
	return (1- easeOutBounce(1 - 2 * t)) / 2;
    } else {
	return (1 + easeOutBounce(2 * t -1)) / 2; 
    }
}
