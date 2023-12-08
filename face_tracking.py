import cv2
import serial
import time

def send_and_receive(ser, data):
    ser.flushInput()
    ser.flushOutput()
    ser.write(data.encode())
    time.sleep(0.1)
    # Removed the delay to prevent blocking camera updates
    # received_data = ser.read(ser.inWaiting()).decode()
    # return received_data
    return 0 

# ESP32-CAM video stream URL, replace with your actual ESP32-CAM URL
stream_url = "http://192.168.0.114:81/stream"

# Open serial port
# Replace '/dev/ttyUSB0' with your serial port name
ser = serial.Serial('/dev/tty.usbmodem2101', 9600, timeout=1)

# Create a VideoCapture object
# cap = cv2.VideoCapture(stream_url)

# Create a VideoCapture object for the default camera
cap = cv2.VideoCapture(0)

# Load the Haar Cascade for face detection
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

# Check if the video stream is opened successfully
if not cap.isOpened():
    print("Cannot open stream")
    exit()

# Threshold for determining "close to edge"
edge_threshold = 20  # You can adjust this value

# Initialize a variable to store the last time a signal was sent
last_signal_time = time.time()  # Set the initial value to the current time

# Loop to read frames from the video stream
while True:
    ret, frame = cap.read()  # Read a frame
    if not ret:
        print("Can't receive frame (stream end?). Exiting ...")
        break

    # Convert frame to grayscale
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Detect faces in the frame
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))
    # Calculate the center of the frame
    frame_center_x = frame.shape[1] // 2
    frame_center_y = frame.shape[0] // 2

    face_detected = False 

    # Check if 5 seconds have passed since the last signal
    # if time.time() - last_signal_time >= 5:
    for (x, y, w, h) in faces:
            
            face_detected = True

            # Calculate face center
            face_center_x = x + w // 2
            face_center_y = y + h // 2
            # Draw rectangles around the faces
            cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 0), 2)

            # Check if the face is close to the edge
            if face_center_x > frame_center_x + edge_threshold:
                received_data = send_and_receive(ser, "1"+str(abs(frame_center_x-face_center_x))+"\n")
                print("Sent:"+"1"+str(abs(frame_center_x-face_center_x))+"\n")
                print("Received:", received_data)
                # last_signal_time = time.time()  # Update the last signal time
            elif face_center_x < frame_center_x - edge_threshold:
                received_data = send_and_receive(ser, "2"+str(abs(frame_center_x-face_center_x))+"\n")
                print("Sent:"+"2"+str(abs(frame_center_x-face_center_x))+"\n")
                print("Received:", received_data)
                # last_signal_time = time.time()  # Update the last signal time
            elif abs(face_center_x - frame_center_x) <= edge_threshold:
                received_data = send_and_receive(ser, "0000\n")
                print("Sent: center")
                print("Received:", received_data)
                # last_signal_time = time.time()  # Update the last signal time
    
    if not face_detected:

        received_data = send_and_receive(ser, "0000\n")
        print("Sent: center")
        print("Received:", received_data)

    # Display the frame with detected faces
    cv2.imshow('ESP32-CAM Stream', frame)

    # Press 'q' to exit the loop
    if cv2.waitKey(1) == ord('q'):
        break

# Release the VideoCapture object and close all windows
cap.release()
cv2.destroyAllWindows()
ser.close()  # Close serial connection
