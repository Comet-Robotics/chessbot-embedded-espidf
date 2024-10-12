from __future__ import annotations
from enum import Enum
import socket
import dearpygui.dearpygui as dpg

from admin.movement_keys import bind_movement_keys
from admin.packet import Packet
from admin.socket_tools import AsyncSocket, read_packets, send_nonblocking_as_blocking
from admin.gui_decl import GuiRobot

JSON_PORT = 80
DEFAULT_IP = '192.168.222.32'

class ConnStatus(Enum):
    EMPTY = "Empty"
    CONNECTING = "Connecting"
    CONNECTED = "Connected"
    DISCONNECTED = "Disconnected"

class RobotConnection:
    active: RobotConnection = None

    sock: socket.socket
    status: str
    ip: str
    macAddress: None | str

    def __init__(self, ip):
        self.ip = ip
        self.status = ConnStatus.EMPTY
        self.macAddress = None

        print('Connecting to', ip)

        self.sock = AsyncSocket(socket.socket(socket.AF_INET, socket.SOCK_STREAM))
        self.sock.s.setblocking(False)

        self.setStatus(ConnStatus.CONNECTING)

        #todo: allow cancel
        status = self.sock.s.connect_ex((ip, JSON_PORT))
        while status != 0:
            status = self.sock.s.connect_ex((ip, JSON_PORT))

    def setStatus(self, newStatus: ConnStatus):
        self.status = newStatus
        dpg.set_value(GuiRobot.conn_status_text, "Connection Status: " + newStatus.value)

    def onHello(self, p: Packet):
        if self.macAddress is not None:
            print('Client sent hello more than once?')

        self.macAddress = p.data['macAddress']
        self.setStatus(ConnStatus.CONNECTED)

        dpg.set_value(GuiRobot.mac_text, "Robot MAC Address: " + self.macAddress)

        bind_movement_keys(RobotConnection.active)

    def tick(self):
        packets = read_packets(self.sock)
        for p in packets:
            if p.type == Packet.Type.CLIENT_HELLO:
                self.onHello(p)

    def send(self, msg: str):
        send_nonblocking_as_blocking(self.sock.s, msg)

activeConnection: RobotConnection