/**
 * main.c
 *
 * Compile (on Windows with MinGW/freeglut):
 * gcc main.c rabi.c qubit.c bloch.c -o QubitSim -lfreeglut -lopengl32 -lglu32 -lws2_32 -lwinmm
 *
 * * Controls:
 * - Click and drag mouse: Rotate the camera.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> // For strstr
#include <complex.h>
#include <math.h>
#include <conio.h> // For _kbhit() and _getch()
#include <windows.h> 
#include <GL/glut.h>

#include "qubit.h"
#include "rabi.h"
#include "bloch.h" // Was blochascii.h, now using the 3D version

#define M_PI 3.14159265358979323846

// --- Global State for Simulation ---
pqubit g_pq; //first qubit.
pqubit g_pq2;//second qubit
int index_qbit = 0; //index of qubits. 
rabidrive g_drive;
rabidrive g_drive2;
double g_brightness = 50.0; // Last known brightness
FILE *g_python_process = NULL;
// Camera rotation state
float g_rotX = 30.0f;
float g_rotY = 30.0f;
int g_lastX, g_lastY;
int g_isRotating = 0;


/**
 * @brief Cleans up and closes the Python pipe when the program exits.
 */
void cleanup() {
    if (g_python_process != NULL) {
        pclose(g_python_process);
        g_python_process = NULL;
        printf("Python pipe closed.\n");
    }
    // Clean up memory
    if (g_pq != NULL) {
        free(g_pq);
        g_pq = NULL;
    }
    if(g_pq2!= NULL){
        free(g_pq2);
        g_pq2 = NULL;
    }
}


/**
 * @brief Reads the latest brightness value from the running Python script.
 * * @return double The new brightness value (0-100), or the last known value if read fails.
 */
double get_brightness_from_camera() {
    char buffer[128];
    
    // Start Python process if not already running
    if (g_python_process == NULL) {
        // --- CRITICAL CHANGE: Removed the '-u' flag to avoid conflict with Python's flush() ---
        g_python_process = popen("python camera_quality.py", "r");
        if (g_python_process == NULL) {
            fprintf(stderr, "ERROR: Failed to run python camera_quality.py\n");
            return g_brightness;
        }
        
        // Wait for "READY" signal
        printf("Waiting for Python model to load...\n");
        fflush(stdout); // Flush C output too
        while (fgets(buffer, sizeof(buffer), g_python_process)) {
            if (strstr(buffer, "READY") != NULL) {
                printf("Python model ready. Starting simulation loop.\n");
                fflush(stdout);
                break;
            }
        }
    }
    
    // Read the latest brightness value
    // A simple fgets read is used here. Because the python script flushes frequently, 
    // this should block for a minimum time (or not at all if a value is ready).
    if (fgets(buffer, sizeof(buffer), g_python_process) != NULL) {
        double new_brightness;
        // The Python script prints a value between 0.1 and 0.9.
        if (sscanf(buffer, "BRIGHTNESS:%lf", &new_brightness) == 1) {
            // new_brightness is a value between 0.1 and 0.9
            g_brightness = new_brightness;
            return g_brightness;
        }
    }
    
    // If the read failed, it often means the python process closed or crashed.
    if (feof(g_python_process) || ferror(g_python_process)) {
        fprintf(stderr, "Python process pipe closed or error occurred. Shutting down.\n");
        cleanup(); // Attempt to clean up and exit gracefully
        exit(1); 
    }

    // return last good value if read fails but pipe is still open
    return g_brightness; 
}

// --- GLUT Callbacks ---

/**
 * @brief The main rendering function.
 */
void display_func(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Camera Positioning and Rotation
    glTranslatef(0.0f, 0.0f, -3.0f); // Move back from the scene
    glRotatef(g_rotX, 1.0f, 0.0f, 0.0f); // Apply X rotation
    glRotatef(g_rotY, 0.0f, 1.0f, 0.0f); // Apply Y rotation

    // Draw the Bloch Sphere elements
    bloch_draw_sphere();
    bloch_draw_axes();
    
    // Draw the Qubit Vector (Blue)
    // The Rabi drive update will use g_brightness (0.1 to 0.9)
    bloch_draw_qubit(g_pq, 0.2f, 0.4f, 1.0f);
    bloch_draw_qubit(g_pq2, 0.2f, 0.4f, 1.0f);
    
    // Get state for text display
    
    double omega_mhz = rabi_get_omega(&g_drive) / (2.0 * M_PI);
    double omega_mhz2 = rabi_get_omega(&g_drive2)/ (2.0 * M_PI);
    
    // Draw Text Overlay
    // Note: We display the raw brightness score (0.1 to 0.9) in the text overlay
    bloch_draw_text(g_pq->alpha, g_pq->beta, omega_mhz, g_brightness * 100.0, g_pq->index); // Display as a percentage 
    bloch_draw_text(g_pq2->alpha, g_pq2->beta, omega_mhz2, g_brightness *100.0, g_pq2->index); 
    glutSwapBuffers();
}

