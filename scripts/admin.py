import pip
import socket
import time
import json

def import_or_install(package):
    try:
        return __import__(package)
    except ImportError:
        pip.main(['install', package])
        return __import__(package)

dearpygui = import_or_install("dearpygui")
import dearpygui.dearpygui as dpg

JSON_PORT = 80
DEFAULT_IP = '192.168.222.32'

KEY_W = 87
KEY_A = 65
KEY_S = 83
KEY_D = 68

def parse_packet(packet: str):
    if packet.endswith(';'):
        packet = packet[:-1]

    data = json.loads(packet)

    return data

robot_ip_box = None
robot_mac_text = None

robot_ip = None
sock = None
sock_data = ""
line = ""
def read_until_newline():
    global sock_data, line
    chunk = None

    while True:
        try:
            chunk = None
            chunk = sock.recv(1024)
        except BlockingIOError:
            pass

        if chunk is not None and len(chunk) != 0:
            print('Got some data: ', chunk.decode('utf-8'))
            
            sock_data += chunk.decode('utf-8')
            if ';' in sock_data:
                pos = sock_data.find(';')
                line = sock_data[0:pos]
                sock_data = sock_data[pos:]
                break

        time.sleep(0.01)

def send_nonblocking(message):
    global sock

    if sock == None:
        return

    message_bytes = message.encode('utf-8')
    total_sent = 0
    message_len = len(message_bytes)

    while total_sent < message_len:
        try:
            sent = sock.send(message_bytes[total_sent:])
            if sent == 0:
                raise RuntimeError("Socket connection broken")
            total_sent += sent
        except BlockingIOError:
            # The socket is not ready for writing, wait and try again
            time.sleep(0.1)
 
def connect_to_robot(sender, app_data, user_data):
    global robot_ip, sock, line

    robot_ip = dpg.get_value(user_data)

    print('Connecting to', robot_ip)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((robot_ip, JSON_PORT))
    sock.setblocking(0)

    # Expect a hello
    read_until_newline()
    print('output', line)

    packet = parse_packet(line)
    if packet['type'] != "CLIENT_HELLO":
        print('Incorrect first packet from robot!')

    macAddress = packet['macAddress']
    dpg.set_value(robot_mac_text, "Robot MAC Address: " + macAddress)

dpg.create_context()

is_w_held = None
is_a_held = None
is_s_held = None
is_d_held = None

def key_down_callback(sender, app_data, user_data):
    global is_w_held, is_a_held, is_s_held, is_d_held

    if app_data[1] == 0: # when first pressed
        print(f"Key pressed: {app_data}")

        if app_data[0] == KEY_W:
            is_w_held = True
            send_nonblocking('{"type":"DRIVE_TANK","left":1,"right":1};')
        elif app_data[0] == KEY_A:
            is_a_held = True
            send_nonblocking('{"type":"DRIVE_TANK","left":-1,"right":1};')
        elif app_data[0] == KEY_S:
            is_s_held = True
            send_nonblocking('{"type":"DRIVE_TANK","left":1,"right":-1};')
        elif app_data[0] == KEY_D:
            is_d_held = True
            send_nonblocking('{"type":"DRIVE_TANK","left":-1,"right":-1};')

def key_up_callback(sender, app_data, user_data):
    global is_w_held, is_a_held, is_s_held, is_d_held

    print(f"Key pressed: {app_data}")
    if app_data == KEY_W:
        is_w_held = False
    elif app_data == KEY_A:
        is_a_held = False
    elif app_data == KEY_S:
        is_s_held = False
    elif app_data == KEY_D:
        is_d_held = False
    
    if not is_w_held and not is_a_held and not is_s_held and not is_d_held:
        send_nonblocking('{"type":"DRIVE_TANK","left":0,"right":0};')

with dpg.handler_registry():
    dpg.add_key_down_handler(callback=key_down_callback)
    dpg.add_key_release_handler(callback=key_up_callback)

with dpg.window(label="ChessBot Local Admin"):
    dpg.add_text("Robot Connection Information")
    robot_ip_box = dpg.add_input_text(label="IP", default_value=DEFAULT_IP)
    dpg.add_button(label="Connect", callback=connect_to_robot, user_data=robot_ip_box)

    robot_mac_text = dpg.add_text("Robot MAC Address: None");
    
    #dpg.add_slider_float(label="float", default_value=0.273, max_value=1)

dpg.create_viewport(title='ChessBot Local Admin', width=1000, height=700)
dpg.setup_dearpygui()
dpg.show_viewport()

while dpg.is_dearpygui_running():


    dpg.render_dearpygui_frame()

dpg.destroy_context()