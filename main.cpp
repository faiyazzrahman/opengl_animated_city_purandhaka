#include <GL/glut.h>
#include <cmath>

// Camera parameters
float camX = 0.0f, camY = 1.0f, camZ = 5.0f;
float yaw = 0.0f, pitch = 0.0f;
float speed = 0.01f;
int lastMouseX = -1, lastMouseY = -1;
int winWidth = 800, winHeight = 600;

// Movement flags
bool moveForward = false, moveBackward = false, moveLeft = false, moveRight = false;

// Mouse look
void mouseMotion(int x, int y) {
    float sensitivity = 0.05f;

    int centerX = winWidth / 2;
    int centerY = winHeight / 2;

    if (lastMouseX == -1 && lastMouseY == -1) {
        lastMouseX = centerX;
        lastMouseY = centerY;
        glutWarpPointer(centerX, centerY);
        return;
    }

    int dx = x - lastMouseX;
    int dy = y - lastMouseY;

    yaw += dx * sensitivity;
    pitch -= dy * sensitivity;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // reset cursor to center
    glutWarpPointer(centerX, centerY);
    lastMouseX = centerX;
    lastMouseY = centerY;
}

// Keyboard input
void keyDown(unsigned char key, int x, int y) {
    if(key == 27) exit(0);
    if(key == 'w') moveForward = true;
    if(key == 's') moveBackward = true;
    if(key == 'a') moveLeft = true;
    if(key == 'd') moveRight = true;
}

void keyUp(unsigned char key, int x, int y) {
    if(key == 'w') moveForward = false;
    if(key == 's') moveBackward = false;
    if(key == 'a') moveLeft = false;
    if(key == 'd') moveRight = false;
}

// Update camera position
void updateCamera() {
    float radYaw = yaw * 3.1415926f / 180.0f;

    // XZ plane movement
    float moveDirX = sin(radYaw);
    float moveDirZ = -cos(radYaw);
    float rightX = sin(radYaw - 3.1415926f/2.0f);
    float rightZ = -cos(radYaw - 3.1415926f/2.0f);

    if(moveForward){ camX += moveDirX * speed; camZ += moveDirZ * speed; }
    if(moveBackward){ camX -= moveDirX * speed; camZ -= moveDirZ * speed; }
    if(moveLeft){ camX += rightX * speed; camZ += rightZ * speed; }
    if(moveRight){ camX -= rightX * speed; camZ -= rightZ * speed; }

    camY = 50.0f; // stick to ground
}
//Draw skyyyyy
void drawSkyCube(float size) {
    glPushMatrix();
    glLoadIdentity();          // ignore camera rotation
    glTranslatef(camX, camY, camZ);
    glDisable(GL_DEPTH_TEST);

    glBegin(GL_QUADS);

    // +Y Top (sky)
    glColor3f(0.5f,0.7f,1.0f);
    glVertex3f(-size,size,-size);
    glVertex3f(size,size,-size);
    glVertex3f(size,size,size);
    glVertex3f(-size,size,size);

    // -Y Bottom (horizon)
    glColor3f(0.5f,0.9f,1.0f);
    glVertex3f(-size,-size,-size);
    glVertex3f(size,-size,-size);
    glVertex3f(size,-size,size);
    glVertex3f(-size,-size,size);

    // +X Right
    glColor3f(0.5f,0.8f,1.0f);
    glVertex3f(size,-size,-size);
    glVertex3f(size,size,-size);
    glVertex3f(size,size,size);
    glVertex3f(size,-size,size);

    // -X Left
    glColor3f(0.5f,0.8f,1.0f);
    glVertex3f(-size,-size,-size);
    glVertex3f(-size,size,-size);
    glVertex3f(-size,size,size);
    glVertex3f(-size,-size,size);

    // +Z Front
    glColor3f(0.5f,0.8f,1.0f);
    glVertex3f(-size,-size,size);
    glVertex3f(size,-size,size);
    glVertex3f(size,size,size);
    glVertex3f(-size,size,size);

    // -Z Back
    glColor3f(0.5f,0.8f,1.0f);
    glVertex3f(-size,-size,-size);
    glVertex3f(size,-size,-size);
    glVertex3f(size,size,-size);
    glVertex3f(-size,size,-size);

    glEnd();

    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
}

