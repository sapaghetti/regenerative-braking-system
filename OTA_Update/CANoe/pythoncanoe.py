import time
from py_canoe import CANoe

canoe_inst = CANoe()
#canoe_inst.open(canoe_cfg=r'C:\Users\Public\Documents\Vector\CANoe\canoe16Project\pythoncanoe.cfg')
#canoe_inst.open(canoe_cfg=r'C:\Users\Public\Documents\Vector\CANoe\Projects\Platooning_SpeedSim\Platooning_SpeedSim.cfg')
canoe_inst.open(canoe_cfg=r'C:\Users\Public\Documents\Vector\CANoe\canoe19_project\ota_project\ota_project.cfg')

canoe_inst.start_measurement()
time.sleep(5)
a = 0
while True:
    a = a + 1
    time.sleep(1)
    #canoe_inst.set_system_variable_value('OTA::Ota_flag', a)
    if a > 10:
        break

time.sleep(5)
canoe_inst.stop_measurement()
#canoe_inst.quit()  # CANoe 종료