/**
 * @brief Handles window resizing.
 */
void reshape_func(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)w / (double)h, 0.1, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

/**
 * @brief Mouse down/up event handler for rotation.
 */
void mouse_func(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            g_isRotating = 1;
            g_lastX = x;
            g_lastY = y;
        } else {
            g_isRotating = 0;
        }
    }
}

/**
 * @brief Mouse drag event handler for rotation.
 */
void motion_func(int x, int y) {
    if (g_isRotating) {
        g_rotY += (x - g_lastX) * 0.5f;
        g_rotX += (y - g_lastY) * 0.5f;
        g_lastX = x;
        g_lastY = y;
        glutPostRedisplay(); // Request a redraw
    }
}
/**
 * @brief Checks for user input to measure the qubit.
 */
//TODO
/**
 * @brief The idle function runs when no events are handled (main loop update).
 */
void idle_func(void) {
    // 1. Get New Brightness (value between 0.1 and 0.9)
    double new_brightness_ratio = get_brightness_from_camera();
    
    
    // 2. Update Rabi Drive
    rabi_set_brightness(&g_drive, new_brightness_ratio); // Use the 0-1 ratio directly
    rabi_set_brightness(&g_drive2, new_brightness_ratio);
    
    // 3. Step the Qubit Simulation
    rabi_step(&g_drive, g_pq);
    rabi_step(&g_drive2, g_pq2);
        //debug for g_pq;
        printf("[IDLE] g_pq=%p measured=%d POST alpha=%.9f+%.9fi beta=%.9f+%.9fi t=%.6f\n",
       (void*)g_pq, g_pq->measured,
       creal(g_pq->alpha), cimag(g_pq->alpha),
       creal(g_pq->beta),  cimag(g_pq->beta),
       g_drive.t);
        fflush(stdout);
        //debug for g_pq2
        printf("[IDLE] g_pq2=%p measured=%d POST alpha=%.9f+%.9fi beta=%.9f+%.9fi t=%.6f\n",
       (void*)g_pq2, g_pq2->measured,
       creal(g_pq2->alpha), cimag(g_pq2->alpha),
       creal(g_pq2->beta),  cimag(g_pq2->beta),
       g_drive.t);
        fflush(stdout);
    // 4. Request Redraw
    glutPostRedisplay();
    // 5. Check for Measurement Key Press
   // check_measurement_key(g_pq);
   // check_measurement_key(g_pq2);
}


// --- Main Function ---

int main(int argc, char **argv) {
    
    // --- Initialize Simulation State ---
    srand((unsigned int)time(NULL));
    
    // Initialize Qubit (in superposition for testing)
    g_pq = (pqubit)malloc(sizeof(qubit));
    g_pq2 = (pqubit)malloc(sizeof(qubit));
    qubit_init(g_pq, index_qbit);
    index_qbit++;
    //this is necessary so that both pq2 and pq are not the same lol. unfortunate, but that's how it works !
    qubit_init(g_pq2, index_qbit);
    index_qbit++;
    g_pq2->alpha = 0.3312 + 0.111*I;
    g_pq2->beta = 0.992 -0.852*I;


    // Initialize Rabi drive
    double max_rabi_freq_mhz = 20.0; // Max freq in MHz
    double dt_per_step = 0.05;      // Timestep per frame
    rabi_init(&g_drive2, max_rabi_freq_mhz-3.0, dt_per_step);
    rabi_init(&g_drive, max_rabi_freq_mhz, dt_per_step+0.01);
    
    // Register cleanup function
    atexit(cleanup);

    // --- Initialize GLUT ---
    printf("Initializing 3D Visualizer...\n");
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("AI Qubit Rabi Simulator");

    // --- Register Callbacks ---
    glutDisplayFunc(display_func);
    glutReshapeFunc(reshape_func);
    glutMouseFunc(mouse_func);
    glutMotionFunc(motion_func);
    glutIdleFunc(idle_func); // The main update loop

    // --- OpenGL Initialization ---
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Dark background
    glEnable(GL_DEPTH_TEST);
    
    // --- Start Main Loop ---
    printf("--- AI Driven Qubit Rabi Simulation ---\n");
    printf("Rotate the view with the mouse.\n");
    printf("The Qubit is driven by your camera's detected image quality.\n");
    
    glutMainLoop(); // GLUT takes over the main loop

    return 0; // Should be unreachable
}