void drawTree(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);

    // Trunk (along Y)
    glColor3f(0.55f, 0.27f, 0.07f);
    glPushMatrix();
        glRotatef(-90, 1, 0, 0); // point along Y
        GLUquadric* trunk = gluNewQuadric();
        gluCylinder(trunk, 0.2f, 0.1f, 1.5f, 16, 4);
        gluDeleteQuadric(trunk);
    glPopMatrix();

    // Leaves (stacked cones along Y)
    glColor3f(0.0f, 0.6f, 0.0f);
    float coneHeight = 1.0f;
    float coneRadius = 0.7f;

    for (int i = 0; i < 3; i++) {
        glPushMatrix();
            glTranslatef(0, 1.5f + i * 0.6f, 0); // above trunk
            glRotatef(-90, 1, 0, 0);             // cone points up
            glutSolidCone(coneRadius, coneHeight, 16, 16);
        glPopMatrix();
        coneRadius *= 0.8f;
    }

    glPopMatrix();
}

void drawBuilding(float x, float y, float z,
                  float width, float height, float depth,
                  float rotationY,float r, float g, float b)   // rotation in degrees
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotationY, 0, 1, 0);

    // Base color (brick)
    glColor3f(r,g,b);

    // Building main block
    glPushMatrix();
        glTranslatef(0, height / 2.0f, 0);
        glScalef(width, height, depth);
        glutSolidCube(1.0f);
    glPopMatrix();

    // Windows
    int floors = int(height);
    int windowsPerFloor = int(width * 2);
    float winWidth = 0.2f;
    float winHeight = 0.3f;

    glColor3f(0.2f, 0.2f, 0.5f); // blue windows
    for (int f = 0; f < floors; f++) {
        float floorY = f + 0.5f;

        for (int w = 0; w < windowsPerFloor; w++) {
            float windowX = -width/2 + 0.3f + w * 0.4f;

            // Front face windows
            glBegin(GL_QUADS);
                glVertex3f(windowX, floorY, depth/2 + 0.01f);
                glVertex3f(windowX + winWidth, floorY, depth/2 + 0.01f);
                glVertex3f(windowX + winWidth, floorY + winHeight, depth/2 + 0.01f);
                glVertex3f(windowX, floorY + winHeight, depth/2 + 0.01f);
            glEnd();

            // Back face windows
            glBegin(GL_QUADS);
                glVertex3f(windowX, floorY, -depth/2 - 0.01f);
                glVertex3f(windowX + winWidth, floorY, -depth/2 - 0.01f);
                glVertex3f(windowX + winWidth, floorY + winHeight, -depth/2 - 0.01f);
                glVertex3f(windowX, floorY + winHeight, -depth/2 - 0.01f);
            glEnd();
        }
    }

    // Roof
    glColor3f(0.5f, 0.1f, 0.1f);
    glPushMatrix();
        glTranslatef(0, height + 0.05f, 0);
        glScalef(width + 0.1f, 0.1f, depth + 0.1f);
        glutSolidCube(1.0f);
    glPopMatrix();

    glPopMatrix();
}

