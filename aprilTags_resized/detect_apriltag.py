import cv2
import numpy as np
import csv
from datetime import datetime
from pupil_apriltags import Detector

# === Settings ===
tag_ids_to_track = {
    'left_shoulder': 0,
    'left_elbow': 1,
    'left_wrist': 2,
    'right_shoulder': 3,
    'right_elbow': 4,
    'right_wrist': 5
}
save_filename = 'dual_arm_tracking_data.csv'
recording = False
tracked_data = []

# === Initialize ===
cap = cv2.VideoCapture(0)
detector = Detector(families='tag36h11')

print("Press 's' to start/stop recording. Press 'q' to quit.")

while True:
    ret, frame = cap.read()
    if not ret:
        break

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    tags = detector.detect(gray)

    # Draw and extract tag data
    current_frame_data = {'time': datetime.now().isoformat()}
    for tag in tags:
        for name, tag_id in tag_ids_to_track.items():
            if tag.tag_id == tag_id:
                center = tuple(int(x) for x in tag.center)
                cv2.circle(frame, center, 5, (0, 255, 0), -1)
                cv2.putText(frame, f"{name} ({tag.tag_id})", center, cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 0, 0), 1)
                # Save positions in frame data
                current_frame_data[f"{name}_x"] = tag.center[0]
                current_frame_data[f"{name}_y"] = tag.center[1]

    # Show "Recording" status on screen
    if recording:
        tracked_data.append(current_frame_data)
        cv2.putText(frame, "RECORDING", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)

    cv2.imshow("AprilTag Tracking - Dual Arm", frame)
    key = cv2.waitKey(1) & 0xFF

    if key == ord('s'):
        recording = not recording
        print("▶️ Started recording." if recording else "⏸️ Paused recording.")
    elif key == ord('q'):
        print("🛑 Quitting...")
        break

# === Save Data to CSV ===
if tracked_data:
    print(f"💾 Saving data to {save_filename}...")
    with open(save_filename, 'w', newline='') as csvfile:
        fieldnames = ['time'] + [f"{joint}_{axis}" for joint in tag_ids_to_track for axis in ['x', 'y']]
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()
        for row in tracked_data:
            writer.writerow(row)
    print("✅ Done saving.")

cap.release()
cv2.destroyAllWindows()
