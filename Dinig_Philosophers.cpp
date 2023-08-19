#include<GL/glut.h>
#include <cmath>
#include <iostream>
using namespace std;
const float PI = 3.14;
const int numPeople = 5;
const int numchopsticks=5;
const float tableRadius = 4.0f;
const float rodHeight = 5.0f;
const float baseRadius = tableRadius * 0.2f;
const float baseHeight = tableRadius * 0.05f;
const float personDistance = tableRadius * 1.2f;
const float bowldistance = tableRadius * 0.7f;
const float stickdistance = tableRadius * 0.5f;
const float ck_diffuse_color[5][4]={{1,0,0,1},{0,1,0,1},{0,0,1,1},{1,1,0,1},{0,1,1,1}};
#define THINKING 1
#define HUNGRY 2
#define EATING 3

#define USED 5
#define NOTUSED 10
int viewMode = 0;
float rotationAngle = 170.0f;
bool rotateTable = false;

struct Philosopher {
    int id;
    int state;
};

struct chop_stick{
    int id;
    int state;
};

const int numPhilosophers = numPeople;
Philosopher philosophers[numPhilosophers];
const int numchops = numchopsticks;
chop_stick ashok[numchops];

void drawChessKing(float radius, float height) {
//original base
    glPushMatrix();
     glColor3f(0.5f, 0.5f, 0.5f);
    glutSolidCube(0.55);
    glPopMatrix();
     //middle
     glPushMatrix();
     glTranslatef(0.0f, height * 0.4f, 0.0f);
     glScalef(0.85,1.5,0.65);
        glutSolidCube(0.5);
        glPopMatrix();
     //middle top
      glTranslatef(0.0f, height * 0.9f, 0.0f);
     glutSolidSphere(0.6 * 0.5f, 20, 20);

    // Head
    glColor3f(1.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, height * 0.35f, 0.0f);
    glutSolidSphere(0.2 * 0.5f, 20, 20);
    //middle


}

void chopstick(float yth) {
         float mat_diff_chop[] = {0.76, 0.64, 0.51, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diff_chop);
    glPushMatrix();
    glPushMatrix();
    glRotatef(yth, 0.0, 1.0, 0.0);
    gluCylinder(gluNewQuadric(), 0.05, 0.08, 2.0, 32, 32);
    glPopMatrix();
    glPopMatrix();
}

void drawbowl() {
    glColor3f(0.1, 0.4, 0.5);
    glPushMatrix();
    glTranslatef(0, 1.3, 0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glScalef(0.5, 0.5, 0.5);
    float mat_diff_bowl[] = {0.82, 0.82, 0.82, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diff_bowl);
    glPushMatrix();
    glTranslatef(0.0, -0.5, 0.0);
    gluCylinder(gluNewQuadric(), 0.8, 1.0, 1.0, 32, 32);
    glPopMatrix();
    glColor3f(0.4, 0.8, 0.8);
    glPushMatrix();
    glTranslatef(0.0, -1.0, 0.0);
    gluDisk(gluNewQuadric(), 0.0, 1.5, 32, 32);
    glPopMatrix();
    glPopMatrix();
}
void print_text(float x, float y, char arr[]) {

  //cout<<string<<len<<endl;
 glRasterPos2f(x,y);
 int i=0;
 while(arr[i] !='\0') {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,arr[i]);
        i++;
    }
}

void display_welcome(){
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glColor3f(0,1,0.5); //spring green
 print_text(350,600,"CMR INSTITUTE OF TECHNOLOGY");
 glColor3f(0.56,0.56,0.74);//light STEEL Blue
 print_text(250,550,"DEPARTMENT OF COMPUTER SCIENCE AND ENGINEERING");
 glColor3f(0.8,0.498,0.196); //gold
 print_text(200,500,"18CSL67 - COMPUTER GRAPHICS WITH MINI PROJECT LABORATORY" );


 glColor3f(0.196,0.6,0.8); //sky blue
 print_text(10,450,"__________________________________________________________________________________________________" );

 glColor3f(0.196,0.6,0.8); //sky blue
 print_text(400,400,"DINING PHILOSOPHERS PROBLEM" );

glColor3f(0.85,0.85,0.1);
  print_text(100,350, "Created By");
  glColor3f(0.45,0.85,0.57);
 print_text(120,300, "ARUN FRANCIS(1CR20CS031)");
  print_text(120,250, "ASHOK K(1CR20CS032)");

glColor3f(0.85,0.85,0.1);
    print_text(600,350, "Guided By");
    glColor3f(0.45,0.85,0.57); //aquamarine
 print_text(620,300, "Dr.Preethi Sheeba, Asst Proffesor");
  print_text(620,250, "Kartheek GCR, Asst Proffesor");

// give any instructions here - Mouse/keyboard
glColor3f(0.85,0.85,0.1);
 print_text(360,100, "PRESS <<ENTER>>  TO BEGIN");
glutSwapBuffers();
}

