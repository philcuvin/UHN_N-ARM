import math
import numpy as np
import time
import cv2 as cv
import matplotlib.pyplot as plt
import matplotlib.ticker as mticker
import datetime
import pandas as pd
from natsort import natsorted


from builtins import input
import argparse

import os

import PySimpleGUI as sg
import os.path

from PIL import Image, ImageTk, ImageDraw, ImageFont

from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

import csv

# Creating folder for saving video

path = "C:/Users/philc/OneDrive/Desktop" # (Change path to desired path of images)

os.chdir(path) # Changing directory to desired file path

os.mkdir("Capstone_Files") # Creating folder for images

img_path = path+"/Capstone_Files"

os.chdir(img_path) # Changing directory to desired image file path

file_avg = []

rows_skip = 7007

for filename in os.listdir(img_path):
    f = os.path.join(img_path, filename)
    # checking if it is a file
    if os.path.isfile(f):

        df = pd.read_csv(f, skiprows=rows_skip)

        df2 = df.iloc[:,1]

        df3 = df.iloc[:,3]

        flow = df2.sum()/(df2.size)

        pressure = df3.sum()/(df3.size)

        file_avg.append([str(f),flow,pressure])


file_length = []

for filename in os.listdir(img_path):
    f = os.path.join(img_path, filename)
    # checking if it is a file
    if os.path.isfile(f):

        df = pd.read_csv(f, skiprows = 7)

        df2 = df.iloc[:,1]

        length = df2.size

        file_length.append([str(f),length])


with open('Flow Averages', 'w') as f:
      
    # using csv.writer method from CSV package
    write = csv.writer(f)

    write.writerows(file_avg)