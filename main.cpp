#include <windows.h> // Keep for Windows
#include <mmsystem.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <string>

// --- Global Settings ---
const int SCR_WIDTH = 1000;
const int SCR_HEIGHT = 600;

// --- Layer Logic ---
const int ROAD_HEIGHT = 100;
const int BUILDING_BASE = 100;
const int BEACH_HEIGHT = 40;
const int SHORE_START = 300;
const int HORIZON_LEVEL = 450;

bool isNight = false;
bool isRaining = false;

// --- Structures ---
struct Hotel {
    float x, w, h;
    float r, g, b;
    int type; // 1=Modern, 2=Traditional, 3=Motel
};

struct Car {
    float x, y;
    float speed;
    float maxSpeed;
    int lane;
    float r, g, b;
    float width;
    int type;
};

struct Cloud {
    float x, y, speed;
};

struct RainDrop {
    float x, y, speed;
};

struct Star {
    float x, y;
    float alpha;
    float glowSpeed;
    int direction;
};

struct Boat {
    float x, y, speed;
};

struct SandDot {
    float x, y;
    float r, g, b;
};

// --- Data ---
std::vector<Hotel> city;
std::vector<Car> traffic;
std::vector<Cloud> clouds;
std::vector<SandDot> sandDots;
std::vector<Star> stars;
RainDrop rain[300];

Boat myBoat;
Boat farBoat;

// --- Helper Functions ---
void drawRect(float x, float y, float w, float h) {
    glBegin(GL_QUADS);
    glVertex2f(x, y); glVertex2f(x+w, y); glVertex2f(x+w, y+h); glVertex2f(x, y+h);
    glEnd();
}

void drawCircle(float cx, float cy, float r, int segments) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * 3.14159f * float(i) / float(segments);
        glVertex2f(r * cosf(theta) + cx, r * sinf(theta) + cy);
    }
    glEnd();
}

// Helper to set color with day/night dimming and extra shading factor
void setShadedColor(float r, float g, float b, float dim, float shadeFactor = 1.0f) {
    glColor3f(r * dim * shadeFactor, g * dim * shadeFactor, b * dim * shadeFactor);
}

void drawBeautifulCloud(float x, float y) {
    glColor3f(1.0f, 1.0f, 1.0f);
    if(isNight) glColor3f(0.5f, 0.5f, 0.6f);

    drawCircle(x, y, 30, 20);
    drawCircle(x + 35, y, 35, 20);
    drawCircle(x + 70, y, 30, 20);
    drawCircle(x + 15, y + 25, 25, 20);
    drawCircle(x + 50, y + 30, 28, 20);
}

// --- INDIVIDUAL HOTEL DRAWING FUNCTIONS ---

// Type 1: Modern Glass Tower
void drawModernHotel(Hotel h) {
    float dim = isNight ? 1.0f : 1.0f;

    // Main structure base color (sleek metal/concrete look)
    setShadedColor(h.r, h.g, h.b, dim);
    drawRect(h.x, BUILDING_BASE, h.w, h.h);

    // Add a darker side edge for 3D depth illusion
    setShadedColor(h.r, h.g, h.b, dim, 0.7f);
    drawRect(h.x + h.w, BUILDING_BASE, 15, h.h);

    // Glass facade panels
    glEnable(GL_BLEND);
    if(isNight) glColor4f(1.0f, 0.95f, 0.8f, 0.7f); // Lit from inside
    else glColor4f(0.7f, 0.8f, 1.0f, 0.5f);         // Reflecting sky

    for(float wy = BUILDING_BASE + 10; wy < BUILDING_BASE + h.h - 10; wy += 30) {
        // Large horizontal glass sheets
        drawRect(h.x + 5, wy, h.w - 10, 25);
    }
    glDisable(GL_BLEND);
}