void display() {

    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float light_dif[] = {1.0, 1.0, 1.0, 1};
    float lightposition[] = {5.0, 5.0, 5.0, 1.0};
    float lightposition2[] = {-5.0, 5.0, 3.0, 1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif);
    glLightfv(GL_LIGHT0, GL_POSITION, lightposition);
    glLightfv(GL_LIGHT1, GL_POSITION, lightposition2);

    float mat_diff[] = {0.7, 0.7, 0.7, 1};
    float mat_shiny[] = {100};

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diff);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shiny);

    if (viewMode == 0) {
        gluLookAt(0.0, 8.0, -15.0,
                  0.0, 0.0, 0.0,
                  0.0, 1.0, 0.0);
    }
    else if (viewMode == 1) {
        gluLookAt(0.0, 15.0, 0.0,
                  0.0, 0.0, 0.0,
                  0.0, 0.0, -1.0);
    }
    else {
        gluLookAt(-15.0, 8.0, 0.0,
                  0.0, 0.0, 0.0,
                  0.0, 1.0, 0.0);
    }

    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);

    // table top
    glPushMatrix();
    float mat_diff_tab[] = {0.5, 0.35, 0.05, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diff_tab);
    glPushMatrix();
    glRotated(-90, 1, 0, 0);
    glPushMatrix();
    glTranslatef(0, 0, tableRadius * 0.1);
    gluDisk(gluNewQuadric(), 0, tableRadius + 0.05, 32, 32);
    glPopMatrix();
    gluCylinder(gluNewQuadric(), tableRadius, tableRadius, tableRadius * 0.1, 32, 32);
    glPopMatrix();
    glPopMatrix();

    // table rod
    glColor3f(0.4f, 0.8f, 0.8f);
    glPushMatrix();
    float mat_diff_rod[] = {0.5, 0.2, 0.05, 1};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diff_rod);
    glTranslatef(0.0f, -rodHeight, 0.0f);
    glRotated(-90, 1, 0, 0);
    gluCylinder(gluNewQuadric(), baseRadius, baseRadius, rodHeight, 32, 32);
    glPopMatrix();

    // table base
    glColor3f(0.4f, 0.8f, 0.8f);
    glPushMatrix();
    glTranslatef(0.0f, -rodHeight - baseHeight / 2.0f, 0.0f);
    glRotated(-90, 1, 0, 0);
    gluDisk(gluNewQuadric(), 0, tableRadius * 0.4f, 32, 32);
    gluCylinder(gluNewQuadric(), tableRadius * 0.4f, tableRadius * 0.4f, baseHeight, 32, 32);
    glPopMatrix();

    // philosophers
    for (int i = 0; i < numPeople; ++i) {
        float angle = 2.0f * PI * i / numPeople;
        glPushMatrix();
        glTranslatef(personDistance * sin(angle), -baseRadius, personDistance * cos(angle));

        switch (philosophers[i].state) {
            case 0: // Idle state
                glColor3f(0.0f, 0.8f, 0.2f); // Green color
                //cout<<"philosopher is in idle state"<<endl;
                break;
            case 1: // Thinking state
                glColor3f(0.8f, 0.0f, 0.2f); // Red color
                //cout<<"philosopher is in thinking state"<<endl;
                break;
            case 2: // Eating state
                glColor3f(0.2f, 0.0f, 0.8f); // Blue color
                //cout<<"philosopher is in eating state"<<endl;
                break;
        }

    glMaterialfv(GL_FRONT, GL_DIFFUSE, ck_diffuse_color[i]);
        drawChessKing(baseRadius * 1.5f, baseRadius * 1.5f);
        glPopMatrix();
    }

    // bowls
    glColor3f(0.0f, 0.0f, 0.0f); // Green color
     float mat_diff_b[] = {0.4, 0.4, 0.8, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diff_b);
    for (int i = 0; i < numPeople; ++i) {
        float angle = 2.0f * PI * i / numPeople;
        glPushMatrix();
        glTranslatef((bowldistance) * sin(angle), -baseRadius, (bowldistance) * cos(angle));
        drawbowl();
        glPopMatrix();
    }

    // chopsticks
    float yth = 0;
    glColor3f(0.0f, 0.0f, 0.0f); // Black color
    glPushMatrix();
    glRotatef(30, 0, 1, 0);
    for (int i = 0; i < numPeople; ++i) {
        float angle = 2.0f * PI * i / numPeople;
        glPushMatrix();
        glTranslatef((stickdistance) * sin(angle), baseRadius - 0.25, (stickdistance) * cos(angle));

        if(ashok[i].state==NOTUSED)
           chopstick(yth);

        yth += 360.0 / 5;
        glPopMatrix();
    }
    glPopMatrix();
    glutSwapBuffers();
    }




