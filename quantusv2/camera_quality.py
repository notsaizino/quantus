import cv2
import numpy as np
import math
import sys
import time

def calculate_image_quality(frame):
    """Calculate image quality using computer vision metrics"""
    
    # 1. Brightness (0-1 scale)
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    brightness = np.mean(gray) / 255.0
    
    # 2. Contrast (using standard deviation)
    contrast = np.std(gray) / 128.0  # Normalize
    
    # 3. Sharpness (using Laplacian variance)
    sharpness = cv2.Laplacian(gray, cv2.CV_64F).var() / 1000.0  # Normalize
    
    # 4. Noise (using median filter difference)
    median = cv2.medianBlur(gray, 5)
    noise = np.mean(np.abs(gray - median)) / 128.0
    
    # Combine metrics (you can adjust weights)
    quality_score = (
        0.4 * brightness + 
        0.3 * contrast + 
        0.2 * min(sharpness, 1.0) + 
        0.1 * max(0, 1.0 - noise)
    )
    
    return max(0.1, min(0.9, quality_score))

print("READY")
sys.stdout.flush()

cap = cv2.VideoCapture(0)
if not cap.isOpened():
    print("Error opening camera")
    exit()

frame_count = 0

try:
    while True:
        ret, frame = cap.read()
        if not ret:
            break

        # Calculate quality using computer vision
        quality_score = calculate_image_quality(frame)
        final_score = quality_score * 100
        brightness = quality_score

        # DEBUG: Print what we're sending
        print(f"DEBUG: final={final_score:.1f}%, brightness={brightness:.3f}")
        
        # Output brightness for C program
        print(f"BRIGHTNESS:{brightness:.3f}")
        sys.stdout.flush()

        # Show camera feed with info
        text = f"Quality: {final_score:.1f}% -> {brightness:.3f} (CV)"
        cv2.putText(frame, text, (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)
        cv2.imshow('Camera Quality', frame)

        frame_count += 1
        
        # Break on 'q' key
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

        # Slow down a bit so we can see changes
        time.sleep(0.1)

finally:
    cap.release()
    cv2.destroyAllWindows()
    print("TERMINATED")