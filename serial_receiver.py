"""Receiving serial data from the NodeMCU connected to the water meter"""
import serial

class SerialReceiver:
    """Serial receiver class"""
    def __init__(self):
        """Init"""
        self._ser = serial.Serial()

    def _parseAndSendData(self,receivedRaw):
        """Parse serial data and send over TCP/IP"""
        pulses, qVolumeLPerSStr, qVolumeM3PerHStr = receivedRaw.strip().split()
        qVolumeLPerS = float(qVolumeLPerSStr)
        qVolumeM3PerH = float(qVolumeM3PerHStr)
        print(f"{pulses} {qVolumeLPerS} {qVolumeM3PerH}")
        print(repr(receivedRaw))

        
    def connectAndRun(self, port):
        """Connect to port and run receiver"""
        self._ser.baudrate = 115200
        self._ser.timeout = 1
        self._ser.port = port
        self._ser.open()

        while(True):
            receivedRaw = self._ser.readline()
            if receivedRaw:
                self._parseAndSendData(receivedRaw)
        
def run():
    """Run main functionality"""
    serialReceiver=SerialReceiver()
    serialReceiver.connectAndRun(port="/dev/ttyUSB0")
    
if __name__=="__main__":
    run()
    