void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'v':
            viewMode = (viewMode + 1) % 3;
            break;
        case 'R':
            rotateTable = !rotateTable;
            break;
        case 'S':
            rotateTable = false;
            break;
        default:
            // state input as integers from 0 to 2
            for (int i = 0; i < numPhilosophers; ++i) {
                if (key == '0' + philosophers[i].id) {
                    int newState = key - '0'; // Convert the key to an integer
                    cout<<newState<<endl;
                    philosophers[i].state = newState;
                    break;
                }
            }
            break;
    }

    glutPostRedisplay();
}

void update(int value) {
    if (rotateTable) {
        rotationAngle += 0.1f;
        if (rotationAngle > 360)
            rotationAngle -= 360;
    }

    glutPostRedisplay();
    glutTimerFunc(10, update, 0);
}
void init_ph() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0, 1, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);

}
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0, (float)width / (float)height, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
}
void menu(int id)
{
    switch(id)
    {

    case 100://0

        if(philosophers[0].state == EATING){
            philosophers[0].state = THINKING;
            ashok[0].state=NOTUSED;ashok[1].state=NOTUSED;
        }
        else{
        philosophers[0].state = HUNGRY;
        if(philosophers[1].state == THINKING && philosophers[4].state == THINKING){
            philosophers[0].state = EATING;

        ashok[0].state=USED;ashok[1].state=USED;
        }
        }

        break;
    case 200:
        if(philosophers[1].state == EATING){
            philosophers[1].state = THINKING;
            ashok[1].state=NOTUSED;ashok[2].state=NOTUSED;
        }
        else{
        philosophers[1].state = HUNGRY;
        if(philosophers[0].state == THINKING && philosophers[2].state == THINKING){
            philosophers[1].state = EATING;
           ashok[1].state=USED;ashok[2].state=USED;
        }
        }
        break;
    case 300:
        if(philosophers[2].state == EATING){
            philosophers[2].state = THINKING;
            ashok[2].state=NOTUSED;ashok[3].state=NOTUSED;
        }
        else{
        philosophers[2].state = HUNGRY;
        if(philosophers[1].state == THINKING && philosophers[3].state == THINKING){
            philosophers[2].state = EATING;
             ashok[2].state=USED;ashok[3].state=USED;
        }
        }
        break;
    case 400:
        if(philosophers[3].state == EATING){
            philosophers[3].state = THINKING;
            ashok[3].state=NOTUSED;ashok[4].state=NOTUSED;
        }
        else{
        philosophers[3].state = HUNGRY;
        if(philosophers[2].state == THINKING && philosophers[4].state == THINKING){
            philosophers[3].state = EATING;
             ashok[3].state=USED;ashok[4].state=USED;
        }
        }
        break;
    case 500:
        if(philosophers[4].state == EATING){
            philosophers[4].state = THINKING;
            ashok[4].state=NOTUSED;ashok[0].state=NOTUSED;
        }
        else{
        philosophers[4].state = HUNGRY;
        if(philosophers[3].state == THINKING && philosophers[0].state == THINKING){
            philosophers[4].state = EATING;
             ashok[4].state=USED;ashok[0].state=USED;
        }
        }
        break;

    }

    glutPostRedisplay();
}

void welcome_keys (unsigned char key, int x, int y) {
 if (key == 13){
         for (int i = 0; i < numPhilosophers; ++i) {
        philosophers[i].id = i;

        philosophers[i].state = THINKING; // Initialize all philosophers in idle state
    }
    for(int i=0;i<numchops;++i)
    {
        ashok[i].id=i;
        ashok[i].state=NOTUSED;

    }
    init_ph();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutTimerFunc(10, update, 0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glShadeModel(GL_SMOOTH);
    glutCreateMenu(menu);
    glutAddMenuEntry("philosopher1",100);
    glutAddMenuEntry("philosopher2",200);
    glutAddMenuEntry("philosopher3",300);
    glutAddMenuEntry("philosopher4",400);
    glutAddMenuEntry("philosopher5",500);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
 }
}
void myinit() {
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0,1000,0,700);
  glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(800, 800);
    glutCreateWindow("DINING PHILOSOPHERS PROBLEM");
    myinit();
    glutDisplayFunc(display_welcome);
    glutKeyboardFunc(welcome_keys);
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();

}
