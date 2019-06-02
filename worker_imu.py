import serial ,time, datetime
import pymysql as pms
from firebase import firebase

my_url='https://worker-6ef67.firebaseio.com/'
firebase = firebase.FirebaseApplication('https://worker-6ef67.firebaseio.com/', None)

IMU_ser = serial.Serial("COM21" , 9600 ,timeout = .1)
IMU_ser.flush()

i = 0
bpm_arr = []

time_temp = time.time()
while True:
    if(IMU_ser.inWaiting() > 0):
        IMU_data = IMU_ser.readline()
        IMU_data = IMU_data.strip('\n')
        IMU_data = IMU_data.strip('\r')
        #print IMU_data
        
        IMU_data = IMU_data.split(';')

        try:
            IMU_data.remove('')
        except ValueError:
            continue

        if len(IMU_data) != 4:
            continue

        if len(IMU_data[0].split(',')) != 3:
            continue

        x = float(IMU_data[0].split(',')[0].replace(" ",""))
        y = float(IMU_data[0].split(',')[1].replace(" ",""))
        z = float(IMU_data[0].split(',')[2].replace(" ",""))
        bpm = int(IMU_data[1])
        temp = float(IMU_data[2]) + 15
        magn = float(IMU_data[3])

        print "Gyro data: ",x ,",", y ,",", z ,"\n", "Heart Rate: ", bpm ,"\n", "Body Temperature: ",temp ,"\n", "Magnetic Intensity: ",magn,"\n-------------"
        
        result = firebase.patch(my_url+'/Worker1/Worker1/gg', {'x':x,})
        result = firebase.patch(my_url+'/Worker1/Worker1/gg', {'y':y,})
        result = firebase.patch(my_url+'/Worker1/Worker1/gg', {'z':z,})
        result = firebase.patch(my_url+'/Worker1/Worker1/', {'m':magn,})
        result = firebase.patch(my_url+'/Worker1/Worker1/', {'p':bpm,})
        result = firebase.patch(my_url+'/Worker1/Worker1/', {'t':temp,})

