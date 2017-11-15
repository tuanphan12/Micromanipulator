import sys
import glob
import serial

def serial_ports():
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in list(range(256))]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            #print("checking port "+port)
            s = serial.Serial(port)
            #print("closing port "+port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result
#-------------------

ports = serial_ports() #generate list of currently connected serial ports 
print (ports)

ser = ports[0]

s = serial.Serial(ser)
print(s)

try:
    while True:
        string_to_write = input()
        s.write(bytes(string_to_write,'UTF-8'))

except KeyboardInterrupt:
    s.close()