// Type 2: Traditional Brick Building
void drawTraditionalHotel(Hotel h) {
    float dim = isNight ? 0.4f : 1.0f;

    // Main Brick Body (textured look by color)
    setShadedColor(h.r, h.g, h.b, dim);
    drawRect(h.x, BUILDING_BASE, h.w, h.h);

    // Fancy Roof Cornice/Trim (Darker)
    setShadedColor(h.r, h.g, h.b, dim, 0.6f);
    drawRect(h.x - 5, BUILDING_BASE + h.h - 15, h.w + 10, 20);

    // Classic individual windows with sills
    for(float wy = BUILDING_BASE + 25; wy < BUILDING_BASE + h.h - 30; wy += 35) {
        for(float wx = h.x + 15; wx < h.x + h.w - 20; wx += 30) {
            // Window Sill (Dark wood/stone)
            setShadedColor(0.3f, 0.2f, 0.1f, dim);
            drawRect(wx - 3, wy - 3, 18, 24);

            // Window Glass
            if(isNight) glColor3f(1.0f, 0.8f, 0.3f); // Warm light
            else glColor3f(0.2f, 0.2f, 0.3f);        // Dark glass
            drawRect(wx, wy, 12, 18);
        }
    }
}

// Type 3: Classic Motel (2 floors, external walkways)
void drawMotel(Hotel h) {
    float dim = isNight ? 1.0f : 1.0f;
    float floorH = h.h / 2.0f;

    // Main Body (Stucco color)
    setShadedColor(h.r, h.g, h.b, dim);
    drawRect(h.x, BUILDING_BASE, h.w, h.h);

    // --- Ground Floor ---
    for(float wx = h.x + 15; wx < h.x + h.w - 30; wx += 50) {
         // Door (Dark Brown)
         setShadedColor(0.4f, 0.2f, 0.1f, dim);
         drawRect(wx, BUILDING_BASE, 14, 28);
         // Window next to door
         if(isNight) glColor3f(1.0f, 0.9f, 0.5f);
         else glColor3f(0.3f, 0.3f, 0.5f);
         drawRect(wx + 18, BUILDING_BASE + 8, 20, 16);
    }

    // --- Second Floor Walkway ---
    // Shadow under walkway
    setShadedColor(0.1f, 0.1f, 0.1f, dim, 0.5f);
    drawRect(h.x, BUILDING_BASE + floorH - 5, h.w, 5);
    // Walkway concrete
    setShadedColor(h.r, h.g, h.b, dim, 0.8f);
    drawRect(h.x - 5, BUILDING_BASE + floorH, h.w + 10, 5);

    // Second floor Doors/Windows
    for(float wx = h.x + 15; wx < h.x + h.w - 30; wx += 50) {
         setShadedColor(0.4f, 0.2f, 0.1f, dim);
         drawRect(wx, BUILDING_BASE + floorH + 5, 14, 28);
         if(isNight) glColor3f(1.0f, 0.9f, 0.5f);
         else glColor3f(0.3f, 0.3f, 0.5f);
         drawRect(wx + 18, BUILDING_BASE + floorH + 13, 20, 16);
    }

    // Railing
    glColor3f(0.3f*dim, 0.3f*dim, 0.3f*dim); // Metal color
    drawRect(h.x - 5, BUILDING_BASE + floorH + 25, h.w + 10, 3); // Top rail
    for(float wx = h.x; wx <= h.x + h.w; wx += 15) {
         drawRect(wx, BUILDING_BASE + floorH + 5, 2, 20); // Posts
    }
}

// Main dispatcher function
void drawHotel(Hotel h) {
    switch(h.type) {
        case 1: drawModernHotel(h); break;
        case 2: drawTraditionalHotel(h); break;
        case 3: drawMotel(h); break;
    }
}

