"""Receiving serial data from the NodeMCU connected to the water meter"""
import serial
import publisher

class SerialReceiver:
    """Serial receiver class"""
    def __init__(self):
        """Init"""
        self._ser = serial.Serial()

    def _parseAndSendData(self,receivedRaw):
        """Parse serial data and send over TCP/IP"""
        pulsesStr, qVolumeLPerSStr, qVolumeM3PerHStr = receivedRaw.strip().split()
        pulses = int(pulsesStr)
        qVolumeLPerS = float(qVolumeLPerSStr)
        qVolumeM3PerH = float(qVolumeM3PerHStr)
        print(f"{pulses} {qVolumeLPerS} {qVolumeM3PerH}")

        dataObj = {'pulses': pulses,
                   'qVolumeLPerS': qVolumeLPerS,
                   'qVolumeM3PerH': qVolumeM3PerH}
        self._publisher.send({'waterMeter': dataObj})
        
    def connectAndRun(self, port, zmqPort):
        """Connect to port and run receiver"""
        self._ser.baudrate = 115200
        self._ser.timeout = 1
        self._ser.port = port
        self._ser.open()

        self._publisher = publisher.Publisher()
        self._publisher.open(port=zmqPort)
        
        while(True):
            receivedRaw = self._ser.readline()
            if receivedRaw:
                self._parseAndSendData(receivedRaw)
        
def run():
    """Run main functionality"""
    serialReceiver=SerialReceiver()
    serialReceiver.connectAndRun(port="/dev/ttyUSB0",
                                 zmqPort=22202)
    
if __name__=="__main__":
    run()
    
