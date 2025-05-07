import cv2
from pupil_apriltags import Detector

# Create detector
detector = Detector(families='tag36h11')

# Open webcam
cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    tags = detector.detect(gray)

    for tag in tags:
        # Draw center
        center = tuple(int(i) for i in tag.center)
        cv2.circle(frame, center, 5, (0, 255, 0), -1)

        # Draw outline
        for i in range(4):
            pt1 = tuple(int(x) for x in tag.corners[i])
            pt2 = tuple(int(x) for x in tag.corners[(i + 1) % 4])
            cv2.line(frame, pt1, pt2, (255, 0, 0), 2)

        # Tag ID
        cv2.putText(frame, f"ID: {tag.tag_id}", center, cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)

    cv2.imshow("AprilTag Detection", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
