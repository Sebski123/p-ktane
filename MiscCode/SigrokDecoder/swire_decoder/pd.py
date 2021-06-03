##
# This file is part of the libsigrokdecode project.
##
# Copyright (C) 2014 Matt Ranostay <mranostay@gmail.com>
##
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
##
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
##
# You should have received a copy of the GNU General Public License
# along with this program; if not, see <http://www.gnu.org/licenses/>.
##

import sigrokdecode as srd

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
               0xC6: "NUM_SOLVES",
               0xC7: "TIME",
               0xC8: "STOP",
               0xC9: "SEED"
               }

byte_to_str_small = {0x86: "A",
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
                     0xC3: "RDY",
                     0xC4: "R",
                     0xC5: "#X",
                     0xC6: "#S",
                     0xC7: "T",
                     0xC8: "Ø",
                     0xC9: "SD"
                     }

id_to_module_type = {0x1: "Widget",
                     0x2: "Button",
                     0x3: "Complicated Wires",
                     0x4: "Maze",
                     0x5: "Memory",
                     0x6: "Morse Code",
                     0x7: "Needy Capacitor Discharge",
                     0x8: "Needy Knob",
                     0x9: "Needy Venting Gas",
                     0xA: "Password",
                     0xB: "Simon Says",
                     0xC: "Simple Wires"
                     }


def bytes_to_config(stripped_bytes):
    config_data = stripped_bytes[1:]
    ports = config_data[0] & 0b1
    batteries = (config_data[0] >> 1) & 0b11111
    indicators = (config_data[0] >> 6) & 0b11
    serial = "".join([chr(x) for x in config_data[1:3]])
    serial += chr(ord('0') + ((config_data[5]) & 0b1111))
    serial += "".join([chr(x) for x in config_data[3:5]])
    serial += chr(ord('0') + ((config_data[5] >> 4) & 0b1111))
    config_str = "%d-%d-%d-%s" % (ports, batteries, indicators, serial)
    return config_str


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

    msg = id_to_module_type[client_id] + ":" + " ".join(converted_bytes)
    short_msg = "0x" + str(client_id) + ":" + " ".join(short_converted_bytes)
    return [msg, short_msg]


