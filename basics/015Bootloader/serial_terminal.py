import serial

ser = serial.Serial()
ser.port = "/dev/ttyUSB0"
ser.baudrate = "9600"
ser.open()
cmd_list = ["version", "start_app", "sector_erase", "mass_erase", "data_read", "data_write"]

def handle_command_flash_image(cmd):
    ser = serial.Serial()
    ser.port = "/dev/ttyUSB0"
    ser.baudrate = "9600"
    ser.open()

    message_size = 1

    cmd_parts = cmd.split()
    filename = cmd_parts[1]
    
    f = open(filename,"rb")
    file_data = f.read()
    file_size = len(file_data)
    print(f"File size is {file_size}")
    f.close()

    f = open(filename,"rb")

    count = 1
    data_bytes = bytearray(message_size)
    termination_char = "\r"
    while True:
    
        while file_size > 0:
            if file_size > message_size:
                current_size = message_size
                file_size -= message_size
            else:
                current_size = file_size
                file_size = 0

            data = f.read(current_size)

            #data = b'A'

            #current_size = 1
            cmd_string = "flash_image" + " " + str(count-1) + " " + str(current_size) + " "
            
            data_bytes.clear()
            data_bytes.extend(bytes(cmd_string,'utf-8'))
            data_bytes.extend(data)
            data_bytes.extend(bytes(termination_char,'utf-8'))

            print(data_bytes)

            ser.write(data_bytes)
            res = ser.readline()
            print(res.decode(encoding='utf-8',errors='ignore').strip('\r\n'))

            print(f"Current count: {count}. Current Size: {current_size}")
            count +=1
        
        if(file_size == 0):
            return

while (True):
    cmd = input("$ ")
    if cmd != "":
        cmd_parts = cmd.split(); 
        if cmd_parts[0] in cmd_list:
            cmd_string = cmd + '\r'
            ser.write(bytes(cmd_string,'utf-8'))
            res = ser.readline()
            print(res.decode(encoding='utf-8',errors='ignore').strip('\r\n'))
        elif cmd_parts[0] == "flash_image":
            handle_command_flash_image(cmd)
        elif cmd == "exit":
            exit(0)