// ... [drawShip, drawCar, initSand remain the same] ...
void drawShip(Boat b, float scale) {
    float dim = isNight ? 0.6f : 1.0f;
    // Hull Bottom
    glColor3f(0.5f * dim, 0.1f * dim, 0.1f * dim);
    glBegin(GL_POLYGON);
        glVertex2f(b.x + 10*scale, b.y - 8*scale);
        glVertex2f(b.x + 80*scale, b.y - 8*scale);
        glVertex2f(b.x + 90*scale, b.y);
        glVertex2f(b.x, b.y);
    glEnd();
    // Hull Upper Deck
    glColor3f(0.9f * dim, 0.9f * dim, 0.95f * dim);
    glBegin(GL_POLYGON);
        glVertex2f(b.x, b.y);
        glVertex2f(b.x + 90*scale, b.y);
        glVertex2f(b.x + 100*scale, b.y + 15*scale);
        glVertex2f(b.x - 5*scale, b.y + 15*scale);
    glEnd();
    // Captain's Bridge
    glColor3f(0.8f * dim, 0.8f * dim, 0.9f * dim);
    drawRect(b.x + 10*scale, b.y + 15*scale, 50*scale, 10*scale);
    drawRect(b.x + 15*scale, b.y + 25*scale, 30*scale, 8*scale);
    // Smokestacks
    glColor3f(1.0f * dim, 0.5f * dim, 0.0f * dim);
    drawRect(b.x + 35*scale, b.y + 25*scale, 6*scale, 12*scale);
    drawRect(b.x + 48*scale, b.y + 15*scale, 6*scale, 15*scale);
    glColor3f(0.1f, 0.1f, 0.1f);
    drawRect(b.x + 34*scale, b.y + 35*scale, 8*scale, 3*scale);
    drawRect(b.x + 47*scale, b.y + 28*scale, 8*scale, 3*scale);
    // Windows
    if(isNight) glColor3f(1.0f, 1.0f, 0.0f);
    else glColor3f(0.2f, 0.3f, 0.4f);
    glPointSize(3.0f * scale);
    glBegin(GL_POINTS);
        for(int i=0; i<4; i++) glVertex2f(b.x + (15*scale) + (i*10*scale), b.y + 20*scale);
        glVertex2f(b.x + 20*scale, b.y + 29*scale);
        glVertex2f(b.x + 30*scale, b.y + 29*scale);
    glEnd();
}

void drawCar(Car c) {
    float dim = isNight ? 0.6f : 1.0f;
    float bodyY = c.y + 10;
    glColor3f(0.1f, 0.1f, 0.1f);
    drawRect(c.x + 5, c.y - 2, c.width, 5);
    if(c.type == 1) { // SUV
        glColor3f(c.r*dim, c.g*dim, c.b*dim);
        drawRect(c.x, bodyY, c.width, 25);
        drawRect(c.x + 10, bodyY + 25, c.width - 20, 20);
        glColor3f(0.7f*dim, 0.9f*dim, 1.0f*dim);
        drawRect(c.x + 15, bodyY + 28, 25, 14);
        drawRect(c.x + 45, bodyY + 28, 25, 14);
    }
    else if (c.type == 2) { // Sports
        glColor3f(c.r*dim, c.g*dim, c.b*dim);
        glBegin(GL_POLYGON);
            glVertex2f(c.x, bodyY);
            glVertex2f(c.x + c.width, bodyY);
            glVertex2f(c.x + c.width, bodyY + 22);
            glVertex2f(c.x + c.width*0.5, bodyY + 28);
            glVertex2f(c.x + c.width*0.2, bodyY + 28);
            glVertex2f(c.x, bodyY + 12);
        glEnd();
        glColor3f(0.1f, 0.1f, 0.1f);
        drawRect(c.x + c.width - 15, bodyY + 22, 15, 4);
        glColor3f(0.2f, 0.2f, 0.3f);
        glBegin(GL_POLYGON);
             glVertex2f(c.x + c.width*0.25, bodyY + 25);
             glVertex2f(c.x + c.width*0.5, bodyY + 25);
             glVertex2f(c.x + c.width*0.6, bodyY + 15);
             glVertex2f(c.x + c.width*0.3, bodyY + 15);
        glEnd();
    }
    else { // Sedan
        glColor3f(c.r*dim, c.g*dim, c.b*dim);
        drawRect(c.x - 5, bodyY, c.width + 10, 20);
        drawRect(c.x + 15, bodyY + 20, c.width - 30, 15);
        glColor3f(0.7f*dim, 0.9f*dim, 1.0f*dim);
        drawRect(c.x + 20, bodyY + 22, (c.width-40)/2 - 2, 10);
        drawRect(c.x + 20 + (c.width-40)/2 + 2, bodyY + 22, (c.width-40)/2 - 2, 10);
    }
    glColor3f(0.1f, 0.1f, 0.1f);
    drawCircle(c.x + 15, c.y + 10, 10, 12);
    drawCircle(c.x + c.width - 15, c.y + 10, 10, 12);
    glColor3f(0.6f, 0.6f, 0.6f);
    drawCircle(c.x + 15, c.y + 10, 6, 8);
    drawCircle(c.x + c.width - 15, c.y + 10, 6, 8);
    if(isNight) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 0.8f, 0.4f);
        glBegin(GL_TRIANGLES);
        if(c.lane == 0) {
            glVertex2f(c.x, bodyY + 15);
            glVertex2f(c.x, bodyY + 5);
            glVertex2f(c.x - 120, bodyY - 10);
        } else {
            glVertex2f(c.x + c.width, bodyY + 15);
            glVertex2f(c.x + c.width, bodyY + 5);
            glVertex2f(c.x + c.width + 120, bodyY - 10);
        }
        glEnd();
        glDisable(GL_BLEND);
    }
}