class Decoder(srd.Decoder):
    api_version = 3
    id = 'swire'
    name = 'SWire'
    longname = 'SWire'
    desc = 'A layer 3 protocol used for the p-ktane project by Sebski123.'
    license = 'gplv2+'
    inputs = ['i2c']
    outputs = ['swire']
    tags = ['custom']
    options = (
        {'id': 'show_nodata', 'desc': 'Show "No Data"?', 'default': 'yes',
            'values': ('yes', 'no')},
    )
    annotations = (
        ('packet_ms', 'SWire Packet Master'),
        ('packet_sl', 'SWire Packet Slave'),
    )
    annotation_rows = (
        ('messages', 'Messages', (0, 1,)),
    )

    def __init__(self):
        self.reset()

    def reset(self):
        self.state = 'IDLE'
        self.current_address = 0x0
        self.startmsg = 0
        self.endmsg = 0
        self.prevmsg = [0, 0, [""]]
        self.dbmss = ""
        self.readData = 0

        self.message_bytes_ms = []
        self.escape_next_ms = False
        self.escape_parity_ms = 0

    def start(self):
        self.out_ann = self.register(srd.OUTPUT_ANN)

    def putxms(self, data):
        self.put(self.startmsg, self.endmsg, self.out_ann, data)

    def generatemsg(self):
        parity = 0
        msgs = [""]
        if self.readData:
            if self.message_bytes_ms[0] == 0x00:
                msgs = ["Empty"]
                if(self.options["show_nodata"] == "yes"):
                    self.putxms([1, msgs])
                    self.startmsg = self.prevmsg[0]
                    self.endmsg = self.prevmsg[1]
                    self.putxms([0, self.prevmsg[2]])
                    self.prevmsg = [0, 0, [""]]
            else:
                if(0xff in self.message_bytes_ms):
                    idx = self.message_bytes_ms.index(0xff)
                    msgs = bytes_to_msgs(self.current_address,
                                         self.message_bytes_ms[1:idx])
                elif(self.message_bytes_ms[1] == 0xC2):
                    stripped_bytes = self.message_bytes_ms[1:]
                    config_str = bytes_to_config(stripped_bytes)
                    msgs = [id_to_module_type[self.current_address] + ":WRITE CONFIG " +
                            config_str, "0x" + str(self.current_address) + ":W C " + config_str]
                self.putxms([1, msgs])
                self.startmsg = self.prevmsg[0]
                self.endmsg = self.prevmsg[1]
                self.putxms([0, self.prevmsg[2]])
                self.prevmsg = [0, 0, [""]]
            self.readData = 0
        else:
            for i in self.message_bytes_ms:
                parity ^= i
            if(self.escape_parity_ms):
                parity = parity ^ 0x9B

            if(self.message_bytes_ms[0] != 0x82):
                msgs = ["ERR: NO START"]
            elif(self.message_bytes_ms[-1] != 0x83):
                msgs = ["ERR: NO END"]
            elif(parity & 0x7F != 0):
                msgs = ["ERR: BAD PARITY %02x" % parity]  # [self.dbmss]
            else:
                stripped_bytes = self.message_bytes_ms[2:-2]
                if(len(stripped_bytes) > 1 and stripped_bytes[0] == 0xC2):
                    config_str = bytes_to_config(stripped_bytes)
                    msgs = [id_to_module_type[self.current_address] + ":WRITE CONFIG " +
                            config_str, "0x" + str(self.current_address) + ":W C " + config_str]
                else:
                    msgs = bytes_to_msgs(
                        self.message_bytes_ms[1], stripped_bytes)

            if self.message_bytes_ms[2] in byte_to_str and byte_to_str[self.message_bytes_ms[2]] == 'READ':
                self.readData = self.current_address

            if not self.readData:
                self.putxms([0, msgs])
            else:
                self.prevmsg = [self.startmsg, self.endmsg, msgs]

        self.message_bytes_ms = []
        self.state = 'IDLE'
        self.dbmss = ""

    def decode(self, ss, es, data):
        cmd, databyte = data

        if cmd == "STOP" and self.state != "IDLE":
            self.endmsg = es
            if self.state == 'MSG READ' and self.current_address == self.readData:
                self.generatemsg()
            self.state = "IDLE"

        # State machine.
        if self.state == 'IDLE':
            # Wait for an I²C START condition.
            if cmd != 'START':
                return
            self.startmsg = ss
            self.state = 'GET SLAVE ADDR'

        elif self.state == 'GET SLAVE ADDR':
            # Wait for an address read/write operation.
            if cmd in ('ADDRESS READ', 'ADDRESS WRITE'):
                self.current_address = databyte
                self.state = 'GET ACK'

        elif self.state == 'GET ACK':
            if cmd == 'NACK':
                self.state = 'IDLE'
                return
            elif cmd == 'ACK':
                self.state = 'MSG READ'

        elif self.state == 'MSG READ':
            if self.current_address == self.readData:
                if databyte != None and type(databyte) != type([]):
                    self.dbmss += "%02x " % databyte
                    self.message_bytes_ms.append(databyte)

            else:
                if len(self.message_bytes_ms) == 0:
                    self.escape_parity_ms = 0
                    if(databyte != 0x82):
                        self.es = es
                        # self.putxms([0, ["BAD"]]) not needed with I2C
                        return

                if(databyte == 0x9B):
                    self.escape_next_ms = True
                    self.escape_parity_ms = 1 - self.escape_parity_ms
                    return

                if(self.escape_next_ms):
                    self.escape_next_ms = False
                    databyte += 0x80

                if databyte != None and type(databyte) != type([]):
                    self.dbmss += "%02x " % databyte
                    self.message_bytes_ms.append(databyte)

                if databyte == 0x83:
                    self.endmsg = es
                    self.generatemsg()
