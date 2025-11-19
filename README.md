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
[IDLE] g_pq=00000218eb966d40 measured=-1 POST alpha=0.172783291+0.091203483i beta=0.978562603+0.065139016i t=60.060000
[IDLE] g_pq2=00000218eb966d80 measured=-1 POST alpha=-0.063192142+-0.038179948i beta=0.966260180+-0.246759617i t=60.060000
[IDLE] g_pq=00000218eb966d40 measured=-1 POST alpha=-0.433670577+-0.345458904i beta=0.491931447+-0.671261073i t=60.120000
[IDLE] g_pq2=00000218eb966d80 measured=-1 POST alpha=-0.615924902+-0.399551476i beta=0.439179992+-0.517799255i t=60.120000
[IDLE] g_pq=00000218eb966d40 measured=-1 POST alpha=-0.460066064+-0.320050901i beta=-0.652685227+-0.509812351i t=60.180000
[IDLE] g_pq2=00000218eb966d80 measured=-1 POST alpha=-0.683691392+-0.446324605i beta=-0.433701254+-0.381134687i t=60.180000
[IDLE] g_pq=00000218eb966d40 measured=-1 POST alpha=0.131483117+0.131451818i beta=-0.922789830+0.337478207i t=60.240000
[IDLE] g_pq2=00000218eb966d80 measured=-1 POST alpha=-0.210153985+-0.141833913i beta=-0.965591761+0.057887779i t=60.240000

<img width="797" height="821" alt="image" src="https://github.com/user-attachments/assets/4c067ef6-056b-44db-a1ff-066614ff620f" />


##📚 Future Work

1. Add decoherence. 

2. Implement explicit measurement-collapse dynamics

3. Extend to coupled-qubit entanglement

##👥 Authors

AH (me) — Core simulation, C implementation, visualization, and integration

KH — AI vision module for brightness estimation

##📄 License

MIT License — see LICENSE for details.


