#include <GL/glut.h>
#include <cmath>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;
const int slowdown = 10;
const int maxIter = 200;

int iter;
bool end;

double xPos=0.0;
double yPos=0.6;
double zPos=-10;
double vx, vy, vz, bri, dist, atanwastaken, xx, yy, zz, x, y, z;
double r, g, b;

void normalize(double &vx, double &vy, double &vz) {
    double vecLen = sqrt((vx * vx) + (vy * vy) + (vz * vz));
    vx /= vecLen;
    vy /= vecLen;
    vz /= vecLen;
}

void keyboard(unsigned char key, int x, int y){
    switch(key){
        case 'a':
            xPos+=0.1;glutPostRedisplay();break;
        case 'd':
            xPos-=0.1;glutPostRedisplay();break;
        case 'w':
            zPos+=0.1;glutPostRedisplay();break;
        case 's':
            zPos-=0.1;glutPostRedisplay();break;
        case 'q':
            yPos+=0.1;glutPostRedisplay();break;
        case 'e':
            yPos-=0.1;glutPostRedisplay();break;
    }
}

void display() {
    for (double xx = -WIDTH/2; xx < WIDTH/2; xx++) {
        glBegin(GL_POINTS);
        for (double yy = -HEIGHT/2; yy < HEIGHT/2; yy++) {
            x = xPos;
            y = yPos;
            z = zPos;
            vx = 0 - ((yy / 130) * 0.5 + (xx / 130) * 0.8660254038);
            vy = (yy / 130) * 0.8660254038 + (xx / 130) * -0.5;
            vz = 1;
            normalize(vx, vy, vz);
            r = 0;
            g = 0;
            b = 0;
            iter = 0;
            end = true;
            while (iter < maxIter) {
                dist = (x * x) + (y * y) + (z * z);
                if (dist < 1) {
                    r = 0;
                    g = 0;
                    b = 0;
                    end = false;
                    break;
                }
                dist += dist;
                if (-0.05 <y && y < 0.05 &&dist < 130){
                    r=1;g=0.6;b=0;
                    dist=sqrt((x*x)+(y*y)+(z*z))/10;
                    // bri =50 +3.75*(8-(8-dist)*(8-dist)/1.7);
                    // atanwastaken=atan(x/z) + 180 * (z < 0);
                    // bri +=0.4 *(30 +30 * (sin(dist * 180) * sin(dist * 180)) +30 * (sin(dist * 120) * (1.2 + sin(8 * atanwastaken)/2)));
                    // bri =(bri * (8 - (dist * dist) / 8)) / 3;
                    //
                    // end=false;
                    // r*=abs(bri/180);
                    // g*=abs(bri/180);
                    // b*=abs(bri/180);
                    r-=dist;
                    b-=dist;
                    g-=dist;
                    break;
                    }
                vx += (0 - x) / (dist * slowdown);
                vy += (0 - y) / (dist * slowdown);
                vz += (0 - z) / (dist * slowdown);
                normalize(vx, vy, vz);
                x += vx / slowdown;
                y += vy / slowdown;
                z += vz / slowdown;
                iter++;
            }
            glColor3f(r, g, b);
            glVertex2i(xx+WIDTH/2, yy+HEIGHT/2);
        }
        glEnd();
        glFlush();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("zwarte gat dingie");

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);

    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