void drawicecreamvan(float x, float z, float heading)
{
    glPushMatrix();
        glTranslatef(x, 0, z);
        glRotatef(heading, 0, 1, 0);

        // Main body
        glPushMatrix();
            glTranslatef(0, 0.45f, 0);
            glScalef(1.4f, 0.7f, 0.9f);
            glColor3f(0.8f, 0.15f, 0.2f);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Roof
        glPushMatrix();
            glColor3f(0.05f, 0.4f, 0.15f);
            glTranslatef(-0.2f, 1.0f, 0);
            glScalef(1.0f, 0.1f, 0.9f);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Roof supports (simplified)
        glColor3f(0.9f, 0.8f, 0.6f);
        for(int i = 0; i < 4; i++) {
            float sx = (i < 2) ? -0.6f : 0.3f;
            float sz = (i % 2 == 0) ? 0.35f : -0.35f;

            glPushMatrix();
                glTranslatef(sx, 0.7f, sz);
                glScalef(0.05f, 0.6f, 0.05f);
                glutSolidCube(1.0f);
            glPopMatrix();
        }

        // Wheels (basic cylinders without spokes)
        GLUquadric* q = gluNewQuadric();
        glColor3f(0.1f, 0.1f, 0.1f);

        // Left wheel
        glPushMatrix();
            glTranslatef(-0.5f, 0.18f, 0.45f);
            glRotatef(90, 0, 1, 0);
            gluDisk(q, 0.12f, 0.22f, 16, 2);
        glPopMatrix();

        // Right wheel
        glPushMatrix();
            glTranslatef(-0.5f, 0.18f, -0.45f);
            glRotatef(90, 0, 1, 0);
            gluDisk(q, 0.12f, 0.22f, 16, 2);
        glPopMatrix();

        gluDeleteQuadric(q);

        // Simple driver
        glPushMatrix();
            glTranslatef(0.7f, 0.45f, 0);

            // Body
            glColor3f(0.3f, 0.2f, 0.1f);
            glPushMatrix();
                glTranslatef(0, 0.3f, 0);
                glScalef(0.3f, 0.6f, 0.2f);
                glutSolidCube(1.0f);
            glPopMatrix();

            // Head
            glColor3f(0.9f, 0.7f, 0.5f);
            glPushMatrix();
                glTranslatef(0, 0.75f, 0);
                glutSolidSphere(0.12f, 12, 8);
            glPopMatrix();
        glPopMatrix();

    glPopMatrix();
}

//draw road function
void drawRoad(float x, float y, float z,
              float length, float width,
              float rotationY)   // rotation in degrees
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotationY, 0, 1, 0); // rotate road around Y-axis

    float halfW = width * 0.5f;

    // Road main surface
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
        glVertex3f(-halfW, 0.01f, 0);
        glVertex3f( halfW, 0.01f, 0);
        glVertex3f( halfW, 0.01f, length);
        glVertex3f(-halfW, 0.01f, length);
    glEnd();

    // Side white stripes
    glColor3f(1, 1, 1);
    float stripeW = 0.1f;

    // Left stripe
    glBegin(GL_QUADS);
        glVertex3f(-halfW, 0.02f, 0);
        glVertex3f(-halfW + stripeW, 0.02f, 0);
        glVertex3f(-halfW + stripeW, 0.02f, length);
        glVertex3f(-halfW, 0.02f, length);
    glEnd();

    // Right stripe
    glBegin(GL_QUADS);
        glVertex3f(halfW - stripeW, 0.02f, 0);
        glVertex3f(halfW, 0.02f, 0);
        glVertex3f(halfW, 0.02f, length);
        glVertex3f(halfW - stripeW, 0.02f, length);
    glEnd();

    // Middle dashed yellow line
    glColor3f(1, 1, 0);
    float dashLength = 2.0f;
    float gap = 1.0f;

    for (float i = 0; i < length; i += dashLength + gap) {
        glBegin(GL_QUADS);
            glVertex3f(-0.05f, 0.03f, i);
            glVertex3f( 0.05f, 0.03f, i);
            glVertex3f( 0.05f, 0.03f, i + dashLength);
            glVertex3f(-0.05f, 0.03f, i + dashLength);
        glEnd();
    }

    glPopMatrix();
}


