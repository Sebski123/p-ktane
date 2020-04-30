ACK     = 0x86
NAK     = 0x95
START   = 0x82
END     = 0x83
WRITE   = 0xD7
READ    = 0xD2
NO_DATA = 0xB0
PING    = 0xB1
ESC     = 0x9B

MAX_MSG_LEN = 16

byte_to_str = {0x86: "ACK",
               0x95: "NAK",
               0x82: "START",
               0x83: "END",
               0xD7: "WRITE",
               0xD2: "READ",
               0xB0: "NO_DATA",
               0xB1: "PING",
               0xC0: "STRIKE",
               0xC1: "SOLVE",
               0xC2: "CONFIG",
               0xC3: "READY",
               0xC4: "RESET",
               0xC5: "NUM_STRIKES",
               }

byte_to_str_small={0x86: "A",
                   0x95: "N",
                   0x82: "S",
                   0x83: "E",
                   0xD7: "W",
                   0xD2: "R",
                   0xB0: "ND",
                   0xB1: "P",
                   0xC0: "XXX",
                   0xC1: "YYY",
                   0xC2: "C",
                   0xC3: "G",
                   0xC4: "R",
                   0xC5: "#S",
                   }

def bytes_to_msgs(client_id, message_bytes):
    converted_bytes = []
    short_converted_bytes = []
    for byte in message_bytes:
        if(byte in byte_to_str):
            converted_bytes.append(byte_to_str[byte])
        else:
            converted_bytes.append(str(byte))
        if(byte in byte_to_str_small):
            short_converted_bytes.append(byte_to_str_small[byte])
        else:
            short_converted_bytes.append(str(byte))

    msg = str(client_id) + ":" + " ".join(converted_bytes)
    short_msg = str(client_id) + ":" + " ".join(short_converted_bytes)
    return [msg, short_msg]

def readPacket(msg: list):
  in_packet = 0
  index = 0
  data_parity = 0
  escape_next = 0
  passed_parity = 0
  buf = []

  message = msg.copy()
  message.reverse()


  while (len(message) > 0):
    rc = message.pop()
    if (rc == START):
      in_packet = 1
      data_parity = START
      index = 0
    
    elif (in_packet == 1):
      data_parity ^= rc
      if (rc != END):
        if(rc == ESC):  # next char was greater than 0x7F, add 0x80 to it.
          escape_next = 1
        else:
          buf.append(rc | (escape_next << 7))
          escape_next = 0
          index += 1
        
      
      if(rc == END or index >= MAX_MSG_LEN):
        index -= 1
        buf[index] = '\0' #purposefully overwrite parity byte.
        passed_parity = ((data_parity & 0x7F) == 0)
        index = 0
        in_packet = 0
        data_parity = 0
        if(passed_parity):
          print("passed parity check")
          return buf
        else:
          print("failed parity check") 
          return buf #should be -1
  return 0

def convertToHex(str: str):
  fin_lst = []
  lst = str.split(" ")
  for byte in lst:
    fin_lst.append(int(byte, 16))
  return fin_lst

string = "82 01 9B 52 52 83"
test = convertToHex(string)
print(test)
test2 = readPacket(test)
test3 = bytes_to_msgs(1,test2)
print(test3[0])