import serial

ser = serial.Serial()
ser.port = "/dev/ttyUSB0"
ser.baudrate = "9600"
ser.open()

def handle_command_ext_flash_write(cmd):
    ser = serial.Serial()
    ser.port = "/dev/ttyUSB0"
    ser.baudrate = "9600"
    ser.open()

    message_name = 1
    message_size = 256
    flash_address0 = 0
    flash_address2 = 0

    termination_char = "\r"

    cmd_bytes = bytearray(5)

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
    
    while True:
    
        while file_size > 0:
            if file_size > message_size:
                current_size = message_size
                file_size -= message_size
            else:
                current_size = file_size
                file_size = 0

            data = f.read(current_size)

            data_bytes.clear()
            data_bytes.extend(data)

            

            cmd_bytes.clear()
            cmd_bytes.extend(message_name.to_bytes(1,'little'))
            cmd_bytes.extend(flash_address2.to_bytes(1,'little'))
            cmd_bytes.extend((count-1).to_bytes(1,'little'))
            cmd_bytes.extend(flash_address0.to_bytes(1,'little'))
            cmd_bytes.extend((current_size-1).to_bytes(1,'little'))

            print(f"Current count: {count}. Current Size: {current_size}")
            count +=1

            print(cmd_bytes)
            ser.write(cmd_bytes)
            res = ser.readline()
            print(res.decode(encoding='utf-8',errors='ignore').strip('\r\n'))

            print(data_bytes)
            ser.write(data_bytes)
            res = ser.readline()
            print(res.decode(encoding='utf-8',errors='ignore').strip('\r\n'))

        
        if(file_size == 0):
            return

while (True):
    cmd = input("$ ")
    if cmd != "":
        cmd_parts = cmd.split(); 
        if cmd_parts[0] == "ext_flash_write":
            handle_command_ext_flash_write(cmd)
        elif cmd == "exit":
            exit(0)