void initSand() {
    sandDots.clear();
    for(int i = 0; i < 1200; i++) {
        SandDot s;
        s.x = rand() % SCR_WIDTH;
        s.y = (rand() % BEACH_HEIGHT) + (SHORE_START - BEACH_HEIGHT);
        float var = (rand() % 10) / 100.0f;
        s.r = 0.8f - var;
        s.g = 0.7f - var;
        s.b = 0.5f - var;
        sandDots.push_back(s);
    }
}
// --------------------------------------------------


void init() {
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, SCR_WIDTH, 0, SCR_HEIGHT);

    // Enable Smooth Graphics (Anti-aliasing)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    initSand();

    // Initialize Ships
    myBoat.x = -150; myBoat.y = SHORE_START + 50; myBoat.speed = 0.3f;
    farBoat.x = 200; farBoat.y = SHORE_START + 110; farBoat.speed = 0.15f;

    // Stars
    for(int i = 0; i < 100; i++) {
        Star s;
        s.x = rand() % SCR_WIDTH;
        s.y = (rand() % (SCR_HEIGHT - HORIZON_LEVEL - 50)) + HORIZON_LEVEL + 50;
        s.alpha = (rand() % 10) / 10.0f;
        s.glowSpeed = 0.01f + ((rand() % 20) / 1000.0f);
        s.direction = 1;
        stars.push_back(s);
    }

    // --- MANUALLY PLACED INDIVIDUAL HOTELS ---
    city.clear();
    // 1. Traditional Brick Hotel (Left side) - Reddish brown
    city.push_back({20, 120, 200,  0.7f, 0.3f, 0.2f, 2});

    // 2. Modern Glass Tower (Tall) - Bluish grey
    city.push_back({160, 90, 280,  0.3f, 0.5f, 0.7f, 1});

    // 3. Seaside Motel (Wide, short) - Sandy beige
    city.push_back({270, 200, 80,  0.8f, 0.8f, 0.6f, 3});

    // 4. Another Modern Hotel (Medium height) - Teal
    city.push_back({480, 100, 220, 0.2f, 0.6f, 0.6f, 1});

    // 5. Traditional Colonial Hotel (Right side) - Cream/Yellowish
    city.push_back({600, 140, 160, 0.8f, 0.7f, 0.5f, 2});

    // --- NEW ADDITIONS TO FILL EMPTY SPACE ---

    // 6. Tall Modern Hotel (Greenish tint)
    city.push_back({755, 100, 310, 0.3f, 0.6f, 0.5f, 1});

    // 7. Wide Traditional/Classic (Dark Stone)
    city.push_back({880, 110, 190, 0.4f, 0.4f, 0.45f, 2});


    // Traffic
    traffic.push_back({900, ROAD_HEIGHT - 35, -3.0f, -3.0f, 0, 1.0f, 0.2f, 0.2f, 80, 0});
    traffic.push_back({600, ROAD_HEIGHT - 35, -2.5f, -2.5f, 0, 0.2f, 0.2f, 1.0f, 90, 1});
    traffic.push_back({200, ROAD_HEIGHT - 35, -2.8f, -2.8f, 0, 0.9f, 0.9f, 0.2f, 80, 0});
    traffic.push_back({100, 20, 3.5f, 3.5f, 1, 0.2f, 0.8f, 0.2f, 80, 0});
    traffic.push_back({300, 20, 2.0f, 2.0f, 1, 0.8f, 0.4f, 0.0f, 110, 2});
    traffic.push_back({700, 20, 3.8f, 3.8f, 1, 1.0f, 1.0f, 1.0f, 80, 0});

    // Clouds
    clouds.push_back({100, 500, 0.5f});
    clouds.push_back({400, 550, 0.3f});
    clouds.push_back({700, 520, 0.6f});

    // Rain
    for(int i=0; i<300; i++) {
        rain[i].x = rand() % SCR_WIDTH;
        rain[i].y = rand() % SCR_HEIGHT;
        rain[i].speed = (rand() % 10) + 15;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // --- 1. SKY ---
    if(isNight) {
        glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 0.1f); glVertex2f(0, SCR_HEIGHT); glVertex2f(SCR_WIDTH, SCR_HEIGHT);
        glColor3f(0.1f, 0.1f, 0.2f); glVertex2f(SCR_WIDTH, HORIZON_LEVEL); glVertex2f(0, HORIZON_LEVEL);
        glEnd();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPointSize(2.0f);
        glBegin(GL_POINTS);
        for(size_t i = 0; i < stars.size(); i++) {
            glColor4f(1.0f, 1.0f, 1.0f, stars[i].alpha);
            glVertex2f(stars[i].x, stars[i].y);
        }
        glEnd();
        glDisable(GL_BLEND);
        glColor3f(1.0f, 1.0f, 1.0f); drawCircle(900, 520, 30, 20);
    } else {
        glBegin(GL_QUADS);
        glColor3f(0.0f, 0.6f, 1.0f); glVertex2f(0, SCR_HEIGHT); glVertex2f(SCR_WIDTH, SCR_HEIGHT);
        glColor3f(0.6f, 0.8f, 1.0f); glVertex2f(SCR_WIDTH, HORIZON_LEVEL); glVertex2f(0, HORIZON_LEVEL);
        glEnd();
        glColor3f(1.0f, 0.9f, 0.0f); drawCircle(900, 520, 45, 20);
    }

    // --- 2. THE SEA ---
    if(isNight) glColor3f(0.0f, 0.0f, 0.3f);
    else glColor3f(0.0f, 0.4f, 0.8f);
    drawRect(0, SHORE_START, SCR_WIDTH, HORIZON_LEVEL - SHORE_START);

    // --- 3. SHIPS ---
    drawShip(farBoat, 0.5f);
    drawShip(myBoat, 1.0f);

    // --- 4. DISTANT GROUND/COAST ---
    if(isNight) glColor3f(0.1f, 0.2f, 0.1f);
    else glColor3f(0.2f, 0.5f, 0.2f);
    drawRect(0, BUILDING_BASE, SCR_WIDTH, SHORE_START - BUILDING_BASE);

    // --- 5. BEACH (Fixed Sand Dots) ---
    if(isNight) glColor3f(0.4f, 0.35f, 0.2f);
    else glColor3f(0.94f, 0.85f, 0.6f);
    drawRect(0, SHORE_START - BEACH_HEIGHT, SCR_WIDTH, BEACH_HEIGHT);
    if(!isNight) {
        glPointSize(2.0f);
        glBegin(GL_POINTS);
        for(size_t i = 0; i < sandDots.size(); i++) {
             glColor3f(sandDots[i].r, sandDots[i].g, sandDots[i].b);
             glVertex2f(sandDots[i].x, sandDots[i].y);
        }
        glEnd();
    }

    // --- 6. CLOUDS ---
    for(size_t i = 0; i < clouds.size(); i++) {
        drawBeautifulCloud(clouds[i].x, clouds[i].y);
    }

    // --- 7. HOTELS (INDIVIDUAL DESIGNS) ---
    for(size_t i = 0; i < city.size(); i++) {
        drawHotel(city[i]);
    }

    // --- 8. ROAD ---
    glColor3f(0.3f, 0.3f, 0.3f);
    drawRect(0, 0, SCR_WIDTH, ROAD_HEIGHT);
    glColor3f(1.0f, 1.0f, 1.0f);
    for(int i=0; i<SCR_WIDTH; i+=60) drawRect(i, ROAD_HEIGHT/2 - 2, 30, 4);
    // Sidewalk
    glColor3f(0.6f, 0.6f, 0.6f);
    drawRect(0, ROAD_HEIGHT, SCR_WIDTH, 10);

    // --- 9. TRAFFIC ---
    for(size_t i = 0; i < traffic.size(); i++) {
        drawCar(traffic[i]);
    }

    // --- 10. RAIN ---
    if(isRaining) {
        glColor3f(0.8f, 0.8f, 1.0f);
        glBegin(GL_LINES);
        for(int i=0; i<300; i++) {
            glVertex2f(rain[i].x, rain[i].y);
            glVertex2f(rain[i].x - 5, rain[i].y - 15);
        }
        glEnd();
    }

    glutSwapBuffers();
}