//stall
void drawFoodStall(float x, float y, float z, float rotY) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotY, 0, 1, 0);

    // Base counter
    glColor3f(0.55f, 0.27f, 0.07f);
    glBegin(GL_QUADS);
        glVertex3f(-1, 0,  1);
        glVertex3f( 1, 0,  1);
        glVertex3f( 1, 1,  1);
        glVertex3f(-1, 1,  1);

        glVertex3f(-1, 0, -1);
        glVertex3f( 1, 0, -1);
        glVertex3f( 1, 1, -1);
        glVertex3f(-1, 1, -1);

        glVertex3f(-1, 0, -1);
        glVertex3f(-1, 0,  1);
        glVertex3f(-1, 1,  1);
        glVertex3f(-1, 1, -1);

        glVertex3f(1, 0, -1);
        glVertex3f(1, 0,  1);
        glVertex3f(1, 1,  1);
        glVertex3f(1, 1, -1);

        // Top counter
        glColor3f(0.7f, 0.7f, 0.7f);
        glVertex3f(-1, 1, -1);
        glVertex3f( 1, 1, -1);
        glVertex3f( 1, 1,  1);
        glVertex3f(-1, 1,  1);
    glEnd();

    // Roof
    glColor3f(0.8f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex3f(-1.2f, 1.2f, -1);
        glVertex3f( 1.2f, 1.2f, -1);
        glVertex3f( 0.0f, 2.0f, -1);

        glVertex3f(-1.2f, 1.2f, 1);
        glVertex3f( 1.2f, 1.2f, 1);
        glVertex3f( 0.0f, 2.0f, 1);
    glEnd();

    glBegin(GL_QUADS);
        glVertex3f(-1.2f, 1.2f, -1);
        glVertex3f(-1.2f, 1.2f,  1);
        glVertex3f( 0.0f, 2.0f,  1);
        glVertex3f( 0.0f, 2.0f, -1);

        glVertex3f(1.2f, 1.2f, -1);
        glVertex3f(1.2f, 1.2f,  1);
        glVertex3f(0.0f, 2.0f,  1);
        glVertex3f(0.0f, 2.0f, -1);
    glEnd();

    // Banner
    glPushMatrix();
    glTranslatef(0, 2.1f, 0);
    glScalef(2.5f, 0.3f, 0.1f);
    glColor3f(1.0f, 1.0f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();



    glPopMatrix();
}




// Display
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawSkyCube(500.0f);
    glLoadIdentity();

    updateCamera();

    float radYaw = yaw * 3.1415926f / 180.0f;
    float radPitch = pitch * 3.1415926f / 180.0f;
    float dirX = cos(radPitch) * sin(radYaw);
    float dirY = sin(radPitch);
    float dirZ = -cos(radPitch) * cos(radYaw);

    gluLookAt(camX, camY, camZ,
              camX + dirX, camY + dirY, camZ + dirZ,
              0.0f, 1.0f, 0.0f);




    // Infinite ground
    glColor3f(0.3f,0.8f,0.3f);
    float step = 1.0f;
    int range = 50;
    glBegin(GL_QUADS);
    for(int x=-range;x<range;x++){
        for(int z=-range;z<range;z++){
            float px = x*step + floor(camX/step)*step;
            float pz = z*step + floor(camZ/step)*step;
            glVertex3f(px,0,pz);
            glVertex3f(px+step,0,pz);
            glVertex3f(px+step,0,pz+step);
            glVertex3f(px,0,pz+step);
        }
    }
    glEnd();

    // Reference cube
    glPushMatrix();
    glTranslatef(0,0.5f,0);
    glColor3f(0.8f,0.2f,0.2f);



//buildings



// x,y,z
//width,height,depth
//rotation
//color



//road1 90 degree

drawBuilding(4,0,1,
            2,4,3,
            90,
            0.82f, 0.78f, 0.70f);

drawBuilding(4,0,3.2,
            2,4,3,
            90,
            0.55f, 0.55f, 0.50f);
drawBuilding(4,0,5.4,
            2,2,3,
            90,
            0.30f, 0.35f, 0.28f);
drawBuilding(4,0,7.6,
            2,2,3,
            90,
            0.85f, 0.78f, 0.55f);
drawBuilding(4,0,9.8,
            2,3,3,
            90,
            0.70f, 0.32f, 0.15f);
drawBuilding(4,0,12,
            2,5,3,
            90,
            0.85f, 0.78f, 0.55f);
drawBuilding(4,0,18,
            2,4,3,
            90,
            0.82f, 0.78f, 0.70f);

drawBuilding(4,0,20.2,
            2,3,3,
            90,
            0.70f, 0.32f, 0.15f);
drawBuilding(4,0,22.4,
            2,2,3,
            90,
            0.30f, 0.35f, 0.28f);
drawBuilding(4,0,24.6,
            2,2,3,
            90,
            0.30f, 0.35f, 0.28f);
drawBuilding(4,0,26.8,
            2,3,3,
            90,
            0.70f, 0.32f, 0.15f);
drawBuilding(4,0,33,
            2,2,3,
            90,
            0.30f, 0.35f, 0.28f);
drawBuilding(4,0,35.2,
            2,4,3,
            90,
            0.55f, 0.55f, 0.50f);
drawBuilding(4,0,37.4,
            2,2,3,
            90,
            0.30f, 0.35f, 0.28f);
drawBuilding(4,0,39.6,
            2,2,3,
            90,
            0.30f, 0.35f, 0.28f);
drawBuilding(4,0,41.8,
            2,4,3,
            90,
            0.55f, 0.55f, 0.50f);
drawBuilding(4,0,44,
            2,5,3,
            90,
            0.85f, 0.78f, 0.55f);
drawBuilding(4,0,46.2,
            2,2,3,
            90,
            0.30f, 0.35f, 0.28f);
drawBuilding(4,0,48.5,
            2,2,3,
            90,
            0.30f, 0.35f, 0.28f);
drawBuilding(4,0,50.7,
            2,3,3,
            90,
            0.70f, 0.32f, 0.15f);
drawBuilding(4,0,58,
            2,2,3,
            90,
            0.30f, 0.35f, 0.28f);





//road1 275 degree

drawBuilding(-4,0,1,
            2,4,3,
            275,
            0.82f, 0.78f, 0.70f);

drawBuilding(-4,0,3.2,
            2,4,3,
            275,
            0.55f, 0.55f, 0.50f);
drawBuilding(-4,0,5.4,
            2,2,3,
            275,
            0.30f, 0.35f, 0.28f);
drawBuilding(-4,0,7.6,
            2,2,3,
            275,
            0.85f, 0.78f, 0.55f);
drawBuilding(-4,0,9.8,
            2,3,3,
        275,
            0.70f, 0.32f, 0.15f);
drawBuilding(-4,0,12,
            2,5,3,
            275,
            0.85f, 0.78f, 0.55f);

drawBuilding(-4,0,15,
            2,4,3,
            275,
            0.82f, 0.78f, 0.70f);

drawBuilding(-4,0,18,
            2,4,3,
            275,
            0.82f, 0.78f, 0.70f);

drawBuilding(-4,0,20.2,
            2,3,3,
            275,
            0.70f, 0.32f, 0.15f);
drawBuilding(-4,0,22.4,
            2,2,3,
            275,
            0.30f, 0.35f, 0.28f);
drawBuilding(-4,0,24.6,
            2,2,3,
            275,
            0.30f, 0.35f, 0.28f);
drawBuilding(-4,0,26.8,
            2,3,3,
            275,
            0.70f, 0.32f, 0.15f);

//food stall
drawFoodStall(-4, 0, 29,275);
//icecream van
drawicecreamvan(-4, 31, 275);

drawBuilding(-4,0,33,
            2,2,3,
            275,
            0.30f, 0.35f, 0.28f);
drawBuilding(-4,0,35.2,
            2,4,3,
            275,
            0.55f, 0.55f, 0.50f);

//tree start z==37.4


drawBuilding(-4,0,55,
            2,3,3,
            275,
            0.70f, 0.32f, 0.15f);
drawBuilding(-4,0,58,
            2,2,3,
            275,
            0.30f, 0.35f, 0.28f);

drawTree(-4,0,37.4);
drawTree(-5,0,37.4);
drawTree(-6,0,37.4);
drawTree(-7,0,37.4);
drawTree(-8,0,37.4);
drawTree(-9,0,37.4);
drawTree(-10,0,37.4);
drawTree(-7.12f, 0, 40.83f);
drawTree(-9.45f, 0, 44.28f);
drawTree(-5.33f, 0, 39.57f);
drawTree(-11.02f, 0, 42.71f);
drawTree(-6.48f, 0, 43.10f);
drawTree(-10.21f, 0, 39.94f);
drawTree(-8.77f, 0, 45.32f);
drawTree(-6.93f, 0, 41.27f);
drawTree(-7.85f, 0, 38.66f);
drawTree(-9.12f, 0, 41.55f);
drawTree(-11.43f, 0, 40.72f);
drawTree(-5.88f, 0, 42.31f);
drawTree(-8.02f, 0, 46.12f);
drawTree(-9.97f, 0, 43.60f);
drawTree(-7.41f, 0, 37.92f);
drawTree(-6.14f, 0, 45.01f);
drawTree(-10.49f, 0, 38.90f);
drawTree(-8.33f, 0, 40.18f);
drawTree(-7.56f, 0, 44.51f);
drawTree(-9.75f, 0, 46.01f);
drawTree(-5.67f, 0, 41.26f);
drawTree(-8.40f, 0, 42.90f);
drawTree(-10.92f, 0, 45.77f);
drawTree(-6.72f, 0, 38.42f);
drawTree(-9.04f, 0, 39.12f);
drawTree(-7.98f, 0, 45.84f);
drawTree(-10.62f, 0, 41.33f);
drawTree(-5.51f, 0, 43.44f);
drawTree(-8.99f, 0, 38.85f);

drawTree(-6.29f, 0, 40.31f);

drawTree(-7.74f, 0, 43.77f);
drawTree(-9.36f, 0, 45.19f);
drawTree(-10.15f, 0, 40.07f);
drawTree(-8.53f, 0, 41.88f);
drawTree(-6.85f, 0, 44.92f);

drawTree(-9.88f, 0, 38.62f);
drawTree(-7.20f, 0, 39.97f);
drawTree(-5.99f, 0, 44.18f);

drawTree(-10.78f, 0, 39.26f);
drawTree(-8.12f, 0, 43.32f);
drawTree(-7.54f, 0, 45.40f);


drawTree(0, 0, 0);
drawTree(2, 0, 0);

drawTree(-2, 0, 0);




//roads
drawRoad(0, 0, 0,60, 5,0);
drawRoad(0, 0, 15,20, 3,90);
drawRoad(0, 0, 30,18, 3,90);
drawRoad(12, 0, 15,15, 3,0);
drawRoad(0, 0, 50,10, 3,275);
drawRoad(-10, 0, 50,10, 3,318);
drawRoad(-10, 0, 50,20, 3,228);
drawRoad(-13, 0, 46,35, 3,180);
drawRoad(0, 0, 55,19, 3,90);
drawRoad(19, 0, 55,19, 3,180);
    glPopMatrix();

    glutSwapBuffers();
}

// Idle
void idle() { glutPostRedisplay(); }

// Reshape
void reshape(int w, int h) {
    winWidth = w;
    winHeight = h;

    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0,(float)w/h,0.1,1000.0);
    glMatrixMode(GL_MODELVIEW);
}


// Main
int main(int argc, char** argv) {
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800,600);
    glutCreateWindow("Walkable 3D Scene");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutPassiveMotionFunc(mouseMotion);
    glutIdleFunc(idle);

    glutSetCursor(GLUT_CURSOR_NONE);

    glutMainLoop();
    return 0;
}
