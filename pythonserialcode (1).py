#!/usr/bin/env python

import serial
import argparse
import io
import os
import time
import sys
import cv2 
from google.cloud import vision

Serial_Port = serial.Serial('/dev/ttyACM0',9600)

aflag = 0
bflag = 0
cflag = 0

while(1): 
    
    data = Serial_Port.read()
    
    data_decoded=data.decode("utf-8")
    
    print("Read input    " + data_decoded + "   from Arduino")
    
    if data_decoded=='z':
        #Serial_Port.write(b'a')  
        cam= cv2.VideoCapture(0)
        ret, img=cam.read()
        cv2.imshow("cam",img)
        cv2.imwrite("/home/pi/Desktop/self.jpg",img)
        cam.release()
        cv2.destroyAllWindows()

        os.environ["GOOGLE_APPLICATION_CREDENTIALS"] = "/home/pi/Downloads/kabadevision-bf33f8d45f1e.json"
        path = "/home/pi/Desktop/self.jpg"
        client = vision.ImageAnnotatorClient()
        with io.open(path, 'rb') as image_file:
            content = image_file.read()
        image = vision.types.Image(content=content)
        response = client.label_detection(image=image)
        labels = response.label_annotations
        print('Labels:')
        for label in labels:
            print(label.description)
            if label.description == 'bottle':
                print("bottle detected")
                aflag=1
            if label.description == 'plastic':
                print("plastic detected")
                aflag=1
            if label.description == 'cup':
                print("cup detected")
                bflag=1
            if label.description == 'cylinder':
                print("cylinder detected")
                bflag=1
            if label.description == 'can':
                print("can detected")
                bflag=1
            if label.description == 'banana':
                print("banana detected")
                cflag=1
            if label.description == 'apple':
                print("bottle detected")
                cflag=1

    elif data_decoded == 'x':
        print("nothing got from arduino")
        aflag = 0
        bflag = 0
        cflag = 0
        #do nothing
    print(aflag,bflag,cflag)
    if aflag == 1:
        Serial_Port.write(b'a')
        
    if bflag == 1:
        Serial_Port.write(b'b')
    if cflag == 1:
        Serial_Port.write(b'c')
        