void update(int value) {
    // Ships
    myBoat.x += myBoat.speed; if(myBoat.x > SCR_WIDTH + 150) myBoat.x = -150;
    farBoat.x += farBoat.speed; if(farBoat.x > SCR_WIDTH + 80) farBoat.x = -80;

    // Stars
    for(size_t i = 0; i < stars.size(); i++) {
        if(stars[i].direction == 1) {
            stars[i].alpha += stars[i].glowSpeed;
            if(stars[i].alpha >= 1.0f) stars[i].direction = -1;
        } else {
            stars[i].alpha -= stars[i].glowSpeed;
            if(stars[i].alpha <= 0.0f) stars[i].direction = 1;
        }
    }
    // Rain
    if(isRaining) {
        for(int i=0; i<300; i++) {
            rain[i].y -= rain[i].speed;
            if(rain[i].y < 0) {
                rain[i].y = SCR_HEIGHT; rain[i].x = rand() % SCR_WIDTH;
            }
        }
    }
    // Clouds
    for(size_t i = 0; i < clouds.size(); i++) {
        clouds[i].x += clouds[i].speed;
        if(clouds[i].x > SCR_WIDTH + 50) clouds[i].x = -100;
    }
    // Traffic
    float safeDist = 160.0f;
    for(size_t i=0; i<traffic.size(); i++) {
        traffic[i].speed = traffic[i].maxSpeed;
    }
    for(int iter=0; iter<3; iter++) {
        for(size_t i=0; i<traffic.size(); i++) {
            for(size_t j=0; j<traffic.size(); j++) {
                if(i == j || traffic[i].lane != traffic[j].lane) continue;
                float dist = (traffic[i].lane == 1) ? (traffic[j].x - traffic[i].x) : (traffic[i].x - traffic[j].x);
                if(dist < -500) dist += SCR_WIDTH + 300;
                if(dist > 0 && dist < safeDist) {
                    if(abs(traffic[i].speed) > abs(traffic[j].speed)) {
                        traffic[i].speed = traffic[j].speed;
                    }
                }
            }
        }
    }
    for(size_t i=0; i<traffic.size(); i++) {
        traffic[i].x += traffic[i].speed;
        if(traffic[i].lane == 1 && traffic[i].x > SCR_WIDTH + 150) traffic[i].x = -150;
        if(traffic[i].lane == 0 && traffic[i].x < -150) traffic[i].x = SCR_WIDTH + 150;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void handleKey(unsigned char key, int x, int y) {
    switch(key) {
        case 'n': isNight = true; break;
        case 'd': isNight = false; break;
        case 'c':
        case 'C':
            PlaySound(TEXT("car-passing.wav"), NULL, SND_ASYNC | SND_FILENAME);
            break;
        case 's':
        case 'S':
            PlaySound(TEXT("ship.wav"), NULL, SND_ASYNC | SND_FILENAME);
            break;
        case 'r':
        case 'R':
            isRaining = !isRaining;
            if(isRaining) {
                PlaySound(TEXT("light-rain.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
            } else {
                PlaySound(NULL, 0, 0);
            }
            break;
        case 27: exit(0); break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
    glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);

    // Center the window on the screen
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    glutInitWindowPosition((screenWidth - SCR_WIDTH) / 2, (screenHeight - SCR_HEIGHT) / 2);

    glutCreateWindow("Coxs Bazar - Full City View");
    init();
    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);
    glutKeyboardFunc(handleKey);
    glutMainLoop();
    return 0;
}
