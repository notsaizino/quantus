#ifndef BLOCH_H
#define BLOCH_H

#include "qubit.h"

/**
 * @brief Draws the main sphere wireframe.
 */
void bloch_draw_sphere();

/**
 * @brief Draws the X, Y, and Z axes and labels.
 */
void bloch_draw_axes();

/**
 * @brief Draws the qubit state vector.
 *
 * @param pq    The qubit to draw.
 * @param r     Red color component (0-1).
 * @param g     Green color component (0-1).
 * @param b     Blue color component (0-1).
 */
void bloch_draw_qubit(pqubit pq, float r, float g, float b);

/**
 * @brief Draws the text overlay (info).
 */
void bloch_draw_text(double complex alpha, double complex beta, double omega_mhz, double brightness, int index);

#endif
