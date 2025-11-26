# 🌀 Quantus V2 — AI-Driven Qubit Behavior Simulator

**Quantus V2** is a real-time **two-qubit simulation environment** designed to visualize and study qubit dynamics on a Bloch sphere.  
The system couples a **Python-based AI vision module** with a **C-based quantum state engine**, allowing environmental light intensity to modulate quantum state evolution through Rabi-type interactions.

---

## ✨ Overview
Quantus V2 explores the intersection of **quantum computation**, **computer vision**, and **real-time systems**.  
Instead of using a fixed or analytic Hamiltonian, the simulator employs a live brightness metric derived from the user’s camera to drive the qubit’s effective Rabi frequencies.  
This approach links real-world optical input to simulated quantum behavior, producing an intuitive visualization of light-driven state transitions.

---

## 🧩 System Architecture

Python AI Module(OpenCV + NumPy) → Computes brightness ∈ [0,1] → (stdout pipe) → C Simulation Core → Quantum state evolution → Rabi dynamics (X, Y, Z) → Measurement logic → Adds Detuning(Randomness)  → OpenGL Bloch rendering

---

## ⚛️ Simulation Model
Each qubit state is represented as  

**|ψ⟩ = α|0⟩ + β|1⟩,   |α|² + |β|² = 1**

The simulator applies discrete-time Rabi rotations on both the X-, Y- and Z- axes of the Bloch sphere.
The rotation amplitudes are scaled by the AI-derived brightness value, effectively treating environmental illumination as a dynamic drive field. 
State evolution, and normalization are handled within the C core.

---

## 🖥️ Features
- Two-qubit real-time Bloch-sphere visualization (OpenGL)  
- Physically consistent state normalization and measurement  
- AI-driven Rabi modulation via live camera input  
- C/Python inter-process communication through standard I/O  
- Detailed debug logging of α, β amplitudes and measurement outcomes  

---

## 🛠️ Requirements
*Must be compiled on MSYS MINGW64, due to freeglut libraries. If you have them on windows, just run the .exe file that's in the source code folder.*
**Python side**
```bash
pip install opencv-python numpy Pillow
# Build simulation core
gcc main.c rabi.c qubit.c bloch.c -o QubitSim -lfreeglut -lopengl32 -lglu32 -lws2_32 -lwinmm -lm

# Run AI module and simulator
./QubitSim
```
## Example output

[IDLE] g_pq2=0000013201631760 measured=-1 POST alpha=0.582977708+-0.397621177i beta=-0.622230643+-0.339152999i t=64.200000
[IDLE] g_pq=0000013201631720 measured=-1 POST alpha=-0.399235658+-0.620586727i beta=0.106103900+-0.666502037i t=64.260000
[IDLE] g_pq2=0000013201631760 measured=-1 POST alpha=0.234273087+0.088455730i beta=-0.732979433+-0.632609038i t=64.260000
[IDLE] g_pq=0000013201631720 measured=-1 POST alpha=-0.775140888+-0.437441509i beta=-0.453577875+-0.045482320i t=64.320000
[IDLE] g_pq2=0000013201631760 measured=-1 POST alpha=-0.221300782+0.534181449i beta=-0.509362199+-0.637485399i t=64.320000
[IDLE] g_pq=0000013201631720 measured=-1 POST alpha=-0.473762045+0.127920892i beta=-0.616943219+0.615277851i t=64.380000
[IDLE] g_pq2=0000013201631760 measured=-1 POST alpha=-0.575923029+0.736227579i beta=-0.053387267+-0.351557613i t=64.380000
[IDLE] g_pq=0000013201631720 measured=-1 POST alpha=0.241569270+0.581511635i beta=-0.241250668+0.738435252i t=64.440000
[IDLE] g_pq2=0000013201631760 measured=-1 POST alpha=-0.667824271+0.602425966i beta=0.426941538+0.094741550i t=64.440000
[IDLE] g_pq=0000013201631720 measured=-1 POST alpha=0.745827964+0.527003045i beta=0.345236124+0.216380354i t=64.500000
[IDLE] g_pq2=0000013201631760 measured=-1 POST alpha=-0.455081568+0.193813402i beta=0.712510793+0.497822099i t=64.500000
[IDLE] g_pq=0000013201631720 measured=-1 POST alpha=0.598414932+0.012022335i beta=0.630070750+-0.494738196i t=64.560000
[IDLE] g_pq2=0000013201631760 measured=-1 POST alpha=-0.034742570+-0.303211761i beta=0.673051282+0.673809242i t=64.560000


<img width="957" height="740" alt="image" src="https://github.com/user-attachments/assets/3f28be39-2580-42b6-b2e2-674342890c66" />



##📚 Future Work

1. Add decoherence. 

2. Implement explicit measurement-collapse dynamics

3. Extend to coupled-qubit entanglement

##👥 Authors

AH (me) — Core simulation, C implementation, visualization, and integration

KH — AI vision module for brightness estimation

##📄 License

MIT License — see LICENSE for details.


