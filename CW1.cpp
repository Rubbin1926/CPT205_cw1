#define FREEGLUT_STATIC
#define M_PI 3.14159265359
#include <GL/freeglut.h>
#include <iostream>
#include <vector>


int nowPage = 0;             // Current page number

float sunRotation = 0.0f;
float rayRotation = 0.0f;

bool leftArrowClicked = false;  // Left arrow clicked state
bool rightArrowClicked = false; // Right arrow clicked state

bool sunHovered = false;    // Sun hover state
bool sunClicked = false;    // Sun click state

bool leftButtonHovered = false, rightButtonHovered = false; // Control left and right triangle hover states


// Set Color with Alpha
void setColor(float r, float g, float b, float a = 1.0f) {
    glColor4f(r, g, b, a);
}

void drawRectangle(float x1, float y1, float x2, float y2) {
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

void drawQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glVertex2f(x4, y4);
    glEnd();
}

void drawCloudArc(float centerX, float centerY, float radius, int numPoints) {
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0); // Set color to white (RGB: 1, 1, 1)
    for (int i = 0; i < numPoints; i++) {
        float angle = 2.0 * M_PI * i / numPoints;
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void drawCloud(float centerX, float centerY, float size) {
    // Draw multiple arc shapes to form a cloud
    float cloudRadius = size * 0.3f;
    int numPoints = 42; // Number of points to approximate the arc

    drawCloudArc(centerX - size * 0.5f, centerY, cloudRadius, numPoints);
    drawCloudArc(centerX, centerY + size * 0.4f, cloudRadius, numPoints);
    drawCloudArc(centerX, centerY, cloudRadius, numPoints);
    drawCloudArc(centerX + size * 0.5f, centerY, cloudRadius, numPoints);
    drawCloudArc(centerX - size * 0.25f, centerY + size * 0.25f, cloudRadius, numPoints);
    drawCloudArc(centerX + size * 0.25f, centerY + size * 0.25f, cloudRadius, numPoints);

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(centerX - size * 0.5f, centerY - cloudRadius);
    glVertex2f(centerX + size * 0.5f, centerY - cloudRadius);
    glVertex2f(centerX, centerY);
    glEnd();
}

void drawHouse(float centerX, float centerY, float size) {
    // Calculate house dimensions based on the specified size
    float roofHeight = size * 1.0f;
    float roofWidth = size * 0.7f;
    float houseWidth = size;
    float houseHeight = size * 0.6f;

    // Draw house body
    glColor3f(0.7f, 0.5f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(centerX - houseWidth / 2, centerY - houseHeight / 2);
    glVertex2f(centerX + houseWidth / 2, centerY - houseHeight / 2);
    glVertex2f(centerX + houseWidth / 2, centerY + houseHeight / 2);
    glVertex2f(centerX - houseWidth / 2, centerY + houseHeight / 2);
    glEnd();

    // Draw chimney
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    float chimneyWidth = size * 0.1f;
    float chimneyHeight = size * 0.25f;
    glVertex2f(centerX + houseWidth / 3, centerY + houseHeight / 2 + chimneyHeight);
    glVertex2f(centerX + houseWidth / 3 + chimneyWidth, centerY + houseHeight / 2 + chimneyHeight);
    glVertex2f(centerX + houseWidth / 3 + chimneyWidth, centerY + houseHeight / 2 + chimneyHeight + chimneyHeight);
    glVertex2f(centerX + houseWidth / 3, centerY + houseHeight / 2 + chimneyHeight + chimneyHeight);
    glEnd();

    // Draw house roof
    glColor3f(0.5f, 0.3f, 0.2f);
    glBegin(GL_TRIANGLES);
    glVertex2f(centerX - roofWidth, centerY + houseHeight / 2);
    glVertex2f(centerX + roofWidth, centerY + houseHeight / 2);
    glVertex2f(centerX, centerY + roofHeight);
    glEnd();

    // Draw door
    glColor3f(0.3f, 0.2f, 0.1f);
    glBegin(GL_QUADS);
    float doorWidth = size * 0.2f;
    float doorHeight = size * 0.4f;
    glVertex2f(centerX - doorWidth / 2, centerY - houseHeight / 2);
    glVertex2f(centerX + doorWidth / 2, centerY - houseHeight / 2);
    glVertex2f(centerX + doorWidth / 2, centerY - houseHeight / 2 + doorHeight);
    glVertex2f(centerX - doorWidth / 2, centerY - houseHeight / 2 + doorHeight);
    glEnd();

    // Draw window
    glColor3f(0.5f, 0.7f, 0.9f); // Blue color for window
    glBegin(GL_QUADS);
    float windowSize = size * 0.2f;
    // Draw window square
    glVertex2f(centerX + houseWidth / 4, centerY);
    glVertex2f(centerX + houseWidth / 4 + windowSize, centerY);
    glVertex2f(centerX + houseWidth / 4 + windowSize, centerY + windowSize);
    glVertex2f(centerX + houseWidth / 4, centerY + windowSize);
    glEnd();

    // Draw window cross
    glColor3f(1.0f, 1.0f, 1.0f); // White color for cross
    glLineWidth(1.0f); // Set line width for cross
    glBegin(GL_LINES);
    // Horizontal line for cross
    glVertex2f(centerX + houseWidth / 4, centerY + windowSize / 2);
    glVertex2f(centerX + houseWidth / 4 + windowSize, centerY + windowSize / 2);
    // Vertical line for cross
    glVertex2f(centerX + houseWidth / 4 + windowSize / 2, centerY);
    glVertex2f(centerX + houseWidth / 4 + windowSize / 2, centerY + windowSize);
    glEnd();
}

struct Balloon {
    float x, y; // position
    float vx, vy; // velocity
    float r, g, b;
};

const int NUM_BALLOONS = 27;
Balloon balloons[NUM_BALLOONS];
bool ifUpdateBalloons = false;

float randomFloatZeroToOne() { return static_cast<float>(rand()) / RAND_MAX; }
float randomFloatNegOneToOne() { return static_cast<float>(rand()) / (RAND_MAX / 2) - 1.0f; }

void generateRandomColor(float& r, float& g, float& b) {
    r = static_cast<float>(rand()) / RAND_MAX; // Random value between 0 and 1 for red component
    g = static_cast<float>(rand()) / RAND_MAX; // Random value between 0 and 1 for green component
    b = static_cast<float>(rand()) / RAND_MAX; // Random value between 0 and 1 for blue component
}

void initBalloons() {
    for (int i = 0; i < NUM_BALLOONS; i++) {
        balloons[i].x = randomFloatNegOneToOne();
        balloons[i].y = randomFloatNegOneToOne() - 0.5f;
        balloons[i].vx = randomFloatNegOneToOne() / 200.0f;
        balloons[i].vy = randomFloatZeroToOne() / 70.0f;
        generateRandomColor(balloons[i].r, balloons[i].g, balloons[i].b);
    }
}

void updateBalloons() {
    for (int i = 0; i < NUM_BALLOONS; i++) {
        balloons[i].x += balloons[i].vx;
        balloons[i].y += balloons[i].vy;
    }
}

void drawBalloons() {
    for (int i = 0; i < NUM_BALLOONS; i++) {
        glColor3f(balloons[i].r, balloons[i].g, balloons[i].b); // Random bright color for the balloon
        glBegin(GL_TRIANGLE_FAN);
        float centerX = balloons[i].x;
        float centerY = balloons[i].y;
        float width = 0.05f;
        float height = 0.075f;
        int numSegments = 100;
        for (int j = 0; j <= numSegments; j++) {
            float angle = 2.0f * M_PI * static_cast<float>(j) / static_cast<float>(numSegments);
            float x = centerX + (width * 0.5f * cos(angle));
            float y = centerY + (height * 0.5f * sin(angle));
            glVertex2f(x, y);
        }
        glEnd();

        // Draw rope
        glColor3f(0.0, 0.0, 0.0); // Black rope
        glBegin(GL_LINE_STRIP);
        glVertex2f(centerX, centerY - height * 0.5f); // Start at the bottom of the balloon

        // First bend
        float bendX = centerX - width * 0.1f;
        float bendY = centerY - height * 0.7f;
        glVertex2f(bendX, bendY);

        // Second bend
        bendX = centerX + width * 0.1f;
        glVertex2f(bendX, bendY);

        // End point
        glVertex2f(centerX, centerY - height * 1.2f); // End below the balloon
        glEnd();
    }
}

void cardBackground(bool isLeft, bool isCover) {

    float backgrondColorR = 1.0f;  // Soft light yellow color
    float backgrondColorG = 0.75f;
    float backgrondColorB = 0.79f;

    float backgrondCoverColorR = 0.8f;
    float backgrondCoverColorG = 0.9f;
    float backgrondCoverColorB = 0.8f;

    if (isCover) {
        setColor(backgrondCoverColorR, backgrondCoverColorG, backgrondCoverColorB);
    }
    else {
        setColor(backgrondColorR, backgrondColorG, backgrondColorB);
    }

    float offset = isCover ? 0.05f : 0.0f;
    float startX = isLeft ? -0.8f + offset : 0.0f;
    float endX = isLeft ? 0.0f : 0.8f - offset;

    float startY = -0.65f + offset;
    float endY = 0.65f - offset;

    drawQuad(startX, startY, endX, startY, endX, endY, startX, endY);
}

// Draw crease
void drawCrease() {
    setColor(0.7f, 0.7f, 0.7f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(0.0f, -0.65f);
    glVertex2f(0.0f, 0.65f);
    glEnd();
}

// draw ellipse
void drawEllipse(float centerX, float centerY, float radiusX, float radiusY, float r, float g, float b) {
    setColor(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX, centerY);
    for (float angle = 0; angle <= 2 * M_PI; angle += 0.005f) {
        float x = centerX - radiusX * cos(angle);
        float y = centerY - radiusY * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

// draw petal
void drawPetal(float centerX, float centerY, float radiusX, float radiusY, float rotationAngle, float r, float g, float b) {
    setColor(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX, centerY);
    for (float angle = 0; angle <= 2 * M_PI; angle += 0.005f) {
        float x = radiusX * cos(angle);
        float y = radiusY * sin(angle);
        float rotatedX = centerX + x * cos(rotationAngle) - y * sin(rotationAngle);
        float rotatedY = centerY + x * sin(rotationAngle) + y * cos(rotationAngle);
        glVertex2f(rotatedX, rotatedY);
    }
    glEnd();
}

// Draw a four-leaf clover
void drawClover(float centerX, float centerY, float radiusX, float radiusY) {
    for (int i = 0; i < 4; ++i) { // Change the loop iteration to 4
        float angle = i * (M_PI / 2); // Adjust the angle of each leaf
        float leafX = centerX + 0.05f * cos(angle);
        float leafY = centerY + 0.05f * sin(angle);
        drawPetal(leafX, leafY, radiusX, radiusY, angle, 0.3f, 0.7f, 0.3f);
    }
}

// Draw decorations
void drawDecorator(float centerX, float centerY, bool withOtherDecorator = true, bool candle = true) {
    if (candle == true) {
        drawEllipse(centerX + 0.2875f, centerY + 0.14f, 0.03f, 0.09f, 0.8f, 0.0f, 0.0f); // Red flame
        drawEllipse(centerX + 0.2875f, centerY + 0.086f, 0.017f, 0.046f, 0.9f, 0.9f, 0.2f); // Yellow flame

        // Candle body
        setColor(1.0f, 0.6f, 0.2f);
        drawQuad(centerX + 0.26f, centerY - 0.2f, centerX + 0.315f, centerY - 0.2f, centerX + 0.315f, centerY + 0.04f, centerX + 0.26f, centerY + 0.04f);
    }

    if (withOtherDecorator == true) {
        // Green leaves
        drawClover(centerX + 0.39f, centerY - 0.2f, 0.045f, 0.0145f);
        drawClover(centerX + 0.185f, centerY - 0.2f, 0.045f, 0.0145f);

        // Draw 7 petals and modify colors
        for (int i = 0; i < 12; i++) {
            float angle = i * (2 * M_PI / 12); // Change the number of petals to 7
            float petalX = centerX + 0.2875f + 0.0275f * cos(angle);
            float petalY = centerY - 0.2f + 0.0275f * sin(angle);
            drawPetal(petalX, petalY, 0.05f, 0.01f, angle, 0.8f, 0.4f, 0.6f);
        }

        // Flower center
        drawEllipse(centerX + 0.2875f, centerY - 0.2f, 0.0275f, 0.0275f, 0.9f, 0.6f, 0.2f); // Modify the flower center color
    }
}

void drawCandleOnCake(float centerX, float centerY) {
    setColor(0.8f, 0.5f, 0.3f);
    drawQuad(centerX - 0.005f, centerY - 0.1f, centerX + 0.005f, centerY - 0.1f,
        centerX + 0.005f, centerY + 0.05f, centerX - 0.005f, centerY + 0.05f);
}

void drawCake(float bottomY) {
    drawEllipse(0.0f, -0.2f + bottomY, 0.3f, 0.1f, 0.6f, 0.3f, 0.0f);

    float color_factor = 0.8f; 
    drawEllipse(0.0f, -0.2f + bottomY, 0.2f, 0.067f, 1.0f * color_factor, 0.7f * color_factor, 0.8f * color_factor);
    drawEllipse(0.0f, 0.0f + bottomY, 0.2f, 0.067f, 1.0f * color_factor, 0.7f * color_factor, 0.8f * color_factor);
    drawRectangle(0.2f, 0.0f + bottomY, -0.2f, -0.2f + bottomY);

    color_factor = 0.9f;
    drawEllipse(0.0f, 0.0f + bottomY, 0.15f, 0.05f, 1.0f * color_factor, 0.7f * color_factor, 0.8f * color_factor);
    drawEllipse(0.0f, 0.2f + bottomY, 0.15f, 0.05f, 1.0f * color_factor, 0.7f * color_factor, 0.8f * color_factor);
    drawRectangle(0.15f, 0.2f + bottomY, -0.15f, 0.0f + bottomY);

    color_factor = 1.0f;
    drawEllipse(0.0f, 0.2f + bottomY, 0.1125f, 0.0375f, 1.0f * color_factor, 0.7f * color_factor, 0.8f * color_factor);
    drawEllipse(0.0f, 0.4f + bottomY, 0.1125f, 0.0375f, 1.0f * color_factor, 0.7f * color_factor, 0.8f * color_factor);
    drawRectangle(0.1125f, 0.4f + bottomY, -0.1125f, 0.2f + bottomY);

    drawCandleOnCake(0.0f, 0.3f);
    drawCandleOnCake(-0.03f, 0.3f);
    drawCandleOnCake(0.03f, 0.3f);
}

// Draw text
void drawText(const std::string& text, float x, float y, float r, float g, float b) {
    setColor(r, g, b);
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void drawSun(bool enlarge) {
    float scale = enlarge ? 1.2f : 1.0f;
    // Draw rotating sun with rays
    glPushMatrix();
    glTranslatef(-0.8f, 0.8f, 0.0f);
    glScalef(scale, scale, 1.0f); // Scale both the sun and the halo

    // Draw sun
    glColor3f(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.1f, 20, 20);

    // Draw sun rays
    for (int i = 0; i < 8; i++) {
        glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
        glPushMatrix();
        glTranslatef(0.1f, 0.0f, 0.0f);
        glRotatef(rayRotation, 0.0f, 0.0f, 1.0f);
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_TRIANGLES);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(0.05f, 0.01f);
        glVertex2f(0.05f, -0.01f);
        glEnd();
        glPopMatrix();
    }

    glPopMatrix();
}

// Draw a three-color isosceles triangle
void drawRotatedTriangle(float centerX, float centerY, float baseWidth, float height, float angle, bool hovered) {
    glPushMatrix();
    glTranslatef(centerX, centerY, 0.0f);  // Translate to the center of the triangle
    glRotatef(angle, 0.0f, 0.0f, 1.0f);   // Rotate by the specified angle
    float scale = hovered ? 1.2f : 1.0f; // Scale up when hovered

    // First color part (bottom)
    setColor(1.0f, 0.8f, 0.5f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-baseWidth / 2 * scale, 0.0f);
    glVertex2f(0.0f, height * scale);
    glVertex2f(baseWidth / 2 * scale, 0.0f);
    glEnd();

    // Second color part (middle)
    setColor(1.0f, 0.7f, 0.4f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-baseWidth / 2 * 2 / 3 * scale, 0.0f);
    glVertex2f(0.0f, height * scale);
    glVertex2f(baseWidth / 2 * 2 / 3 * scale, 0.0f);
    glEnd();

    // Third color part (top)
    setColor(1.0f, 0.6f, 0.3f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-baseWidth / 2 / 3 * scale, 0.0f);
    glVertex2f(0.0f, height * scale);
    glVertex2f(baseWidth / 2 / 3 * scale, 0.0f);
    glEnd();

    glPopMatrix();
}

struct Firework {
    float x, y;          // Stripe position
    float speed;         // Ascending speed
    float color[3];      // Stripe color
    float angle;         // Angle of ascent for the stripe
    float alpha;         // Stripe opacity
};

std::vector<Firework> stripes; // Store the stripes

// Draw a stripe
void drawStripe(Firework& stripe) {
    glColor4f(stripe.color[0], stripe.color[1], stripe.color[2], stripe.alpha); // Using transparency
    glBegin(GL_LINES);
    glVertex2f(stripe.x, stripe.y);
    glVertex2f(stripe.x + 0.2f * cos(stripe.angle), stripe.y + 0.1f); // Using an angle to offset the stripe
    glEnd();
}

// Create stripes
void createStripes(float startX, float startY, bool isLeft) {
    stripes.clear();
    int numStripes = rand() % 10 + 5; // Generate 5 to 15 random stripes
    for (int i = 0; i < numStripes; ++i) {
        Firework stripe;
        stripe.x = startX + static_cast<float>(rand() % 20 - 10) / 100.0f; // Random x offset
        stripe.y = startY;
        stripe.speed = static_cast<float>(rand() % 3 + 1) / 1000.0f; // Random speed
        stripe.angle = (isLeft ? static_cast<float>(rand() % 30) - 60.0f : -static_cast<float>(rand() % 30)) * M_PI / 180.0f + 90.0f; // 0 to 30 degrees on the left, -30 to 0 degrees on the right
        stripe.alpha = 1.0f; // Initial alpha value is 1.0f
        generateRandomColor(stripe.color[0], stripe.color[1], stripe.color[2]);
        stripes.push_back(stripe);
    }
}

// Update the position of the stripes
void updateStripes() {
    auto it = stripes.begin();
    while (it != stripes.end()) {
        it->x += it->speed * cos(it->angle); // Move the stripe diagonally
        it->y += it->speed * sin(it->angle); // Ascend
        it->alpha -= 0.005f; // Decrease alpha value each time

        // Remove the stripe when alpha is below the threshold
        if (it->alpha <= 0.0f) {
            it = stripes.erase(it); // Erase the stripe
        }
        else {
            ++it;
        }
    }
    glutPostRedisplay();
}

void drawTree(float x) {
    float y = 0.15; // Set y position for tree on the grass

    // Brown tree trunk
    glColor3f(0.5f, 0.3f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(x - 0.025f, y - 0.15f);
    glVertex2f(x - 0.025f, y);
    glVertex2f(x + 0.025f, y);
    glVertex2f(x + 0.025f, y - 0.15f);
    glEnd();

    // Green tree top
    glColor3f(0.0f, 0.6f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y + 0.1f);
    glVertex2f(x - 0.1f, y);
    glVertex2f(x + 0.1f, y);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(x, y + 0.05f);
    glVertex2f(x - 0.05f, y + 0.05f);
    glVertex2f(x - 0.05f, y + 0.1f);
    glVertex2f(x, y + 0.1f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(x, y + 0.05f);
    glVertex2f(x + 0.05f, y + 0.05f);
    glVertex2f(x + 0.05f, y + 0.1f);
    glVertex2f(x, y + 0.15f);
    glEnd();
}

void drawGrassAndTrees() {
    // Draw green grass
    glColor3f(0.2f, 0.8f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(1.0f, -1.0f);
    glEnd();

    // Draw trees evenly distributed on the grass

    int numTrees = 5;
    float grassWidth = 2.0f;
    float treeSpacing = grassWidth / (numTrees + 1);

    for (int i = 1; i <= numTrees; i++) {
        float x = -1.0f + i * treeSpacing; // Calculate x position for tree
        drawTree(x);
    }
}

// Initialize the window
void initWindow() {
    glClearColor(0.0f, 0.6f, 0.9f, 1.0f); // White background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f); // Set up the coordinate system
    glMatrixMode(GL_MODELVIEW);
}

enum ArrowState { NONE, LEFT, RIGHT };
ArrowState hoveredArrow = NONE;  // Current hovered arrow state

// Draw page turning arrows with mouse hover visual feedback
void drawArrows(bool drawLeftArrow, bool drawRightArrow) {
    glPushMatrix();
    if (drawLeftArrow) {
        if (hoveredArrow == LEFT) {
            glTranslatef(-0.875f, -0.8f, 0.0f);
            glScalef(1.2f, 1.2f, 1.0f);
            glTranslatef(0.875f, 0.8f, 0.0f);
            setColor(1.0f, 0.0f, 0.0f); // Red color
        }
        else {
            setColor(0.2f, 0.2f, 0.2f); // Dark gray color
        }
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.9f, -0.8f);
        glVertex2f(-0.85f, -0.75f);
        glVertex2f(-0.85f, -0.85f);
        glEnd();
    }
    if (drawRightArrow) {
        if (hoveredArrow == RIGHT) {
            glTranslatef(0.875f, -0.8f, 0.0f);
            glScalef(1.2f, 1.2f, 1.0f);
            glTranslatef(-0.875f, 0.8f, 0.0f);
            setColor(1.0f, 0.0f, 0.0f); // Red color
        }
        else {
            setColor(0.2f, 0.2f, 0.2f); // Dark gray color
        }
        glBegin(GL_TRIANGLES);
        glVertex2f(0.9f, -0.8f);
        glVertex2f(0.85f, -0.75f);
        glVertex2f(0.85f, -0.85f);
        glEnd();
    }
    glPopMatrix();
}

// Mouse move event handler
void mouseMove(int x, int y) {
    int win_width = glutGet(GLUT_WINDOW_WIDTH);
    int win_height = glutGet(GLUT_WINDOW_HEIGHT);
    float normalizedX = (float)(x - win_width / 2) / (win_width / 2);
    float normalizedY = (float)(win_height / 2 - y) / (win_height / 2);

    // Check if the mouse is hovering over the left triangle
    leftButtonHovered = (normalizedX >= -0.46f && normalizedX <= -0.34f && normalizedY >= -0.3f && normalizedY <= -0.1f);
    // Check if the mouse is hovering over the right triangle
    rightButtonHovered = (normalizedX >= 0.34f && normalizedX <= 0.46f && normalizedY >= -0.3f && normalizedY <= -0.1f);

    // Check arrow hover state
    if (normalizedX >= -0.92f && normalizedX <= -0.83f && normalizedY >= -0.87f && normalizedY <= -0.73f) {
        hoveredArrow = LEFT;
    }
    else if (normalizedX >= 0.83f && normalizedX <= 0.92f && normalizedY >= -0.87f && normalizedY <= -0.73f) {
        hoveredArrow = RIGHT;
    }
    else {
        hoveredArrow = NONE;
    }

    // Check light bulb hover state
    float sunCenterX = -0.8f;
    float sunCenterY = 0.8f;
    float sunRadius = 0.1f;
    float distance = sqrt(pow(normalizedX - sunCenterX, 2) + pow(normalizedY - sunCenterY, 2));
    sunHovered = (distance <= sunRadius);

    glutPostRedisplay();
}

// Mouse click event handler
void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int win_width = glutGet(GLUT_WINDOW_WIDTH);
        int win_height = glutGet(GLUT_WINDOW_HEIGHT);
        float normalizedX = static_cast<float>(x - win_width / 2) / (win_width / 2);
        float normalizedY = static_cast<float>(win_height / 2 - y) / (win_height / 2);

        if (normalizedX >= -0.92f && normalizedX <= -0.83f && normalizedY >= -0.87f && normalizedY <= -0.73f && nowPage > 0) {
            nowPage--;
        }
        else if (normalizedX >= 0.83f && normalizedX <= 0.92f && normalizedY >= -0.87f && normalizedY <= -0.73f && nowPage < 2) {
            nowPage++;
        }

        if (leftButtonHovered) {
            createStripes(-0.4f, -0.1f, true);
        }
        if (rightButtonHovered) {
            createStripes(0.4f, -0.1f, false);
        }

        float sunCenterX = -0.8f;
        float sunCenterY = 0.8f;
        float sunRadius = 0.1f;
        float distance = sqrt(pow(normalizedX - sunCenterX, 2) + pow(normalizedY - sunCenterY, 2));
        if (distance <= sunRadius) {
            sunClicked = true;
            ifUpdateBalloons = true;
        }

        glutPostRedisplay();
    }
}

void drawClouds() {
    drawCloud(-0.6, 0.5, 0.2);
    drawCloud(0.0, 0.5, 0.2);
    drawCloud(0.6, 0.5, 0.2);
}

void Allbackground() {
    drawGrassAndTrees();
    drawClouds();
    drawHouse(-0.5, -0.5, 0.4);
    drawText("Press Q for quit.", -0.65f, -0.75f, 0.3f, 0.3f, 0.3f);
}

// Render the scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(0.0f, 0.0f, 1.0f, 0.0f);

    switch (nowPage) {
    case 0:
        Allbackground();
        cardBackground(false, false);
        drawDecorator(0.115f, 0.05f, true, false);
        drawText("HAPPY BIRTHDAY", 0.214f, 0.2f, 1.0f, 1.0f, 1.0f);
        drawSun(false);

        drawArrows(false, true);
        break;
    case 1:
        Allbackground();
        cardBackground(true, false);
        cardBackground(false, false);
        cardBackground(true, true);
        cardBackground(false, true);

        drawCake(-0.2);
        drawText("Happy BirthDay!!!", -0.14f, 0.4f, 0.3f, 0.3f, 0.3f);

        drawCrease();

        drawRotatedTriangle(-0.4f, -0.2f, 0.12f, 0.2f, -220.0f, leftButtonHovered);
        drawRotatedTriangle(0.4f, -0.2f, 0.12f, 0.2f, 220.0f, rightButtonHovered);

        for (auto& stripe : stripes) {
            drawStripe(stripe);
        }

        drawText("<- Click the Sun", -0.65f, 0.75f, 0.3f, 0.3f, 0.3f);

        drawText("<- Click", 0.5f, -0.3f, 0.3f, 0.3f, 0.3f);

        drawSun(sunHovered);
        drawBalloons();

        drawArrows(true, true);
        break;
    case 2:
        Allbackground();
        cardBackground(true, false);
        drawText("By Zichen Qiu", -0.6f, 0.0f, 1.0f, 1.0f, 1.0f);
        drawText("2252705", -0.6f, -0.1f, 1.0f, 1.0f, 1.0f);
        drawSun(false);

        drawArrows(true, false);
        break;
    }

    glPopMatrix();
    glutSwapBuffers();
}

void update(int value) {
    sunRotation += 1.0; // Increment sun rotation angle
    if (sunRotation > 360) {
        sunRotation -= 360;
    }

    rayRotation += 2.0; // Increment ray rotation angle
    if (rayRotation > 360) {
        rayRotation -= 360;
    }

    if (ifUpdateBalloons == true) {
        updateBalloons(); // Update balloons position
    }

    glutPostRedisplay(); // Call display function
    glutTimerFunc(16, update, 0); // Set the next update
}

// Keyboard event handler
void keyPress(unsigned char key, int x, int y) {
    if (key == 'q' || key == 'Q') { // 27 is the ASCII code for the Esc key
        exit(0);
    }
}

int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(nullptr))); // Set random seed

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 599);
    glutCreateWindow("Birthday Card");

    initWindow();
    initBalloons();
    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);
    glutPassiveMotionFunc(mouseMove);
    glutIdleFunc(updateStripes);
    glutKeyboardFunc(keyPress);
    glutTimerFunc(25, update, 0); // Start the update loop

    glutMainLoop();
    return 0;
}